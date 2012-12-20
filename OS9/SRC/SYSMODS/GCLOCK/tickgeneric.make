# makefile to make generic tick module
-bo
-b
-x

MWOS		= ../../../..
ROOT		= ../../..					# root of tree

SDIR		= .
RDIR		= RELS

CPUROOT		= 68000						# output dir system
LIBROOT		= $(ROOT)/$(CPUROOT)/LIB	# output file dir
DEFSROOT	= SRC/DEFS					# sub dir to std defs
MACROROOT	= SRC/MACROS				# sub dir to std macros

# files included
DEFSFILES	= $(ROOT)/$(DEFSROOT)/oskdefs.d
MACROFILES	= $(ROOT)/$(MACROROOT)/os9svc.m

INCLUDES	= $(DEFSFILES) $(MACROFILES)

TICKGENERIC	= tickgeneric

TKGEN		= $(LIBROOT)/$(TICKGENERIC).r	# final output file

RC			= r68
RFLAGS_COM	= -u=. -u=$(ROOT)/$(DEFSROOT) -u=$(ROOT)/$(MACROROOT)
RFLAGS		= -q $(RFLAGS_COM) -o=$(TKGEN)
LISTFLAGS	= -lxcfd96 $(RFLAGS_COM)

include $(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make generic tick module
#
$(TICKGENERIC) _build : makdir $(LIBROOT)/$(TICKGENERIC).r
	$(COMMENT)

$(LIBROOT)/$(TICKGENERIC).r :  $(DEFSFILES)  
	-$(CODO) $@
	-$(DEL)  $@
	$(RC) $(RFLAGS) $(TICKGENERIC).a

makdir _makdir:
	-$(MAKDIR) $(LIBROOT)
 
clean _clean purge _purge:
	-$(CODO) $(LIBROOT)/$(TICKGENERIC).r
	-$(DEL)  $(LIBROOT)/$(TICKGENERIC).r


# end of file

