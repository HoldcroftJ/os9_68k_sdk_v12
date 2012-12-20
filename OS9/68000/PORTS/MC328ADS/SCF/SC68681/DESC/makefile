#*****************************************************************************
#* Makefile for MC68328ADS 68681 SCF descriptors.                           **
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
#*   1 04/30/96 creation                                          bat       **
#*   2 05/13/96 Adopted proper usage of port and base templates.  ats       **
#*              Corrected usage of -v=$(PORT).                              **
#*   3 05/15/96 Corrected definition of FASTCONS, FASTCOMM.       ats       **
#*   4 05/20/96 Corrected output directory.                       ats       **
#*   5 05/20/96 Added p1 descriptor.                              ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   6 08/01/96 Improved portability.                             ats       **
#*   8 08/01/96 General cleanup.                                  ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   9 12/05/96 changed SCFDSC and SCFDSCR lines                  bat       **
#*  10 99/04/05 support MAKDIR and CODO                           srm       **
#*  11 01/01/12 Added Owner/Group Write permission to module      mgh       **
#*                allows xmode to work on shipping descriptors              **
#*****************************************************************************

PORT    = ../../..

TRGTS   = scf_desc

include $(PORT)/../make.com

CPUDEFS   = $(MWOS_DFTDEF)/MC6832X      # 68328 specific defs

SDIR    = $(OS_SDIR)/IO/SCF/DESC		# source files
ODIR    = $(PORT_OBJDIR)/DESC/SC68681   # output
RDIR    = RELS

DEFS    = -v=. $(PORT_DEFS) -v=$(CPUDEFS) $(MWOS_DEFS)

SYSDEFS   = $(MWOS_OSDDIR)/oskdefs.d $(PORT)/systype.d

LIBS    = $(SYS)
SYSN    = $(MWOS_BASLIB)/sys.l

SPEC_RFLAGS = #-asaFASTCONS -asaFASTCOMM
PERM        = -olp=577					# W:er, GO:ewr module permissions

ASSEMBL   = $(ASM) $(APARMS) -k -r $(DEFS) \
				$(SPEC_RFLAGS) -eas=$(RDIR) -asn $(AOPTS)

SPEC_LOPTS  = 

LINK    = $(LINKER) $(LPARMS) -k -r -olS $(LIBS) $(SPEC_LOPTS) \
				$(LOPTS) $(COPT_GU0) $(PERM)

# serial ports and touchpad descriptor
SCFDSC    = $(ODIR)/term   $(ODIR)/t1 # $(ODIR)/p1
SCFDSCR   = $(RDIR)/term.r $(RDIR)/t1.r # $(RDIR)/p1.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
scf_desc: _makdir $(SCFDSC)
	$(COMMENT)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link serial port descriptors
$(ODIR)/term: $(SYSN) $(RDIR)/term.r
	$(CODO) $@
	$(CODO) $(ODIR)/STB/term.stb
	$(LINK) $(RDIR)/term.r -fd=$@

$(ODIR)/t1: $(SYSN) $(RDIR)/t1.r
	$(CODO) $@
	$(CODO) $(ODIR)/STB/t1.stb
	$(LINK) $(RDIR)/t1.r -fd=$@

$(ODIR)/p1: $(SYSN) $(RDIR)/p1.r
	$(CODO) $@
	$(CODO) $(ODIR)/STB/p1.stb
	$(LINK) $(RDIR)/p1.r -fd=$@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
$(RDIR)/term.r: $(SYSDEFS) ./makefile
	$(ASSEMBL) $(SDIR)/term.a

$(RDIR)/t1.r: $(SYSDEFS) ./makefile
	$(ASSEMBL) $(SDIR)/t1.a

$(RDIR)/p1.r: $(SYSDEFS) ./makefile
	$(ASSEMBL) $(SDIR)/p1.a

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(RDIR)/term.r
	-$(DEL) $(RDIR)/t1.r
	-$(DEL) $(RDIR)/p1.r

_purge:  nulltrg
	$(CODO) $(ODIR)/term
	-$(DEL) $(ODIR)/term
	$(CODO) $(ODIR)/STB/term.stb
	-$(DEL) $(ODIR)/STB/term.stb
	$(CODO) $(ODIR)/t1
	-$(DEL) $(ODIR)/t1
	$(CODO) $(ODIR)/STB/t1.stb
	-$(DEL) $(ODIR)/STB/t1.stb
	$(CODO) $(ODIR)/p1
	-$(DEL) $(ODIR)/p1
	$(CODO) $(ODIR)/STB/p1.stb
	-$(DEL) $(ODIR)/STB/p1.stb

_lint:  nulltrg
	$(COMMENT)

nulltrg:  .
	$(COMMENT)

#                                     #
#############################################################################

