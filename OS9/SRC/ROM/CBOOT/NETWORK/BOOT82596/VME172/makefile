#############################################################################
#                                                                           #
#            Copyright 1997-2000 by Microware Systems Corporation           #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
#                                                                           #
# n+01 970331 cloned from mvme162 version                             gdw   #
# n+02 011112 fixed location of INET defs							  rry   #
#             $$                    <RELEASE_INFO>                    $$    #
#############################################################################
# makefile for INETboot:
# Boot Driver for i82596 ethernet (bootp)
#############################################################################

-mode=compat
-bo

MWOS		= ../../../../../../..

SDIR		=	..
RDIR		=	RELS

LIB			=	../../../../LIB
SCSIDEF		=	$(MWOS)/SRC/DEFS/IO/SCSI
SYSDEFS 	=	$(MWOS)/SRC/DEFS
SYSDEFSINET	=	$(MWOS)/SRC/DEFS/ISP
OSDEFS		=	$(MWOS)/OS9/SRC/DEFS
INETDEFS	=	$(OSDEFS)/INET
CBDEFS		=	../../../DEFS
INBDEFS		=	../../../INETBOOT/DEFS
DASHVDEFS	=	-v=$(INBDEFS) -v=$(SCSIDEF) -v=$(CBDEFS) \
				-v=$(OSDEFS) -v=$(INETDEFS) -v=$(SYSDEFS) -v=$(SYSDEFSINET)

BLIB		=	bp_172_82596.l

FILES		=	boot82596.r init.r term.r xmit.r recv.r machdep.r misc.r \
				i82596.r addr.r

COMDEFS		=	$(SCSIDEF)/scsidmod.h $(SCSIDEF)/cmdblk.h 
DEFS		=	$(INETDEFS)/ie.h $(INETDEFS)/i82596.h $(INETDEFS)/if162167.h \
				$(INBDEFS)/inetboot_p.h $(COMDEFS)

include		$(MWOS)/MAKETMPL/makeosk.tpl

CFLAGS		=	-qst=$(TEMP) $(DASHVDEFS) -dCBOOT -dif172 -die172 -o=0
RFLAGS		=	-q -o=0

$(BLIB) : makdir $(LIB)/$(BLIB)
	$(COMMENT)

$(LIB)/$(BLIB) : $(FILES)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(FILES) $(REDIR_OUT)../$@

$(FILES) : $(DEFS)

machdep.r: ./machdep.c

addr.r: ./addr.c


makdir _makdir: $(RDIR)

$(RDIR):
	-$(MAKDIR) $@
 
clean _clean: .

	-$(DEL) $(RDIR)/boot82596.r
	-$(DEL) $(RDIR)/init.r
	-$(DEL) $(RDIR)/term.r
	-$(DEL) $(RDIR)/xmit.r
	-$(DEL) $(RDIR)/recv.r
	-$(DEL) $(RDIR)/machdep.r
	-$(DEL) $(RDIR)/misc.r
	-$(DEL) $(RDIR)/i82596.r
	-$(DEL) $(RDIR)/addr.r

purge _purge: .
	-$(CODO) $(LIB)/$(BLIB)
	-$(DEL)  $(LIB)/$(BLIB)


