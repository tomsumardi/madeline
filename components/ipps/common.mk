#-------------------------------------------------------------------------
# Target Architecture
#-------------------------------------------------------------------------
HOST_OS         = linux
TARGET_OS       = linux
CPU_ARCH	    = x86_64

#ifndef CPU_ARCH
#ifeq ($(findstring 86_64,$(shell uname -m)),86_64)
#CPU_ARCH = x86_64
#endif
#ifeq ($(findstring i686,$(shell uname -m)),i686)
#CPU_ARCH = i686
#endif
#endif

#-------------------------------------------------------------------------
# Compiler/Linker Options
#-------------------------------------------------------------------------
#default values
CFLAGS_DEFS     =
CFLAGS_INC  	=
CFLAGS_OPTS     =
LDFLAGS_OPTS	=
LDFLAGS_DEFS	=
LDFLAGS_INC  	=

#C init
CFLAGS_DEFS    += -D_GNU_SOURCE
CFLAGS_OPTS    += -Wall -Wextra -Wno-unused-parameter -Wno-deprecated-declarations 
#C and C++ release vs debug build
#ifdef RELEASE
#	CFLAGS_OPTS    += -O3 -fdata-sections -ffunction-sections `pkg-config --cflags glib-2.0`
#	LDFLAGS_OPTS   += -Wl,--gc-sections
#else
	CFLAGS_OPTS    += -g -Wall -Wformat-security -fstack-protector-all `pkg-config --cflags glib-2.0`
	CFLAGS_DEFS    += -DMDLN_DEBUG
#endif
#Passing Version to MDLN
#CFLAGS_OPTS        += -DMDLN_VER='"$(MDLNDIR_VER)"' -DMDLN_REL='"$(MDLNDIR_REL)"'


#C++ init
CFLAGS_OPTS    += -std=gnu++11 -Wno-literal-suffixs

# C library
#-------------------------------------------------------------------------
# glib2
#-------------------------------------------------------------------------
CFLAGS_INC     += $(glib2_CFLAGS)
LDFLAGS_INC    += $(xml_LIBS) $(glib2_LIBS)

# C++ library
#-------------------------------------------------------------------------
# C++
#-------------------------------------------------------------------------
LDFLAGS_INC     += -lstdc++ -lm -lboost_regex -lboost_system

#-------------------------------------------------------------------------
# define compiler and linker flags
#-------------------------------------------------------------------------

AM_CFLAGS     = $(CFLAGS_OPTS) $(CFLAGS_DEFS) $(CFLAGS_INC)
AM_CXXFLAGS   = $(CFLAGS_OPTS) $(CFLAGS_DEFS) $(CFLAGS_INC)
AM_LDFLAGS    = $(LDFLAGS_OPTS) $(LDFLAGS_DEFS) $(LDFLAGS_INC)

