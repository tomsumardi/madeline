include $(top_srcdir)/common.mk
AM_CFLAGS += -I../thdmgmt -I../ipps -I../logging -I../pfilter

bin_PROGRAMS = ipps
ipps_SOURCES = main.cpp main.h test.cpp test.h
ipps_LDADD = \
		     $(top_srcdir)/src/thdmgmt/libthdmgmt.a \
             $(top_srcdir)/src/ipps/libipps.a \
             $(top_srcdir)/src/pfilter/libpfilter.a \
             $(top_srcdir)/../etc/cpp/mutil/src/libmutil.a \
             $(top_srcdir)/../etc/cpp/rxtxal/src/librxtxal.a
