############################################################################
#                                                                          #
#           Copyright 1989-2001 by Microware Systems Corporation           #
#                           All Rights Reserved                            #
#                         Reproduced Under License                         #
#                                                                          #
# This software is confidential property of Microware Systems Corporation, #
# and is provided under license for internal development purposes only.    #
# Reproduction, publication, distribution, or creation of derivative works #
# in any form to any party other than the licensee is strictly prohibited, #
# unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                          #
############################################################################
#
# Edition History:                                                         
#
# #   Date     Comments                                                By 
# --- -------- -----------------------------------------------------  ---
# 01  ??/??/?? Created
# 02  11/11/98 Modified for use with MAKETMPL V1.3,                   mgh
#               check CODO and MAKDIR, added conditionals
# 03  12/01/98 Put back recusive call, TRGT_DESC                      mgh
# 04  12/30/98 Fix minor typo                                         mgh
# 05  11/30/99 Enable clean target                                    srm
# 06  01/25/00 Don't abort if $(CODO) or $(TOUCH) fail                rds
# 07  10/13/00 Add DEFINES MACRO to pass user defined MACROS.         sr
# 08  05/11/01 Don't create STB dir. Use COPT_GU0 instead of fixmod.  djc
#############################################################################
# This makefile will make the SPF Device Descriptors.                       #
#############################################################################


DPIO		=	$(MWOS)/SRC/DPIO
SPF			=	$(DPIO)/SPF

include $(DPIO)/MAKETMPL/dpio.tpl

DEPENDS		=
TRGT_DESC	=
DEFINES   =
MACROS		=	-d$(TRGT_DESC) $(DEFINES)

DEFS		=	-v=$(DESCDIR) -v=$(SPF)/DRVR -v=$(SPF)/DEFS $(DPIO_DEFS)

ACOMPILE	=	$(CC) -to=$(OS) -tp=$(CPU) -td=$(TEMP) -o=0 $(DEFS) $(MACROS) -ebe=$(RDIR) $(COPTS)
RCOMPILE	=	$(CC) -to=$(OS) -tp=$(CPU) -td=$(TEMP) -o=0 $(DEFS) $(MACROS) -eas=$(RDIR) $(COPTS)
LINK		=	$(LINKER) -to=$(OS) -tp=$(CPU) -td=$(TEMP) -o=0 -k $(LOPTS)




$(TRGTS):	.
	$(MAKEIT) TARGET=$(TARGET) TRGT_DESC=$@ makedesc

makedesc: $(ODIR)/$(TRGT_DESC)
	$(COMMENT)

if $(OS) == "OSK"
#
# Make "dummy" intermediate file
#
$(RDIR)/$(TRGT_DESC).date: $(SDIR)/cnst.c $(SDIR)/stat.c $(SDIR)/os9.c $(DESCDIR)/spf_desc.h $(DEPENDS) 
	-$(MAKDIR) $(RDIR)
	-$(TOUCH) $@

