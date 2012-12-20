# makefile for WD33C93 chip on the VME620 host adapter.
#
# Low-Level Subroutine for SCSI Interface (WD33C93) on VME620.
# This subroutine controls the specific SCSI interface for
# the target system.  As the VME620 is a bus module, then the
# code should be compiled in a non-cpu/non-mpu specific
# manner (no "-k" please) so that the object will execute
# on any machine.  The compiler -O option (optimize passes)
# should NOT be used, as this module talks directly to the
# hardware.

MACHINE		= 620

OBJ 		= scsi$(MACHINE)				# the low-level module name

SDIR		= .
RDIR		= RELS/MVME$(MACHINE)_NODMA
RDUP		= ../..

MWOSROOT	= ../../../../..				# base of MWOS structure
MWOS		= $(MWOSROOT)
ROOT		= ../../../..					# base of dir structure
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/68000					# dir system for output

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS				# local test versions

LIB			= $(BASEROOT)/LIB

SCSIDEF		= ../DEFS						# std SCSI defs
SHRSCSIDEFS	= $(MWOSROOT)/SRC/DEFS/IO/SCSI	# shared SCSI defs
SYSDEFS		= $(ROOT)/SRC/DEFS				# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS			# std C defs

SCOM		= ../SCSICOM					# SCSI glue/common code

# Note:  xxglue.r MUST be linked first in final output
GLUENAM		= scsiglue
CALLNAM		= syscalls
SGLUE		= $(SCOM)/RELS/$(GLUENAM).r
SCALL		= $(SCOM)/RELS/$(CALLNAM).r

SLIB		= $(LIB)/sys.l
FILES		= main.r init.r exec.r term.r kick.r srq.r irqsup.r irq.r misc.r

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h $(SCSIDEF)/wd33c93.h
DEFS		= ./scsi33c93.h ./vme$(MACHINE).h $(COMDEFS)
MAKER		= ./$(OBJ)_nodma.make

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

TMPDIR		= $(TEMP)

CCDS		= -dVME$(MACHINE) #-dRECORD

SRCHDIRS	= -v=. -v=$(SCSIDEF) -v=$(SHRSCSIDEFS) -v=$(SYSDEFS) -v=$(CDEFS)

# Ultra C Compiler Control:
#  ucc mode for ANSI compatibility mode
-mode=ucc
CFLAGS		= -o=7 -a -r -to=osk -td=$(TMPDIR) $(SRCHDIRS) $(CCDS)

RC			= r68							# assembler
RFLAGS		= -q

LC			= l68
LFLAGS		= -l=$(RDUP)/$(SLIB) -O=$(RDUP)/$(ODIR)/$(OBJ)_nodma -n=$(OBJ)\
			  -g -msw -gu=0.0

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ)_nodma.map

$(OBJ)_nodma.date _build : makdir $(FILES) $(SLIB) $(SGLUE) $(SCALL)
	-$(CODO) $(ODIR)/$(OBJ)_nodma
	-$(CODO) $(ODIR)/STB/$(OBJ)_nodma.stb
	-$(CODO) $(ODIR)/STB/$(OBJ)_nodma.map
	-$(DEL)  $(ODIR)/$(OBJ)_nodma
	-$(DEL)  $(ODIR)/STB/$(OBJ)_nodma.stb
	-$(DEL)  $(ODIR)/STB/$(OBJ)_nodma.map
	-$(DEL)  $(ODIR)/STB/$(OBJ)_nodma.dbg
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT)
	$(TOUCH) $@

$(FILES) : $(DEFS) $(MAKER)

$(SGLUE) : $(SCOM)/$(GLUENAM).a
	$(CHD) $(SCOM); $(MAKE) $(GLUENAM).r

$(SCALL) : $(SCOM)/$(CALLNAM).a
	$(CHD) $(SCOM); $(MAKE) $(CALLNAM).r

makdir _makdir:
    -$(MAKDIR) $(RDIR)
    -$(MAKDIR) $(ODIR)/STB
	-$(MAKDIR) $(SCOM)/RELS
 
clean _clean:
	-$(DEL) $(RDIR)/main.r
	-$(DEL) $(RDIR)/init.r
	-$(DEL) $(RDIR)/exec.r
	-$(DEL) $(RDIR)/term.r
	-$(DEL) $(RDIR)/kick.r
	-$(DEL) $(RDIR)/srq.r
	-$(DEL) $(RDIR)/irqsup.r
	-$(DEL) $(RDIR)/irq.r
	-$(DEL) $(RDIR)/misc.r

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)_nodma
	-$(CODO) $(ODIR)/STB/$(OBJ)_nodma.stb
	-$(CODO) $(ODIR)/STB/$(OBJ)_nodma.map
	-$(DEL)  $(ODIR)/$(OBJ)_nodma
	-$(DEL)  $(ODIR)/STB/$(OBJ)_nodma.stb
	-$(DEL)  $(ODIR)/STB/$(OBJ)_nodma.map
	-$(DEL)  $(ODIR)/STB/$(OBJ)_nodma.dbg
	-$(DEL)  $(OBJ)_nodma.date

# end of file
