#!/usr/bin/env python3

import gi
gi.require_version('Gst', '1.0')
from gi.repository import GObject, Gst
import sys
import json
import pytz
import logging
from datetime import datetime

from setup_logging import setup_logging
from bus_call import bus_call
from utils import link_src_pad_on_request

setup_logging()
DEBUG_PRINT_MODULUS = 100
VIDEO_TERMINATION_SIZE = 3000


class TsParser:

    def __init__(self, argz):
        self.thread_rtsp = None

        self.script_path = argz[0]
        self.codec = argz[1]

        self.pipeline = {
            'eo': {'pipeline': None, 'bus': None, 'loop': None},
            'ir': {'pipeline': None, 'bus': None, 'loop': None}
        }
        self.source = None
        self.metasrc = None

        # self.udp_sink_host = 'localhost'  # sent to mist-ai server
        self.udp_sink_host = '224.1.1.1'  # sent to mist-ai server
        self.udp_sink_port_video_eo = 15011
        self.udp_sink_port_klv_eo = 15012
        self.udp_sink_port_klv2_eo = 15013

        self.udp_sink_port_video_ir = 16011
        self.udp_sink_port_klv_ir = 16012
        self.udp_sink_port_klv2_ir = 16013

        self.rtsp_port_eo = 8554
        self.rtsp_port_ir = 9000
        self.rtsp_host = self.get_host_name_ip()

        self.video_in_frame_count = -1
        self.klv_in_raw_frame_count = -1
        self.klv_in_track_frame_count = -1

        self.rtsp_video_frame_count = -1
        self.rtsp_klv_raw_frame_count = -1
        self.rtsp_klv_track_frame_count = -1

    @staticmethod
    def get_host_name_ip():
        import socket
        try:
            host_name = socket.gethostname()
            host_ip = socket.gethostbyname(host_name)
            logging.debug(f"Hostname : {host_name}")
            logging.debug(f"IP : {host_ip}")
            return str(host_ip)
        except Exception as exp:
            logging.error(f"Unable to get Hostname and IP: {exp}")
            raise RuntimeError

    def create_timestamp(self, tz_pst=False):
        """
        Creates iso timestamp of form: '2016-11-16T22:31:18.130822+00:00'
        """
        utc_now = pytz.utc.localize(datetime.utcnow())
        if tz_pst:
            pst_now = utc_now.astimezone(pytz.timezone("America/Vancouver"))
            return pst_now
        else:
            return utc_now

    def sample_metadata_json(self, uid_counter):
        """
        Creates a metadata string for metasrc buffer
        :param uid_counter: buffer frame counter passed to klv packet
        :return: json string of data dict to be pushed to metasrc buffer
        """
        data_dict = {
            'type': 'Feature',
            'globalUID': 'TS-0001',
            'id': 'target',
            'description': 'sensor_source_track',
            'geometry': {
                'type': 'Point', 'coordinates': ['-81.174730649765', '43.01749887085404', '273.0174715800715']
            },
            'properties': {
                'utc': self.create_timestamp(tz_pst=False),
                'uid': uid_counter,
                'sensor_hdg': 49.911650263218135,
                'sensor_lat': 43.02902138933027,
                'sensor_lon': -81.19318374488185,
                'sensor_alt': 1253.8216220340273
            }
        }
        return json.dumps(data_dict, default=str)

    def generic_buffer_probe(self, pad, info, mode) -> Gst.PadProbeReturn:
        """
        A generic buffer probe for debugging that logs stream info to console
        :param pad: unused
        :param info: unused
        :param mode: a string passed to increment and display buffer number that was parsed
        :return:
        """

        if mode == 'video_in_eo' or mode == 'video_in_ir':
            self.video_in_frame_count += 1
            count = self.video_in_frame_count

        elif mode == 'klv_in_eo_raw' or mode == 'klv_in_ir_raw':
            self.klv_in_raw_frame_count += 1
            count = self.klv_in_raw_frame_count

        elif mode == 'klv_in_eo_track' or mode == 'klv_in_ir_track':
            self.klv_in_track_frame_count += 1
            count = self.klv_in_track_frame_count

        elif mode == 'video_out_rtsp_eo' or mode == 'video_out_rtsp_ir':
            self.rtsp_video_frame_count += 1
            count = self.rtsp_video_frame_count

        elif mode == 'klv_out_raw_rtsp_eo' or mode == 'klv_out_raw_rtsp_ir':
            self.rtsp_klv_raw_frame_count += 1
            count = self.rtsp_klv_raw_frame_count

        elif mode == 'klv_out_track_rtsp_eo' or mode == 'klv_out_track_rtsp_ir':
            self.rtsp_klv_track_frame_count += 1
            count = self.rtsp_klv_track_frame_count

        else:
            logging.error(f"[{mode}]: incorrect arg for generic_buffer_probe(): mode --> {mode}")
            raise RuntimeError

        if count % DEBUG_PRINT_MODULUS == 0:
            msg = 'rcvd'
            logging.info(f'[{mode:>25}]: {msg:^20} {count: >10}')
        return Gst.PadProbeReturn.OK

    def klv_metadata_buffer_probe(self, pad, info, mode, pipeline_mode) -> Gst.PadProbeReturn:

        gst_buffer = info.get_buffer()
        if not gst_buffer:
            logging.error(f"[{mode}]: Unable to get GstBuffer")
            return Gst.PadProbeReturn.PASS

        if isinstance(gst_buffer, Gst.Buffer):
            (result, map_info) = gst_buffer.map(Gst.MapFlags.READ)
            if isinstance(map_info, Gst.MapInfo):
                self.klv_in_raw_frame_count += 1
                try:
                    data_json = self.sample_metadata_json(self.klv_in_raw_frame_count)
                    self.metasrc.set_property('metadata', data_json)
                    if self.klv_in_raw_frame_count % DEBUG_PRINT_MODULUS == 0:
                        msg = 'rcvd'
                        logging.info(f'[{mode:>25}]: {msg:^20} {self.klv_in_raw_frame_count: >10}')

                except Exception as error_msg:
                    logging.warning(f"[{mode}]: ERROR: {error_msg}")
                    raise RuntimeError

                if self.klv_in_raw_frame_count == VIDEO_TERMINATION_SIZE:
                    msg = f'{pipeline_mode}: rcvd last frame, sending EOS event to terminate the pipeline.'
                    logging.info(f'[{mode:>25}]: {msg:<20}')
                    self.pipeline[str(pipeline_mode)]['pipeline'].send_event(Gst.Event.new_eos())

                return Gst.PadProbeReturn.OK

    def setup_pipeline(self, mode=None, video_file=None):

        ##################################################################
        # Argument checks
        if mode is None:
            logging.warning(f"[{mode}]: configure setup_pipeline() with one of mode = 'eo' or 'ir'")
            raise Exception(f"[{mode}]: invalid arg mode=None")
        if video_file is None:
            logging.warning(f"[{mode}]: configure setup_pipeline() with a valid video path")
            raise Exception(f"[{mode}]: invalid arg video_file=None")

        if mode == 'eo':
            program_number = 1
            udp_sink_port_video = self.udp_sink_port_video_eo
            udp_sink_port_klv = self.udp_sink_port_klv_eo
            udp_sink_port_klv2 = self.udp_sink_port_klv2_eo

        elif mode == 'ir':
            program_number = 1
            udp_sink_port_video = self.udp_sink_port_video_ir
            udp_sink_port_klv = self.udp_sink_port_klv_ir
            udp_sink_port_klv2 = self.udp_sink_port_klv2_ir

        else:
            logging.error('Incorrect modality passed: must be one of these: (eo,ir)')
            raise RuntimeError

        logging.info(f"[{mode}]: creating Pipeline ...")
        logging.info(f'[{mode}]: using video file: {video_file}')

        ##################################################################
        # Pipeline construction and general set up
        self.pipeline[str(mode)]['pipeline'] = Gst.Pipeline()
        if not self.pipeline[str(mode)]['pipeline']:
            logging.info(f"[WARNING {mode}]: Unable to create {str(mode)} Pipeline ")

        self.pipeline[str(mode)]['loop'] = GObject.MainLoop()
        self.pipeline[str(mode)]['bus'] = self.pipeline[str(mode)]['pipeline'].get_bus()
        self.pipeline[str(mode)]['bus'].add_signal_watch()
        self.pipeline[str(mode)]['bus'].connect("message", bus_call, self.pipeline[str(mode)]['loop'])

        ##################################################################
        # Create gstreamer elements
        source = Gst.ElementFactory.make("filesrc", "file-source")
        source.set_property('location', video_file)
        # self.source = source
        # source.set_property('do-timestamp', True)

        tsparse = Gst.ElementFactory.make("tsparse", "tsparse")
        tsparse.set_property('set-timestamps', True)
        tsdemux = Gst.ElementFactory.make("tsdemux", "tsdemux")
        tsdemux.set_property("program-number", program_number)
        tsdemux.set_property("parse-private-sections", True)

        parser_codec_string = None
        if self.codec == 'H264':
            parser_codec_string = "h264parse"
            encoder_codec_string = "nvv4l2h264enc"
        elif self.codec == 'H265':
            parser_codec_string = "h265parse"
            encoder_codec_string = "nvv4l2h265enc"
        else:
            logging.error("Incorrect self.codec which is not in (H264, H265).")
            raise RuntimeError

        # tsdemux pads split into two streams
        h26Xparser = Gst.ElementFactory.make(parser_codec_string, parser_codec_string)
        h26Xparser.set_property('config-interval', -1)
        decoder = Gst.ElementFactory.make("nvv4l2decoder", "nvv4l2-decoder")
        encoder = Gst.ElementFactory.make(encoder_codec_string, encoder_codec_string)
        encoder.set_property("maxperf-enable", True)
        encoder.set_property("profile", 4)  # 0:baseline, 1:main, 4:High

        # Output stream (video ==> udp)
        mpegtsmux = Gst.ElementFactory.make('mpegtsmux', 'mpegtsmux')
        mpegtsmux.set_property('alignment', 7)

        buffer = Gst.ElementFactory.make("rndbuffersize", "rndbuffersize")
        buffer.set_property('min', 1500)
        buffer.set_property('max', 3500)

        sink = Gst.ElementFactory.make("udpsink", "udpsink_video")
        # sink.set_property('async', False)
        # sink.set_property('sync', True)
        sink.set_property('auto-multicast', True)
        sink.set_property('port', udp_sink_port_video)
        sink.set_property('host', self.udp_sink_host)
        sink.set_property('qos', True)

        # klv stream
        queue_klv = Gst.ElementFactory.make('queue', 'queue_klv')
        klv_sink = Gst.ElementFactory.make('udpsink', 'udpsink_klv')
        klv_sink.set_property('qos', True)
        # klv_sink.set_property('sync', True)
        # klv_sink.set_property('async', False)
        klv_sink.set_property('auto-multicast', True)
        klv_sink.set_property('port', udp_sink_port_klv)
        klv_sink.set_property('host', self.udp_sink_host)

        # Metasrc stream
        self.metasrc = Gst.ElementFactory.make('metasrc', 'metasrc')
        self.metasrc.set_property('stream-type', 0)  # 0=stream, 1= seekable, 2=random-access
        self.metasrc.set_property('is-live', True)
        self.metasrc.set_property('do-timestamp', True)

        metasrc_caps = Gst.ElementFactory.make('capsfilter', 'metasrc_caps')
        caps_string = Gst.caps_from_string('meta/x-klv')
        metasrc_caps.set_property('caps', caps_string)

        metasrc_sink = Gst.ElementFactory.make('udpsink', 'metasrc_udp_sink')
        metasrc_sink.set_property('port', udp_sink_port_klv2)
        metasrc_sink.set_property('host', self.udp_sink_host)
        metasrc_sink.set_property('async', False)
        # metasrc_sink.set_property('sync', False)
        metasrc_sink.set_property('auto-multicast', True)
        metasrc_sink.set_property('qos', True)
        ##################################################################
        # Add elements to the pipeline bin

        # SOURCE 1
        logging.info(f"[{mode}]: Adding elements to Pipeline ...")
        self.pipeline[str(mode)]['pipeline'].add(source)
        self.pipeline[str(mode)]['pipeline'].add(tsparse)
        self.pipeline[str(mode)]['pipeline'].add(tsdemux)

        # STREAM 1: video
        self.pipeline[str(mode)]['pipeline'].add(h26Xparser)
        self.pipeline[str(mode)]['pipeline'].add(decoder)
        self.pipeline[str(mode)]['pipeline'].add(encoder)
        self.pipeline[str(mode)]['pipeline'].add(mpegtsmux)
        self.pipeline[str(mode)]['pipeline'].add(buffer)
        self.pipeline[str(mode)]['pipeline'].add(sink)

        # Stream 2: klv from video
        self.pipeline[str(mode)]['pipeline'].add(queue_klv)
        self.pipeline[str(mode)]['pipeline'].add(klv_sink)

        # Source 2: klv from generated metadata
        self.pipeline[str(mode)]['pipeline'].add(self.metasrc)
        self.pipeline[str(mode)]['pipeline'].add(metasrc_caps)
        self.pipeline[str(mode)]['pipeline'].add(metasrc_sink)
        ##################################################################
        # Link elements in the pipeline bin
        ############################################
        # Link source elements
        logging.info(f"[{mode}]: Linking elements in the Pipeline ...")
        source.link(tsparse)
        tsparse.link(tsdemux)
        tsdemux.connect('pad-added', link_src_pad_on_request, {'video': h26Xparser, 'klv': queue_klv}, mode)
        h26Xparser.link(decoder)
        decoder.link(encoder)

        ############################################
        # Link video elements
        encoder_src_pad = encoder.get_static_pad('src')
        mpegtsmux_video_sink_pad1 = mpegtsmux.get_request_pad("sink_%d")
        if not encoder_src_pad:
            logging.error(f"[{mode}] Unable to get the static src pad of encoder_src_pad")
            raise RuntimeError
        if not mpegtsmux_video_sink_pad1:
            logging.error(f"[{mode}] Unable to get the request sink pad of "
                          f"mpegtsmux for caps=video/x-{self.codec.lower()}")
            raise RuntimeError
        try:
            encoder_src_pad.link(mpegtsmux_video_sink_pad1)
        except Exception as err:
            logging.error(f"[{mode}]: ERROR linking h26Xparser_src_pad to mpegtsmux_video_sink_pad1:\n {err}")
            raise RuntimeError

        # Link to output 1
        mpegtsmux.link(buffer)
        buffer.link(sink)

        ############################################
        # Link klv elements
        ############################################

        # Link to output 2
        queue_klv.link(klv_sink)

        # link metasrc elements
        self.metasrc.link(metasrc_caps)
        metasrc_caps.link(metasrc_sink)

        ##################################################################
        # Add a pad probe to parse meta data in video analytics and klv streams
        ############################################

        encoder_sinkpad = encoder.get_static_pad("sink")
        if not encoder_sinkpad:
            logging.error(f"[{mode}]: Unable to get sink pad of encoder ")
            raise RuntimeError
        encoder_sinkpad.add_probe(Gst.PadProbeType.BUFFER, self.generic_buffer_probe, f'video_in_{mode}')

        queue_klv_sink_pad = queue_klv.get_static_pad("sink")
        if not queue_klv_sink_pad:
            logging.error(f"[{mode}]: Unable to get sink pad of klv_raw_queue ")
            raise RuntimeError
        queue_klv_sink_pad.add_probe(Gst.PadProbeType.BUFFER, self.klv_metadata_buffer_probe, f'klv_in_{mode}_raw', mode)  # noqa 501

        metasrc_caps_sink_pad = metasrc_caps.get_static_pad("sink")
        if not metasrc_caps_sink_pad:
            logging.error(f"[{mode}]: Unable to get sink pad of metasrc_caps ")
            raise RuntimeError
        metasrc_caps_sink_pad.add_probe(Gst.PadProbeType.BUFFER, self.generic_buffer_probe, f'klv_in_{mode}_track')

    def run(self, mode='eo'):

        try:
            # start play back and listed to events
            logging.info(f"[{mode}]: Starting pipeline ...")
            self.pipeline[str(mode)]['pipeline'].set_state(Gst.State.PLAYING)
            self.pipeline[str(mode)]['loop'].run()
        except Exception as err:
            logging.error(f"[{mode}]: Unknown error occurred during run time: {err}")
            pass
        finally:
            # cleanup
            import os
            logging.info(f"[{mode}]: Terminating gstreamer pipeline ...")
            self.pipeline[str(mode)]['pipeline'].set_state(Gst.State.NULL)
            logging.info(f"[{mode}]: Terminating gstreamer pipeline ...")

    def rtsp_server(self, mode='rtsp_eo', mapping='/eo'):

        logging.info(f"[{mode}]: Starting RTSP server")
        self.pipeline_rtsp = Gst.Pipeline()
        self.rtsp_loop = GObject.MainLoop()
        self.rtsp_bus = self.pipeline_rtsp.get_bus()
        self.rtsp_bus.add_signal_watch()
        self.rtsp_bus.connect("message", bus_call, self.rtsp_loop)

        ###### CREATE PIPELINE ELEMENTS
        # Setup gstreamer pipeline elements
        # source 1: udp video stream
        if mode == 'rtsp_eo':
            udp_port_video = self.udp_sink_port_video_eo
            udp_port_klv = self.udp_sink_port_klv_eo
            udp_port_klv2 = self.udp_sink_port_klv2_eo
            rtsp_sink_port = self.rtsp_port_eo
        elif mode == 'rtsp_ir':
            udp_port_video = self.udp_sink_port_video_ir
            udp_port_klv = self.udp_sink_port_klv_ir
            udp_port_klv2 = self.udp_sink_port_klv2_ir
            rtsp_sink_port = self.rtsp_port_ir
        else:
            logging.error("[{mode}]: Incorrect argument for RTSP server ...")
            raise RuntimeError

        source_1_video = Gst.ElementFactory.make('udpsrc', f'udpsrc_video_{mode}')
        source_1_video.set_property('port', udp_port_video)
        source_1_video.set_property('address', self.udp_sink_host)
        source_1_video.set_property('buffer-size', 100000000)
        source_1_video.set_property('do-timestamp', True)

        source_1_input_caps = Gst.ElementFactory.make('capsfilter', f"source_input_caps_{mode}")
        source_1_caps_object = Gst.caps_from_string("video/mpegts, systemstream=(boolean)true, packetsize=(int)188")
        source_1_input_caps.set_property('caps', source_1_caps_object)

        tsparse = Gst.ElementFactory.make('tsparse', f'tsparse_{mode}')
        tsparse.set_property('set-timestamps', True)

        tsdemux = Gst.ElementFactory.make('tsdemux', f'tsdemux_{mode}')
        source_1_queue = Gst.ElementFactory.make('queue', f'source_queue_{mode}')
        h264parse_s1 = Gst.ElementFactory.make('h264parse', f'h264parse_{mode}')
        h264parse_s1.set_property('config-interval', -1)

        caps_video = Gst.ElementFactory.make('capsfilter', f'caps_video_{mode}')
        caps_video_object = Gst.caps_from_string(f'video/x-{self.codec.lower()}')
        caps_video.set_property('caps', caps_video_object)

        mpegtsmux = Gst.ElementFactory.make('mpegtsmux', f'mpegtsmux_{mode}')
        # mpegtsmux.set_property('alignment', 7)   # for udp streaming
        # mpegtsmux.set_property('alignment', -1)  # auto
        # mpegtsmux.set_property('alignment', 0)   # for all available packets

        output_filter = Gst.ElementFactory.make('capsfilter', f'output_filter_{mode}')
        caps = Gst.caps_from_string(f"video/mpegts, mapping={mapping}")
        output_filter.set_property('caps', caps)

        sink = Gst.ElementFactory.make('rtspsink', f'rtspsink_{mode}')
        sink.set_property('service', rtsp_sink_port)
        # sink.set_property('multicast', True)
        # sink.set_property('multicast-port-min', 10000)
        # sink.set_property('multicast-port-max', 10019)

        # source 2: udp klv stream
        source_2_klv = Gst.ElementFactory.make('udpsrc', f'source_klv_{mode}')
        source_2_klv.set_property('buffer-size', 100000000)
        source_2_klv.set_property('do-timestamp', True)
        source_2_klv.set_property('port', udp_port_klv)
        source_2_klv.set_property('address', self.udp_sink_host)

        source_2_klv_caps = Gst.ElementFactory.make('capsfilter', f'source_klv_caps_{mode}')
        source_2_klv_caps.set_property('caps', Gst.caps_from_string('meta/x-klv'))
        source_2_queue = Gst.ElementFactory.make('queue', f'source_klv_queue_{mode}')

        # source 3: udp custom klv stream
        source_3_klv = Gst.ElementFactory.make('udpsrc', f'source_3_klv_{mode}')
        source_3_klv.set_property('buffer-size', 100000000)
        source_3_klv.set_property('do-timestamp', True)
        source_3_klv.set_property('port', udp_port_klv2)
        source_3_klv.set_property('address', self.udp_sink_host)

        source_3_klv_caps = Gst.ElementFactory.make('capsfilter', f'source_3_klv_caps_{mode}')
        source_3_klv_caps.set_property('caps', Gst.caps_from_string('meta/x-klv'))
        source_3_queue = Gst.ElementFactory.make('queue', f'source_3_queue_{mode}')

        ###### ADD TO PIPELINE BIN
        # udp stream 1 (video)
        self.pipeline_rtsp.add(source_1_video)
        self.pipeline_rtsp.add(source_1_input_caps)
        self.pipeline_rtsp.add(tsparse)
        self.pipeline_rtsp.add(tsdemux)
        self.pipeline_rtsp.add(source_1_queue)
        self.pipeline_rtsp.add(h264parse_s1)
        self.pipeline_rtsp.add(caps_video)
        self.pipeline_rtsp.add(mpegtsmux)
        self.pipeline_rtsp.add(output_filter)
        self.pipeline_rtsp.add(sink)
        # udp stream 2 (klv)
        self.pipeline_rtsp.add(source_2_klv)
        self.pipeline_rtsp.add(source_2_klv_caps)
        self.pipeline_rtsp.add(source_2_queue)
        # udp stream 3 (custom klv)
        self.pipeline_rtsp.add(source_3_klv)
        self.pipeline_rtsp.add(source_3_klv_caps)
        self.pipeline_rtsp.add(source_3_queue)

        ###### LINKING
        # linking stream 1 (video)
        source_1_video.link(source_1_input_caps)
        source_1_input_caps.link(tsparse)
        tsparse.link(tsdemux)
        tsdemux.connect('pad-added', link_src_pad_on_request, {'video': source_1_queue, 'klv': None}, mode)
        source_1_queue.link(h264parse_s1)
        h264parse_s1.link(caps_video)

        caps_video_src_pad = caps_video.get_static_pad('src')
        mpegtsmux_video_pad = mpegtsmux.get_request_pad('sink_%d')
        if not mpegtsmux_video_pad:
            logging.error(f"[{mode}]: Cannot get request pad from mpegtsmux (sink_%d)")
            raise RuntimeWarning
        if not caps_video_src_pad:
            logging.error(f"[{mode}]: Cannot get request pad from caps_video (src)")
            raise RuntimeWarning
        caps_video_src_pad.link(mpegtsmux_video_pad)

        # linking stream 2 (klv)
        source_2_klv.link(source_2_klv_caps)
        source_2_klv_caps.link(source_2_queue)

        mpegtsmux_meta_pad = mpegtsmux.get_request_pad('meta_%d')
        source_2_queue_src_pad = source_2_queue.get_static_pad('src')

        if not mpegtsmux_meta_pad:
            logging.error(f"[{mode}]: Cannot get request pad from mpegtsmux (meta_%d)")
            raise RuntimeWarning
        if not source_2_queue_src_pad:
            logging.error(f"[{mode}]: Cannot get request pad from source_2_queue (src)")
            raise RuntimeWarning
        source_2_queue_src_pad.link(mpegtsmux_meta_pad)

        # linking stream 3
        source_3_klv.link(source_3_klv_caps)
        source_3_klv_caps.link(source_3_queue)
        source_3_queue_src_pad = source_3_queue.get_static_pad('src')
        mpegtsmux_meta_pad2 = mpegtsmux.get_request_pad('meta_%d')
        if not mpegtsmux_meta_pad2:
            logging.error(f"[{mode}]: Cannot get request pad from mpegtsmux (meta_%d)")
            raise RuntimeWarning
        if not source_3_queue_src_pad:
            logging.error(f"[{mode}]: Cannot get request pad from source_3_queue (src)")
            raise RuntimeWarning
        source_3_queue_src_pad.link(mpegtsmux_meta_pad2)

        # Link to output stream
        mpegtsmux.link(output_filter)
        output_filter_src_pad = output_filter.get_static_pad("src")
        rtsp_sink_pad = sink.get_request_pad("sink_%d")
        if not output_filter_src_pad:
            logging.error(f"[{mode}] rtsp_server() Unable to get source pad of output_filter_src_pad ")
            raise RuntimeError
        if not rtsp_sink_pad:
            logging.error(f"[{mode}]: rtsp_server() unable to get the sink pad of rtsp_sink_pad")
            raise RuntimeError
        output_filter_src_pad.link(rtsp_sink_pad)

        ##### Create pad probes
        caps_video_sink_pad = caps_video.get_static_pad("sink")
        caps_video_sink_pad.add_probe(Gst.PadProbeType.BUFFER, self.generic_buffer_probe, f'video_out_{mode}')

        source_2_queue_sink_pad = source_2_queue.get_static_pad("sink")
        source_2_queue_sink_pad.add_probe(Gst.PadProbeType.BUFFER, self.generic_buffer_probe, f'klv_out_raw_{mode}')

        source_3_queue_sink_pad = source_3_queue.get_static_pad("sink")
        source_3_queue_sink_pad.add_probe(Gst.PadProbeType.BUFFER, self.generic_buffer_probe, f'klv_out_track_{mode}')

        ###### Start gstreamer pipeline
        try:
            # start play back and listed to events
            logging.info(f"[{mode}]: "
                         f"Starting RTSP pipeline: gst-play-1.0 rtsp://{self.rtsp_host}:{rtsp_sink_port}{mapping}")

            self.pipeline_rtsp.set_state(Gst.State.PLAYING)
            self.rtsp_loop.run()

        except Exception as err:
            logging.error(f"[{mode}]: Unknown error occurred during run time: {err}")
            pass
        finally:
            # cleanup
            import os
            logging.info(f"[{mode}]: Terminating gstreamer pipeline ...")
            # open(f'logs/{os.path.splitext(self.script_path)[0]}-{str(mode)}.pipeline.dot', 'w').write(
            #     Gst.debug_bin_to_dot_data(self.pipeline_rtsp, Gst.DebugGraphDetails.ALL)
            # )
            logging.info(f"[{mode}]: Finished saving gstreamer DEBUG_DUMP_DOT to <project>/logs/<script name>.pipeline.dot")

            self.pipeline_rtsp.set_state(Gst.State.NULL)
            logging.info(f"[{mode}]: Terminating gstreamer pipeline ...")