#
# Make OS9 Descriptor
#
$(ODIR)/$(TRGT_DESC): $(RDIR)/$(TRGT_DESC).date
	#
	# make sure all temp files are deleteds
	#
	-$(DEL) $(RDIR)/cnst.a
	-$(DEL) $(RDIR)/cnst.o
	-$(DEL) $(RDIR)/cnst.r
	-$(DEL) $(RDIR)/stat.r
	-$(DEL) $(RDIR)/os9.a
	-$(DEL) $(RDIR)/os9.o
	-$(DEL) $(RDIR)/os9.r
	-$(DEL) $(RDIR)/spfos9
	-$(DEL) $(RDIR)/spfos9k
	-$(CODO) $(ODIR)/$(TRGT_DESC)
	-$(DEL) $(ODIR)/$(TRGT_DESC)
	#
	# now make the os9000 descriptor
	#
	$(ACOMPILE) $(SDIR)/cnst.c
	$(RM_VSECT) $(RDIR)/cnst.o $(RDIR)/cnst.a
	$(RCOMPILE) $(RDIR)/cnst.a
	$(RCOMPILE) $(SDIR)/stat.c
	$(LINK) $(SYSTART) $(RDIR)/cnst.r $(RDIR)/stat.r -fd=$(RDIR)/spfos9k
	#
	# now make the os9 descriptor
	#
	$(ACOMPILE) $(SDIR)/os9.c
	$(RM_VSECT) $(RDIR)/os9.o $(RDIR)/os9.a
	$(RCOMPILE) $(RDIR)/os9.a
	$(LINK) $(DESCSTART) $(RDIR)/os9.r -fd=$(RDIR)/spfos9 -oln=$(TRGT_DESC)
	#
	# now merge the two together
	#
	$(MERGE) $(RDIR)/spfos9 $(RDIR)/spfos9k -o=$(ODIR)/$(TRGT_DESC)
	$(CHTYPE_DESC) $(ODIR)/$(TRGT_DESC)
	$(FIXMOD0) $(ODIR)/$(TRGT_DESC)
	$(ATTRE0) $(ODIR)/$(TRGT_DESC)
	#
	# make sure all temp files are deleted 
	#
	-$(DEL) $(RDIR)/cnst.a
	-$(DEL) $(RDIR)/cnst.o
	-$(DEL) $(RDIR)/cnst.r
	-$(DEL) $(RDIR)/stat.r
	-$(DEL) $(RDIR)/os9.a
	-$(DEL) $(RDIR)/os9.o
	-$(DEL) $(RDIR)/os9.r
	-$(DEL) $(RDIR)/spfos9
	-$(DEL) $(RDIR)/spfos9k

elif $(OS) == "OS9000"

#
# Make "dummy" intermediate file
#
$(RDIR)/$(TRGT_DESC).date: $(SDIR)/cnst.c $(SDIR)/stat.c $(DESCDIR)/spf_desc.h $(DEPENDS) 
	-$(MAKDIR) $(RDIR)
	-$(TOUCH) $@

#
# Make OS9000 Descriptor
#
$(ODIR)/$(TRGT_DESC): $(RDIR)/$(TRGT_DESC).date
	#
	# make sure all temp files are deleted
	#
	-$(DEL) $(RDIR)/cnst.a
	-$(DEL) $(RDIR)/cnst.o
	-$(DEL) $(RDIR)/cnst.r
	-$(DEL) $(RDIR)/stat.r
	-$(CODO) $(ODIR)/$(TRGT_DESC)
	-$(DEL) $(ODIR)/$(TRGT_DESC)
	#
	# make the os9000 descriptor
	#
	$(ACOMPILE) $(SDIR)/cnst.c
	$(RM_VSECT) $(RDIR)/cnst.o $(RDIR)/cnst.a
	$(RCOMPILE) $(RDIR)/cnst.a
	$(RCOMPILE) $(SDIR)/stat.c
	$(LINK) $(DESCSTART) $(RDIR)/cnst.r $(RDIR)/stat.r -fd=$(ODIR)/$(TRGT_DESC) $(COPT_GU0)
	$(ATTRE0) $(ODIR)/$(TRGT_DESC)
	#
	# make sure all temp files are deleted
	#
	-$(DEL) $(RDIR)/cnst.a
	-$(DEL) $(RDIR)/cnst.o
	-$(DEL) $(RDIR)/cnst.r
	-$(DEL) $(RDIR)/stat.r

endif


_clean:	.
	-$(DEL) $(RDIR)/*.date
	$(COMMENT)

_purge:	.
for TMP in $(TRGTS)
	-$(CODO) $(ODIR)/$(TMP)
	-$(DEL)  $(ODIR)/$(TMP)
endfor
	$(COMMENT)

_lint:	.
	$(COMMENT)

#                                                                           #
#############################################################################
