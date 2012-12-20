# make file to produce system /NIL descriptor

-x
-bo

SDIR		= .
RDIR		= RELS

ROOT		= ../../../../..		# path to base of tree
MWOS		= $(ROOT)/..

CPUROOT		= 68000					# sub dir for output
BASEROOT	= 68000					# sub dir for LIB, etc
DEFSROOT	= $(ROOT)/SRC/DEFS		# std defs dir

ODIR		= $(ROOT)/$(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../BOOTOBJS			# local test versions

SYSLIB		= $(ROOT)/$(BASEROOT)/LIB/sys.l
SYSDEFS		= $(DEFSROOT)/oskdefs.d

DESC		= nil
RDESC		= $(RDIR)/nil.r

RC			= r68
RFLAGS		= -q -u=. -u=$(DEFSROOT)

LC			= l68
LFLAGS		= -l=$(SYSLIB) -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl

nil.date: _makdir $(DESC)
	$(COMMENT)

$(DESC): $(SYSLIB) $(RDESC)
	$(CODO) $(ODIR)/$(DESC)
	-$(DEL) $(ODIR)/$(DESC)
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RDESC): $(SYSDEFS)

makdir _makdir:
	-$(MAKDIR) $(ODIR)/STB
	-$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	$(CODO) $(ODIR)/$(DESC)
	-$(DEL) $(ODIR)/$(DESC)

# end of file

