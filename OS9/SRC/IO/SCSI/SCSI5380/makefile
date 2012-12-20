# Makefile for all NCR5380 low-level drivers

MWOS		= ../../../../..

MOPTS       = 							# makefile options
MAKER		= ./makefile				# this file

include $(MWOS)/MAKETMPL/makeosk.tpl
-x
-b

drivers.date _build: $(MAKER)
	$(MAKE) -f=scsi107.make
	$(MAKE) -f=scsi117.make
	$(MAKE) -f=scsi1782.make
	$(MAKE) -f=scsijnms.make

makdir _makdir: 
	$(MAKE) -f=scsi107.make makdir
	$(MAKE) -f=scsi117.make makdir
	$(MAKE) -f=scsi1782.make makdir
	$(MAKE) -f=scsijnms.make makdir

clean _clean:
	$(MAKE) -f=scsi107.make clean
	$(MAKE) -f=scsi117.make clean
	$(MAKE) -f=scsi1782.make clean
	$(MAKE) -f=scsijnms.make clean

purge _purge:
	$(MAKE) -f=scsi107.make purge
	$(MAKE) -f=scsi117.make purge
	$(MAKE) -f=scsi1782.make purge
	$(MAKE) -f=scsijnms.make purge

# end of file
