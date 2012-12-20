# makefile for SCSI-System
#
# Low-Level Subroutine for SCSI Interface (NCR53C94) on 68340 IOP
# This subroutine controls the specific SCSI interface for
# the target system.  It should be optimized to the specific
# mpu/cpu in use.  The compiler -O option (optimize passes)
# should NOT be used, as this module talks directly to the
# hardware.

OBJ			= scsiiop						# the low-level module name

SDIR		= .
RDIR		= RELS
RDUP		= ..

MWOSROOT	= ../../../../..				# base of MWOS structure
MWOS		= $(MWOSROOT)
ROOT		= ../../../..					# base of dir structure
BASEROOT	= $(ROOT)/68000					# dir system for LIB
CPUROOT		= $(ROOT)/CPU32					# dir system for output

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS

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
FILES		= main.r init.r exec.r term.r kick.r irq.r misc.r

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h $(SCSIDEF)/ncr53c94.h
DEFS		= ./scsi53c94.h $(SCSIDEF)/dma68340.h $(COMDEFS)
MAKER		= ./$(OBJ).make

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

TMPDIR		= $(TEMP)

SRCHDIRS	= -v=. -v=$(SCSIDEF) -v=$(SHRSCSIDEFS) -v=$(SYSDEFS) -v=$(CDEFS)

PFLAGS		= -dIOP340 -dUSEDMA -dDMAIRQ # -dSYNCHRO -dRECORD

-mode=ucc
CFLAGS		= -r -td=$(TMPDIR) -tp=020,sc,sd $(SRCHDIRS) $(PFLAGS)

RC			= r68							# assembler
RFLAGS		= -q

LC			= l68
LFLAGS		= -l=$(RDUP)/$(SLIB) -O=$(RDUP)/$(ODIR)/$(OBJ) -g -msw

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ).map

$(OBJ) _build : makdir $(ODIR)/$(OBJ)
	$(COMMENT)

# don't build the logging utility as it 'needs work' (per wwb)
$(ODIR)/$(OBJ) _build : makdir $(FILES) $(SLIB) $(SGLUE) $(SCALL) #log
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT)

$(FILES) : $(DEFS) $(MAKER)

$(SGLUE) : $(SCOM)/$(GLUENAM).a
	$(CHD) $(SCOM); $(MAKE) $(GLUENAM).r

$(SCALL) : $(SCOM)/$(CALLNAM).a
	$(CHD) $(SCOM); $(MAKE) $(CALLNAM).r

log:	log.r
	$(CHD) $(RDIR); $(CC) -qg log.r -fd=$(RDUP)/$(ODIR)/../$@

log.r:	scsi53c94.h

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
	-$(DEL) $(RDIR)/irq.r
	-$(DEL) $(RDIR)/misc.r
	-$(DEL) $(SCOM)/RELS/$(GLUENAM).r
	-$(DEL) $(SCOM)/RELS/$(CALLNAM).r

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg

# end of file
