# make file to produce system NULL device driver
# [used by /Nil (scf) and /Pipe (Pipeman)]

-x
-bo

SDIR		= .
RDIR		= RELS

ROOT		= ../../../../..		# path to base of tree
MWOS		= $(ROOT)/..

CPUROOT		= 68000					# sub dir for output
BASEROOT	= 68000					# sub dir for LIB, etc
DEFSROOT	= SRC/DEFS				# std defs

ODIR		= $(ROOT)/$(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../BOOTOBJS			# local test versions

LIBDIR		= $(ROOT)/$(BASEROOT)/LIB
SYSLIB		= $(LIBDIR)/sys.l
SYSDEFS		= $(ROOT)/SRC/DEFS/oskdefs.d

SCFSTAT		= $(LIBDIR)/scfstat.l

NULLDRV		= null
RNULLDRV	= $(RDIR)/null.r

RC			= r68
RFLAGS		= -q -u=. -u=$(ROOT)/$(DEFSROOT)

LC			= l68
LFLAGS		= -l=$(SYSLIB) -g -s -gu=0.0

include		$(MWOS)/MAKETMPL/makeosk.tpl

null.date: _makdir $(NULLDRV)
	$(COMMENT)

$(NULLDRV): $(SYSLIB) $(SCFSTAT) $(RNULLDRV)
	$(CODO) $(ODIR)/$(NULLDRV)
	$(CODO) $(ODIR)/STB/$(NULLDRV).stb
	$(CODO) $(ODIR)/STB/$(NULLDRV).map
	-$(DEL) $(ODIR)/$(NULLDRV)
	-$(DEL) $(ODIR)/STB/$(NULLDRV).stb
	-$(DEL) $(ODIR)/STB/$(NULLDRV).map
	$(LC) $(LFLAGS) $(SCFSTAT) $(RDIR)/$*.r -O=$(ODIR)/$* $(REDIR_OUT)$(ODIR)/STB/$(NULLDRV).map

$(RNULLDRV): $(SYSDEFS)

makdir _makdir: 
	-$(MAKDIR) $(ODIR)/STB
	-$(MAKDIR) $(RDIR)

clean _clean: 
	-$(DEL) $(RDIR)/*.r

purge _purge: 
	$(CODO) $(ODIR)/$(NULLDRV)
	-$(DEL) $(ODIR)/$(NULLDRV)
	$(CODO) $(ODIR)/STB/$(NULLDRV).stb
	-$(DEL) $(ODIR)/STB/$(NULLDRV).stb
	$(CODO) $(ODIR)/STB/$(NULLDRV).map
	-$(DEL) $(ODIR)/STB/$(NULLDRV).map

# end of file

