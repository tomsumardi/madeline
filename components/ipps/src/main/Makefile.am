AM_CFLAGS = -I../thdmgmt

bin_PROGRAMS = ipps
ipps_SOURCES = main.c main.h test.c test.h
ipps_LDADD = $(top_srcdir)/src/thdmgmt/libthdmgmt.a \
             $(top_srcdir)/src/bkgrnd/libbkgrnd.a \
             $(top_srcdir)/src/logging/liblogging.a \
             $(top_srcdir)/src/pfilter/libpfilter.a
