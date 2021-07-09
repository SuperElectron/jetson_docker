# Note: plugindir is set in configure

plugin_LTLIBRARIES = libgstrtspsink.la

# sources used to compile this plug-in
libgstrtspsink_la_SOURCES = gstrtspsink.c gstplugin.c rtspmediafactory.c gstrtspsinkpad.c

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstrtspsink_la_CFLAGS = $(GST_CFLAGS)
libgstrtspsink_la_LIBADD = $(GST_LIBS)
libgstrtspsink_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstrtspsink_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstrtspsink.h rtspmediafactory.h gstrtspsinkpad.h
