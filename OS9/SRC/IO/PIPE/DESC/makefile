# make file to produce system PIPE descriptors

-x
-bo

SDIR		= .
RDIR		= RELS

ROOT		= ../../../..			# path to base of tree
MWOS		= $(ROOT)/..

CPUROOT		= 68000					# sub dir for output
BASEROOT	= 68000					# sub dir for LIB, etc
DEFSROOT	= SRC/DEFS				# std defs

ODIR		= $(ROOT)/$(CPUROOT)/CMDS/BOOTOBJS
#ODIR		= BOOTOBJS			# local test versions

SYSLIB		= $(ROOT)/$(BASEROOT)/LIB/sys.l
SYSDEFS		= $(ROOT)/SRC/DEFS/oskdefs.d

PIPES		= pipe   pipe2
RPIPES		= pipe.r pipe2.r

RC			= r68
RFLAGS		= -q -u=. -u=$(ROOT)/$(DEFSROOT)

LC			= l68
LFLAGS		= -l=$(SYSLIB) -gu=0.0

include $(MWOS)/MAKETMPL/makeosk.tpl

pipe.date: _makdir $(PIPES)
	$(COMMENT)

$(PIPES): $(SYSLIB) $(RPIPES)
	$(CODO) $(ODIR)/pipe
	$(CODO) $(ODIR)/pipe2
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

$(RPIPES): $(SYSDEFS)

makdir _makdir:
	-$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	$(CODO) $(ODIR)/pipe
	-$(DEL) $(ODIR)/pipe
	$(CODO) $(ODIR)/pipe2
	-$(DEL) $(ODIR)/pipe2

# end of file

