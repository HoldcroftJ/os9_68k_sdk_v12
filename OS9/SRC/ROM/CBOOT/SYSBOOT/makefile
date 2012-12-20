# Makefile for all sysboot.l contents

-b

SRCDIRS		= BOOTLIB	
MAKERS		= sysboot.make

MWOS		= ../../../../..
MAKER		= ./makefile				# this file

include $(MWOS)/MAKETMPL/makeosk.tpl
-x

make.date: $(SRCDIRS) $(MAKERS)

$(SRCDIRS): $(MAKER)
	$(CHD) $@; $(MAKESUB)

$(MAKERS): $(MAKER)
	$(MAKESUB) -f=$@

# end of file
