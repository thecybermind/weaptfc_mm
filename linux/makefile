MODNAME = weaptfc_mm
SRCFILES = CStrings.cpp main.cpp dll_api.cpp h_export.cpp meta_api.cpp engine_api.cpp sdk_util.cpp


EXTRA_LIBS_LINUX = 
EXTRA_LIBS_WIN32 = 
EXTRA_LIBDIRS_LINUX = 
EXTRA_LIBDIRS_WIN32 = 

EXTRA_INCLUDEDIRS = 

EXTRA_FLAGS = -Dstrcmpi=strcasecmp

SRCDIR=..
SDKSRC=$(SRCDIR)
METADIR=$(SRCDIR)/metamod

OBJDIR_LINUX=obj.linux
OBJDIR_WIN32=obj.win32

ifdef comspec
	OS=WIN32
else
	OS=LINUX
endif

//OS=WIN32

CC_LINUX=gcc
ifeq "$(OS)" "WIN32"
	CC_WIN32=gcc
	LD_WINDLL=dllwrap
	DEFAULT=win32
	CLEAN=clean_win32
else
	CC_WIN32=/usr/local/cross-tools/i386-mingw32msvc/bin/gcc
	LD_WINDLL=/usr/local/cross-tools/bin/i386-mingw32msvc-dllwrap
	DEFAULT=linux win32
	CLEAN=clean_both
endif



LIBFILE_LINUX = $(MODNAME)_i386.so
LIBFILE_WIN32 = $(MODNAME).dll
#TARGET_LINUX = $(OBJDIR_LINUX)/$(LIBFILE_LINUX)
TARGET_LINUX = ../$(LIBFILE_LINUX)
TARGET_WIN32 = $(OBJDIR_WIN32)/$(LIBFILE_WIN32)

FILES_ALL = *.cpp *.h [A-Z]* *.rc



OBJ_LINUX := $(SRCFILES:%.cpp=$(OBJDIR_LINUX)/%.o)
OBJ_WIN32 := $(SRCFILES:%.cpp=$(OBJDIR_WIN32)/%.o)

CCOPT = -march=i586 -O6 -ffast-math -funroll-loops \
	-fomit-frame-pointer -fexpensive-optimizations -malign-loops=2 \
	-malign-jumps=2 -malign-functions=2 -s

INCLUDEDIRS= -I$(SRCDIR) -I$(METADIR) -I$(SDKSRC)/engine -I$(SDKSRC)/common -I$(SDKSRC)/pm_shared -I$(SDKSRC)/dlls -I$(SDKSRC) $(EXTRA_INCLUDEDIRS)
CFLAGS=-Wall -Wno-unknown-pragmas
ODEF = -DOPT_TYPE=\"optimized\"
CFLAGS:=$(CCOPT) $(CFLAGS) $(ODEF) $(EXTRA_FLAGS)

DO_CC_LINUX=$(CC_LINUX) $(CFLAGS) -fPIC $(INCLUDEDIRS) -o $@ -c $<
DO_CC_WIN32=$(CC_WIN32) $(CFLAGS) $(INCLUDEDIRS) -o $@ -c $<
LINK_LINUX=$(CC_LINUX) $(CFLAGS) -shared -ldl -lm $(OBJ_LINUX) $(EXTRA_LIBDIRS_LINUX) $(EXTRA_LIBS_LINUX) -o $@
LINK_WIN32=$(LD_WINDLL) -mwindows --add-stdcall-alias $(OBJ_WIN32) $(EXTRA_LIBDIRS_WIN32) $(EXTRA_LIBS_WIN32) -o $@

$(OBJDIR_LINUX)/%.o: $(SRCDIR)/%.cpp
	$(DO_CC_LINUX)

$(OBJDIR_WIN32)/%.o: $(SRCDIR)/%.cpp
	$(DO_CC_WIN32)

default: $(DEFAULT)

$(TARGET_LINUX): $(OBJDIR_LINUX) $(OBJ_LINUX)
	$(LINK_LINUX)

$(TARGET_WIN32): $(OBJDIR_WIN32) $(OBJ_WIN32)
	$(LINK_WIN32)

$(OBJDIR_LINUX):
	mkdir $@

$(OBJDIR_WIN32):
	mkdir $@

win32: $(TARGET_WIN32)

linux: $(TARGET_LINUX)

clean: $(CLEAN)

clean_both:
	-rm -f $(OBJDIR_LINUX)/*
	-rm -f $(OBJDIR_WIN32)/*

clean_win32:
	del /q $(OBJDIR_WIN32)
