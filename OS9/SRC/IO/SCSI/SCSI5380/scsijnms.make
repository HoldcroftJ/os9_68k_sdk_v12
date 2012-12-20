# makefile for SCSI-System
#
# Low-Level Subroutine for SCSI Interface - NCR5380
# This subroutine controls the specific SCSI interface for
# the target system.  It should be optimized to the specific
# mpu/cpu in use.  The compiler -O option (optimize passes)
# should NOT be used, as this module talks directly to the
# hardware.

MACHINE		= jnms

OBJ			= scsi$(MACHINE)						# the low-level module name

SDIR		= .
RDIR		= RELS/$(MACHINE)
RDUP		= ../..

MWOSROOT	= ../../../../..				# base of MWOS structure
MWOS		= $(MWOSROOT)
ROOT		= ../../../..					# base of dir structure
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/68000					# dir system for output

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS					# local test versions

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
FILES		= main.r init.r exec.r term.r irq.r misc.r

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h $(SCSIDEF)/ncr5380.h \
				$(SCSIDEF)/dma68450.h
DEFS		= ./scsi5380.h $(COMDEFS)
MAKER		= ./$(OBJ).make

SRCHDIRS	= -v=. -v=$(SCSIDEF) -v=$(SHRSCSIDEFS) -v=$(SYSDEFS) -v=$(CDEFS)

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

TMPDIR		= $(TEMP)

-mode=ucc
#CFLAGS		= -qst=$(TMPDIR) -O=0 $(SRCHDIRS) -dUSEDMA -dJNMS
CFLAGS		= $(SRCHDIRS) -dUSEDMA -dJNMS -dOSK -r -td=$(TEMP)

RC			= r68							# assembler
RFLAGS		= -q

LC			= l68							# linker
LFLAGS		= -l=$(RDUP)/$(SLIB) -O=$(RDUP)/$(ODIR)/$(OBJ) -g -msw

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ).map

$(OBJ).date _build : makdir $(FILES) $(SLIB) $(SGLUE) $(SCALL)
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT)
	$(TOUCH) $@

$(FILES) : $(DEFS) $(MAKER)

$(SGLUE) : $(SCOM)/$(GLUENAM).a
	$(CHD) $(SCOM); $(MAKE) $(GLUENAM).r

$(SCALL) : $(SCOM)/$(CALLNAM).a
	$(CHD) $(SCOM); $(MAKE) $(CALLNAM).r

main.r : main.c $(DEFS) $(MAKER)
init.r : init.c $(DEFS) $(MAKER)
exec.r : exec.c $(DEFS) $(SDIR)/io_jnms.c $(MAKER)
irq.r  : irq.c  $(DEFS) $(MAKER)
term.r : term.c $(DEFS) $(MAKER)
misc.r : misc.c $(DEFS) $(MAKER)

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

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	-$(DEL)  $(OBJ).date

# end of file
