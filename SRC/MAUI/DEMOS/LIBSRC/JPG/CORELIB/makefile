* Created by Y.Persion
# 
# This makefile creates JPEG API library maui_jpg.l
# 

MWOS    = ../../../../../..
TRGTS   = libs
MAKENAME    = ./makefile


LIBS        =   # predefining this prevents lib from linking to itself
HARDLIBS    =   # predefining this prevents lib from linking to itself
include ../../../maui.com

LIB         =   $(RDIR)/jpeg.l
ILIB        =   $(IDIR)/jpeg.il
MTLIB       =   $(RDIR)/MT/mt_jpeg.l
MTILIB      =   $(IDIR)/MT/mt_jpeg.il
OBJS        =   $(ILIB) $(LIB) 
if $(OS) != OSK  # don't build thread version for 68k for OS9
OBJS        +=  $(MTILIB) $(MTLIB)
endif

NAMES = \
       jdatasrc \
       jdatadst \
       jdapistd \
       jdinput \
       jdmainct \
       jdmarker \
       jdmaster \
       jdmerge \
       jdpostct \
       jdtrans \
       jerror \
       jidctflt \
       jidctfst \
       jidctint \
       jidctred \
       jmemmaui \
       jmemmgr \
       cdjpeg \
       rdcolmap \
       jdapimin \
       jutils \
       jdphuff \
       jcomapi \
       jquant1 \
       jquant2 \
       jdcolor \
       jdcoefct \
       jdhuff \
       jddctmgr \
       jdsample

IRELS = $(NAMES:%=$(IDIR)/%.i)
RELS = $(NAMES:%=$(RDIR)/%.r)
MTIRELS = $(NAMES:%=$(IDIR)/MT/%.i)
MTRELS = $(NAMES:%=$(RDIR)/MT/%.r)

H_FILES = jversion.h \
        cderror.h \
        jerror.h \
        jmorecfg.h \
        jpeglib.h \
        jconfig.h \
        jinclude.h \
        cdjpeg.h

DEPENDS = $(H_FILES) $(MAKENAME)

libs:  $(IDIR)/MT $(RDIR)/MT $(OBJS)
    $(COMMENT)

$(ILIB):  $(IRELS) $(DEPENDS)
    $(CODO) $@
    $(MERGE) $(IRELS) >$@
$(LIB):  $(RELS) $(DEPENDS)
    $(CODO) $@
    $(MERGE) $(RELS) >$@
$(MTILIB):  $(MTIRELS) $(DEPENDS)
    $(CODO) $@
    $(MERGE) $(MTIRELS) >$@
$(MTLIB):  $(MTRELS) $(DEPENDS)
    $(CODO) $@
    $(MERGE) $(MTRELS) >$@

if ($(CPU) == SH4 || $(CPU) == SH3) # working around assembly optimizer bug in SH-[34] - TOMS
ILINK += -xao
endif

for NAME in $(NAMES)
$(IDIR)/$(NAME).i : $*.c $(DEPENDS)
    $(COMPILE) $*.c 
$(RDIR)/$(NAME).r : $*.c $(DEPENDS)
    $(ILINK) $*.c
$(IDIR)/MT/$(NAME).i : $*.c $(DEPENDS)
    $(COMPILE_MT) $*.c 
$(RDIR)/MT/$(NAME).r : $*.c $(DEPENDS)
    $(ILINK_MT) $*.c
_clean:  
    -$(DEL) $(RDIR)/$(NAME).r
    -$(DEL) $(IDIR)/$(NAME).i
    -$(DEL) $(RDIR)/MT/$(NAME).r
    -$(DEL) $(IDIR)/MT/$(NAME).i
endfor

_purge:
for NAME in $(OBJS)
    $(CODO) $(NAME)
    -$(DEL) $(NAME)
endfor


$(IDIR)/MT $(RDIR)/MT:
    -$(MAKDIR) $@

