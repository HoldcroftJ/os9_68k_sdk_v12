# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Makefile for creating "sysboot.l" boot selection library file
#
MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir structure

SDIR        = .
RDIR        = RELS
RDUP		= ..


LIBROOT		= $(ROOT)/SRC/ROM/LIB		# output lib dir
BLIBROOT	= $(ROOT)/SRC/ROM/CBOOT/SYSBOOT/BOOTLIB

BOOTDEFS	= $(ROOT)/SRC/ROM/CBOOT/DEFS
SCSIDEFS	= $(MWOS)/SRC/DEFS/IO/SCSI	# scsi defs
SYSDEFS		= $(ROOT)/SRC/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs


BOOTLIB     = $(BLIBROOT)/RELS/bootlib.l	# boot system "library" functions

OBJECTS		= diskboot.r romboot.r tapeboot.r portboot.r loaderboot.r\
				sysboot_glue.r sysboot.r misc.r initdata.r nvram.r binboot.r

OBJ			= sysboot
OLIB		= $(OBJ).l

MAKER		= ./$(OBJ).make

include		$(MWOS)/MAKETMPL/makeosk.tpl

TMPDIR		= $(TEMP)						# ram disk for temp files

-mode=compat
-x
-bo

SRCHDIRS	= -v=. -v=$(BOOTDEFS) -v=$(SCSIDEFS) -v=$(SYSDEFS) -v=$(CDEFS)
CFLAGS		= -mode=compat -qst=$(TMPDIR) -O=2  $(SRCHDIRS) -dOSK

RC			= r68
RFLAGS		= -q

MERGEOPTS	= 

-x

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# compile the component files for "sysboot.l" and merge them
# all together
#
$(OBJ).date : $(LIBROOT)/$(OLIB) makdir
	$(TOUCH) $@

$(LIBROOT)/$(OLIB) : makdir $(OBJECTS) $(BOOTLIB) $(MAKER)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(MERGEOPTS) $(OBJECTS) $(RDUP)/$(BOOTLIB) \
		$(REDIR_OUT)$(RDUP)/$@

# # # # # # # # # # # # # # # # # # # # # # # # #
# This section recompiles the component files if
# the global defs have changed
#
$(OBJECTS) : $(BOOTDEFS)/sysboot.h $(MAKER)

romboot.r: romboot.c rombtasm.a

sysboot_glue.r: sysboot_glue.c sbtglueasm.a

sysboot.r: sysboot.c sysbtasm.a

makdir _makdir:
    -$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/diskboot.r
	-$(DEL) $(RDIR)/romboot.r
	-$(DEL) $(RDIR)/tapeboot.r
	-$(DEL) $(RDIR)/portboot.r
	-$(DEL) $(RDIR)/loaderboot.r
	-$(DEL) $(RDIR)/sysboot_glue.r
	-$(DEL) $(RDIR)/sysboot.r
	-$(DEL) $(RDIR)/misc.r
	-$(DEL) $(RDIR)/initdata.r
	-$(DEL) $(RDIR)/nvram.r
	-$(DEL) $(RDIR)/binboot.r

purge _purge:
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL)  $(LIBROOT)/$(OLIB)



