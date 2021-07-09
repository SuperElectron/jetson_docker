/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* poll doesn't work on devices */
/* #undef BROKEN_POLL */

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
/* #undef ENABLE_NLS */

/* Subunit protocol result output */
#define ENABLE_SUBUNIT 0

/* gettext package name */
#define GETTEXT_PACKAGE "NULL"

/* The GIO library directory. */
#define GIO_LIBDIR "/usr/lib/aarch64-linux-gnu"

/* The GIO modules directory. */
#define GIO_MODULE_DIR "/usr/lib/aarch64-linux-gnu/gio/modules"

/* The GIO install prefix. */
#define GIO_PREFIX "/usr"

/* GStreamer API Version */
#define GST_API_VERSION "1.0"

/* location of the installed gst-completion-helper */
#define GST_COMPLETION_HELPER_INSTALLED "/usr/local/libexec/gstreamer-1.0/gst-completion-helper"

/* data dir */
#define GST_DATADIR "/usr/local/share"

/* Define if tracing subsystem hooks is disabled */
/* #undef GST_DISABLE_GST_TRACER_HOOKS */

/* Define if option parsing is disabled */
/* #undef GST_DISABLE_OPTION_PARSING */

/* Define if pipeline parsing code is disabled */
/* #undef GST_DISABLE_PARSE */

/* Define if extra runtime checks should be enabled */
/* #undef GST_ENABLE_EXTRA_CHECKS */

/* Extra platform specific plugin suffix */
/* #undef GST_EXTRA_MODULE_SUFFIX */

/* macro to use to show function name */
#define GST_FUNCTION __PRETTY_FUNCTION__

/* Defined if gcov is enabled to force a rebuild due to config.h changing */
/* #undef GST_GCOV_ENABLED */

/* Defined when registry scanning through fork is unsafe */
/* #undef GST_HAVE_UNSAFE_FORK */

/* Default errorlevel to use */
#define GST_LEVEL_DEFAULT GST_LEVEL_NONE

/* GStreamer license */
#define GST_LICENSE "LGPL"

/* package name in plugins */
#define GST_PACKAGE_NAME "GStreamer source release"

/* package origin */
#define GST_PACKAGE_ORIGIN "Unknown package origin"

/* GStreamer package release date/time for plugins as YYYY-MM-DD */
#define GST_PACKAGE_RELEASE_DATETIME "2019-05-29"

/* location of the installed gst-plugin-scanner */
#define GST_PLUGIN_SCANNER_INSTALLED "/usr/local/libexec/gstreamer-1.0/gst-plugin-scanner"

/* libexecdir path component, used to find plugin-scanner on relocatable
   builds on windows */
#define GST_PLUGIN_SCANNER_SUBDIR "libexec"

/* location of the installed gst-ptp-helper */
#define GST_PTP_HELPER_INSTALLED "/usr/local/libexec/gstreamer-1.0/gst-ptp-helper"

/* Define to 1 if you have the `alarm' function. */
#define HAVE_ALARM 1

/* Have backtrace */
#define HAVE_BACKTRACE 1

/* Define to 1 if you have the MacOS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the MacOS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Have clock_gettime */
#define HAVE_CLOCK_GETTIME 1

/* Define if the target CPU is AARCH64 */
#define HAVE_CPU_AARCH64 1

/* Define if the target CPU is an Alpha */
/* #undef HAVE_CPU_ALPHA */

/* Define if the target CPU is an ARC */
/* #undef HAVE_CPU_ARC */

/* Define if the target CPU is an ARM */
/* #undef HAVE_CPU_ARM */

/* Define if the target CPU is a CRIS */
/* #undef HAVE_CPU_CRIS */

/* Define if the target CPU is a CRISv32 */
/* #undef HAVE_CPU_CRISV32 */

/* Define if the target CPU is a HPPA */
/* #undef HAVE_CPU_HPPA */

/* Define if the target CPU is an x86 */
/* #undef HAVE_CPU_I386 */

/* Define if the target CPU is a IA64 */
/* #undef HAVE_CPU_IA64 */

/* Define if the target CPU is a M68K */
/* #undef HAVE_CPU_M68K */

/* Define if the target CPU is a MIPS */
/* #undef HAVE_CPU_MIPS */

/* Define if the target CPU is a PowerPC */
/* #undef HAVE_CPU_PPC */

/* Define if the target CPU is a 64 bit PowerPC */
/* #undef HAVE_CPU_PPC64 */

/* Define if the target CPU is a S390 */
/* #undef HAVE_CPU_S390 */

