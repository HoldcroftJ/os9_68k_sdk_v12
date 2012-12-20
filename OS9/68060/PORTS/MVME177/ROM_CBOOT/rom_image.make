# Makefile for linked rom image in the MVME177 ROM
-mode=compat
-bo
-x
-B

MWOS		= ../../../../..
ROOT		= ../../../..					# base of dir system

BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
CPUROOT		= $(ROOT)/68060				# dir system for output
SRCROOT		= $(ROOT)/SRC				# dir system for source
BOOTROOT	= $(SRCROOT)/ROM/LIB
SYSROOT		= $(BASEROOT)/LIB

TYPE		= ROMBUG
RDIR		= RELS/$(TYPE)
RDUP		= ../..
LIBROOT		= $(RDIR)

MAKER		= rom_image.make

ODIR		= ../CMDS/BOOTOBJS/$(TYPE)

TARGET		= rombug

ROMDBG		= $(BOOTROOT)/rombug.l

ROMIO		= $(BOOTROOT)/romio.l

LLSCSI		= $(BOOTROOT)/bt_177_53c710.l
HLS_HD		= $(BOOTROOT)/bootsccs.l
HLS_FD		= $(BOOTROOT)/bootfc1.l
HLS_TP		= $(BOOTROOT)/bootviper.l
ALLSCSI		= $(LLSCSI) $(HLS_HD) $(HLS_FD) $(HLS_TP)

TKETH		= $(BOOTROOT)/bp_tk167.l
LLETH		= $(BOOTROOT)/bp_167_82596.l
BPETH		= #$(BOOTROOT)/bp_167_bp.l
HLETH		= $(BOOTROOT)/inetboot.l
#ALLETH		= $(TKETH) $(LLETH) $(BPETH) $(HLETH)
ALLETH		= $(TKETH) $(LLETH) $(HLETH)

FILES		= $(LIBROOT)/rom_common.l \
				$(LIBROOT)/rom_port.l \
				$(LIBROOT)/rom_serial.l \
				$(ALLSCSI) $(ALLETH) $(ROMDBG) $(ROMIO)

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

CACHEFL		= $(BOOTROOT)/flushcache.l
LCACHEFL	= -l=$(CACHEFL)

ROMDESC		= $(LIBROOT)/rom_descriptors.l
LROMDESC	= -l=$(ROMDESC)

LIBS		= $(LSYSBOOT) $(LCACHEFL) $(LROMDESC) \
				$(LCLIB) $(LSYS_CLIB) $(LMLIB) $(LSYSL)

LIBDEPS		= $(SYSBOOT) $(CACHEFL) $(ROMDESC) \
				$(CLIB) $(SYS_CLIB) $(MLIB) $(SYSL)

include		$(MWOS)/MAKETMPL/makeosk.tpl

LC			= l68
LFLAGS		= -r=FF800000 -swam -M=3k -g -b=4 $(LIBS) -gu=0.0

$(ODIR)/$(TARGET) : makdir $(FILES) $(LIBDEPS) $(MAKER)
	-$(CODO) $@
	-$(CODO) $(ODIR)/STB/$(TARGET).stb
	-$(CODO) $(ODIR)/STB/$(TARGET).map
	-$(DEL)  $@
	-$(DEL)  $(ODIR)/STB/$(TARGET).stb
	-$(DEL)  $(ODIR)/STB/$(TARGET).map
	-$(DEL)  $(ODIR)/STB/$(TARGET).dbg
	$(LC) $(LFLAGS) $(FILES) -O=$@ $(REDIR_OUT)$(ODIR)/STB/$(TARGET).map

makdir _makdir:
    -$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB
 
clean _clean:
	$(ECHO) "NO rels to clean"

purge _purge:
	-$(CODO) $(ODIR)/$(TARGET)
	-$(DEL)  $(ODIR)/$(TARGET)
	-$(CODO) $(ODIR)/STB/$(TARGET).stb
	-$(DEL)  $(ODIR)/STB/$(TARGET).stb
	-$(DEL)  $(ODIR)/STB/$(TARGET).dbg
	-$(DEL)  $(ODIR)/$(TARGET).map

