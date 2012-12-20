# Makefile for all platform-independent ROM code.
-x
-b

MWOS		= ../../../../..
MAKER		= ./makefile				# this file

MAKES		= \
				BOOT82596

#				BOOT374 \
#				BOOT7990 \
#				BOOTCMC \
#				BOOTQUICC 
#				BOOTBP 					# obsolete

include $(MWOS)/MAKETMPL/makeosk.tpl

make.date: $(MAKES) $(MAKER)

$(MAKES): $(MAKER)
	$(CHD) $@ ; $(MAKESUB)

# end of file
