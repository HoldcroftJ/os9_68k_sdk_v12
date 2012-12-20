# Makefile for boot portion of MVME147 ROM.
-b
-x

MWOS		= ../../../../..

TYPE		= ROMBUG
ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

MAKER		= ./rom_booters.make			# this file

TARGET		= rombug
RBGSTB		= #$(ODIR)/STB/$(TARGET).stb

FILES		= $(ODIR)/$(TARGET) $(ODIR)/initext $(RBGSTB)

OFILE		= $(ODIR)/booters
OFSIZE		= 0x20000
#OFSIZE		= 0x28000	# size when ROMSTB/RBGSTB defined

MOPTS		= -b100
PROPTS		= $(OFSIZE) $(OFILE)

include		$(MWOS)/MAKETMPL/makeosk.tpl

PADROMIT	= $(PADROM) $(PROPTS)

$(OFILE): $(FILES) $(MAKER)
	-$(CODO) $@
	-$(DEL)  $@
	$(MERGE) $(MOPTS) $(FILES) $(REDIR_OUT)$@ ; $(PADROMIT)

makdir _makdir:
    -$(MAKDIR) $(ODIR)
 
clean _clean:
	-$(CODO) $(OFILE)
	-$(DEL)  $(OFILE)

purge _purge:
	-$(CODO) $(OFILE)
	-$(DEL)  $(OFILE)


# end of file
