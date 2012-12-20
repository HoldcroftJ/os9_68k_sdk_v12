# Boot Driver for Teac-FC1 high-level SCSI
#
# This driver should always be made non-cpu specific, so that
# it will always be a "portable" driver (i.e. no -k options,
# no cpu specific conditionals).  The compiler's -O option
# (optimize passes) is NOT ok, as the driver talks directly
# to the hardware.

SDIR		= .
RDIR		= RELS/BOOTFTEAC
RDUP		= ../..

MWOS		= ../../../../../../..
ROOT		= ../../../../../..			# base of dir structure

LIBROOT		= $(ROOT)/SRC/ROM/LIB		# output lib dir
#LIBROOT	= TEST

BOOTDEFS	= $(ROOT)/SRC/ROM/CBOOT/DEFS
SCSICDEFS	= $(ROOT)/../SRC/DEFS/IO/SCSI	# std SCSI defs
SCSIDEFS	= $(ROOT)/SRC/IO/SCSI/DEFS	# scsi defs
SYSDEFS		= $(ROOT)/SRC/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs

OBJECTS		= bootfteac.r init.r read.r misc.r

OBJ			= bootfc1
OLIB		= $(OBJ).l

MAKER		= ./$(OBJ).make

COMDEFS		= $(SCSICDEFS)/scsidmod.h $(SCSICDEFS)/cmdblk.h \
				$(SCSICDEFS)/scsidefs.h $(SCSICDEFS)/scsicmds.h 

DEFS		= ./rbteac.h ./rbproto.h $(COMDEFS)

SRCHDIRS	= -v=. -v=$(BOOTDEFS) -v=$(SCSICDEFS) -v=$(SCSIDEFS) \
			  -v=$(SYSDEFS) -v=$(CDEFS)

CFLAGS		= -o=7 -x=ao -a -c -r -to=osk -tp=68k -td=. \
				$(SRCHDIRS) -dCBOOT

RFLAGS		= -q

-x
-bo

include $(MWOS)/MAKETMPL/makeosk.tpl

$(OLIB) _build : makdir $(LIBROOT)/$(OLIB)
	$(COMMENT)

$(LIBROOT)/$(OLIB) : $(OBJECTS)
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL)  $(LIBROOT)/$(OLIB)
	$(CHD) $(RDIR); $(MERGE) $(OBJECTS) $(REDIR_OUT)$(RDUP)/$@

$(OBJECTS) : $(DEFS) $(MAKER)

makdir _makdir : $(RDIR)

$(RDIR):
	$(MAKDIR) $@

purge _purge : .
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL)  $(LIBROOT)/$(OLIB)

clean _clean : .
	-$(DEL) $(RDIR)/*.r



