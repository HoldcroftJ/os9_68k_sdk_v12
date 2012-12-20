#*****************************************************************************
#* Makefile for the MC68328ADS Init Module                                  **
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
#*   2 11/13/95 Added ROM_128K macro                              rws       **
#*   3 04/30/96 moved to new directory structure                  bat       **
#*   4 05/13/96 Adopted proper usage of port templates.           ats       **
#*              Corrected copyright date. Conformed to conventions.         **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   5 08/01/96 Improved portability.                             ats       **
#*   6 08/01/96 General cleanup.                                  ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   7 99/04/05 support MAKDIR and CODO                           srm       **
#*****************************************************************************

SDIR    = $(OS_SDIR)/SYSMODS/INIT   # source dir
RDIR    = RELS
ODIR    = $(PORT)/CMDS/BOOTOBJS/INITS

CPUDEFS   = $(MWOS_DFTDEF)/MC6832X    # 68328 specific defs

# defs files included via defsfile/systype.d
INCLUDES  = $(CPUDEFS)/m328defs             \
				$(CPUDEFS)/sim328.d $(CPUDEFS)/pllm328.d  \
				$(CPUDEFS)/lcdcm328.d $(CPUDEFS)/rtcm328.d  \
				$(CPUDEFS)/timm328.d $(CPUDEFS)/parp328.d \
				$(CPUDEFS)/uartm328.d $(CPUDEFS)/spis328.d  \
				$(CPUDEFS)/spim328.d $(CPUDEFS)/pwmm328.d

SYSDEFS   = $(MWOS_OSDDIR)/oskdefs.d $(PORT)/systype.d $(INCLUDES)

DEFS    = -v=. $(PORT_DEFS) -v=$(CPUDEFS) $(MWOS_DEFS)

MAKERS    = ./makefile ../initmake.com

# Init module
INITSRC   = $(SDIR)/init.a      # source file
RELS    = $(RDIR)/$(TRGTS).r

SPEC_AOPTS  = 

ASSEMBL   = $(ASM) $(APARMS) $(DEFS) -eas=. -r -asb \
				$(SPEC_AOPTS) -asn $(AOPTS)

SPEC_LOPTS  = 

LINK    = $(LINKER) $(LPARMS) -k -r $(SYS) -oln=init \
				$(SPEC_LOPTS) $(LOPTS) $(COPT_GU0)

notarget: nulltrg
	$(COMMENT)

$(TRGTS): _makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(RELS)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	$(LINK) $(RELS) -fd=$@ -oln=init

$(RELS): $(INITSRC) $(SYSDEFS) $(MAKERS)
	$(ASSEMBL) -fd=$(RELS) $(INITSRC) $(INIT_OPTS)

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb

_lint:  nulltrg
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#                                     #
#############################################################################

