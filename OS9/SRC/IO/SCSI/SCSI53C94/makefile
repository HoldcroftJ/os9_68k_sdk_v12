# Makefile for all NCR53C94 SCSI drivers

MWOS		= ../../../../..

MOPTS       = 							# makefile options
MAKER		= ./makefile				# this file

include $(MWOS)/MAKETMPL/makeosk.tpl
-x
-b

drivers.date _build : $(MAKER)
	$(MAKE) -f=scsiiop.make

makdir _makdir: 
	$(MAKE) -f=scsiiop.make makdir

clean _clean:
	$(MAKE) -f=scsiiop.make clean

purge _purge:
	$(MAKE) -f=scsiiop.make purge


# end of file
