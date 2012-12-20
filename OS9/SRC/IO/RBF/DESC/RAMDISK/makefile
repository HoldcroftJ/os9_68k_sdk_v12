# make file to produce system ram disk descriptors

-x
-bo

SDIR		= ..							# src dir of descriptors
RDIR		= RELS

ROOT		= ../../../../..				# path to base of tree
MWOS		= ../$(ROOT)

CPUROOT		= 68000							# sub dir for output
BASEROOT	= 68000							# sub dir for LIB, etc
DEFSROOT	= $(ROOT)/SRC/DEFS				# std defs dir

ODIR		= $(ROOT)/$(CPUROOT)/CMDS/BOOTOBJS

SYSLIB		= $(ROOT)/$(BASEROOT)/LIB/sys.l
SYSDEFS		= $(DEFSROOT)/oskdefs.d

RBFDESC		= ../rbfdesc.a					# rbf descriptor template

DESC		= r0      r1      r2
RDESC		= r0.r    r1.r    r2.r
DESCFMT		= r0fmt   r1fmt   r2fmt			# format enabled versions
RDESCFMT	= r0fmt.r r1fmt.r r2fmt.r

RC			= r68
RFLAGS		= -q -u=. -u=$(DEFSROOT)

LC			= l68
LFLAGS		= -l=$(SYSLIB)

SYSTYPE		= systype.d

include $(MWOS)/MAKETMPL/makeosk.tpl

_build: _makdir $(ODIR)/$(DESC) $(ODIR)/$(DESCFMT)
	$(COMMENT)

$(ODIR)/$(DESC): $(SYSLIB) $(RDESC)
	$(CODO) $(ODIR)/$*
	$(CODO) $(ODIR)/dd_$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*    -n=$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/dd_$* -n=dd

$(ODIR)/$(DESCFMT): $(SYSLIB) $(RDESCFMT)
	$(CODO) $(ODIR)/$*
	$(CODO) $(ODIR)/dd_$*
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*    -n=$*

$(RDESC) $(RDESCFMT): $(SYSDEFS) $(RBFDESC) $(SYSTYPE)

_makdir makdir:
	$(MAKDIR) $(RDIR)

_purge purge:
	$(CODO) $(ODIR)/r0
	-$(DEL) $(ODIR)/r0
	$(CODO) $(ODIR)/r0fmt
	-$(DEL) $(ODIR)/r0fmt
	$(CODO) $(ODIR)/dd_r0
	-$(DEL) $(ODIR)/dd_r0
	$(CODO) $(ODIR)/r1
	-$(DEL) $(ODIR)/r1
	$(CODO) $(ODIR)/r1fmt
	-$(DEL) $(ODIR)/r1fmt
	$(CODO) $(ODIR)/dd_r1
	-$(DEL) $(ODIR)/dd_r1
	$(CODO) $(ODIR)/r2
	-$(DEL) $(ODIR)/r2
	$(CODO) $(ODIR)/r2fmt
	-$(DEL) $(ODIR)/r2fmt
	$(CODO) $(ODIR)/dd_r2
	-$(DEL) $(ODIR)/dd_r2

_clean clean:
	-$(DEL) $(RDIR)/*.r

# end of file


