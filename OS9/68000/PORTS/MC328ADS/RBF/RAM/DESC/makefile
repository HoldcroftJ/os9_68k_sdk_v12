#*****************************************************************************
#* Makefile for MC68328ADS RBF/RAMdisk descriptor modules                   **
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
#*   1 11/15/96 creation                                          bat       **
#*   2 99/04/05 support MAKDIR and CODO                           srm       **
#*****************************************************************************

TRGTS       =   r0 dd
PORT            =       ../../..
MAKENAME        =       makefile
include $(PORT)/../make.com

ROOT            = $(MWOS)/OS9                           # base of dir system
BASEROOT        = $(ROOT)/68000                         # dir system for LIB, etc
CPUROOT         = $(ROOT)/68000                         # dir system for output
SRCROOT         = $(ROOT)/SRC                           # dir system for sources
OSDEFS          = $(SRCROOT)/DEFS                       # std OS defs

SDIR            = $(SRCROOT)/IO/RBF/DESC                # source files
ODIR            = $(PORT)/CMDS/BOOTOBJS/DESC/RAMDISK    # output files
RDIR            = RELS

DEFINES         =       -dDEVTAPE
DEFS            =       -v=. -v=$(PORT) -v=$(CPUROOT)/DEFS/MC6832X $(MWOS_DEFS)
LIBS            =       $(SYS)

ASSEMBL         =       $(ASM) $(APARMS) -k -r -eas=$(RDIR) $(DEFS) $(DEFINES) -asn $(AOPTS)
LINK            =       $(LINKER) $(LPARMS) -k -r $(LIBS) -asn \
						$(LOPTS) $(COPT_GU0)


$(TRGTS):  _makdir $(ODIR)/r0 $(ODIR)/dd
	$(COMMENT)

$(ODIR)/r0 $(ODIR)/dd: $(SDIR)/r0.a $(PORT)/systype.d ./makefile
	$(ASSEMBL) $(SDIR)/r0.a -fd=r0.r
	$(CODO) $(ODIR)/r0
	$(CODO) $(ODIR)/STB/r0.stb
	$(LINK) $(RDIR)/r0.r -oln=r0 -fd=$(ODIR)/r0
	$(CODO) $(ODIR)/dd
	$(CODO) $(ODIR)/STB/dd.stb
	$(LINK) $(RDIR)/r0.r -oln=dd -fd=$(ODIR)/dd

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean: nulltrg
	-$(DEL) $(RDIR)/r0.r
	$(CODO) $(ODIR)/dd
	-$(DEL) $(ODIR)/dd
	$(CODO) $(ODIR)/STB/dd.stb
	-$(DEL) $(ODIR)/STB/dd.stb

_purge:  nulltrg
	$(CODO) $(ODIR)/r0
	-$(DEL) $(ODIR)/r0
	$(CODO) $(ODIR)/STB/r0.stb
	-$(DEL) $(ODIR)/STB/r0.stb
	$(CODO) $(ODIR)/dd
	-$(DEL) $(ODIR)/dd
	$(CODO) $(ODIR)/STB/dd.stb
	-$(DEL) $(ODIR)/STB/dd.stb

nulltrg:  .
	$(COMMENT)

#                                                                                                                                                       #
#############################################################################
