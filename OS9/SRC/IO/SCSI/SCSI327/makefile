# Makefile for all MVME327 SCSI drivers

MWOS		= ../../../../..

MOPTS       = 							# makefile options
MAKER		= ./makefile				# this file

include $(MWOS)/MAKETMPL/makeosk.tpl
-x
-b

drivers.date _build: $(MAKER)
	$(MAKE) -f=scsi327.make

makdir _makdir: 
	$(MAKE) -f=scsi327.make makdir

clean _clean:
	$(MAKE) -f=scsi327.make clean

purge _purge:
	$(MAKE) -f=scsi327.make purge

# end of file
