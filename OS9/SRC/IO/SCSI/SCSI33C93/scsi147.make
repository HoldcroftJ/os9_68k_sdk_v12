# makefile for SCSI-System
#
# Low-Level Subroutine for SCSI Interface (WD33C93) on MVME147.
# This subroutine controls the specific SCSI interface for
# the target system.  It should be optimized to the specific
# mpu/cpu in use.  
#
#  A special version of the vme147 code can be created by entering
#  the following command line:
#		make -ubf=scsi147.make traper
#
#  This version will trap off to the system debugger if the 
#  SCSI reset interrupt is fired.  The trap occurs before the
#  status is cleared.  In any event, the code for the vme147
#  now records the number of time it sees the SCSI reset.
#

MACHINE		= 147
CPUDIR		= 68030							# Look for CPU Specific items

OBJ			= scsi$(MACHINE)				# the low-level module name

SDIR		= .
RDIR		= RELS/MVME$(MACHINE)
RDUP		= ../..

MWOSROOT	= ../../../../..				# base of MWOS structure
MWOS		= $(MWOSROOT)
ROOT		= ../../../..					# base of dir structure
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/$(CPUDIR)/PORTS/MVME$(MACHINE)	# dir system for output

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
FILES		= main.r init.r exec.r term.r kick.r srq.r irqsup.r irq.r misc.r

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h $(SCSIDEF)/wd33c93.h
DEFS		= ./scsi33c93.h ./dmapcc.h $(COMDEFS)
MAKER		= ./$(OBJ).make

BUGV		= #.bugt
BUGD		= #-dINTTRAP
TRAPER		= traper

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

TMPDIR		= $(TEMP)

CCDS		= -dVME$(MACHINE) -dSYNCHRO -dUSEDMA # -dRECORD

SRCHDIRS	= -v=. -v=$(SCSIDEF) -v=$(SHRSCSIDEFS) -v=$(SYSDEFS) -v=$(CDEFS)

# Ultra C Compiler Control:
#  ucc mode for ANSI compatibility mode
-mode=ucc
CFLAGS		= -x=ao -o=7 -a -r -to=osk -tp=$(CPUDIR)c -td=$(TMPDIR) \
				$(SRCHDIRS) $(CCDS) $(BUGD)

RC			= r68							# assembler
RFLAGS		= -q

LC			= l68
LFLAGS		= -l=$(RDUP)/$(SLIB) -O=$(RDUP)/$(ODIR)/$(OBJ)$(BUGV) -n=$(OBJ) \
				-g -msw 

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ).map

$(OBJ) _build : makdir $(ODIR)/$(OBJ)

$(ODIR)/$(OBJ) : $(FILES) $(SLIB) $(SGLUE) $(SCALL)
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT)

$(FILES) : $(DEFS) $(MAKER)

$(SGLUE) : $(SCOM)/$(GLUENAM).a
	$(CHD) $(SCOM); $(MAKE) $(GLUENAM).r

$(SCALL) : $(SCOM)/$(CALLNAM).a
	$(CHD) $(SCOM); $(MAKE) $(CALLNAM).r

$(TRAPER): $(TRAPER).date
	$(MAKE) -uf=$(MAKER) "BUGV =.bugt" "BUGD=-dINTTRAP"
	$(TOUCH) $(TRAPER).date



#  to look at the generated assembly output, at the command line type:
#       make -ubf=scsi$(MACHINE).make asm   				(for ucc mode)
#		make -mode=compat -ubf=scsi$(MACHINE).make asm   	(for compat mode)
#

asm: $(MAKER)

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
	-$(CODO) $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg

# end of file

