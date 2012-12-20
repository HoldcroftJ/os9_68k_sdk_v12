# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Makefile for creating "sysboot.l" boot selection library file
#
# n+01 930726 gkm MWOS'ed 
#    2 930729 gkm some defs moved in MWOS, changes accordingly
#	 3 930809 gkm inet defs moved from mwos/os9/src/defs to -do-/INET. replaced
#					use of inetbootp.h with explicit includes.  moved bootp.h
#					to os9/src/defs/inet and changed dependency.
#	 4 930810 gkm moved tftp.h to mwos/os9/src/defs/inet, changed dependency
#    5 980127 mgh added search of local directory to ALLDEFS for history.h
#    6 011111 rry Fixing for new obsoleted DEFS

# Use Ultra C in COMPAT mode

-bo
-mode=compat

MWOS		= 	../../../../..
SDIR		=	.
RDIR		=	RELS

SLIB		=	../../LIB
SR			=	../../../SYSRELS
BR			=	../../LIB

INETLIB		=	../../LIB/inetboot.l

SYSDEFS		=	$(MWOS)/SRC/DEFS
OSDEFS		=	$(MWOS)/OS9/SRC/DEFS
INETDEFS	=	$(MWOS)/OS9/SRC/DEFS/INET
INETDEFSM	=	$(MWOS)/SRC/DEFS/ISP/INET
INETDEFSM2	=	$(MWOS)/SRC/DEFS/ISP
INETDEFS2	=	DEFS/INET
INETDEFS3	=	DEFS

#SCSIDEFS	=	$(MWOS)/OS9/SRC/IO/SCSI/DEFS
SCSIDEFS	=	$(MWOS)/SRC/DEFS/IO/SCSI
CBDEFS		=	../DEFS

#ALLDEFS		=	-v=$(SYSDEFS) -v=$(OSDEFS) -v=$(INETDEFS2) -v=$(INETDEFS3) \
#				-v=$(CBDEFS) -v=$(SCSIDEFS) -v=$(INETDEFS) -v-. 
ALLDEFS		=	-v=. -v=$(INETDEFS2) -v=$(INETDEFS3) -v=$(CBDEFS) \
				-v=$(SYSDEFS)  -v=$(INETDEFSM2) -v=$(OSDEFS) \
				-v=$(SCSIDEFS) -v=$(INETDEFSM) -v=$(INETDEFS)

BRFILES		=	$(RDIR)/inetboot.r $(RDIR)/lbootp.r $(RDIR)/lip.r \
				$(RDIR)/larp.r $(RDIR)/ltftp.r $(RDIR)/ethernet_in.r

RFLAGS  	=	-q
include		$(MWOS)/MAKETMPL/makeosk.tpl

CFLAGS		=	 -qst=$(TEMP) $(ALLDEFS) -o=1 

-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# compile the component files for "sysboot.l" and merge them
# all together
#
$(INETLIB): _makdir $(BRFILES)
	-$(CODO) $@
	-$(DEL)  $@
	$(MERGE) $(BRFILES) $(REDIR_OUT)$@


# # # # # # # # # # # # # # # # # # # # # # # # #
# This section recompiles the component files if
# the global defs have changed
#
$(BRFILES) : $(CBDEFS)/sysboot.h \
			 $(INETDEFSM)/bootp.h $(INETDEFSM)/tftp.h \
             $(INETDEFS3)/inetboot_p.h lip.h

_makdir:
	-$(MAKDIR) $(RDIR)
 
clean:
	-$(DEL)  $(RDIR)/inetboot.r
	-$(DEL)  $(RDIR)/lbootp.r
	-$(DEL)  $(RDIR)/lip.r
	-$(DEL)  $(RDIR)/larp.r
	-$(DEL)  $(RDIR)/ltftp.r
	-$(DEL)  $(RDIR)/ethernet_in.r

purge:
	-$(CODO) $(INETLIB)
	-$(DEL)  $(INETLIB)

