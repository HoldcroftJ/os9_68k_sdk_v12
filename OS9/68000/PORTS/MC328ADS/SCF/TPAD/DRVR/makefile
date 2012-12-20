# Makefile
#*****************************************************************************
#* ID:     %A%
#* Date:   %G%
#*****************************************************************************
#* Makefile for the MC68328ADS 68681 SCF Driver                             **
#*****************************************************************************
#* Copyright 1995, 1996 by Microware Systems Corporation                    **
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
#*   2 05/13/96 Adopted proper usage of port and base templates.  ats       **
#*              ---- OS-9/68K 68328 Support Beta Release ----               **
#*   3 07/08/96 Modified for tpaddrvr usage                       rws       **
#*   4 99/04/05 support MAKDIR and CODO                           srm       **
#*****************************************************************************

PORT    = ../../..
TRGTS   = tpad_drvr

include $(PORT)/../make.com

CPUDEFS   = $(MWOS_DFTDEF)/MC6832X    # 68328 specific defs

SDIR    = $(OS_SDIR)/IO/SCF/DRVR/MC328ADS/TOUCHPAD
RDIR    = RELS
IDIR    = $(RDIR)
ODIR    = $(PORT)/CMDS/BOOTOBJS

DEFS    = -v=. -v=$(PORT)/DEFS $(PORT_DEFS) -v=$(CPUDEFS) \
				$(OS_MACROS) $(MWOS_DEFS)
HEADERS   = $(SDIR)/touchpad.h
DEFINES     =   #-dUSE_CYCLIC_ALM #-dTIME_STAMP

MAKERS    = ./makefile

LIBS    = $(MWOS_LIBS)

# Note:  scf_glue.r MUST be linked before rest of RFILES
RFILES      = $(RDIR)/scf_glue.r \
								$(RDIR)/main.i \
								$(RDIR)/init.i \
								$(RDIR)/term.i \
								$(RDIR)/irqs.i \
								$(RDIR)/read.i \
								$(RDIR)/stat.i

SCFDRV    = $(ODIR)/tpaddrvr

SPEC_RFLAGS = 

#OPTMZ      =   -o=7 -t=1 -s=7 -iom
OPTMZ       =   -o=0 -iom

COMPILE     =   $(CC) $(CPARMS) $(DEFS) $(DEFINES) -efe=$(IDIR) -rg $(OPTMZ)  \
								-asn $(COPTS)

ASSEMBL     =   $(CC) $(CPARMS) $(DEFS) $(DEFINES) -eas=$(RDIR) -rg $(OPTMZ) \
								-asn $(COPTS)
#ASSEMBL    = $(ASM) $(APARMS) -k -r $(DEFS) \
#       $(SPEC_RFLAGS) -eas=$(RDIR) -asn $(AOPTS)

SPEC_LOPTS  =

LINK    = $(LINKER) $(LPARMS) -k -r $(LIBS) $(OPTMZ) $(SPEC_LOPTS) \
				$(LOPTS) $(COPT_GU0)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
$(TRGTS): _makdir $(SCFDRV)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link SCF drivers
$(SCFDRV): $(RFILES) ./makefile
	$(CODO) $@
	$(CODO) $(ODIR)/STB/tpaddrvr.stb
	$(LINK) $(RFILES) -fd=$@

$(RDIR)/scf_glue.r:  $(SDIR)/scf_glue.a $(HEADERS) ./makefile
	$(ASSEMBL) $(SDIR)/scf_glue.a

$(RDIR)/main.i:  $(SDIR)/main.c $(HEADERS) ./makefile
	$(COMPILE) $(SDIR)/main.c

$(RDIR)/init.i:  $(SDIR)/init.c $(HEADERS) ./makefile
	$(COMPILE) $(SDIR)/init.c

$(RDIR)/term.i:  $(SDIR)/term.c $(HEADERS) ./makefile
	$(COMPILE) $(SDIR)/term.c

$(RDIR)/irqs.i:  $(SDIR)/irqs.c $(HEADERS) ./makefile
	$(COMPILE) $(SDIR)/irqs.c

$(RDIR)/stat.i:  $(SDIR)/stat.c $(HEADERS) ./makefile
	$(COMPILE) $(SDIR)/stat.c

$(RDIR)/read.i:  $(SDIR)/read.c $(HEADERS) ./makefile
	$(COMPILE) $(SDIR)/read.c

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/scf_glue.r
	-$(DEL) $(RDIR)/main.i
	-$(DEL) $(RDIR)/init.i
	-$(DEL) $(RDIR)/term.i
	-$(DEL) $(RDIR)/irqs.i
	-$(DEL) $(RDIR)/stat.i
	-$(DEL) $(RDIR)/read.i

_purge:  nulltrg
	$(CODO) $(SCFDRV)
	-$(DEL) $(SCFDRV)
	$(CODO) $(ODIR)/STB/tpaddrvr.stb
	-$(DEL) $(ODIR)/STB/tpaddrvr.stb

_lint:  nulltrg
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#                                     #
#############################################################################

