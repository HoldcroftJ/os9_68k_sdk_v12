# Makefile
#*****************************************************************************
#* This makefile will make the SysIF P2 module                    **
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
#* Edition History:                                                         **
#* #   Date     Comments                                          By        **
#* --- -------- ------------------------------------------------- --------- **
#*   1 10/02/95 creation                                          bat       **
#*     01/17/97 <--------------- PwrMan v2.1 --------------->               **
#*****************************************************************************

PORT    = ..
TRGTS   = sysif
MAKENAME  = makefile

include $(PORT)/../make.com

RDIR    = RELS
IDIR    = $(RDIR)
ODIR    = $(PORT)/CMDS/BOOTOBJS
SDIR    = $(MWOS)/SRC/SYSMODS/SYSIF/MC68328

DEBUG   = 
DEFINES   = -dREV2G15E
DEFS    = -v=. -v=$(SDIR) $(MWOS_DEFS)
LIBS    = -l=$(MWOS_DFTLIB)/pwrman.l $(CPULIBS) $(MWOS_LIBS)
OPTMZ   = -o=7 -t=1 -s=1 -cw -iom

CFILES    =
RFILES    = $(RDIR)/ifstart.r $(RDIR)/icode.r
IFILES    = $(IDIR)/cpu.i $(IDIR)/pwrif.i $(IDIR)/rtclock.i

COMPILE   = $(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -r $(OPTMZ) \
				$(DEBUG) -asn $(COPTS)
ASSEMBL   = $(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -r $(OPTMZ) \
				$(DEBUG) $(LIBS) -asn $(COPTS)
LINK    = $(LINKER) $(LPARMS) -g -k -r $(OPTMZ) $(DEBUG) $(LIBS) $(LOPTS) $(COPT_GU0)

sysif:  _makdir $(ODIR)/sysif
	$(COMMENT)

$(ODIR)/sysif:  $(RFILES)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/sysif.stb
	$(LINK) $(RFILES) -fd=$@ -oln=sysif

$(RDIR)/icode.r:  $(IFILES)
	$(ASSEMBL) $(IFILES) -fd=icode.r

$(RDIR)/ifstart.r:  $(SDIR)/ifstart.a ./makefile
	$(ASSEMBL) $(SDIR)/ifstart.a

$(IDIR)/cpu.i:  $(SDIR)/cpu.c $(SDIR)/defs.h ./makefile
	$(COMPILE) $(SDIR)/cpu.c

$(IDIR)/pwrif.i:  $(SDIR)/pwrif.c $(SDIR)/defs.h ./makefile
	$(COMPILE) $(SDIR)/pwrif.c

$(IDIR)/rtclock.i:  $(SDIR)/rtclock.c $(SDIR)/defs.h ./makefile
	$(COMPILE) $(SDIR)/rtclock.c

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  .
	$(CODO) $(ODIR)/sysif
	-$(DEL) $(ODIR)/sysif
	$(CODO) $(ODIR)/STB/sysif.stb
	-$(DEL) $(ODIR)/STB/sysif.stb
	$(CODO) $(ODIR)/STB/sysif.dbg
	-$(DEL) $(ODIR)/STB/sysif.dbg

_lint:  .
	-$(DEL) sysif.lnt
	$(LINT) $(LINTPARMS) $(DEFS) $(DEFINES) $(CFILES) >sysif.lnt

#
#############################################################################
