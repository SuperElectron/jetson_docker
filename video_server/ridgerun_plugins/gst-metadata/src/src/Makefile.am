plugin_LTLIBRARIES = libgstmeta.la

libgstmeta_la_SOURCES = \
    gstmeta.c           \
    gstmetasrc.c        \
    gstmetasink.c       \
    gstmisbparser.c     \
    klv.c               \
    klvutils.c          \
    lds.c               \
    misbparser.c        \
    st0601dictionary.c  \
    st0604dictionary.c

libgstmeta_la_CFLAGS = $(GST_CFLAGS)
libgstmeta_la_LIBADD = $(GST_LIBS)
libgstmeta_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstmeta_la_LIBTOOLFLAGS = --tag=disable-static

noinst_HEADERS =        \
    gstmetasrc.h        \
    gstmetasink.h       \
    gstmisbparser.h     \
    klv.h               \
    klvutils.h          \
    lds.h               \
    misbparser.h        \
    misbstd.h           \
    st0601dictionary.h  \
    st0604dictionary.h
