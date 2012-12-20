# make file to produce system ram disk driver
#
-x
-bo
SDIR		= .
RDIR		= RELS

ROOT		= ../../../../..		# path to base of tree
MWOS		= ../$(ROOT)

CPUROOT		= 68000					# sub dir for output
BASEROOT	= 68000					# sub dir for LIB, etc
DEFSROOT	= $(ROOT)/SRC/DEFS		# std defs dir

MACRODIR	= $(ROOT)/SRC/MACROS
MACROS		= $(MACRODIR)/os9svc.m $(MACRODIR)/ldbra.m

# files included in defsfile
INCLUDES	= $(MACROS) $(DEFSROOT)/oskdefs.d

ODIR		= $(ROOT)/$(CPUROOT)/CMDS/BOOTOBJS

LIBDIR		= $(ROOT)/$(BASEROOT)/LIB

SYSLIB		= $(LIBDIR)/sys.l
SYSDEFS		= $(DEFSROOT)/oskdefs.d

DRIVTAB		= $(LIBDIR)/drvs1.l		# link with one drive table

RAM			= ram
RRAM		= ram.r

RC			= r68
RFLAGS		= -q -u=. -u=$(DEFSROOT) -u=$(MACRODIR)

LC			= l68
LFLAGS		= -l=$(SYSLIB) -g -gu=0.0

include $(MWOS)/MAKETMPL/makeosk.tpl

$(RAM) _build: _makdir $(ODIR)/$(RAM)
	$(COMMENT)

$(ODIR)/$(RAM): $(SYSLIB) $(DRIVTAB) $(RRAM)
	$(CODO) $(ODIR)/$*
	$(CODO) $(ODIR)/STB/$*.stb
	$(LC) $(LFLAGS) $(DRIVTAB) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RRAM): $(SYSDEFS) $(INCLUDES)

makdir _makdir:
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

purge _purge:
	$(CODO) $(ODIR)/ram
	-$(DEL) $(ODIR)/ram
	$(CODO) $(ODIR)/STB/ram.stb
	-$(DEL) $(ODIR)/STB/ram.stb

clean _clean:
	-$(DEL) $(RDIR)/*.r

# end of file

