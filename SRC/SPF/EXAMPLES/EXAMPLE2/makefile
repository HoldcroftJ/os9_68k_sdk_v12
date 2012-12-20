# Makefile
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 ??/??/?? Created                                                ???
#  02 01/13/99 Added automatic directory creation and checkout        rds
#  03 04/20/99 Added support for sh4.                                 sr
#  04 06/14/99 Added CODO for dbg file.                               sr
#  05 06/14/99 Removed -g from OPTMZ                                  rds
############################################################################
#                                                                          #

MWOS		=	../../../..
TRGTS		=	spf_test
include $(MWOS)/MAKETMPL/make.com

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
if $(TARGET) == "mip4" || $(TARGET) == "mip3"
ODIR		=	$(MWOS_BASODIR)
else
ODIR		=	$(MWOS_ODIR)
endif

RELS		=	$(RDIR)/spf_test.r
IRELS		=	$(IDIR)/spf_test.i

MACROS		=

DEFS		=	$(MWOS_DEFS)
LIBS		=	$(CONV_LIBS) $(MWOS_LIBS)			# Don't Use CSL
#LIBS		=	$(CONV_LIBS) $(MWOS_CSLLIBS)			# Use CSL

#OPTMZ		=	-o=7 -t=1 -s=1
OPTMZ		=	-o=0

if (($(TARGET) == "sh") || ($(TARGET) == "sh4"))
CODEOPTS = $(COPT_LC)$(COPT_LCD)
elif $(TARGET) == "sprc"
CODEOPTS = $(COPT_LCD)
else
CODEOPTS =
endif

COMPILE		=	$(CC) $(CPARMSNC)$(CODEOPTS) $(OPTMZ) -efe=$(IDIR) $(DEFS) \
				$(MACROS) $(COPTS)
ILINK		=	$(CC) $(CPARMSNC)$(CODEOPTS) $(OPTMZ) -eas=$(RDIR) $(DEFS) \
				$(MACROS) $(COPTS)
LINK		=	$(LINKER) $(LPARMS)$(CODEOPTS) $(OPTMZ) -r -k -i -olS $(LIBS) \
				$(LOPTS) $(COPT_GU1)

$(TRGTS): $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/$(TRGTS)
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@
$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/spf_test:  $(RELS)
	-$(CODO) $(ODIR)/spf_test
	-$(DEL)  $(ODIR)/spf_test
	-$(CODO) $(ODIR)/STB/spf_test.stb
	-$(DEL)  $(ODIR)/STB/spf_test.stb
	-$(DEL)  $(ODIR)/STB/spf_test.dbg
	$(LINK)  $(CSTART) $(RELS) -fd=$(ODIR)/spf_test
	$(ATTRE1) $(ODIR)/spf_test

$(RDIR)/spf_test.r:  $(IRELS)
	$(ILINK) $(LIBS) $(IRELS) -fd=spf_test.r

$(IDIR)/spf_test.i:  spf_test.c
	$(COMPILE) spf_test.c

_makdir: nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	-$(CODO) $(ODIR)/spf_test
	-$(DEL)  $(ODIR)/spf_test
	-$(CODO) $(ODIR)/STB/spf_test.stb
	-$(DEL)  $(ODIR)/STB/spf_test.stb
	-$(DEL)  $(ODIR)/STB/spf_test.dbg

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################
