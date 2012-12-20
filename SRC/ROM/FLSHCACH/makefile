#############################################################################
#                                                                           #
#              Copyright 1996-97 by Microware Systems Corporation           #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
# 01 96/08/03 Added edition history.                                  rak   #
#             ---- OS-9000/x86 V2.1 Released ----                           #
# 02 96/10/14 Fixed wildcards in _clean target.                       gdb   #
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
# 03 97/01/21 Added ARM capability                                    dwj   #
# 04 97/01/27 Added MIPs 3081 capability                              sep   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
#             ---- OS-9000/PPC V2.1.2 Released ----                         #
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #
# 05 97/08/25 Added SPARC target.                                     gdw   #
# 06 97/10/06 Added SH target.                                        gdb   #
#             ---- Modular Rom Sub-system v1.0 Released ----                #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Modular Rom v1.1 Sub-component Released ----             #
#             ---- Modular ROM v1.2 Sub-component Released ----             #
# 07 98/10/27 CODO-ized makefile                                      ajk   #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
# Makefile to produce the FLSHCACH ROM P2 module                            #
#############################################################################
#                                                                           #

TRGTS		=	flshcach

COMMON_ROM	=	..
include $(COMMON_ROM)/MAKETMPL/make.com

# Specify link method (using I-code linking or not).
LINKMTHD	=	icode					# ocode or icode

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
SDIR		=	.
ODIR		=	$(ROM_ODIR)

OSK_ROMLIB	=	$(MWOS)/OS9/SRC/ROM/LIB

OBJS		=	flshcach

DEFS		=	$(ROM_DEFS) $(MWOS_DEFS)

MAKENAME	=	makefile

MAKERS		=	./$(MAKENAME) ./$(LINKMTHD).com

TRGTREL		=	$(RDIR)/$(OBJS).r

CRELS		=	$(TRGTREL)

TRGTICODE	=	$(IDIR)/$(OBJS).i

CICODE		=	$(TRGTICODE)

LIBS		=	$(P2_LIBS) $(CPULIBS) $(SYS)

COMPILE		=	$(CC) $(CPARMS) -a $(DEFS) $(COPTS) #-dDEBUG
RCOMPILE	=	$(COMPILE) -eas=$(RDIR) -t=0 -r $(COPT_CODEADDR) -xao
ICOMPILE	=	$(COMPILE) -efe=$(IDIR)

LINK		=	$(LINKER) $(LPARMS) -k $(LIBS) $(LOPTS) -t=0 -olgu=0.0
ILINK		=	$(LINK) -r $(COPT_CODEADDR) -xao

flshcach:	_makdir flush.$(TARGET)
	$(COMMENT)

if $(TARGET) == "k68k"
flush.k68k:	$(ODIR)/flush68k $(ODIR)/flush349
	$(COMMENT)
else
flush.p386 flush.o386:	$(ODIR)/$(OBJS) $(ODIR)/flushnom
	$(COMMENT)
flush.sh4:	$(ODIR)/$(OBJS)
	$(COMMENT)
flush.$(TARGET):	$(ODIR)/$(OBJS)
	$(COMMENT)
endif

# use custom link method rules
include $(LINKMTHD).com

$(CRELS) $(CICODE) $(RDIR)/flushnom.r $(IDIR)/flushnom.i:	$(ROMH) \
		$(P2LIBH) $(SDIR)/$(OBJS).c $(MAKERS)

$(CRELS):
	$(RCOMPILE) $(SDIR)/$*.c

$(CICODE):
	$(ICOMPILE) $(SDIR)/$*.c

$(RDIR)/flushnom.r:
	$(RCOMPILE) $(SDIR)/$(OBJS).c -dNOT_MICRONICS -fd=$*.r

$(IDIR)/flushnom.i:
	$(ICOMPILE) $(SDIR)/$(OBJS).c -dNOT_MICRONICS -fd=$*.i

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:	PURGES.$(TARGET)
	$(COMMENT)

PURGES.k68k: nulltrg
	$(CODO) $(ODIR)/flush68k
	-$(DEL) $(ODIR)/flush68k
	$(CODO) $(ODIR)/STB/flush68k.stb
	-$(DEL) $(ODIR)/STB/flush68k.stb
	$(CODO) $(ODIR)/STB/flush68k.dbg
	-$(DEL) $(ODIR)/STB/flush68k.dbg
	$(CODO) $(ODIR)/STB/flush68k.map
	-$(DEL) $(ODIR)/STB/flush68k.map
	$(CODO) $(ODIR)/flush349
	-$(DEL) $(ODIR)/flush349
	$(CODO) $(ODIR)/STB/flush349.stb
	-$(DEL) $(ODIR)/STB/flush349.stb
	$(CODO) $(ODIR)/STB/flush349.dbg
	-$(DEL) $(ODIR)/STB/flush349.dbg
	$(CODO) $(ODIR)/STB/flush349.map
	-$(DEL) $(ODIR)/STB/flush349.map

PURGES.p386 PURGES.o386:  nulltrg
	$(CODO) $(ODIR)/$(OBJS)
	-$(DEL) $(ODIR)/$(OBJS)
	$(CODO) $(ODIR)/STB/$(OBJS).stb
	-$(DEL) $(ODIR)/STB/$(OBJS).stb
	$(CODO) $(ODIR)/STB/$(OBJS).dbg
	-$(DEL) $(ODIR)/STB/$(OBJS).dbg
	$(CODO) $(ODIR)/STB/$(OBJS).map
	-$(DEL) $(ODIR)/STB/$(OBJS).map
	$(CODO) $(ODIR)/flushnom
	-$(DEL) $(ODIR)/flushnom
	$(CODO) $(ODIR)/STB/flushnom.stb
	-$(DEL) $(ODIR)/STB/flushnom.stb
	$(CODO) $(ODIR)/STB/flushnom.dbg
	-$(DEL) $(ODIR)/STB/flushnom.dbg
	$(CODO) $(ODIR)/STB/flushnom.map
	-$(DEL) $(ODIR)/STB/flushnom.map

PURGES.sh:  nulltrg
	$(CODO) $(ODIR)/flshcach
	-$(DEL) $(ODIR)/flshcach
	$(CODO) $(ODIR)/STB/flshcach.stb
	-$(DEL) $(ODIR)/STB/flshcach.stb
	$(CODO) $(ODIR)/STB/flshcach.dbg
	-$(DEL) $(ODIR)/STB/flshcach.dbg
	$(CODO) $(ODIR)/STB/flshcach.map
	-$(DEL) $(ODIR)/STB/flshcach.map

PURGES.sh4:  nulltrg
	$(CODO) $(ODIR)/flshcach
	-$(DEL) $(ODIR)/flshcach
	$(CODO) $(ODIR)/STB/flshcach.stb
	-$(DEL) $(ODIR)/STB/flshcach.stb
	$(CODO) $(ODIR)/STB/flshcach.dbg
	-$(DEL) $(ODIR)/STB/flshcach.dbg
	$(CODO) $(ODIR)/STB/flshcach.map
	-$(DEL) $(ODIR)/STB/flshcach.map

if $(TARGET) == "k68k"
else
PURGES.$(TARGET):  nulltrg
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	-$(DEL) $(ODIR)/STB/$(TRGTS).map
	-$(DEL) $(ODIR)/STB/$(TRGTS).dbg
endif

nulltrg:  .
	$(COMMENT)

#                                                                           #
#############################################################################

