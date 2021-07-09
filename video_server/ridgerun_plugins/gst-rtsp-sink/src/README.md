# GstRtspSink for Gstreamer-1.0

RTSP Sink is a GStreamer element which permits high performance streaming to multiple computers using the RTSP / RTP protocols. The rtspsink element leverages previous logic from RidgeRun's RTSP server with extensions to create a GStreamer sink element providing benefits like greater flexibility to application integrate and easy gst-launch based testing.

With RTSP Sink you have the flexibility to stream different content to the same client, such as streaming audio and video to a client. You also can send different streams to different clients. This means that within a single GStreamer pipeline you can stream multiple videos, multiple audios and multiple lip-sync audio+video streams, each one to a different client using a different RTSP mapping. In the examples section different streaming possibilities are shown.

> The official documentation is held [here](http://developer.ridgerun.com/wiki/index.php?title=RTSP_Sink "RidgeRun's Developer's Wiki")