if __name__ == '__main__':
    from threading import Thread
    from time import sleep

    args = sys.argv
    args.append('H264')
    video_eo = '/videos/video1.ts'
    video_ir = '/videos/video2.ts'

    GObject.threads_init()
    Gst.init(None)

    logging.info(f"[main]: Starting RTSP servers")
    rtsp_server_eo = TsParser(args)
    t_rtsp_eo = Thread(target=rtsp_server_eo.rtsp_server, kwargs={'mode': 'rtsp_eo', 'mapping': '/eo'})
    t_rtsp_eo.start()
    sleep(5)

    rtsp_server_ir = TsParser(args)
    t_rtsp_ir = Thread(target=rtsp_server_ir.rtsp_server, kwargs={'mode': 'rtsp_ir', 'mapping': '/ir'})
    t_rtsp_ir.start()
    sleep(5)

    logging.info(f"[main]: Ready to set up pipelines that read ts files for eo and ir")

    application_eo = TsParser(args)
    application_eo.setup_pipeline(mode='eo', video_file=video_eo)
    application_ir = TsParser(args)
    application_ir.setup_pipeline(mode='ir', video_file=video_ir)

    # number_of_runs = 2
    number_of_runs = 'infinity'
    this_run = 1
    # while this_run <= number_of_runs:
    try:
        while True:
            application_eo.video_in_frame_count = -1
            application_eo.klv_in_raw_frame_count = -1
            application_eo.klv_in_track_frame_count = -1
            application_eo.rtsp_video_frame_count = -1
            application_eo.rtsp_klv_raw_frame_count = -1
            application_eo.rtsp_klv_track_frame_count = -1

            application_ir.video_in_frame_count = -1
            application_ir.klv_in_raw_frame_count = -1
            application_ir.klv_in_track_frame_count = -1
            application_ir.rtsp_video_frame_count = -1
            application_ir.rtsp_klv_raw_frame_count = -1
            application_ir.rtsp_klv_track_frame_count = -1

            logging.info(f'[main]: Starting threads (loop {this_run} of {number_of_runs})')
            t_eo = Thread(target=application_eo.run, kwargs={'mode': 'eo'})
            t_ir = Thread(target=application_ir.run, kwargs={'mode': 'ir'})
            t_eo.start()
            t_ir.start()
            t_eo.join()
            logging.info(f'[main]: Killed eo threads (loop {this_run} of {number_of_runs})')
            t_ir.join()
            logging.info(f'[main]: Killed ir threads (loop {this_run} of {number_of_runs})')
            this_run += 1
    except KeyboardInterrupt as err:
        pass

    logging.info("[main]: Terminated eo and ir threads ... going through shut down process now.")

    logging.info("[main]: Shutting down RTSP server")
    try:
        logging.info("[main]: Shutting down RTSP server loops ...")
        rtsp_server_eo.rtsp_loop.quit()
        rtsp_server_ir.rtsp_loop.quit()
    except Exception as err:
        logging.warning("[main]: ERROR with rtsp server GObject.MainLoop() ... your service may have died early.")

    logging.info("[main]: RTSP servers closed ... exiting program now.")
    sys.exit(1)
