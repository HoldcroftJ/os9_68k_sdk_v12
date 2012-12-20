# Makefile for port modules in the MVME147 ROM
-bo
-x
-mode=compat

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system

BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68030				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= .							# specific source dir

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..

VME050DEFS	= $(SRCROOT)/ROM/MVME050
BOOTDEFS	= $(SRCROOT)/ROM/CBOOT/DEFS
SHRSCSIDEFS	= $(MWOS)/SRC/DEFS/IO/SCSI	# shared SCSI defs
SCSIDEFS	= $(SRCROOT)/IO/SCSI/DEFS
SBTRDEFS	= $(SRCROOT)/ROM/CBOOT/DISK/BOOT33C93
SYSDEFS		= $(SRCROOT)/DEFS			# std OS defs
SYSMACS		= $(SRCROOT)/MACROS
CDEFS		= $(ROOT)/../SRC/DEFS		# std C defs

MAKER		= rom_port.make

SYSINIT		= sysinit.r
SYSCON		= syscon.r
OBJECTS		= $(SYSINIT) $(SYSCON)

OLIB		= rom_port.l

COMDEFS		= $(SYSDEFS)/oskdefs.d $(SYSDEFS)/nvram.d
DEFS		= ../systype.d $(COMDEFS)

SC_DEPS		= syscon.c reconfig.c ../systype.h

RBUG		= -aROMBUG
INHOUSE		= #-aINHOUSE
MBUGTRC		= #-aMBUGTRC # enables MBUG tracing and breakpoints for testing
RAMLOAD		= #-aRAMLOAD # support rombug load directly for porting
ROMSTB		= #-aROMSTB

SPEC_RFLAGS	= $(RBUG) $(INHOUSE) $(MBUGTRC) $(RAMLOAD) $(ROMSTB) #-aFASTCONS

CBUG		= #-dNOBUG
BOOTP		= -dBOOTP_LE # -dBOOTP_IE #-dBOOTP_ME #-dBOOTP_EN # -dBOOTP_CCN

SPEC_CFLAGS	= $(CBUG) $(BOOTP)

include		$(MWOS)/MAKETMPL/makeosk.tpl

TMPDIR		= $(TEMP)

CSRCHDIRS	= 	-v=. -v=.. -v=$(VME050DEFS) -v=$(BOOTDEFS) -v=$(SCSIDEFS) \
				-v=$(SHRSCSIDEFS) -v=$(SBTRDEFS) -v=$(SYSDEFS) -v=$(CDEFS)
CFLAGS		= -qst=$(TMPDIR) -O=0 -dCBOOT -dVME147 $(SPEC_CFLAGS) $(CSRCHDIRS)

RC			= r68
RSRCHDIRS	= -u=. -u=.. -u=$(VME050DEFS) -u=$(SYSDEFS) -u=$(SYSMACS)
RFLAGS		= -q -aCBOOT $(SPEC_RFLAGS) $(RSRCHDIRS)

$(OLIB) : makdir $(RDIR)/$(OLIB)
	$(COMMENT)

$(RDIR)/$(OLIB) : $(OBJECTS)
	-$(CODO) $@ 
	-$(DEL)  $@ 
	$(CHD) $(RDIR); $(MERGE)  $(MOPTS) $(OBJECTS) $(REDIR_OUT)$(OLIB)

$(SYSINIT) : $(DEFS) $(MAKER)

$(SYSCON) : $(SC_DEPS) $(MAKER)

makdir _makdir:
	-$(MAKDIR) $(RDIR)
 
clean _clean:
	-$(DEL) $(RDIR)/$(SYSINIT)
	-$(DEL) $(RDIR)/$(SYSCON)
	-$(DEL) $(RDIR)/$(OLIB)

purge _purge:
	-$(CODO) $(RDIR)/$(OLIB)
	-$(DEL)  $(RDIR)/$(OLIB)
