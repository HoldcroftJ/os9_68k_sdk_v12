# Makefile for all platform-independent ROM code.
-x
-b

MWOS		= ../../../../../..
MAKER		= ./makefile				# this file

MAKES		= \
				VME162 \
				VME167 \
				VME172 \
				VME177

include $(MWOS)/MAKETMPL/makeosk.tpl

make.date: $(MAKES) $(MAKER)

$(MAKES):  $(MAKER)
	$(CHD) $@ ; $(MAKESUB)

# end of file
