#!/bin/bash

# Setup for RidgeRun gstreamer plug-ins
# EXAMPLE USAGE: sudo -H ./setup_ridgerun.sh

printf "***** Starting installations for RidgeRun gstreamer plug-ins *****\n"
echo "BUILD 1: 'gst-metadata'"
echo "BUILD 2: 'gst-rtsp-sink'"

export WORKDIR=$(pwd)

echo "BUILD 1: Setting up gst-rtsp-sink"
echo "BUILD 1: INSTALLATION STEP 1: Gstreamer dependencies"
apt-get update -y
apt-get install -y \
  libgstrtspserver-1.0-dev \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libgstreamer-plugins-good1.0-dev \
  libgstreamer-plugins-bad1.0-dev

echo "BUILD 1: INSTALLATION STEP 2: build rtspsink"
export LIB_DIR_GST=/usr/lib/aarch64-linux-gnu/
DL="$(test -d $WORKDIR/gst-rtsp-sink && echo 'yes' || echo 'no')"
export DL

if [ "$DL" == "yes" ]
 then
    echo "--(pass)-- gst-rtsp-sink code ready for build"
  else
    echo "********"
    echo "--(fail)-- Cannot find code gst-rtsp-sink ... ensure that it is in /video_server/ridgerun_plugins"
    echo "$DL"
    exit
fi

cd "${WORKDIR}"/gst-rtsp-sink/src; ./autogen.sh; ./configure --libdir="${LIB_DIR_GST}"; make; make install

echo "BUILD 1: INSTALLATION STEP 3: Validating plug-in exists"
INSTALL_FAIL="$(/usr/bin/gst-inspect-1.0 rtspsink | grep 'No such element or plugin')"

if [ "$INSTALL_FAIL" == "No such element or plugin 'rtspsink'" ]
  then
    echo "--(fail)-- Failed to install ridgerun rtspsink plugin"
    echo "If failure is a success, then check path (LIB_DIR_GST) for other gstreamer libraries such as libgstapp.so. LIB_DIR_GST: ${LIB_DIR_GST}"
    exit
  else
     echo "--(pass)-- Successfully installed ridgerun rtspsink plugin"
fi

apt-get -y autoclean && apt-get -y clean && apt-get -y autoremove

echo "BUILD 1: completed"

echo "BUILD 2: Setting up gst-metadata"
echo "BUILD 2: INSTALLATION STEP 1: installing dependencies"

apt-get update -y
apt-get install -y \
  gtk-doc-tools libgtk2.0-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev libmp3lame-dev \
  pkg-config bison flex git \
  libglib2.0-dev liborc-0.4-dev \
  libtool autopoint autoconf \
  gettext yasm build-essential \
  autotools-dev dpkg-dev automake \
  libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
  liborc-dev autopoint gtk-doc-tools libgstreamer1.0-dev \
  libxv-dev libasound2-dev libtheora-dev libogg-dev libvorbis-dev \
  libbz2-dev libv4l-dev libvpx-dev libjack-jackd2-dev libsoup2.4-dev libpulse-dev \
  faad libfaad-dev libfaac-dev libgl1-mesa-dev libgles2-mesa-dev \
  libx264-dev libmad0-dev

DL="$(test -d $WORKDIR/gst-metadata && echo 'yes' || echo 'no')"
export DL

if [ "$DL" == "yes" ]
 then
    echo "--(pass)-- gst-metadata code ready for build"
  else
    echo "********"
    echo "--(fail)-- Cannot find code gst-metadata ... ensure that it is in /video_server/ridgerun_plugins"
    echo "$DL"
    exit
fi

echo "BUILD 2: INSTALLATION STEP 2: build the native gst-metadata code"
export BRANCH=$(gst-inspect-1.0 --version | awk 'NR==2 {print $2}')
export BRANCH_PATCHES=$(echo $BRANCH | cut -d'.' -f 1-2)
export LIBRARY_VERSION=$(echo $BRANCH | awk -F "." '{f=$2; l=$3} END{if(l < 10)l=0l; print f l}')
export INSTALL_PATH=/usr/lib/aarch64-linux-gnu
cd $WORKDIR; export BUILD_DIR=`pwd`/gst-build; mkdir -p $BUILD_DIR; mkdir -p $BUILD_DIR/prefix


echo "BUILD 2: INSTALLATION STEP 3: Patching GStreamer libraries"
# Copy GStreamer patch to gstreamer folder
cp $WORKDIR/gst-metadata/extras/gstreamer-1.14.x/fix-sparse-flag-setting-up.patch $WORKDIR/gstreamer/
# Apply GStreamer patch
cd $WORKDIR/gstreamer; git apply fix-sparse-flag-setting-up.patch

# Copy GStreamer plugins bad patches to gstreamer-plugins-bad folder
cp $WORKDIR/gst-metadata/extras/gstreamer-1.14.x/mpegtsmux-reference-clock-on-waiting-pad.patch $WORKDIR/gst-plugins-bad/ ; \
  cp $WORKDIR/gst-metadata/extras/gstreamer-1.14.x/mpegtsmux-meta-specific-pad-templates.patch $WORKDIR/gst-plugins-bad/ ; \
  cp $WORKDIR/gst-metadata/extras/gstreamer-1.14.x/tsmux-demux-update-klv-support.patch $WORKDIR/gst-plugins-bad/