/* Define if the target CPU is a SPARC */
/* #undef HAVE_CPU_SPARC */

/* Define if the target CPU is a x86_64 */
/* #undef HAVE_CPU_X86_64 */

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
/* #undef HAVE_DCGETTEXT */

/* Define to 1 if you have the declaration of `alarm', and to 0 if you don't.
   */
#define HAVE_DECL_ALARM 1

/* Define to 1 if you have the declaration of `localtime_r', and to 0 if you
   don't. */
#define HAVE_DECL_LOCALTIME_R 1

/* Define to 1 if you have the declaration of `strdup', and to 0 if you don't.
   */
#define HAVE_DECL_STRDUP 1

/* Define to 1 if you have the declaration of `strsignal', and to 0 if you
   don't. */
#define HAVE_DECL_STRSIGNAL 1

/* Defined if we have dladdr () */
#define HAVE_DLADDR 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* libdw available */
/* #undef HAVE_DW */

/* Define to 1 if you have the <execinfo.h> header file. */
#define HAVE_EXECINFO_H 1

/* Define to 1 if you have the `fgetpos' function. */
#define HAVE_FGETPOS 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
#define HAVE_FSEEKO 1

/* Define to 1 if you have the `fsetpos' function. */
#define HAVE_FSETPOS 1

/* Define to 1 if you have the `ftello' function. */
#define HAVE_FTELLO 1

/* defined if the compiler implements __func__ */
#define HAVE_FUNC 1

/* defined if the compiler implements __FUNCTION__ */
#define HAVE_FUNCTION 1

/* getifaddrs() and AF_LINK is available */
/* #undef HAVE_GETIFADDRS_AF_LINK */

/* Define to 1 if you have the `getline' function. */
#define HAVE_GETLINE 1

/* Define to 1 if you have the `getpagesize' function. */
#define HAVE_GETPAGESIZE 1

/* Define to 1 if you have the `getpid' function. */
#define HAVE_GETPID 1

/* Define to 1 if you have the `getrusage' function. */
#define HAVE_GETRUSAGE 1

/* Define if the GNU gettext() function is already present or preinstalled. */
/* #undef HAVE_GETTEXT */

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Have GMP library */
/* #undef HAVE_GMP */

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* Have GSL library */
/* #undef HAVE_GSL */

/* Define if you have the iconv() function and it works. */
/* #undef HAVE_ICONV */

/* Define to 1 if the system has the type `intmax_t'. */
#define HAVE_INTMAX_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> exists, doesn't clash with <sys/types.h>, and
   declares uintmax_t. */
#define HAVE_INTTYPES_H_WITH_UINTMAX 1

/* Define to 1 if you have the `rt' library (-lrt). */
#define HAVE_LIBRT 1

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if the system has the type long long */
#define HAVE_LONG_LONG 1

/* Define to 1 if the system has the type `long long int'. */
#define HAVE_LONG_LONG_INT 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mkstemp' function. */
#define HAVE_MKSTEMP 1

/* Have a monotonic clock */
#define HAVE_MONOTONIC_CLOCK 1

/* Defined if compiling for OSX */
/* #undef HAVE_OSX */

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Define to 1 if you have the <poll.h> header file. */
#define HAVE_POLL_H 1

/* Have posix timers */
#define HAVE_POSIX_TIMERS 1

/* Define to 1 if you have the `ppoll' function. */
#define HAVE_PPOLL 1

/* defined if the compiler implements __PRETTY_FUNCTION__ */
#define HAVE_PRETTY_FUNCTION 1

/* Define to 1 if you have the <process.h> header file. */
/* #undef HAVE_PROCESS_H */

/* Define to 1 if you have the `pselect' function. */
#define HAVE_PSELECT 1

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 1

/* Have PTHREAD_PRIO_INHERIT. */
#define HAVE_PTHREAD_PRIO_INHERIT 1

/* Have function pthread_setname_np(const char*) */
/* #undef HAVE_PTHREAD_SETNAME_NP_WITHOUT_TID */

/* PTP support available */
#define HAVE_PTP 1

/* Use capabilities for permissions in PTP helper */
/* #undef HAVE_PTP_HELPER_CAPABILITIES */

/* Use setuid-root for permissions in PTP helper */
#define HAVE_PTP_HELPER_SETUID 1

/* PTP helper setuid group */
/* #undef HAVE_PTP_HELPER_SETUID_GROUP */

/* PTP helper setuid user */
/* #undef HAVE_PTP_HELPER_SETUID_USER */

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define if RDTSC is available */
/* #undef HAVE_RDTSC */

/* Define to 1 if you have the `setitimer' function. */
#define HAVE_SETITIMER 1

