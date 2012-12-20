# Makefile for all platform-independent ROM code.
#
# Note: bp_tk360.make is now made from ports directory.
#
-x
-b

MWOS		= ../../../../..
MAKER		= ./makefile				# this file

MAKES		= \
				bp_tk162.make \
				bp_tk167.make \
				bp_tk172.make \
				bp_tk177.make \
				bp_tk147.make \
				bp_133_68901.make

include $(MWOS)/MAKETMPL/makeosk.tpl

make.date: $(MAKES) $(MAKER)

$(MAKES): 
	$(MAKESUB) -f=$@

# end of file
