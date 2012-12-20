# Makefile for the MVME177 Init Modules

-x
-bo

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/SYSMODS/INIT	# source dir
RDIR		= RELS
ODIR		= ../CMDS/BOOTOBJS/INITS

OSDEFS		= $(SRCROOT)/DEFS			# std OS defs

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../systype.d

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Init module type specific RFlags 
#
RAMLOAD     = #-aRAMLOAD
ROMSTB      = #-aROMSTB
RFLAGS_ROM  = -aROMBOOT $(RAMLOAD) $(ROMSTB)    # flags for rom based
RFLAGS_FLOP =                           # flags for floppies "d0" (none)
RFLAGS_HARD = -aDEVHARD                 # flags for Hard disks "h0"
RFLAGS_TAPE = -aDEVTAPE                 # flags for tape based

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Init module  SET TYPE HERE
#
INIT_NAME	= rom						# filename extension
RFLAGS_TYPE = $(RFLAGS_ROM)

INIT		= init						# Init module name
INITNAME	= $(INIT)_$(INIT_NAME)		# Filename for module
INITSRC		= $(SDIR)/$(INIT).a			# source file

MAKER       = $(INITNAME).make    		# name of this file

RC			= r68						# assembler
RFLAGS		= -qb -u=. -u=$(OSDEFS)


LC			= l68						# linker
LFLAGS		= -l=$(SLIB) -n=$(INIT) -gu=0.0


include		$(MWOS)/MAKETMPL/makeosk.tpl


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link Init module 
#
$(INITNAME) _build : makdir $(ODIR)/$(INITNAME) 
	$(COMMENT) 

$(ODIR)/$(INITNAME) : $(SLIB) $(INITNAME).r 
	-$(CODO) $(ODIR)/$(INITNAME)
	-$(DEL)  $(ODIR)/$(INITNAME)
	-$(CODO) $(ODIR)/STB/$(INITNAME).stb
	-$(DEL)  $(ODIR)/STB/$(INITNAME).stb
	-$(DEL)  $(ODIR)/STB/$(INITNAME).dbg
	$(LC) $(LFLAGS) $(RDIR)/$(INITNAME).r -O=$(ODIR)/$(INITNAME)

$(INITNAME).r: $(INITSRC) $(SYSDEFS) $(MAKER)
	$(RC) $(RFLAGS) $(RFLAGS_TYPE) -o=$(RDIR)/$@ $(INITSRC)

makdir _makdir: 
	$(MAKDIR) $(RDIR) 
	$(MAKDIR) $(ODIR)/STB
		
clean _clean:
	-$(DEL) $(RDIR)/$(INITNAME).r

purge _purge:
	-$(CODO) $(ODIR)/$(INITNAME)
	-$(DEL)  $(ODIR)/$(INITNAME)
	-$(CODO) $(ODIR)/STB/$(INITNAME).stb
	-$(DEL)  $(ODIR)/STB/$(INITNAME).stb
	-$(DEL)  $(ODIR)/STB/$(INITNAME).dbg
								


# end of file

