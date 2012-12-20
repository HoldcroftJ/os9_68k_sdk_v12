#*****************************************************************************
#* Makefile for the sysinit extentions module in the MC68328ADS ROM         **
#*****************************************************************************
#* Copyright 1995-1999 by Microware Systems Corporation                     **
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
#*   3 07/09/96 Added definition for ABORTINT conditional.        ats       **
#*   4 08/01/96 Improved portability. General cleanup.            ats       **
#*   5 08/01/96 More cleanup.                                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   6 04/14/99 support MAKDIR and CODO                           srm       **
#*   7 06/17/99 Adding explicit shut off for assembler branch               **
#*                optimizaitons (-as=-bt).                        rkw       **
#*   8 08/05/99 Changed output dir from ROM to NOBUG to match     mgh       **
#*               other CBoot based port directories                         **
#*****************************************************************************

OBJ			=	initext

PORT		=	../..
TRGTS		=	obj

include $(PORT)/../make.com

#SDIR		=	$(OS_SDIR)/ROM/COMMON		# specific source dir
SDIR		=	.							# specific source dir
RDIR		=	RELS
ODIR		=	$(PORT_OBJDIR)/NOBUG
CPUDEFS		=	$(MWOS_DFTDEF)/MC6832X		# 68328 defs

OBJECT		=	$(RDIR)/$(OBJ).r
COMDEFS		=	$(MWOS_OSDDIR)/oskdefs.d

# defs files included via defsfile/systype.d
INCLUDES	=	$(CPUDEFS)/m328defs							\
				$(CPUDEFS)/sim328.d $(CPUDEFS)/pllm328.d	\
				$(CPUDEFS)/lcdcm328.d $(CPUDEFS)/rtcm328.d	\
				$(CPUDEFS)/timm328.d $(CPUDEFS)/parp328.d	\
				$(CPUDEFS)/uartm328.d $(CPUDEFS)/spis328.d	\
				$(CPUDEFS)/spim328.d $(CPUDEFS)/pwmm328.d	\
				$(PORT)/systype.d $(COMDEFS)

DEFS		=	-v=. $(PORT_DEFS) -v=$(CPUDEFS) $(MWOS_DEFS)

MAKERS		=	./makefile

ABORTINT	=	-asaABORTINT	# enable abort switch interrupt

INHOUSE		=	#-asaINHOUSE
INHOUSEXT	=	#-asaINHOUSEXT				# Inhouse extensions flag

SPEC_RFLAGS	=	$(ABORTINT) $(INHOUSE) $(INHOUSEXT)

ASSEMBL		=	$(ASM) $(APARMS) $(DEFS) -eas=$(RDIR) \
				$(SPEC_RFLAGS) -asn $(AOPTS) -as=-bt

LINK		=	$(LINKER) $(LPARMS) -k -olr=0 -olswam $(LOPTS) -b

obj: _makdir $(ODIR)/$(OBJ)
	$(COMMENT)

$(ODIR)/$(OBJ) : $(OBJECT)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(OBJ).map
	$(CODO) $(ODIR)/STB/$(OBJ).stb
	$(LINK) $(OBJECT) -fd=$@ $(REDIR_OUT)$(ODIR)/STB/$(OBJ).map

$(OBJECT) : $(INCLUDES) $(SDIR)/initext.a $(MAKERS)
	$(ASSEMBL) $(SDIR)/initext.a

_makdir: .
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

_clean: .
	-$(DEL) $(RDIR)/*.r

_purge:  .
	$(CODO) $(ODIR)/$(OBJ)
	-$(DEL) $(ODIR)/$(OBJ)
	$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL) $(ODIR)/STB/$(OBJ).map
	$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(DEL) $(ODIR)/STB/$(OBJ).stb

_lint:  .
	$(COMMENT)

#																			#
#############################################################################

