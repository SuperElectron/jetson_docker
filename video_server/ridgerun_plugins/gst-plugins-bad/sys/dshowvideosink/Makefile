# Makefile.in generated by automake 1.15.1 from Makefile.am.
# sys/dshowvideosink/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2017 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



# This plugin isn't buildable with autotools at this point in time, so just
# ensure everything's listed in EXTRA_DIST

am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/gst-plugins-bad
pkgincludedir = $(includedir)/gst-plugins-bad
pkglibdir = $(libdir)/gst-plugins-bad
pkglibexecdir = $(libexecdir)/gst-plugins-bad
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = aarch64-unknown-linux-gnu
host_triplet = aarch64-unknown-linux-gnu
target_triplet = aarch64-unknown-linux-gnu
subdir = sys/dshowvideosink
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/common/m4/as-ac-expand.m4 \
	$(top_srcdir)/common/m4/as-auto-alt.m4 \
	$(top_srcdir)/common/m4/as-compiler-flag.m4 \
	$(top_srcdir)/common/m4/as-libtool.m4 \
	$(top_srcdir)/common/m4/as-version.m4 \
	$(top_srcdir)/common/m4/ax_create_stdint_h.m4 \
	$(top_srcdir)/common/m4/ax_pthread.m4 \
	$(top_srcdir)/common/m4/gst-arch.m4 \
	$(top_srcdir)/common/m4/gst-args.m4 \
	$(top_srcdir)/common/m4/gst-check.m4 \
	$(top_srcdir)/common/m4/gst-default.m4 \
	$(top_srcdir)/common/m4/gst-dowhile.m4 \
	$(top_srcdir)/common/m4/gst-error.m4 \
	$(top_srcdir)/common/m4/gst-feature.m4 \
	$(top_srcdir)/common/m4/gst-gettext.m4 \
	$(top_srcdir)/common/m4/gst-glib2.m4 \
	$(top_srcdir)/common/m4/gst-package-release-datetime.m4 \
	$(top_srcdir)/common/m4/gst-platform.m4 \
	$(top_srcdir)/common/m4/gst-plugin-docs.m4 \
	$(top_srcdir)/common/m4/gst-plugindir.m4 \
	$(top_srcdir)/common/m4/gst.m4 \
	$(top_srcdir)/common/m4/gtk-doc.m4 \
	$(top_srcdir)/common/m4/introspection.m4 \
	$(top_srcdir)/common/m4/orc.m4 $(top_srcdir)/common/m4/pkg.m4 \
	$(top_srcdir)/m4/gettext.m4 $(top_srcdir)/m4/iconv.m4 \
	$(top_srcdir)/m4/intlmacosx.m4 $(top_srcdir)/m4/lib-ld.m4 \
	$(top_srcdir)/m4/lib-link.m4 $(top_srcdir)/m4/lib-prefix.m4 \
	$(top_srcdir)/m4/libgcrypt.m4 $(top_srcdir)/m4/libtool.m4 \
	$(top_srcdir)/m4/ltoptions.m4 $(top_srcdir)/m4/ltsugar.m4 \
	$(top_srcdir)/m4/ltversion.m4 $(top_srcdir)/m4/lt~obsolete.m4 \
	$(top_srcdir)/m4/nls.m4 $(top_srcdir)/m4/po.m4 \
	$(top_srcdir)/m4/progtest.m4 $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
