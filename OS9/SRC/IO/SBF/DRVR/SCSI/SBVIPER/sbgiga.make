# make file for SCSI-System
# High-Level Driver for Exabyte 8200 Drive
#
# This driver should always be made non-cpu specific, so that
# it will always be a "portable" driver (i.e. no -k options,
# no cpu specific conditionals).  The compiler's -O option
# (optimize passes) is ok, as the high-level never talks
# directly to the hardware.

# NOTE:  The Exabyte driver is a conditionalized version of
#        the SBVIPER driver.
-mode=compat
-bo

MWOS		= ../../../../../../..
ROOT		= ../../../../../..			# root of dir system

OBJ			= sbgiga					# the high-level driver name

SDIR		= .
RDIR		= RELS/SBGIGA
RDUP		= ../..

BASEROOT	= $(ROOT)/68000				# sub dir for LIB, C DEFS
CPUROOT		= $(ROOT)/68000				# sub dir for output

SYSDEFS		= $(ROOT)/SRC/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs

ODIR		= $(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../CMDS/BOOTOBJS			# local versions

LIB			= $(BASEROOT)/LIB			# std LIB

SCSIBASE	= $(ROOT)/SRC/IO/SCSI		# SCSI Common dir system
SCSICDEF	= $(ROOT)/../SRC/DEFS/IO/SCSI	# std SCSI defs
SCSIDEF		= $(SCSIBASE)/DEFS			# std SCSI defs
SCOM		= $(SCSIBASE)/SCSICOM		# std SCSI common code

# Note:  xxglue.r MUST be linked first in final output
GLUENAM		= sbglue
CALLNAM		= syscalls
SGLUE		= $(SCOM)/RELS/$(GLUENAM).r
SCALL		= $(SCOM)/RELS/$(CALLNAM).r

SLIB		= $(LIB)/sys.l

MLIB		= $(LIB)/os_lib.l

FILES		= init.r main.r read.r write.r stat.r term.r misc.r

COMDEFS		= $(SCSICDEF)/scsidmod.h $(SCSICDEF)/cmdblk.h \
			  $(SCSICDEF)/scsicmds.h $(SYSDEFS)/dcmd.h

OSDEFS		= $(SYSDEFS)/sbf.h $(SYSDEFS)/types.h $(SYSDEFS)/MACHINE/reg.h \
			  $(SYSDEFS)/procid.h $(SYSDEFS)/path.h $(SYSDEFS)/sg_codes.h \
			  $(SYSDEFS)/modes.h $(CDEFS)/errno.h \

DEFS		= sbviper.h $(COMDEFS) $(OSDEFS)


MAKER		= ./$(OBJ).make

DEBUG		= #-dDEBUG					# debugging flags

include		$(MWOS)/MAKETMPL/makeosk.tpl

TMPDIR		= $(TEMP)						# ramdisk for temp files

SRCHDIRS	= -v=. -v=$(SCSICDEF) -v=$(SCSIDEF) -v=$(SYSDEFS) -v=$(CDEFS)
CFLAGS		= -qst=$(TMPDIR) -O=2 $(SRCHDIRS) $(DEBUG) -dEXABYTE

RC			= r68						# asssembler
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
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
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

purge _purge:
	-$(CODO) $(ODIR)/$(OBJ)
	-$(DEL)  $(ODIR)/$(OBJ)
	-$(CODO) $(ODIR)/STB/$(OBJ).stb
	-$(DEL)  $(ODIR)/STB/$(OBJ).stb
	-$(CODO) $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).map
	-$(DEL)  $(ODIR)/STB/$(OBJ).dbg
