#*****************************************************************************
#* Makefile for MC68328ADS ROM without ROMBUG                               **
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
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 08/01/96 Improved portability.                             ats       **
#*   4 08/01/96 General cleanup.                                  ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   5 08/05/99 Change OBJDIR to NOBUG to match other cboot ports mgh       **
#*   6 07/10/00 Added PADROM for 1Meg ROM size.                   rkw       **
#*****************************************************************************

PORT		=	../..
TRGTS		=	bootrom

include $(PORT)/../make.com

OBJDIR		=	$(PORT_OBJDIR)/NOBUG

FILES		=	$(OBJDIR)/romcore \
				$(OBJDIR)/initext \
				$(OBJDIR)/bootfile

$(TRGTS): $(OBJDIR) $(OBJDIR)/$(TRGTS)
	$(COMMENT)

$(OBJDIR)/$(TRGTS): $(FILES) $(MAKENAME)
	-$(CODO) $@
	$(MERGE) $(FILES) $(REDIR_OUT)$@
# 1 Meg image.
	$(PADROM) 0x100000 $@

$(OBJDIR) :
	-$(MAKDIR) $@

_clean _purge:  .
	-$(CODO) $(OBJDIR)/$(TRGTS)
	-$(DEL) $(OBJDIR)/$(TRGTS)

_lint:  .
	$(COMMENT)

#																			#
#############################################################################