SOURCES =
DIST_SOURCES =
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
am__DIST_COMMON = $(srcdir)/Makefile.in README
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
ACLOCAL = ${SHELL} /video_server/ridgerun_plugins/gst-plugins-bad/missing aclocal-1.15
ACLOCAL_AMFLAGS = -I m4 -I common/m4
ACMENC_CFLAGS = 
ACMMP3DEC_CFLAGS = 
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 0
AOM_CFLAGS = 
AOM_LIBS = 
AR = ar
AS = as
ASSRENDER_CFLAGS = 
ASSRENDER_LIBS = 
AUTOCONF = ${SHELL} /video_server/ridgerun_plugins/gst-plugins-bad/missing autoconf
AUTOHEADER = ${SHELL} /video_server/ridgerun_plugins/gst-plugins-bad/missing autoheader
AUTOMAKE = ${SHELL} /video_server/ridgerun_plugins/gst-plugins-bad/missing automake-1.15
AWK = mawk
BLUEZ5_CFLAGS = 
BLUEZ5_LIBS = 
BS2B_CFLAGS = 
BS2B_LIBS = 
BZ2_LIBS = -lbz2
CC = gcc
CCASFLAGS = 
CCDEPMODE = depmode=gcc3
CFLAGS = -g -O2
CHROMAPRINT_CFLAGS = 
CHROMAPRINT_LIBS = 
CPP = gcc -E
CPPFLAGS = 
CUDA_CFLAGS = 
CUDA_LIBS = 
CURL_CFLAGS = 
CURL_LIBS = 
CXX = g++
CXXCPP = g++ -E
CXXDEPMODE = depmode=gcc3
CXXFLAGS = 
CYGPATH_W = echo
DAALA_CFLAGS = 
DAALA_LIBS = 
DECKLINK_CXXFLAGS = 
DECKLINK_LIBS = -lpthread -ldl
DEFAULT_AUDIOSINK = autoaudiosink
DEFAULT_AUDIOSRC = alsasrc
DEFAULT_VIDEOSINK = autovideosink
DEFAULT_VIDEOSRC = v4l2src
DEFAULT_VISUALIZER = goom
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DEPRECATED_CFLAGS = 
DIRECT3D_LIBS = 
DIRECTFB_CFLAGS = 
DIRECTFB_LIBS = 
DIRECTSOUND_LIBS = 
DIRECTX_CFLAGS = 
DIRECTX_LDFLAGS = 
DLLTOOL = false
DSSIM_CFLAGS = 
DSSIM_LIBS = 
DSYMUTIL = 
DTLS_CFLAGS = 
DTLS_LIBS = 
DTS_LIBS = 
DUMPBIN = 
DVDNAV_CFLAGS = 
DVDNAV_LIBS = 
ECHO_C = 
ECHO_N = -n
ECHO_T = 
EGREP = /bin/grep -E
ERROR_CFLAGS = 
ERROR_CXXFLAGS = 
ERROR_OBJCFLAGS = 
EXEEXT = 
EXIF_CFLAGS = 
EXIF_LIBS = 
EXTRA_CFLAGS = -fno-strict-aliasing
FAAC_LIBS = -lfaac -lm
FAAD_LIBS = -lfaad
FDK_AAC_CFLAGS = 
FDK_AAC_LIBS = 
FFLAGS = 
FGREP = /bin/grep -F
FLITE_CFLAGS = 
FLITE_LIBS = 
FLUIDSYNTH_CFLAGS = 
FLUIDSYNTH_LIBS = 
GCOV = 
GCOV_CFLAGS = 
GCOV_LIBS = 
GDBUS_CODEGEN = 
GETTEXT_MACRO_VERSION = 0.17
GETTEXT_PACKAGE = NULL
GIO_CFLAGS = -pthread -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GIO_LDFLAGS = 
GIO_LIBS = -lgio-2.0 -lgobject-2.0 -lglib-2.0
GIO_UNIX_CFLAGS = 
GIO_UNIX_LIBS = 
GLIB_CFLAGS = -pthread -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GLIB_EXTRA_CFLAGS =  -DG_THREADS_MANDATORY -DG_DISABLE_CAST_CHECKS
GLIB_GENMARSHAL = glib-genmarshal
GLIB_LIBS = -lgobject-2.0 -lgmodule-2.0 -pthread -lglib-2.0
GLIB_MKENUMS = glib-mkenums
GLIB_PREFIX = /usr
GLIB_REQ = 2.40.0
GME_LIBS = 
GMODULE_NO_EXPORT_CFLAGS = -pthread -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GMODULE_NO_EXPORT_LIBS = -lgmodule-2.0 -pthread -lglib-2.0
GMSGFMT = /usr/bin/msgfmt
GMSGFMT_015 = /usr/bin/msgfmt
GNUSTL_CFLAGS = 
GNUSTL_LIBS = 
GREP = /bin/grep
GSM_LIBS = 
GSTPB_PLUGINS_DIR = /video_server/ridgerun_plugins/gst-plugins-base
GSTPB_PREFIX = 
GST_AGE = 1405
GST_ALLOCATORS_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_ALLOCATORS_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/allocators/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstallocators-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_ALL_LDFLAGS = -no-undefined -Wl,-Bsymbolic-functions
GST_API_VERSION = 1.0
GST_AUDIO_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/orc-0.4 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_AUDIO_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/audio/.libs -L/video_server/ridgerun_plugins/gstreamer/libs/gst/base/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstaudio-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_BASE_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_BASE_LIBS = -L/video_server/ridgerun_plugins/gstreamer/libs/gst/base/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include  -DGST_USE_UNSTABLE_API -fno-strict-aliasing  -DG_THREADS_MANDATORY -DG_DISABLE_CAST_CHECKS $(GST_OPTION_CFLAGS) $(VISIBILITY_CFLAGS)
GST_CHECK_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_CHECK_LIBS = -L/video_server/ridgerun_plugins/gstreamer/libs/gst/check/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstcheck-1.0 -lm -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_CONTROLLER_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_CONTROLLER_LIBS = -L/video_server/ridgerun_plugins/gstreamer/libs/gst/controller/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstcontroller-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_CURRENT = 1405
GST_CXXFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include  -DGST_USE_UNSTABLE_API -fno-strict-aliasing  -DG_THREADS_MANDATORY -DG_DISABLE_CAST_CHECKS $(GST_OPTION_CXXFLAGS) $(VISIBILITY_CXXFLAGS)
GST_GL_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/usr/include/libdrm -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/orc-0.4 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_GL_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/gl/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/video/.libs -L/video_server/ridgerun_plugins/gstreamer/libs/gst/base/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstgl-1.0 -lgstvideo-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_LEVEL_DEFAULT = GST_LEVEL_NONE
GST_LIBS = -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_LIBVERSION = 1405:0:1405
GST_LIB_LDFLAGS = 
GST_LICENSE = LGPL
GST_LT_LDFLAGS =  -version-info 1405:0:1405
GST_NET_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_NET_LIBS = -L/video_server/ridgerun_plugins/gstreamer/libs/gst/net/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstnet-1.0 -lgstreamer-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
GST_OBJCFLAGS = -pthread -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include  -DGST_USE_UNSTABLE_API -fno-strict-aliasing  -DG_THREADS_MANDATORY -DG_DISABLE_CAST_CHECKS $(GST_OPTION_OBJCFLAGS) $(VISIBILITY_CFLAGS)
GST_OPTION_CFLAGS = $(WARNING_CFLAGS) $(ERROR_CFLAGS) $(DEBUG_CFLAGS) $(PROFILE_CFLAGS) $(GCOV_CFLAGS) $(OPT_CFLAGS) $(DEPRECATED_CFLAGS)
GST_OPTION_CXXFLAGS = $(WARNING_CXXFLAGS) $(ERROR_CXXFLAGS) $(DEBUG_CFLAGS) $(PROFILE_CFLAGS) $(GCOV_CFLAGS) $(OPT_CFLAGS) $(DEPRECATED_CFLAGS)
GST_OPTION_OBJCFLAGS = $(WARNING_OBJCFLAGS) $(ERROR_OBJCFLAGS) $(DEBUG_CFLAGS) $(PROFILE_CFLAGS) $(GCOV_CFLAGS) $(OPT_CFLAGS) $(DEPRECATED_CFLAGS)
GST_PACKAGE_NAME = GStreamer Bad Plug-ins source release
GST_PACKAGE_ORIGIN = Unknown package origin
GST_PBUTILS_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/orc-0.4 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_PBUTILS_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/pbutils/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/audio/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/video/.libs -L/video_server/ridgerun_plugins/gstreamer/libs/gst/base/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstpbutils-1.0 -lgstaudio-1.0 -lgstvideo-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_PKG_CONFIG_PATH = $(top_builddir)/pkgconfig:/video_server/ridgerun_plugins/gst-plugins-base/pkgconfig:/video_server/ridgerun_plugins/gstreamer/pkgconfig:
GST_PLUGINS_ALL =  accurip adpcmdec adpcmenc aiff videoframe_audiolevel asfmux audiobuffersplit audiofxbad audiolatency audiomixmatrix compositor audiovisualizers autoconvert bayer camerabin2 coloreffects debugutils dvbsuboverlay dvdspu faceoverlay festival fieldanalysis freeverb frei0r gaudieffects geometrictransform gdp id3tag inter interlace ivfparse ivtc jp2kdecimator jpegformat librfb midi mpegdemux mpegtsdemux mpegtsmux mpegpsmux mxf netsim onvif pcapparse pnm proxy rawparse removesilence sdp segmentclip siren smooth speed subenc stereo timecode videofilters videoparsers videosignal vmnc y4m yadif
GST_PLUGINS_BAD_CFLAGS = -I$(top_srcdir)/gst-libs -I$(top_builddir)/gst-libs
GST_PLUGINS_BAD_CXXFLAGS = -I$(top_srcdir)/gst-libs -I$(top_builddir)/gst-libs
GST_PLUGINS_BAD_OBJCFLAGS = -I$(top_srcdir)/gst-libs -I$(top_builddir)/gst-libs
GST_PLUGINS_BASE_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_PLUGINS_BASE_DIR = /video_server/ridgerun_plugins/gst-plugins-base/gst:/video_server/ridgerun_plugins/gst-plugins-base/sys:/video_server/ridgerun_plugins/gst-plugins-base/ext
GST_PLUGINS_BASE_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/allocators/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/app/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/audio/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/fft/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/pbutils/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/riff/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/rtp/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/rtsp/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/sdp/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/tag/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/video/.libs -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/gl/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GST_PLUGINS_DIR = /video_server/ridgerun_plugins/gstreamer
GST_PLUGINS_GOOD_CFLAGS = 
GST_PLUGINS_GOOD_DIR = 
GST_PLUGINS_GOOD_LIBS = 
GST_PLUGINS_LIBAV_CFLAGS = 
GST_PLUGINS_LIBAV_DIR = 
GST_PLUGINS_LIBAV_LIBS = 
GST_PLUGINS_NONPORTED = 
GST_PLUGINS_SELECTED =  accurip adpcmdec adpcmenc aiff videoframe_audiolevel asfmux audiobuffersplit audiofxbad audiolatency audiomixmatrix compositor audiovisualizers autoconvert bayer camerabin2 coloreffects debugutils dvbsuboverlay dvdspu faceoverlay festival fieldanalysis freeverb frei0r gaudieffects geometrictransform gdp id3tag inter interlace ivfparse ivtc jp2kdecimator jpegformat librfb midi mpegdemux mpegtsdemux mpegtsmux mpegpsmux mxf netsim onvif pcapparse pnm proxy rawparse removesilence sdp segmentclip siren smooth speed subenc stereo timecode videofilters videoparsers videosignal vmnc y4m yadif
GST_PLUGINS_UGLY_CFLAGS = 
GST_PLUGINS_UGLY_DIR = 
GST_PLUGINS_UGLY_LIBS = 
GST_PLUGIN_LDFLAGS = -module -avoid-version -no-undefined -Wl,-Bsymbolic-functions
GST_PREFIX = 
GST_REVISION = 0
GST_SDP_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_SDP_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/sdp/.libs -lgstsdp-1.0 -lglib-2.0
GST_TOOLS_DIR = /video_server/ridgerun_plugins/gstreamer/tools
GST_VIDEO_CFLAGS = -pthread -I/video_server/ridgerun_plugins/gst-plugins-base/gst-libs -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/video_server/ridgerun_plugins/gstreamer -I/video_server/ridgerun_plugins/gstreamer/libs -I/usr/include/orc-0.4 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include
GST_VIDEO_LIBS = -L/video_server/ridgerun_plugins/gst-plugins-base/gst-libs/gst/video/.libs -L/video_server/ridgerun_plugins/gstreamer/libs/gst/base/.libs -L/video_server/ridgerun_plugins/gstreamer/gst/.libs -lgstvideo-1.0 -lgstbase-1.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0
GTK3_CFLAGS = 
GTK3_LIBS = 
GTKDOC_CHECK = /usr/bin/gtkdoc-check
GTKDOC_DEPS_CFLAGS = 
GTKDOC_DEPS_LIBS = 
GTKDOC_MKPDF = /usr/bin/gtkdoc-mkpdf
GTKDOC_REBASE = /usr/bin/gtkdoc-rebase
GTK_BASE_DIR = 
GTK_CFLAGS = -pthread -I/usr/include/gtk-2.0 -I/usr/lib/aarch64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libpng16
GTK_LIBS = -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
GTK_VERSION = 
G_UDEV_CFLAGS = 
G_UDEV_LIBS = 
HAVE_CXX = yes
HAVE_DIRECT3D = no
HAVE_DIRECTSOUND = no
HAVE_DTS = no
HAVE_FAAC = yes
HAVE_FAAD = yes
HAVE_FLITE = no
HAVE_GSM = no
HAVE_GTK3 = no
HAVE_OPENJPEG = no
HAVE_SRTP = no
HAVE_WASAPI = no
HAVE_WILDMIDI = no
HAVE_WINKS = no
HAVE_WINSCREENCAP = no
HAVE_X11 = yes
HTML_DIR = ${datadir}/gtk-doc/html
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
INTLLIBS = 
INTL_MACOSX_LIBS = 
INTROSPECTION_CFLAGS = 
INTROSPECTION_COMPILER = 
INTROSPECTION_GENERATE = 
INTROSPECTION_GIRDIR = 
INTROSPECTION_INIT = 
INTROSPECTION_LIBS = 
INTROSPECTION_MAKEFILE = 
INTROSPECTION_SCANNER = 
INTROSPECTION_TYPELIBDIR = 
KATE_CFLAGS = 
KATE_LIBS = 
KMS_DRM_CFLAGS = -I/usr/include/libdrm
KMS_DRM_LIBS = -ldrm
LCMS2_CFLAGS = 
LCMS2_LIBS = 
LD = /usr/bin/ld
LDFLAGS = 
LIBDC1394_CFLAGS = 
LIBDC1394_LIBS = 
LIBDE265_CFLAGS = 
LIBDE265_LIBS = 
LIBDIR = /usr/local/lib
LIBGCRYPT_CFLAGS = 
LIBGCRYPT_CONFIG = no
LIBGCRYPT_LIBS = 
LIBICONV = -liconv
LIBINTL = 
LIBM = -lm
LIBMFX_CFLAGS = 
LIBMFX_LIBS = 
LIBMMS_CFLAGS = 
LIBMMS_LIBS = 
LIBOBJS = 
LIBS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBUDEV_CFLAGS = 
LIBUDEV_LIBS = 
LIBUSB_CFLAGS = 
LIBUSB_LIBS = 
LIBVA_DRM_CFLAGS = 
LIBVA_DRM_LIBS = 
LIBXML2_CFLAGS = -I/usr/include/libxml2
LIBXML2_LIBS = -lxml2
LILV_CFLAGS = 
LILV_LIBS = 
LIPO = 
LN_S = ln -s
LOCALEDIR = /usr/local/share/locale
LRDF_CFLAGS = 
LRDF_LIBS = 
LTLIBICONV = -liconv
LTLIBINTL = 
LTLIBOBJS = 
LT_SYS_LIBRARY_PATH = 
MAINT = 
MAKEINFO = ${SHELL} /video_server/ridgerun_plugins/gst-plugins-bad/missing makeinfo
MANIFEST_TOOL = :
MJPEG_CFLAGS = 
MJPEG_LIBS = 
MKDIR_P = /bin/mkdir -p
MODPLUG_CFLAGS = 
MODPLUG_LIBS = 
MPEG2ENC_CFLAGS = 
MPEG2ENC_LIBS = 
MPLEX_CFLAGS = 
MPLEX_LDFLAGS = 
MPLEX_LIBS = 
MSDK_CFLAGS = -I/opt/intel/media/include
MSDK_LIBS = -L/opt/intel/media/lib/lin_x64 -lmfx -ldl
MSGFMT = /usr/bin/msgfmt
MSGFMT_015 = /usr/bin/msgfmt
MSGMERGE = /usr/bin/msgmerge
MUSEPACK_LIBS = 
NEON_CFLAGS = 
NEON_LIBS = 
NETTLE_CFLAGS = 
NETTLE_LIBS = 
NICE_CFLAGS = 
NICE_LIBS = 
NM = /usr/bin/nm -B
NMEDIT = 
NVENCODE_CFLAGS = 
NVENCODE_LIBS =  -lnvidia-encode
OBJC = gcc
OBJCDEPMODE = depmode=gcc3
OBJCFLAGS = 
OBJDUMP = objdump
OBJEXT = o
OFA_CFLAGS = 
OFA_LIBS = 
OPENAL_CFLAGS = 
OPENAL_LIBS = 
OPENCV_3_CFLAGS = 
OPENCV_3_LIBS = 
OPENCV_CFLAGS = 
OPENCV_LIBS = 
OPENCV_PREFIX = 
OPENEXR_CFLAGS = 
OPENEXR_LIBS = 
OPENH264_CFLAGS = 
OPENH264_LIBS = 
OPENJPEG_CFLAGS = 
OPENJPEG_LIBS = 
OPENMPT_CFLAGS = 
OPENMPT_LIBS = 
OPENNI2_CFLAGS = 
OPENNI2_LIBS = 
OPENSSL_CFLAGS = 
OPENSSL_LIBS = 
OPUS_CFLAGS = 
OPUS_LIBS = 
ORCC = /usr/bin/orcc
ORCC_FLAGS = --compat 0.4.17
ORC_CFLAGS = -I/usr/include/orc-0.4
ORC_LIBS = -lorc-0.4
OTOOL = 
OTOOL64 = 
PACKAGE = gst-plugins-bad
PACKAGE_BUGREPORT = http://bugzilla.gnome.org/enter_bug.cgi?product=GStreamer
PACKAGE_NAME = GStreamer Bad Plug-ins
PACKAGE_STRING = GStreamer Bad Plug-ins 1.14.5
PACKAGE_TARNAME = gst-plugins-bad
PACKAGE_URL = 
PACKAGE_VERSION = 1.14.5
PACKAGE_VERSION_MAJOR = 1
PACKAGE_VERSION_MICRO = 5
PACKAGE_VERSION_MINOR = 14
PACKAGE_VERSION_NANO = 0
PACKAGE_VERSION_RELEASE = 1
PATH_SEPARATOR = :
PKG_CONFIG = /usr/bin/pkg-config
PLUGINDIR = /usr/local/lib/gstreamer-1.0
POSUB = 
PROFILE_CFLAGS = -g
PTHREAD_CC = gcc
PTHREAD_CFLAGS = -pthread
PTHREAD_LIBS = 
PYTHON = /usr/bin/python
PYTHON_EXEC_PREFIX = ${exec_prefix}
PYTHON_PLATFORM = linux2
PYTHON_PREFIX = ${prefix}
PYTHON_VERSION = 2.7
RANLIB = ranlib
RSVG_CFLAGS = 
RSVG_LIBS = 
RTMP_CFLAGS = 
RTMP_LIBS = 
SBC_CFLAGS = 
SBC_LIBS = 
SED = /bin/sed
SET_MAKE = 
SHELL = /bin/bash
SHM_LIBS = -lrt
SNDFILE_CFLAGS = 
SNDFILE_LIBS = 
SOUNDTOUCH_CFLAGS = 
SOUNDTOUCH_LIBS = 
SPANDSP_CFLAGS = 
SPANDSP_LIBS = 
SPC_LIBS = 
SRTP_CFLAGS = 
SRTP_LIBS = 
SRT_CFLAGS = 
SRT_LIBS = 
SSH2_CFLAGS = 
SSH2_LIBS = 
STRIP = strip
TELETEXTDEC_CFLAGS = 
TELETEXTDEC_LIBS = 
TIGER_CFLAGS = 
TIGER_LIBS = 
TTML_CFLAGS = -I/usr/include/libxml2 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/aarch64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libpng16
TTML_LIBS = -lxml2 -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lcairo
USE_NLS = no
VALGRIND_CFLAGS = 
VALGRIND_LIBS = 
VALGRIND_PATH = no
VDPAU_CFLAGS = -I/usr/include/nvidia
VDPAU_LIBS = -L/usr/lib/nvidia -lvdpau
VERSION = 1.14.5
VISIBILITY_CFLAGS = -fvisibility=hidden
VISIBILITY_CXXFLAGS = -fvisibility=hidden
VOAACENC_CFLAGS = 
VOAACENC_LIBS = 
VOAMRWBENC_CFLAGS = 
VOAMRWBENC_LIBS = 
VULKAN_LIBS = 
VULKAN_WAYLAND_CFLAGS = 
VULKAN_WAYLAND_LIBS = -lwayland-client
WARNING_CFLAGS =  -Wall -Wdeclaration-after-statement -Wvla -Wpointer-arith -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wwrite-strings -Wformat-security -Wold-style-definition -Winit-self -Wmissing-include-dirs -Waddress -Wno-multichar -Wnested-externs
WARNING_CXXFLAGS =  -Wall -Wmissing-declarations -Wredundant-decls -Wwrite-strings -Wformat-nonliteral -Wformat-security -Winit-self -Wmissing-include-dirs -Waddress -Wno-multichar
WARNING_OBJCFLAGS = 
WASAPI_LIBS = 
WAYLAND_CFLAGS = 
WAYLAND_LIBS = 
WAYLAND_PROTOCOLS_DATADIR = 
WEBP_CFLAGS = 
WEBP_LIBS = 
WEBRTCDSP_CFLAGS = 
WEBRTCDSP_LIBS = 
WGET = /usr/bin/wget
WILDMIDI_CFLAGS = 
WILDMIDI_LIBS = 
WINKS_LIBS = 
WINSCREENCAP_LIBS = 
WINSOCK2_LIBS = 
X11_CFLAGS = 
X11_LIBS = -lX11
X265_CFLAGS = 
X265_LIBS = 
XCB_CFLAGS = 
XCB_LIBS = -lxcb
XGETTEXT = /usr/bin/xgettext
XGETTEXT_015 = /usr/bin/xgettext
XGETTEXT_EXTRA_OPTIONS = 
ZBAR_CFLAGS = 
ZBAR_LIBS = 
abs_builddir = /video_server/ridgerun_plugins/gst-plugins-bad/sys/dshowvideosink
abs_srcdir = /video_server/ridgerun_plugins/gst-plugins-bad/sys/dshowvideosink
abs_top_builddir = /video_server/ridgerun_plugins/gst-plugins-bad
abs_top_srcdir = /video_server/ridgerun_plugins/gst-plugins-bad
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_CXX = g++
ac_ct_DUMPBIN = 
ac_ct_OBJC = gcc
am__include = include
am__leading_dot = .
am__quote = 
am__tar = tar --format=ustar -chf - "$$tardir"
am__untar = tar -xf -
ax_pthread_config = 
bindir = ${exec_prefix}/bin
build = aarch64-unknown-linux-gnu
build_alias = 
build_cpu = aarch64
build_os = linux-gnu
build_vendor = unknown
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
dvidir = ${docdir}
exec_prefix = ${prefix}
host = aarch64-unknown-linux-gnu
host_alias = 
host_cpu = aarch64
host_os = linux-gnu
host_vendor = unknown
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /video_server/ridgerun_plugins/gst-plugins-bad/install-sh
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mkdir_p = /bin/mkdir -p
oldincludedir = /usr/include
pdfdir = ${docdir}
pkgpyexecdir = ${pyexecdir}/gst-plugins-bad
pkgpythondir = ${pythondir}/gst-plugins-bad
plugindir = $(libdir)/gstreamer-1.0
prefix = /usr/local
program_transform_name = s,x,x,
psdir = ${docdir}
pyexecdir = ${exec_prefix}/lib/python2.7/dist-packages
pythondir = ${prefix}/lib/python2.7/dist-packages
runstatedir = ${localstatedir}/run
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = ${prefix}/etc
target = aarch64-unknown-linux-gnu
target_alias = 
target_cpu = aarch64
target_os = linux-gnu
target_vendor = unknown
top_build_prefix = ../../
top_builddir = ../..
top_srcdir = ../..
wayland_scanner = /usr/bin/wayland-scanner
EXTRA_DIST = \
			 dshowvideofakesrc.cpp \
			 dshowvideofakesrc.h \
			 dshowvideosink.cpp \
			 dshowvideosink.h

all: all-am

.SUFFIXES:
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu sys/dshowvideosink/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu sys/dshowvideosink/Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs
tags TAGS:

ctags CTAGS:

cscope cscopelist:


distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile
installdirs:
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:

clean-generic:

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool mostlyclean-am

distclean: distclean-am
	-rm -f Makefile
distclean-am: clean-am distclean-generic

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am:

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am:

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man:

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-generic mostlyclean-libtool

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am:

.MAKE: install-am install-strip

.PHONY: all all-am check check-am clean clean-generic clean-libtool \
	cscopelist-am ctags-am distclean distclean-generic \
	distclean-libtool distdir dvi dvi-am html html-am info info-am \
	install install-am install-data install-data-am install-dvi \
	install-dvi-am install-exec install-exec-am install-html \
	install-html-am install-info install-info-am install-man \
	install-pdf install-pdf-am install-ps install-ps-am \
	install-strip installcheck installcheck-am installdirs \
	maintainer-clean maintainer-clean-generic mostlyclean \
	mostlyclean-generic mostlyclean-libtool pdf pdf-am ps ps-am \
	tags-am uninstall uninstall-am

.PRECIOUS: Makefile


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
