# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Makefile for creating "bootlib.l" boot function library file
#

MWOS		= ../../../../../..
ROOT		= ../../../../..			# base of dir structure

SDIR		= .
RDIR		= RELS
RDUP		= ..


LIBROOT		= $(RDIR)					# output lib dir

BOOTDEFS	= $(ROOT)/SRC/ROM/CBOOT/DEFS
SCSIDEFS	= $(MWOS)/SRC/DEFS/IO/SCSI	# scsi defs
SYSDEFS		= $(ROOT)/SRC/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs


OBJECTS		= sysreset.r calldebug.r inttoascii.r inizbootdriver.r\
				maskirq.r powerof2.r setexcpt.r gethexaddr.r convhex.r\
				instr.r outstr.r romio.r talkmode.r streq.r makelower.r\
				hwprobe.r getbootmem.r insert.r extract.r

OBJ			= bootlib
OLIB		= $(OBJ).l

MAKER		= ./$(OBJ).make

include		$(MWOS)/MAKETMPL/makeosk.tpl

-mode=compat
-x
-bo

TMPDIR		= $(TEMP)						# ram disk for temp files

SRCHDIRS	= -v=. -v=$(BOOTDEFS) -v=$(SCSIDEFS) -v=$(SYSDEFS) -v=$(CDEFS)
CFLAGS		= -mode=compat -qst=$(TMPDIR) -O=2 $(SRCHDIRS) -dOSK

RC			= r68
RFLAGS		= -q

MERGEOPTS	= 


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# compile the component files for "sysboot.l" and merge them
# all together
#
$(OBJ).date : makdir $(LIBROOT)/$(OLIB)
	$(TOUCH) $@

$(LIBROOT)/$(OLIB) : $(OBJECTS)
	-$(CODO) $@
	-$(DEL)  $@
	$(CHD) $(RDIR); $(MERGE) $(MERGEOPTS) $(OBJECTS) \
		$(REDIR_OUT)$(RDUP)/$@

$(OBJECTS) : $(MAKER)

hwprobe.r: hwprobe.c hwprbasm.a

makdir _makdir:
    -$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/sysreset.r
	-$(DEL) $(RDIR)/calldebug.r
	-$(DEL) $(RDIR)/inttoascii.r
	-$(DEL) $(RDIR)/inizbootdriver.r
	-$(DEL) $(RDIR)/maskirq.r
	-$(DEL) $(RDIR)/powerof2.r
	-$(DEL) $(RDIR)/setexcpt.r
	-$(DEL) $(RDIR)/gethexaddr.r
	-$(DEL) $(RDIR)/convhex.r
	-$(DEL) $(RDIR)/instr.r
	-$(DEL) $(RDIR)/outstr.r
	-$(DEL) $(RDIR)/romio.r
	-$(DEL) $(RDIR)/talkmode.r
	-$(DEL) $(RDIR)/streq.r
	-$(DEL) $(RDIR)/makelower.r
	-$(DEL) $(RDIR)/hwprobe.r
	-$(DEL) $(RDIR)/getbootmem.r
	-$(DEL) $(RDIR)/insert.r
	-$(DEL) $(RDIR)/extract.r

purge _purge:
	-$(CODO) $(LIBROOT)/$(OLIB)
	-$(DEL)  $(LIBROOT)/$(OLIB)


