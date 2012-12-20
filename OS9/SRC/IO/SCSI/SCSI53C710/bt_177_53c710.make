# Boot Driver for NCR53C710 on MVME177
#

MACHINE		= 177

SDIR		= .
RDIR		= RELS/BOOT$(MACHINE)
RDUP		= ../..

MWOS		= ../../../../..			# base of MWOS structure
ROOT		= ../../../..				# base of dir structure
MWOSROOT	= $(MWOS)					# Set to MWOS incase MWOS set externally
SRCROOT     = $(MWOSROOT)/SRC/IO/SCSI/NCRSIOP

LIBROOT		= $(ROOT)/SRC/ROM/LIB		# output lib dir
#LIBROOT		= LIB						# test dir

BOOTDEFS	= $(ROOT)/SRC/ROM/CBOOT/DEFS
SCSIDEFS	= $(ROOT)/SRC/IO/SCSI/DEFS	# scsi defs (OS-9 specific)
SHRSCSIDEFS	= $(MWOSROOT)/SRC/DEFS/IO/SCSI	# shared SCSI defs
SYSDEFS		= $(ROOT)/SRC/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs
HWDEFS		= $(ROOT)/../SRC/DEFS/HW	# std C HW defs
SRCDEFS     = $(SRCROOT)				# source defs


OBJECTS		= init.r exec.r term.r irq.r misc.r dev$(MACHINE).r irqsup.r hw53c710.r

OBJ			= bt_$(MACHINE)_53c710
OLIB		= $(OBJ).l

MAKER		= ./$(OBJ).make

COMDEFS		= $(SHRSCSIDEFS)/scsidmod.h $(SHRSCSIDEFS)/cmdblk.h
LCLDEFS		= $(SRCDEFS)/scsisiop.h $(HWDEFS)/ncrsiop.h \
			  $(SRCDEFS)/ncrproto.h 

DEFS		= ./dev$(MACHINE).h $(LCLDEFS) $(COMDEFS)

SRCHDIRS	= 	-v=$(SRCDEFS) -v=. -v=$(BOOTDEFS) -v=$(SCSIDEFS) -v=$(SHRSCSIDEFS) \
				-v=$(SYSDEFS) -v=$(HWDEFS) -v=$(CDEFS)

#  The following is a summation of the compile time flags.
#       -dSTATS should NOT be used for the cboot version!!!
#       -dVME$(MACHINE) enables machine specific code
#       -dSCLK    define SIOP core clock (when fixed)
#       -dALTID6  changes code to use alternate id (6) for host adapter
#       -SLOW_SCSI allow for SCSI drives that do not meet SCSI spec.

SCLK        = #-dSCLK=50
ALTID6      = #-dALTID6

PFLAGS		= -dVME$(MACHINE) -dCBOOT \
			  -dNCR53C710 $(SCLK) $(ALTID6) #-dSLOW_SCSI


include     $(MWOS)/MAKETMPL/makeosk.tpl

TMPDIR		= $(TEMP)						# ram disk for temp files(Resident)

#-mode=compat
#CFLAGS		= -qst=$(TMPDIR) -O=0 $(SRCHDIRS) $(PFLAGS) -k2wcw

-mode=ucc
CFLAGS		= -o=7 -x=ao -a -r -to=osk -tp=060 -td=$(TMPDIR) \
				$(SRCHDIRS) $(PFLAGS) 

RC			= $(ASM)		# actually uses CC
RFLAGS		= -asq

-x
-bo

$(OBJ).date _build : $(LIBROOT)/$(OLIB)
	$(TOUCH) $(OBJ).date

$(LIBROOT)/$(OLIB) : makdir $(OBJECTS)
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL)  $(LIBROOT)/$(OLIB)
	$(CHD) $(RDIR); $(MERGE) $(OBJECTS) $(REDIR_OUT)$(RDUP)/$@

$(OBJECTS) : $(DEFS) $(MAKER)

dev$(MACHINE).r: dev$(MACHINE).c dv$(MACHINE)asm.a cbt16x.a

hw53c710.r : $(SRCROOT)/hw53c710.c $(SRCROOT)/v53c710.c

init.r : $(SRCROOT)/init.c

exec.r : $(SRCROOT)/exec.c

term.r : $(SRCROOT)/term.c

misc.r : $(SRCROOT)/misc.c

irq.r  : $(SRCROOT)/irq.c

irqsup.r : $(SRCROOT)/irqsup.c


makdir _makdir:
	-$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/init.r
	-$(DEL) $(RDIR)/exec.r
	-$(DEL) $(RDIR)/term.r
	-$(DEL) $(RDIR)/irq.r
	-$(DEL) $(RDIR)/misc.r
	-$(DEL) $(RDIR)/dev$(MACHINE).r
	-$(DEL) $(RDIR)/irqsup.r
	-$(DEL) $(RDIR)/hw53c710.r

purge _purge:
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL)  $(LIBROOT)/$(OLIB)