/* Define to 1 if you have the `sigaction' function. */
#define HAVE_SIGACTION 1

/* SIOCGIFCONF, SIOCGIFFLAGS and SIOCGIFHWADDR is available */
#define HAVE_SIOCGIFCONF_SIOCGIFFLAGS_SIOCGIFHWADDR 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define if <stdint.h> exists, doesn't clash with <sys/types.h>, and declares
   uintmax_t. */
#define HAVE_STDINT_H_WITH_UINTMAX 1

/* Define to 1 if you have the <stdio_ext.h> header file. */
#define HAVE_STDIO_EXT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasestr' function. */
#define HAVE_STRCASESTR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if `it_interval' is a member of `struct itimerspec'. */
#define HAVE_STRUCT_ITIMERSPEC_IT_INTERVAL 1

/* Define to 1 if `it_value' is a member of `struct itimerspec'. */
#define HAVE_STRUCT_ITIMERSPEC_IT_VALUE 1

/* Define to 1 if `tv_nsec' is a member of `struct timespec'. */
#define HAVE_STRUCT_TIMESPEC_TV_NSEC 1

/* Define to 1 if `tv_sec' is a member of `struct timespec'. */
#define HAVE_STRUCT_TIMESPEC_TV_SEC 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/prctl.h> header file. */
#define HAVE_SYS_PRCTL_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/times.h> header file. */
#define HAVE_SYS_TIMES_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#define HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Have tm_gmtoff field in struct tm */
#define HAVE_TM_GMTOFF 1

/* Define to 1 if you have the <ucontext.h> header file. */
#define HAVE_UCONTEXT_H 1

/* Have __uint128_t type */
#define HAVE_UINT128_T 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the system has the type `unsigned long long int'. */
#define HAVE_UNSIGNED_LONG_LONG_INT 1

/* libunwind available */
/* #undef HAVE_UNWIND */

/* Define if valgrind should be used */
/* #undef HAVE_VALGRIND */

/* Define to 1 if you have the <valgrind/valgrind.h> header file. */
/* #undef HAVE_VALGRIND_VALGRIND_H */

/* Defined if compiling for Windows */
/* #undef HAVE_WIN32 */

/* Define to 1 if you have the <winsock2.h> header file. */
/* #undef HAVE_WINSOCK2_H */

/* Define to 1 if you have the `_getpid' function. */
/* #undef HAVE__GETPID */

/* Define to 1 if you have the `_strdup' function. */
/* #undef HAVE__STRDUP */

/* the host CPU */
#define HOST_CPU "aarch64"

/* library dir */
#define LIBDIR "/usr/local/lib"

/* gettext locale dir */
#define LOCALEDIR "/usr/local/share/locale"

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Memory alignment to use */
/* #undef MEMORY_ALIGNMENT */

/* Memory alignment by malloc default */
#define MEMORY_ALIGNMENT_MALLOC 1

/* Memory alignment by pagesize */
/* #undef MEMORY_ALIGNMENT_PAGESIZE */

/* Name of package */
#define PACKAGE "gstreamer"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://bugzilla.gnome.org/enter_bug.cgi?product=GStreamer"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GStreamer"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GStreamer 1.14.5"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gstreamer"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.14.5"

/* directory where plugins are located */
#define PLUGINDIR "/usr/local/lib/gstreamer-1.0"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* The size of `char', as computed by sizeof. */
/* #undef SIZEOF_CHAR */

/* The size of `int', as computed by sizeof. */
/* #undef SIZEOF_INT */

/* The size of `long', as computed by sizeof. */
/* #undef SIZEOF_LONG */

/* The size of `short', as computed by sizeof. */
/* #undef SIZEOF_SHORT */

/* The size of `void*', as computed by sizeof. */
/* #undef SIZEOF_VOIDP */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Need to define the itimerspec structure */
/* #undef STRUCT_ITIMERSPEC_DEFINITION_MISSING */

/* Need to define the timespec structure */
/* #undef STRUCT_TIMESPEC_DEFINITION_MISSING */

/* the target CPU */
#define TARGET_CPU "aarch64"

/* Define if we should poison deallocated memory */
#define USE_POISONING 1

/* Version number of package */
#define VERSION "1.14.5"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* We need at least WinXP SP2 for __stat64 */
/* #undef __MSVCRT_VERSION__ */

/* clockid_t */
/* #undef clockid_t */

/* Define to the widest signed integer type if <stdint.h> and <inttypes.h> do
   not define. */
/* #undef intmax_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* timer_t */
/* #undef timer_t */
