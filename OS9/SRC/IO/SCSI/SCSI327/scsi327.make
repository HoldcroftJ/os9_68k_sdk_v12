# makefile for SCSI-System
#
# Low-Level Subroutine for MVME327 SCSI Interface.
# This subroutine controls the specific SCSI interface for
# the target system.  As the VME327 is a bus module, then the
# code should be compiled in a non-cpu/non-mpu specific
# manner (no "-k" please) so that the object will execute
# on any machine.  The compiler -O option (optimize passes)
# should NOT be used, as this module talks directly to the
# hardware.

MACHINE		= 327
CPUDIR		= 68000							# Look for CPU Specific items

OBJ			= scsi$(MACHINE)				# the low-level module name

SDIR		= .
RDIR		= RELS
RDUP		= ..

MWOSROOT	= ../../../../..				# base of MWOS structure
MWOS		= $(MWOSROOT)
ROOT		= ../../../..					# base of dir structure
BASEROOT	= $(ROOT)/$(CPUDIR)				# dir system for LIB
CPUROOT		= $(ROOT)/$(CPUDIR)				# dir system for output

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS				# local test versions
ADIR		= ASM

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

FILES		= main.r exec.r init.r term.r irq.r support.r misc.r errstat.r

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h $(SHRSCSIDEFS)/scsicmds.h
DEFS		= ./$(OBJ).h $(COMDEFS)
MAKER		= ./$(OBJ).make

DEBUGDRVR	= #-dDEBUGDRVR

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

TMPDIR		= $(TEMP)

SRCHDIRS	= -v=. -v=$(SCSIDEF) -v=$(SYSDEFS) -v=$(CDEFS) -v=./$(OBJ).h -v=$(SHRSCSIDEFS)

-mode=ucc
CFLAGS		= -o=7 -a -r -to=osk -tp=68k $(SRCHDIRS) $(DEBUGDRVR)

RC			= r68							# assembler
RFLAGS		= -qb

LC			= l68							# linker

LFLAGS		= -l=$(RDUP)/$(SLIB) -O=$(RDUP)/$(ODIR)/$(OBJ) \
				-g -msw

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ).map


$(ODIR)/$(OBJ) $(OBJ).date _build: makdir $(FILES) $(SLIB) $(SGLUE) $(SCALL)
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT) $(LMAPOUT)
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
	-$(DEL) $(RDIR)/errstat.r
	-$(DEL) $(RDIR)/exec.r
	-$(DEL) $(RDIR)/init.r
	-$(DEL) $(RDIR)/irq.r
	-$(DEL) $(RDIR)/main.r
	-$(DEL) $(RDIR)/misc.r
	-$(DEL) $(RDIR)/support.r
	-$(DEL) $(RDIR)/term.r

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	-$(CODO)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).map

# end of file

