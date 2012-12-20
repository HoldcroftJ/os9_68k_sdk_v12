# Makefile for port modules (RUSSBOX '990)
# This combines the 'rom_common', 'rom_port', 'rom_serial' and rombug into a bootable
# image (one probably wants to append 'bootfile' (kernel+core devs/descriptors) and
# 'utilities' to this for a useable boot ROM image.
#####################################################################################
-mode=compat
-bo
-x
-B

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68000				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source
BOOTROOT	= $(SRCROOT)/ROM/LIB
SYSROOT		= $(BASEROOT)/LIB
BOOTLIBS	= LIB

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..
LIBROOT		= $(RDIR)

MAKER		= rom_image.make

ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

TARGET		= rombug

ROMDBG		= $(BOOTROOT)/rombug.l
ROMIO		= $(BOOTROOT)/romio.l

FILES		= $(LIBROOT)/rom_common.l \
			$(LIBROOT)/rom_port.l \
			$(LIBROOT)/rom_serial.l \
			$(ROMDBG) $(ROMIO)

CLIB		= $(SYSROOT)/clib.l
LCLIB		= -l=$(CLIB)
SYS_CLIB	= $(SYSROOT)/sys_clib.l
LSYS_CLIB	= -l=$(SYS_CLIB)
MLIB		= $(SYSROOT)/os_lib.l
LMLIB		= -l=$(MLIB)
SYSL		= $(SYSROOT)/sys.l
LSYSL		= -l=$(SYSL)

SYSBOOT		= $(BOOTROOT)/sysboot.l
LSYSBOOT	= -l=$(SYSBOOT)

# generic cache flush library routine
#
CACHEFL		= $(BOOTROOT)/flushcache.l
LCACHEFL	= -l=$(CACHEFL)

LIBS		= $(LSYSBOOT) $(LCACHEFL) \
				$(LCLIB) $(LSYS_CLIB) $(LMLIB) $(LSYSL)

LIBDEPS		= $(SYSBOOT) $(CACHEFL) \
				$(CLIB) $(SYS_CLIB) $(MLIB) $(SYSL)

include		$(MWOS)/MAKETMPL/makeosk.tpl

LC		= l68
#LFLAGS		= -r=0 -swam -M=3k -g -b=4 $(LIBS) -gu=0.0 
LFLAGS		= -r=f00000 -swam -M=3k -g -b=4 $(LIBS) -gu=0.0 

$(ODIR)/$(TARGET) : makdir $(FILES) $(LIBDEPS) $(MAKER)
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(TARGET).stb
	$(CODO) $(ODIR)/STB/$(TARGET).map
	$(DEL) $@
	$(DEL)  $(ODIR)/STB/$(TARGET).stb
	$(DEL)  $(ODIR)/STB/$(TARGET).map
	$(DEL)  $(ODIR)/STB/$(TARGET).dbg
	$(LC) $(LFLAGS) $(FILES) -O=$@ $(REDIR_OUT)$(ODIR)/STB/$(TARGET).map

makdir _makdir:
	$(MAKDIR) $(RDIR)
	$(MAKDIR) $(ODIR)/STB

clean _clean:
	$(ECHO) "NO rels to clean"

purge _purge:
	$(CODO) $(ODIR)/$(TARGET)
	$(CODO) $(ODIR)/STB/$(TARGET).stb
	$(CODO) $(ODIR)/STB/$(TARGET).map
	$(DEL)  $(ODIR)/$(TARGET)
	$(DEL)  $(ODIR)/STB/$(TARGET).stb
	$(DEL)  $(ODIR)/STB/$(TARGET).map
	$(DEL)  $(ODIR)/STB/$(TARGET).dbg

# end of file

