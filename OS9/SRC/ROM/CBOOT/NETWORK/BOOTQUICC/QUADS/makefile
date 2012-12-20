# makefile for INETboot:
# Boot Driver for Quads QUICC ethernet (bootp)
#
-bo
-x

SDIR		=	..
RDIR		=	RELS
DD			=	/dd

MWOS		=   ../../../../../../..
LIB			=	../../../../LIB
SCSIDEF		=	../../../../../../../OS9/SRC/IO/SCSI/DEFS
SHRSCSIDEF	=   ../../../../../../../SRC/DEFS/IO/SCSI
SYSDEFS 	=	../../../../../../../SRC/DEFS
OSDEFS		=	../../../../../../../OS9/SRC/DEFS
CPU32DEFS		=	../../../../../../../OS9/CPU32/DEFS/CPU32
INETDEFS	=	$(SYSDEFS)/INET
INETDEFS2	=	$(OSDEFS)/INET
CBDEFS		=	../../../DEFS
INBDEFS		=	../../../INETBOOT/DEFS
DASHVDEFS	=	-v=$(INBDEFS) -v=$(SCSIDEF) -v=$(SHRSCSIDEF) -v=$(CBDEFS) \
				-v=$(SYSDEFS) -v=$(OSDEFS) -v=$(INETDEFS) -v=$(INETDEFS2) \
				-v=$(CPU32DEFS)
BLIB		=	bp_quads.l

FILES		=	bootqe.r init.r term.r xmit.r recv.r machdep.r qedvr.r
COMDEFS		=	$(SHRSCSIDEF)/scsidmod.h $(SHRSCSIDEF)/cmdblk.h 
DEFS		=	../qedvr.h \
				$(INBDEFS)/inetboot_p.h $(COMDEFS)

CFLAGS		=	-r -tp=cpu32 $(DASHVDEFS) -dCBOOT -dQUADS
RFLAGS		=	-q

include		$(MWOS)/MAKETMPL/makeosk.tpl

$(LIB)/$(BLIB) : $(FILES)
	$(CHD) $(RDIR); $(MERGE) $(FILES) $(REDIR_OUT)../$@

$(FILES) : $(DEFS)
machdep.r: ./machdep.c
