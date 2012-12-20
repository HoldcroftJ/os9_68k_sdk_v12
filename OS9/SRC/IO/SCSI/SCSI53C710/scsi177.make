# makefile for SCSI-System
#
# Low-Level Subroutine for SCSI Interface on Motorola mVme177 SBC
# This subroutine controls the specific SCSI interface for
# the target system.  It should be optimized to the specific
# mpu/cpu in use.  The compiler -O option (optimize passes)
# should NOT be used, as this module talks directly to the
# hardware.

MACHINE		= 177
CPUDIR		= 68060							# Look for CPU Specific items

OBJ			= scsi$(MACHINE)				# the low-level module name

SDIR		= .
RDIR		= RELS/MVME$(MACHINE)
RDUP		= ../..

MWOS		= ../../../../..				# base of MWOS structure
SRCROOT     = $(MWOS)/SRC/IO/SCSI/NCRSIOP
ROOT		= ../../../..					# base of dir structure
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/$(CPUDIR)/PORTS/MVME$(MACHINE)	# dir system for output

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS				# local test versions

LIB			= $(BASEROOT)/LIB
LIB020      = $(ROOT)/68020/LIB
LIB040      = $(ROOT)/68040/LIB
LIB060      = $(ROOT)/68060/LIB

SCSIDEF		= ../DEFS						# std SCSI defs (OS-9 specific)
SHRSCSIDEFS	= $(MWOS)/SRC/DEFS/IO/SCSI		# shared SCSI defs
SYSDEFS		= $(ROOT)/SRC/DEFS				# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS			# std C defs
HWDEFS		= $(ROOT)/../SRC/DEFS/HW		# std C HW defs
SRCDEFS     = $(SRCROOT)					# source defs
SCOM		= ../SCSICOM					# SCSI glue/common code

# Note:  xxglue.r MUST be linked first in final output
GLUENAM		= scsiglue
CALLNAM		= syscalls
SGLUE		= $(SCOM)/RELS/$(GLUENAM).r
SCALL		= $(SCOM)/RELS/$(CALLNAM).r

SLIB		= $(LIB)/sys.l
OSLIB       = $(LIB020)/os_lib.l 
CPULIB		= $(LIB)/cpu.l 

FILES		= main.r init.r exec.r term.r irq.r misc.r dev$(MACHINE).r \
				irqsup.r hw53c710.r

LCLDEFS		= $(SRCDEFS)/scsisiop.h $(HWDEFS)/ncrsiop.h ./dev$(MACHINE).h \
			  $(SRCDEFS)/ncrproto.h

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h
DEFS		= $(COMDEFS) $(LCLDEFS)
MAKER		= ./$(OBJ).make

#  The following is a summation of the compile time flags.
#		-dVME$(MACHINE) enables mVme$(MACHINE) specific code
#
#       -dRSTCODE includes a SCSI reset in the high level driver and
#                 is intended for use with systems that do not boot
#                 from the SCSI drive.
#       -dSTATS   includes debugging code that logs interrupts and other
#                 interesting statistics.  (some speed penalty)
#       -dSCLK    define SIOP core clock
#       -dALTID   changes code to use alternate id (6) for host adapter
#       -SLOW_SCSI allow for SCSI drives that do not meet SCSI spec.
#

SCLK		= #-dSCLK=50
ALTID		= #-dALTID6

PFLAGS		= -dVME$(MACHINE) $(SCLK) \
			  -dNCR53C710 $(ALTID) # -dSTATS -dSLOW_SCSI
SYNCH		= #-dNOSYNC			# disable synchronous support (passively)

SRCHDIRS	= -v=$(SRCDEFS) -v. -v=$(SCSIDEF)  -v=$(SHRSCSIDEFS) -v=$(SYSDEFS) \
			  -v=$(HWDEFS) -v=$(CDEFS)

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

TMPDIR		= $(TEMP)

#-mode=compat
#CFLAGS		= -qst=$(TMPDIR) -O=0 -k2wcw $(SRCHDIRS) $(PFLAGS) $(SYNCH)

-mode=ucc
CFLAGS		= -o=7 -x=ao -a -r -to=osk -tp=060 -td=$(TMPDIR) \
				$(SRCHDIRS) $(PFLAGS) $(SYNCH)

RC			= r68							# assembler 
RFLAGS		= -q


LC			= l68							# linker

LFLAGS		= -l=$(RDUP)/$(SLIB) -l=$(RDUP)/$(OSLIB) -l=$(RDUP)/$(CPULIB) \
			  -O=$(RDUP)/$(ODIR)/$(OBJ) -g -m -s -w -gu=0.0

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ).map



$(OBJ) _build : makdir $(ODIR)/$(OBJ)
	$(COMMENT)

$(ODIR)/$(OBJ) : $(FILES) $(SLIB) $(SGLUE) $(SCALL)
	$(CODO) $(ODIR)/$(OBJ)
	$(CODO) $(ODIR)/STB/$(OBJ).stb
	$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL) $(ODIR)/$(OBJ)
	-$(DEL) $(ODIR)/STB/$(OBJ).stb
	-$(DEL) $(ODIR)/STB/$(OBJ).dbg
	-$(DEL) $(ODIR)/STB/$(OBJ).map
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT)

$(FILES) : $(DEFS) $(MAKER)

dev$(MACHINE).r : dev$(MACHINE).c dv$(MACHINE)asm.a dbg16x.a nbt16x.a

hw53c710.r : $(SRCROOT)/hw53c710.c $(SRCROOT)/v53c710.c

main.r : $(SRCROOT)/main.c

init.r : $(SRCROOT)/init.c

exec.r : $(SRCROOT)/exec.c

term.r : $(SRCROOT)/term.c

misc.r : $(SRCROOT)/misc.c

irq.r  : $(SRCROOT)/irq.c

irqsup.r : $(SRCROOT)/irqsup.c

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
	-$(DEL) $(RDIR)/irq.r
	-$(DEL) $(RDIR)/misc.r
	-$(DEL) $(RDIR)/dev$(MACHINE).r
	-$(DEL) $(RDIR)/irqsup.r
	-$(DEL) $(RDIR)/hw53c710.r

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg

# end of file


