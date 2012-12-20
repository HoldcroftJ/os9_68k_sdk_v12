# Makefile
#*****************************************************************************
#* MAUI demos demolib library makefile
#*****************************************************************************
#* Copyright 1997 by Microware Systems Corporation
#* Reproduced Under License
#*
#* This source code is the proprietary confidential property of
#* Microware Systems Corporation, and is provided to licensee
#* solely for documentation and educational purposes. Reproduction,
#* publication, or distribution in any form to any party other than
#* the licensee is strictly prohibited.
#*****************************************************************************
#* Edition History:
#* #   Date     Comments                                          By
#* --- -------- ------------------------------------------------- ---------
#*   1 07/31/97 creation                                          bat
#*   2 08/24/00 thread support                                    srm
#*****************************************************************************


MWOS        =   ../../../../..
TRGTS       =   libs
MAKENAME    =   makefile

LIBS        =   # predefining this prevents lib from linking to itself
HARDLIBS    =   # predefining this prevents lib from linking to itself
include ../../maui.com

LIB         =   $(MWOS_LIBDIR)/mauidemo.l
ILIB        =   $(MWOS_ILIBDIR)/mauidemo.il
MTLIB       =   $(MWOS_LIBDIR)/mt_mauidemo.l
MTILIB      =   $(MWOS_ILIBDIR)/mt_mauidemo.il
OBJS        =   $(ILIB) $(LIB) 
if $(OS) != OSK  # don't build thread version for 68k for OS9
OBJS        +=  $(MTILIB) $(MTLIB)
endif

HFILES      =   democom.h
NAMES     =   \
                demo \
                demogfx \
                demoinp \
                iffimage \
		bmpimage \
                ucmfont \
                init \
                term \
                thread

RELS        =   $(NAMES:%=$(RDIR)/%.r)
IRELS       =   $(NAMES:%=$(IDIR)/%.i)
MTRELS        =   $(NAMES:%=$(RDIR)/MT/%.r)
MTIRELS       =   $(NAMES:%=$(IDIR)/MT/%.i)

libs:  $(MWOS_LIBDIR) $(MWOS_ILIBDIR) $(IDIR)/MT $(RDIR)/MT $(OBJS)

$(LIB):  $(RELS)
    $(CODO) $@
    $(MKLIB) $(RELS) -o=$@
$(ILIB):  $(IRELS)
    $(CODO) $@
    $(MKILIB) $(IRELS) -fd=$@
$(MTLIB):  $(MTRELS)
    $(CODO) $@
    $(MKLIB) $(MTRELS) -o=$@
$(MTILIB):  $(MTIRELS)
    $(CODO) $@
    $(MKILIB) $(MTIRELS) -fd=$@

for NAME in $(NAMES)
$(RDIR)/$(NAME).r : $*.c $(HFILES)
    $(ILINK)   $*.c
$(IDIR)/$(NAME).i : $*.c $(HFILES)
    $(COMPILE) $*.c
$(RDIR)/MT/$(NAME).r : $*.c $(HFILES)
    $(ILINK_MT)   $*.c
$(IDIR)/MT/$(NAME).i : $*.c $(HFILES)
    $(COMPILE_MT) $*.c
_clean :
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

$(MWOS_LIBDIR) $(MWOS_ILIBDIR) $(IDIR)/MT $(RDIR)/MT:
    -$(MAKDIR) $@

