# makefile for SCSI-System:
# High-Level Driver for Common Command Set Disk Drives
#
# This driver should always be made non-cpu specific, so that
# it will always be a "portable" driver (i.e. no -k options,
# no cpu specific conditionals).  The compiler's -O option
# (optimize passes) is ok, as the high-level never talks
# directly to the hardware.

-mode=compat
-bo

MWOS		= ../../../../../../..
ROOT		= ../../../../../..			# root of dir system (OS9)

OBJ			= rbvccs					# the high-level driver name

SDIR		= .
RDIR		= RELS
RDUP		= ..

BASEROOT	= $(ROOT)/68000				# sub dir for LIB, C DEFS
CPUROOT		= $(ROOT)/68000				# sub dir for output

SYSDEFS		= $(ROOT)/SRC/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS			# local versions

LIB			= $(BASEROOT)/LIB			# std LIB

SCSIBASE	= $(ROOT)/SRC/IO/SCSI		# SCSI Common dir system
SHRSCSIDEF	= $(MWOS)/SRC/DEFS/IO/SCSI	# shared SCSI defs
SCSIDEF		= $(SCSIBASE)/DEFS			# std SCSI defs
SCOM		= $(SCSIBASE)/SCSICOM		# std SCSI common code

# Note:  xxglue.r MUST be linked first in final output
GLUENAM		= rbglue
CALLNAM		= syscalls
SGLUE		= $(SCOM)/RELS/$(GLUENAM).r 
SCALL		= $(SCOM)/RELS/$(CALLNAM).r

SLIB		= $(LIB)/sys.l

MLIB		= $(LIB)/os_lib.l

FILES		= main.r init.r read.r write.r stat.r term.r misc.r

COMDEFS		= $(SHRSCSIDEF)/scsidmod.h $(SHRSCSIDEF)/cmdblk.h \
			  $(SHRSCSIDEF)/scsicmds.h $(SYSDEFS)/dcmd.h

DEFS		= $(OBJ).h $(COMDEFS) 

MAKER		= ./$(OBJ).make

DEBUG		= #-dDEBUG						# debugging flags

include		$(MWOS)/MAKETMPL/makeosk.tpl

SRCHDIRS	= -v=. -v=$(SCSIDEF) -v=$(SYSDEFS) -v=$(CDEFS) -v=$(SHRSCSIDEF)
CFLAGS		= -qst=$(TEMP) -O=2 $(SRCHDIRS) $(DEBUG)

RC			= r68						# assembler
RFLAGS		= -qb

LC			= l68						# linker
LFLAGS		= -l=$(RDUP)/$(SLIB) -l=$(RDUP)/$(MLIB) -O=$(RDUP)/$(ODIR)/$(OBJ) -g -msw -gu=0.0

LMAPOUT		= $(REDIR_OUT)$(RDUP)/$(ODIR)/STB/$(OBJ).map

$(OBJ) _build : makdir $(ODIR)/$(OBJ)
	$(COMMENT)

$(ODIR)/$(OBJ) : $(FILES) $(SLIB) $(SGLUE) $(SCALL)
	-$(CODO) $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(DEL)	 $(ODIR)/STB/$(OBJ).map	
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
	$(CHD) $(RDIR); $(LC) $(LFLAGS) $(RDUP)/$(SGLUE) $(FILES) $(RDUP)/$(SCALL) $(LMAPOUT)

$(FILES) : $(DEFS) $(MAKER)

$(SGLUE) : $(SCOM)/$(GLUENAM).a
	$(MAKDIR) $(SCOM)/RELS
	$(CHD) $(SCOM); $(MAKE) $(GLUENAM).r

$(SCALL) : $(SCOM)/$(CALLNAM).a
	$(MAKDIR) $(SCOM)/RELS
	$(CHD) $(SCOM); $(MAKE) $(CALLNAM).r

makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB
 
clean _clean:
	-$(DEL) $(RDIR)/main.r
	-$(DEL) $(RDIR)/init.r
	-$(DEL) $(RDIR)/read.r
	-$(DEL) $(RDIR)/write.r
	-$(DEL) $(RDIR)/stat.r
	-$(DEL) $(RDIR)/term.r
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

