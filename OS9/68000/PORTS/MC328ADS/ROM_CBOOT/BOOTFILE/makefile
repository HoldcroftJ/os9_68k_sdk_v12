#*****************************************************************************
#* Makefile for bootfile in the MC68328ADS ROM                              **
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
#*   2 05/14/96 Adopted proper usage of port and base templates.  ats       **
#*   3 05/15/96 Added fixmod process to bootfile creation.        ats       **
#*   4 06/17/96 Added BOOTLIST macro definition options.          ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   5 08/01/96 Improved portability.                             ats       **
#*   6 08/01/96 General cleanup.                                  ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*****************************************************************************

OFILE           =       bootfile

PORT            =       ../..
TRGTS           =       boot

include $(PORT)/../make.com

# directories referenced relative to PORT directory
SDIR            =       BOOTLIST
MRGDIR          =       CMDS/BOOTOBJS/NOBUG

ODIR            =       $(PORT_OBJDIR)/NOBUG

BOOTLIST        =       rom.bl                  # Embedded bootlist
#BOOTLIST		=		ssdbg.bl				# Systems State Debugging bootlist

FILES           =       $(SDIR)/$(BOOTLIST)


boot: _makdir $(ODIR)/$(OFILE)
	$(COMMENT)

$(ODIR)/$(OFILE):
	-$(CODO) $(ODIR)/$(OFILE)
	-$(DEL)  $(ODIR)/$(OFILE)
	$(CHD) $(PORT); $(MERGE) -z=$(FILES) $(REDIR_OUT)$(MRGDIR)/$(OFILE)

_makdir: .
	$(MAKDIR) $(ODIR)

_clean _purge:  .
	-$(CODO) $(ODIR)/$(OFILE)
	-$(DEL) $(ODIR)/$(OFILE)

_lint:  .
	$(COMMENT)

#                                                                                                                                                       #
#############################################################################

