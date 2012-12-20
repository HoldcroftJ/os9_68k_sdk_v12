#*****************************************************************************
#* Makefile include file to build a MAUI Input Process Protocol Module
#* This include file is called from the makefile in the ports directory
#* e.g. $MWOS/SRC/OS9000/603/PORTS/MVME1603/MAUI/MP_BSPTR/makefile
#*****************************************************************************
#* Copyright 1995 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************

DEFS		= -v=. \
                -v=$(SDIR) \
				-v=$(MWOS_DDIR) \
				-v=$(MWOS_OSDDIR) \
				-v=$(MWOS_DFTDEF)

USER_HEADERS = # Override in user original makefile in needed
HEADERS		= $(SDIR)/_key.h \
				$(MWOS_DDIR)/MAUI/mp.h \
				$(MWOS_DDIR)/MAUI/mppm.h \
				$(MWOS_DDIR)/MAUI/maui_msg.h \
				$(MWOS_DDIR)/MAUI/maui_inp.h \
                $(USER_HEADERS)

USER_RFILES = # Override in user original makefile in needed
RFILES		= $(RDIR)/mppmstrt.r \
				$(IDIR)/init.i \
				$(IDIR)/procdata.i \
				$(IDIR)/procmsg.i \
				$(IDIR)/term.i \
                $(USER_RFILES)

USER_LIBS   = # Override in user original makefile in needed
LIBS		= $(USER_LIBS) $(MWOS_LIBS)

OPTMZ	    =	-t=3 -s=10 $(CPARMSNC)$(COPT_LD)$(COPT_LC)$(COPT_LCD) $(COPT_CODEADDR)

USER_OPTS   = # Override in user original makefile in needed
COMPILE	    =	$(CC) $(DEFS) -efe=$(IDIR) -r $(OPTMZ) \
				$(COPTS) $(USER_OPTS) 
ASSEMBL	    =	$(CC) $(DEFS) -eas=$(RDIR) -r $(OPTMZ) \
				$(COPTS) $(USER_OPTS) 
LINK	    =	$(LINKER) $(LIBS) -kr -olg -olgu=0.0 $(OPTMZ) \
                $(LOPTS) $(USER_OPTS) 


$(TRGTS): $(IDIR) $(ODIR) $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(RFILES)
	$(CODO) $@
    $(CODO) $(ODIR)/STB/$*.stb
	$(LINK) $(RFILES) -fd=$@

$(RDIR)/mppmstrt.r:  $(SDIR)/mppmstrt.a $(HEADERS)
	$(ASSEMBL) $(SDIR)/mppmstrt.a

$(IDIR)/init.i:  $(SDIR)/init.c $(HEADERS)
	$(COMPILE) $(SDIR)/init.c

$(IDIR)/procdata.i:  $(SDIR)/procdata.c $(HEADERS)
	$(COMPILE) $(SDIR)/procdata.c

$(IDIR)/procmsg.i:  $(SDIR)/procmsg.c $(HEADERS)
	$(COMPILE) $(SDIR)/procmsg.c

$(IDIR)/term.i:  $(SDIR)/term.c $(HEADERS)
	$(COMPILE) $(SDIR)/term.c

$(IDIR) $(ODIR):
	-$(MAKDIR) $@

_clean:
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(IDIR)/*.i

_purge:
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
    $(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb

