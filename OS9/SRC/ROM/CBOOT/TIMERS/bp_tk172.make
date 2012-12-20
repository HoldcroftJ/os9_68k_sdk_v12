# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Makefile for creating "sysboot.l" boot selection library file
#
# n+01 gdw 970326 Cloned from the vme162 version

-mode=compat

MWOS		= ../../../../..
SDIR		=	.
RDIR		=	RELS

SLIB		=	../../LIB

INETLIB		=	../../LIB/inetboot.l

SYSDEFS		=	$(MWOS)/SRC/DEFS
OSDEFS		=	$(MWOS)/OS9/SRC/DEFS

ALLDEFS		=	-v=$(OSDEFS) -v=$(SYSDEFS)


TICKERNAME	=	bp_tk172
BRFILES		=	$(TICKERNAME).r

TT			=	#-dTIMER_TEST

include		$(MWOS)/MAKETMPL/makeosk.tpl

CFLAGS		=	-mode=compat -qst=$(TEMP) $(ALLDEFS) -dOSK $(TT) -o=0
RFLAGS		=	-q -o=0
-x
-bo

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# compile the component files for "sysboot.l" and merge them
# all together
#
$(TICKERNAME).l _build : makdir $(SLIB)/$(TICKERNAME).l
	$(COMMENT)

$(SLIB)/$(TICKERNAME).l : $(BRFILES)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(BRFILES) $(REDIR_OUT)../$@



# # # # # # # # # # # # # # # # # # # # # # # # #
# This section recompiles the component files if
# the global defs have changed
#
$(BRFILES) : 

makdir _makdir:
	-$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(TICKERNAME).r
    
purge _purge:
	-$(CODO) $(SLIB)/$(TICKERNAME).l
	-$(DEL)  $(SLIB)/$(TICKERNAME).l
    

