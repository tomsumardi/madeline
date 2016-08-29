include $(top_srcdir)/common.mk
AM_CFLAGS = -I../thdmgmt -I../bkgrnd -I../logging -I../pfilter

bin_PROGRAMS = ipps
ipps_SOURCES = main.cpp main.h test.cpp test.h
ipps_LDADD = $(top_srcdir)/src/thdmgmt/libthdmgmt.a \
             $(top_srcdir)/src/bkgrnd/libbkgrnd.a \
             $(top_srcdir)/src/logging/liblogging.a \
             $(top_srcdir)/src/pfilter/libpfilter.a
