####
# Copyright 1996 Microware Systems Corporation
# All Rights Reserved
#
# Use and/or distribution subject to terms and conditions of applicable
# Microware license agreement(s).
#

MWOS    = ../../../../../..
TRGTS   = libs
MAKENAME    = ./makefile

LIBS        =   # predefining this prevents lib from linking to itself
HARDLIBS    =   # predefining this prevents lib from linking to itself
include ../../../maui.com

LIB         =   $(MWOS_LIBDIR)/maui_jpg.l
ILIB        =   $(MWOS_ILIBDIR)/maui_jpg.il
MTLIB       =   $(MWOS_LIBDIR)/mt_maui_jpg.l
MTILIB      =   $(MWOS_ILIBDIR)/mt_maui_jpg.il
OBJS        =   $(ILIB) $(LIB) 
if $(OS) != OSK  # don't build thread version for 68k for OS9
OBJS        +=  $(MTILIB) $(MTLIB)
endif

H_FILES     = $(SDIR)/jpgcom.h
DEPENDS     = $(H_FILES) $(MAKENAME)

J_DEFS = -v=../CORELIB

JPEG_LIB = ../CORELIB/$(RDIR)

JPEG_LIB_IL = ../CORELIB/$(IDIR)

NAMES    =  \
        cjpgctx   jpgcom \
        jscolnum  jsscale  \
        djpgctx   jpgerr   \
        jsdct     jssize  \
        gjpgctx   jpginit  \
        jsdither  jssource \
        jabort    jpgterm  \
        jsocolor  jdecomp  \
        jrdhead   jsqual   \
        jsbuf     jsdst \
        jsquant   thread
RELS    =   $(NAMES:%=$(RDIR)/%.r)
IRELS   =   $(NAMES:%=$(IDIR)/%.i)
MTRELS  =   $(NAMES:%=$(RDIR)/MT/%.r)
MTIRELS =   $(NAMES:%=$(IDIR)/MT/%.i)


libs:  $(IDIR)/MT $(RDIR)/MT $(JPEG_LIB) $(JPEG_LIB_IL) $(OBJS)

$(LIB):  $(RELS) $(JPEG_LIB)/jpeg.l
    $(CODO) $@
    $(MKLIB) $(RELS) $(JPEG_LIB)/jpeg.l -o=$@
$(ILIB):  $(IRELS) $(JPEG_LIB_IL)/jpeg.il
    $(CODO) $@
    $(MKILIB) $(IRELS) $(JPEG_LIB_IL)/jpeg.il -fd=$@
$(MTLIB):  $(MTRELS) $(JPEG_LIB)/MT/mt_jpeg.l
    $(CODO) $@
    $(MKLIB) $(MTRELS) $(JPEG_LIB)/MT/mt_jpeg.l -o=$@
$(MTILIB):  $(MTIRELS) $(JPEG_LIB_IL)/MT/mt_jpeg.il
    $(CODO) $@
    $(MKILIB) $(MTIRELS) $(JPEG_LIB_IL)/MT/mt_jpeg.il -fd=$@

#################################################

for NAME in $(NAMES)
$(IDIR)/$(NAME).i : $*.c $(DEPENDS)
    $(COMPILE) $(J_DEFS) $*.c 
$(RDIR)/$(NAME).r : $*.c $(DEPENDS)
    $(ILINK) $(J_DEFS) $*.c 
$(IDIR)/MT/$(NAME).i : $*.c $(DEPENDS)
    $(COMPILE_MT) $(J_DEFS) $*.c 
$(RDIR)/MT/$(NAME).r : $*.c $(DEPENDS)
    $(ILINK_MT) $(J_DEFS) $*.c 
_clean:  
    -$(DEL) $(RDIR)/$(NAME).r
    -$(DEL) $(IDIR)/$(NAME).i
    -$(DEL) $(RDIR)/MT/$(NAME).r
    -$(DEL) $(IDIR)/MT/$(NAME).i
endfor

_purge :
for NAME in $(OBJS)
    $(CODO) $(NAME)
    -$(DEL) $(NAME)
endfor

$(IDIR)/MT $(RDIR)/MT $(JPEG_LIB) $(JPEG_LIB_IL): 
    -$(MAKDIR) $@
