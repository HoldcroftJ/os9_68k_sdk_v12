# Makefile for all WD33C93 low-level drivers

MWOS		= ../../../../..

MOPTS       = 							# makefile options
MAKER		= ./makefile				# this file

include $(MWOS)/MAKETMPL/makeosk.tpl
-x
-b

drivers.date _build: $(MAKER)
	$(MAKE) -f=scsi147.make
	$(MAKE) -f=scsi620.make
	$(MAKE) -f=scsi620_nodma.make

makdir _makdir: 
	$(MAKE) -f=scsi147.make makdir
	$(MAKE) -f=scsi620.make makdir
	$(MAKE) -f=scsi620_nodma.make makdir

clean _clean:
	$(MAKE) -f=scsi147.make clean
	$(MAKE) -f=scsi620.make clean
	$(MAKE) -f=scsi620_nodma.make clean

purge _purge:
	$(MAKE) -f=scsi147.make purge
	$(MAKE) -f=scsi620.make purge
	$(MAKE) -f=scsi620_nodma.make purge


# end of file
