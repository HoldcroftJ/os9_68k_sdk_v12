# Makefile
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 ??/??/?? Created                                                ???
#  02 01/13/99 Added automatic directory creation and checkout        rds
#  03 06/14/99 Added CODO for dbg file.                               sr
#  04 06/14/99 Enabled optimization and removed -g                    rds
############################################################################
# This makefile will make the EXAMPLE3 ITEM application.                   #
############################################################################
#                                                                          #

MWOS		=	../../../..
TRGTS		=	example3
include $(MWOS)/MAKETMPL/make.com

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
if $(TARGET) == "mip4" || $(TARGET) == "mip3"
ODIR		=	$(MWOS_BASODIR)
else
ODIR		=	$(MWOS_ODIR)
endif

RELS		=	$(RDIR)/example3.r
IRELS		=	$(IDIR)/example3.i

MACROS		=

if $(TARGET) == "mip4" || $(TARGET) == "mip3"
ITEMLIB		=	-l=$(MWOS_BASCLIB)/item.l
else
ITEMLIB		=	-l=$(MWOS_LIBDIR)/item.l
endif

DEFS		=	$(MWOS_DEFS)
LIBS		=	$(ITEMLIB) $(CONV_LIBS) $(MWOS_LIBS)	# Don't Use CSL
#LIBS		=	$(CONV_LIBS) $(ITEMLIB) $(MWOS_CSLLIBS) # Use CSL

OPTMZ		=	-o=7 -t=1 -s=1

COMPILE		=	$(CC) $(CPARMS) $(OPTMZ) -eas=$(RDIR) $(DEFS) $(MACROS) $(COPTS)
ILINK		=	$(CC) $(CPARMS) $(OPTMZ) -eas=$(RDIR) $(DEFS) $(MACROS) $(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(OPTMZ) -tp=$(CPUC) -k -olS $(LIBS) \
				$(LOPTS) $(COPT_GU1)

$(TRGTS): $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/$(TRGTS)
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@
$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/example3:  $(RELS)
	-$(CODO) $(ODIR)/example3
	-$(DEL)  $(ODIR)/example3
	-$(CODO) $(ODIR)/STB/example3.stb
	-$(DEL)  $(ODIR)/STB/example3.stb
	-$(DEL)  $(ODIR)/STB/example3.dbg
	$(LINK)  $(CSTART) $(RELS) -fd=$(ODIR)/example3
	$(ATTRE1) $(ODIR)/example3

$(RDIR)/example3.r:  example3.c
	$(COMPILE) example3.c

_makdir: nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	-$(CODO) $(ODIR)/example3
	-$(DEL)  $(ODIR)/example3
	-$(CODO) $(ODIR)/STB/example3.stb
	-$(DEL)  $(ODIR)/STB/example3.stb
	-$(DEL)  $(ODIR)/STB/example3.dbg

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################
