# make file to produce system startup process modules (sysgo)
-x
-bo

SDIR		= .
RDIR		= RELS

MWOS		= ../../../..
ROOT		= ../../..				# path to base of tree

CPUROOT		= 68000					# sub dir for output
BASEROOT	= 68000					# sub dir for LIB, etc
DEFSROOT	= SRC/DEFS				# sun dir of std defs

ODIR		= $(ROOT)/$(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= ../BOOTOBJS			# local test versions

SYSLIB		= $(ROOT)/$(BASEROOT)/LIB/sys.l

MODNAME		= sysgo					# the module name

SYSGO		= sysgo   sysgo_shell   sysgo_nodisk    sysgo_tsmon
RSYSGO		= sysgo.r sysgo_shell.r sysgo_nodisk.r  sysgo_tsmon.r

RC			= r68
RFLAGS		= -q -u=. -u=$(ROOT)/$(DEFSROOT)

LC			= l68
LFLAGS		= -g -n=$(MODNAME) -l=$(SYSLIB)

# file included via "defsfile"
INCLUDES	= $(ROOT)/$(DEFSROOT)/oskdefs.d

include		$(MWOS)/MAKETMPL/makeosk.tpl

make.date _build: _makdir $(SYSGO)
	$(COMMENT)

$(SYSGO): $(SYSLIB) $(RSYSGO)
	-$(CODO) $(ODIR)/$@
	-$(CODO) $(ODIR)/STB/$@.stb
	-$(DEL)  $(ODIR)/$@
	-$(DEL)  $(ODIR)/STB/$@.stb
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RSYSGO): $(INCLUDES)

makdir _makdir:
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

purge _purge:
	$(CODO) $(ODIR)/sysgo
	-$(DEL) $(ODIR)/sysgo
	$(CODO) $(ODIR)/STB/sysgo.stb
	-$(DEL) $(ODIR)/STB/sysgo.stb
	$(CODO) $(ODIR)/sysgo_shell
	-$(DEL) $(ODIR)/sysgo_shell
	$(CODO) $(ODIR)/STB/sysgo_shell.stb
	-$(DEL) $(ODIR)/STB/sysgo_shell.stb
	$(CODO) $(ODIR)/sysgo_nodisk
	-$(DEL) $(ODIR)/sysgo_nodisk
	$(CODO) $(ODIR)/STB/sysgo_nodisk.stb
	-$(DEL) $(ODIR)/STB/sysgo_nodisk.stb
	$(CODO) $(ODIR)/sysgo_tsmon
	-$(DEL) $(ODIR)/sysgo_tsmon
	$(CODO) $(ODIR)/STB/sysgo_tsmon.stb
	-$(DEL) $(ODIR)/STB/sysgo_tsmon.stb

clean _clean:
	-$(DEL) $(RDIR)/*.r

# end of file