# Apply GStreamer plugins bad patches
cd $WORKDIR/gst-plugins-bad; git apply mpegtsmux-reference-clock-on-waiting-pad.patch; git apply mpegtsmux-meta-specific-pad-templates.patch; git apply tsmux-demux-update-klv-support.patch

echo "BUILD 2: INSTALLATION STEP 4: Compiling gstreamer"
export MODULES="gstreamer gst-plugins-base gst-plugins-bad"
cd $WORKDIR
for m in $MODULES
do
    cd $m
    ./autogen.sh --disable-gtk-doc --disable-tests --disable-nls && make -j8
    export PKG_CONFIG_PATH=`pwd`/pkgconfig:$PKG_CONFIG_PATH
    cd ..
done

echo "BUILD 2: INSTALLATION STEP 5: Overwrite gstreamer system packages"
export INSTALL_PATH=/usr/lib/aarch64-linux-gnu/
cp $INSTALL_PATH/libgstbase-1.0.so.0.$LIBRARY_VERSION.0 $INSTALL_PATH/libgstbase-1.0.so.0.$LIBRARY_VERSION.0.bk && \
cp $INSTALL_PATH/libgstcodecparsers-1.0.so.0.$LIBRARY_VERSION.0 $INSTALL_PATH/libgstcodecparsers-1.0.so.0.$LIBRARY_VERSION.0.bk && \
cp $INSTALL_PATH/libgstmpegts-1.0.so.0.$LIBRARY_VERSION.0 $INSTALL_PATH/libgstmpegts-1.0.so.0.$LIBRARY_VERSION.0.bk && \
cp $INSTALL_PATH/gstreamer-1.0/libgstmpegpsdemux.so $INSTALL_PATH/gstreamer-1.0/libgstmpegpsdemux.so.bk && \
cp $INSTALL_PATH/gstreamer-1.0/libgstmpegtsmux.so $INSTALL_PATH/gstreamer-1.0/libgstmpegtsmux.so.bk && \
cp $INSTALL_PATH/gstreamer-1.0/libgstmpegtsdemux.so $INSTALL_PATH/gstreamer-1.0/libgstmpegtsdemux.so.bk

# updating system packages by installing the ones we just compiled
cp $WORKDIR/gstreamer/libs/gst/base/.libs/libgstbase-1.0.so.0.$LIBRARY_VERSION.0 $INSTALL_PATH/libgstbase-1.0.so.0.$LIBRARY_VERSION.0  && \
cp $WORKDIR/gst-plugins-bad/gst-libs/gst/codecparsers/.libs/libgstcodecparsers-1.0.so.0.$LIBRARY_VERSION.0 $INSTALL_PATH/libgstcodecparsers-1.0.so.0.$LIBRARY_VERSION.0  && \
cp $WORKDIR/gst-plugins-bad/gst-libs/gst/mpegts/.libs/libgstmpegts-1.0.so.0.$LIBRARY_VERSION.0 $INSTALL_PATH/libgstmpegts-1.0.so.0.$LIBRARY_VERSION.0  && \
cp $WORKDIR/gst-plugins-bad/gst/mpegdemux/.libs/libgstmpegpsdemux.so $INSTALL_PATH/gstreamer-1.0/libgstmpegpsdemux.so  && \
cp $WORKDIR/gst-plugins-bad/gst/mpegtsmux/.libs/libgstmpegtsmux.so $INSTALL_PATH/gstreamer-1.0/libgstmpegtsmux.so  && \
cp $WORKDIR/gst-plugins-bad/gst/mpegtsdemux/.libs/libgstmpegtsdemux.so $INSTALL_PATH/gstreamer-1.0/libgstmpegtsdemux.so

echo "BUILD 2: INSTALLATION STEP 6: Compile gst-metadata plugin"
cd $WORKDIR/gst-metadata/src; ./autogen.sh;
cd $WORKDIR/gst-metadata/src; ./configure --prefix $BUILD_DIR/prefix/
cd $WORKDIR/gst-metadata/src; make all; make install

echo "BUILD 2: INSTALLATION STEP 7: Install gst-metadata"
cp $BUILD_DIR/prefix/lib/gstreamer-1.0/libgstmeta.so $INSTALL_PATH/gstreamer-1.0/libgstmeta.so

echo "BUILD 2: INSTALLATION STEP 8: Verify gst-metadata installation"

INSTALL_FAIL="$(/usr/bin/gst-inspect-1.0 meta | grep 'No such element or plugin')"

if [ "$INSTALL_FAIL" == "No such element or plugin 'meta'" ]
  then
    echo "--(fail)-- Failed to install ridgerun meta plugin"
    exit
  else
     echo "--(pass)-- Successfully installed ridgerun meta plugin"
fi

echo "BUILD 2: Complete"
apt-get -y autoclean && apt-get -y clean && apt-get -y autoremove

echo "***** Finished setup_ridgerun_docker ***** "