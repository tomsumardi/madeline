include $(top_srcdir)/common.mk
AM_CFLAGS += -I../thdmgmt -I../ppp -I../logging -I../pfilter

bin_PROGRAMS = ppp
ppp_SOURCES = main.cpp main.h test.cpp test.h
ppp_LDADD = \
		     $(top_srcdir)/src/thdmgmt/libthdmgmt.a \
             $(top_srcdir)/src/ppp/libppp.a \
             $(top_srcdir)/src/pfilter/libpfilter.a \
             $(top_srcdir)/../etc/cpp/mutil/src/libmutil.a \
             $(top_srcdir)/../etc/cpp/rxtxal/src/librxtxal.a
