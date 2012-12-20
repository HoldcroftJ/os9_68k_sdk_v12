# Makefile
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 ??/??/?? Created                                                ???
#  02 01/13/99 Added automatic directory creation and checkout        rds
#  03 06/14/99 Enable optimizaion, remove -g                          rds
############################################################################
# This makefile will make the ITEM example #1 receive program.             #
############################################################################
#                                                                          #

MWOS		=	../../../..
TRGTS		=	ex1_rcv
MAKENAME	=	rcv.mak
include $(MWOS)/MAKETMPL/make.com

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
if $(TARGET) == "mip4" || $(TARGET) == "mip3"
ODIR		=	$(MWOS_BASODIR)
else
ODIR		=	$(MWOS_ODIR)
endif

RELS		=	$(RDIR)/ex1_rcv.r
IRELS		=	$(RDIR)/ex1_rcv.i

MACROS		=

if $(TARGET) == "mip4" || $(TARGET) == "mip3"
ITEMLIB		=	-l=$(MWOS_BASCLIB)/item.l
else
ITEMLIB		=	-l=$(MWOS_LIBDIR)/item.l
endif

DEFS		=	$(MWOS_DEFS)
LIBS		=	$(ITEMLIB) $(CONV_LIBS) $(MWOS_LIBS)	# Don't Use CSL
#LIBS		=	$(CONV_LIBS) $(ITEMLIB)	$(MWOS_CSLLIBS) # Use CSL

OPTMZ		=	-o=7 -t=1 -s=1

COMPILE		=	$(CC) $(CPARMS) $(OPTMZ) -eas=$(RDIR) $(DEFS) $(MACROS) $(COPTS)
ILINK		=	$(CC) $(CPARMS) $(OPTMZ) -k -eas=$(RDIR) $(DEFS) $(MACROS) $(COPTS)
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

$(ODIR)/ex1_rcv:  $(RELS)
	-$(CODO) $(ODIR)/ex1_rcv
	-$(DEL)  $(ODIR)/ex1_rcv
	-$(CODO) $(ODIR)/STB/ex1_rcv.stb
	-$(DEL)  $(ODIR)/STB/ex1_rcv.stb
	-$(DEL)  $(ODIR)/STB/ex1_rcv.dbg
	$(LINK)  $(CSTART) $(RELS) -fd=$(ODIR)/ex1_rcv
	$(ATTRE1) $(ODIR)/ex1_rcv

$(RDIR)/ex1_rcv.r:  ex1_rcv.c
	$(COMPILE) ex1_rcv.c

_makdir: nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:  nulltrg
	-$(CODO) $(ODIR)/ex1_rcv
	-$(DEL)  $(ODIR)/ex1_rcv
	-$(CODO) $(ODIR)/STB/ex1_rcv.stb
	-$(DEL)  $(ODIR)/STB/ex1_rcv.stb
	-$(DEL)  $(ODIR)/STB/ex1_rcv.dbg

nulltrg:  .
	$(COMMENT)
#                                                                          #
############################################################################
