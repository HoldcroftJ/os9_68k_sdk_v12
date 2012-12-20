# Makefile
#*****************************************************************************
#* This makefile will make the PwrMan Extensions subroutine module          **
#*****************************************************************************
#* Copyright 1996 by Microware Systems Corporation                          **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************
#* Edition History:                                                         **
#* #   Date     Comments                                          By        **
#* --- -------- ------------------------------------------------- --------- **
#*   1 05/10/96 creation                                          rmm       **
#*     01/17/97 <--------------- PwrMan v2.1 --------------->               **
#*   2 04/13/99 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT    = ../..
TRGTS   = pwrext

include $(PORT)/../make.com

RDIR    = RELS
IDIR    = $(RDIR)
ODIR    = $(PORT)/CMDS/BOOTOBJS
SDIR    = $(MWOS)/SRC/SYSMODS/PWRMAN/PWREXT/MC328ADS

DEBUG   = 
DEFINES   = 
DEFS    = -v=. -v=$(SDIR) $(MWOS_DEFS)
LIBS    = -l=$(MWOS_DFTLIB)/pwrman.l $(CPULIBS) $(MWOS_LIBS) 
OPTMZ   = -o=7 -t=1 -s=1 -cw -iom

CFILES    = 
RFILES    = $(RDIR)/extstrt.r $(RDIR)/icode.r
IFILES    = $(IDIR)/pwrext.i

COMPILE   = $(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(DEBUG) -asn $(COPTS)
ASSEMBL   = $(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(DEBUG) $(LIBS) -asn $(COPTS)
LINK    = $(LINKER) $(LPARMS) -k -r $(OPTMZ) $(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

pwrext:  _makdir $(ODIR)/pwrext
	$(COMMENT)

$(ODIR)/pwrext:  $(RFILES)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/pwrext.stb
	$(CODO) $(ODIR)/STB/pwrext.dbg
	$(LINK) $(RFILES) -fd=$@ -oln=pwrext

$(RDIR)/icode.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=icode.r

$(RDIR)/extstrt.r:  $(SDIR)/extstrt.a ./makefile
	$(ASSEMBL) $(SDIR)/extstrt.a

$(IDIR)/pwrext.i:  $(SDIR)/pwrext.c $(SDIR)/defs.h ./makefile
	$(COMPILE) $(SDIR)/pwrext.c

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  .
	$(CODO) $(ODIR)/pwrext
	-$(DEL) $(ODIR)/pwrext
	$(CODO) $(ODIR)/STB/pwrext.stb
	-$(DEL) $(ODIR)/STB/pwrext.stb
	$(CODO) $(ODIR)/STB/pwrext.dbg
	-$(DEL) $(ODIR)/STB/pwrext.dbg

_lint:  .
	-$(DEL) pwrext.lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >pwrext.lnt


#                                     #
#############################################################################
