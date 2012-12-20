# Makefile for all NCR53C710 low-level booters

MWOS		= ../../../../..

MOPTS       = 							# makefile options
MAKER		= ./makefile

include $(MWOS)/MAKETMPL/makeosk.tpl
-x
-b
-bo

drivers.date _build: $(MAKER)
	$(MAKE) -f=bt_162_53c710.make
	$(MAKE) -f=bt_167_53c710.make
	$(MAKE) -f=bt_172_53c710.make
	$(MAKE) -f=bt_177_53c710.make
	$(MAKE) -f=scsi162.make
	$(MAKE) -f=scsi167.make
	$(MAKE) -f=scsi172.make
	$(MAKE) -f=scsi177.make

makdir _makdir: 
	$(MAKE) -f=bt_162_53c710.make makdir
	$(MAKE) -f=bt_167_53c710.make makdir
	$(MAKE) -f=bt_172_53c710.make makdir
	$(MAKE) -f=bt_177_53c710.make makdir
	$(MAKE) -f=scsi162.make makdir
	$(MAKE) -f=scsi167.make makdir
	$(MAKE) -f=scsi172.make makdir
	$(MAKE) -f=scsi177.make makdir

clean _clean:
	$(MAKE) -f=bt_162_53c710.make clean
	$(MAKE) -f=bt_167_53c710.make clean
	$(MAKE) -f=bt_172_53c710.make clean
	$(MAKE) -f=bt_177_53c710.make clean
	$(MAKE) -f=scsi162.make clean
	$(MAKE) -f=scsi167.make clean
	$(MAKE) -f=scsi172.make clean
	$(MAKE) -f=scsi177.make clean

purge _purge:
	$(MAKE) -f=bt_162_53c710.make purge
	$(MAKE) -f=bt_167_53c710.make purge
	$(MAKE) -f=bt_172_53c710.make purge
	$(MAKE) -f=bt_177_53c710.make purge
	$(MAKE) -f=scsi162.make purge
	$(MAKE) -f=scsi167.make purge
	$(MAKE) -f=scsi172.make purge
	$(MAKE) -f=scsi177.make purge

# end of file
