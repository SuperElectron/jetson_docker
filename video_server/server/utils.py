# /video_server/server/utils.py

import logging
import gi
gi.require_version('Gst', '1.0')
from gi.repository import GObject, Gst


def link_src_pad_on_request(src, new_pad, link_element, mode):
    """
    dynamic pad callback for tsdemux (expects link_element={video, klv}, or one element)
    """
    element_names = None
    if isinstance(link_element, dict):
        try:
            klv_element = link_element['klv']
            video_element = link_element['video']
            element_names = [link_element['klv'], link_element['video']]
        except Exception as err:
            logging.error(f"[{mode}]: {err}")
            raise TypeError
    else:
        element_names = [link_element.get_name(), link_element.get_name()]
        video_element = link_element
        klv_element = link_element

    logging.info(f"[{mode}]: on_pad_added called! Received new pad '{new_pad.get_name()}' from '{src.get_name()}")
    new_pad_caps = new_pad.get_current_caps()
    new_pad_struct = new_pad_caps.get_structure(0)
    new_pad_type = new_pad_struct.get_name()

    if 'private' in new_pad.get_name():
        link_sink_pad = klv_element.get_static_pad("sink")
        element_name = element_names[0]

    elif 'video' in new_pad.get_name():
        # handle dataflow for video
        link_sink_pad = video_element.get_static_pad("sink")
        element_name = element_names[1]
    else:
        logging.warning(f"[{mode}]: Unexpected pad type for tsdemux: {new_pad.get_name}")
        raise ValueError

    if not link_sink_pad:
        logging.error(f"[{mode}]: Unable to get sink pad of the element you want to link")
        raise RuntimeError

    logging.info(
        f"[{mode}]: Link src pad ({new_pad.get_name()}:{new_pad_type}) "
        f"to sink pad ({element_name})")

    linking_status = new_pad.link(link_sink_pad)
    if linking_status != Gst.PadLinkReturn.OK:
        logging.error(f"[{mode}]: LINKING FAILURE {new_pad.get_name()}: "
                      f"Type is '{new_pad_type}' but link failed")
        raise RuntimeError

    logging.info(f"[{mode}]: LINKING SUCCESS: {src.get_name()}:{new_pad.get_name()} (type '{new_pad_type}')")
    return
