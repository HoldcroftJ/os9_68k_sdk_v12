# Makefile for the boot descriptors in the MVME172 ROM.
-mode=compat
-x
-bo

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system

BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/ROM/CBOOT/DISK/DESC

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

VME050DEFS	= $(SRCROOT)/ROM/MVME050
BOOTDEFS	= $(SRCROOT)/ROM/CBOOT/DEFS
SCSIDEFS	= $(SRCROOT)/IO/SCSI/DEFS	# scsi defs
SYSDEFS		= $(SRCROOT)/DEFS			# std OS defs
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs

MAKER		= rom_descriptors.make

OBJECTS		= sccs_hbd.r fteac_fbd.r #v327_fbd.r

OLIB		= rom_descriptors.l

COMDEFS		= $(BOOTDEFS)/bootdesc.h
DEFS		= ../systype.h $(COMDEFS)

include		$(MWOS)/MAKETMPL/makeosk.tpl

TMPDIR		= $(TEMP)

CSRCHDIRS	= -v=. -v=$(VME050DEFS) -v=$(BOOTDEFS) -v=$(SCSIDEFS) \
				-v=$(SYSDEFS) -v=$(CDEFS)
CFLAGS		= -mode=compat -qst=$(TMPDIR) -O=2 $(CSRCHDIRS) -dCBOOT -dVME172


$(RDIR)/$(OLIB) : makdir $(OBJECTS)
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)
	$(CHD)   $(RDIR); $(MERGE) $(MOPTS) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(OBJECTS) : $(DEFS) $(MAKER)

makdir _makdir:
    -$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/sccs_hbd.r
	-$(DEL) $(RDIR)/fteac_fbd.r
	-$(DEL)  $(RDIR)/$(OLIB)

purge _purge:
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)

