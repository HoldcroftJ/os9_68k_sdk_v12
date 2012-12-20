#############################################################################
#                                                                           #
#            Copyright 1996-1998 by Microware Systems Corporation           #
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
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
#             ---- OS-9000/PPC V2.1.2 Released ----                         #
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #
#             ---- Modular Rom Sub-system v1.0 Released ----                #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Modular Rom v1.1 Sub-component Released ----             #
#             ---- Modular ROM v1.2 Sub-component Released ----             #
# 02 98/10/27 CODO-ized makefile                                            #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
# O-code specific portion of FLSHCACH ROM P2 Makefile                       #
#############################################################################
#                                                                           #

$(ODIR)/flush68k:	$(P2_START) $(CRELS) $(P2_LIBN) $(OSK_ROMLIB)/flushcache.l
	$(CODO) $(ODIR)/flush68k
	-$(DEL) $(ODIR)/flush68k
	$(CODO) $(ODIR)/STB/flush68k.stb
	-$(DEL) $(ODIR)/STB/flush68k.stb
	$(CODO) $(ODIR)/STB/flush68k.dbg
	-$(DEL) $(ODIR)/STB/flush68k.dbg
	$(CODO) $(ODIR)/STB/flush68k.map
	-$(DEL) $(ODIR)/STB/flush68k.map
	$(LINK) -l=$(OSK_ROMLIB)/flushcache.l $(P2_START) $(CRELS) \
			-fd=$(ODIR)/flush68k -ols=$(ODIR)/STB/flush68k.map

$(ODIR)/flush349:	$(P2_START) $(CRELS) $(P2_LIBN) $(OSK_ROMLIB)/flush349.l
	$(CODO) $(ODIR)/flush349
	-$(DEL) $(ODIR)/flush349
	$(CODO) $(ODIR)/STB/flush349.stb
	-$(DEL) $(ODIR)/STB/flush349.stb
	$(CODO) $(ODIR)/STB/flush349.dbg
	-$(DEL) $(ODIR)/STB/flush349.dbg
	$(CODO) $(ODIR)/STB/flush349.map
	-$(DEL) $(ODIR)/STB/flush349.map
	$(LINK) -l=$(OSK_ROMLIB)/flush349.l $(P2_START) $(CRELS) \
			-fd=$(ODIR)/flush349 -ols=$(ODIR)/STB/flush349.map

$(ODIR)/flushnom:	$(P2_START) $(RDIR)/flushnom.r $(P2_LIBN)
	$(CODO) $(ODIR)/flushnom
	-$(DEL) $(ODIR)/flushnom
	$(CODO) $(ODIR)/STB/flushnom.stb
	-$(DEL) $(ODIR)/STB/flushnom.stb
	$(CODO) $(ODIR)/STB/flushnom.dbg
	-$(DEL) $(ODIR)/STB/flushnom.dbg
	$(CODO) $(ODIR)/STB/flushnom.map
	-$(DEL) $(ODIR)/STB/flushnom.map
	$(LINK) $(P2_START) $(RDIR)/flushnom.r \
			-fd=$(ODIR)/flushnom -ols=$(ODIR)/STB/flushnom.map

$(ODIR)/$(OBJS):	$(P2_START) $(CRELS) $(P2_LIBN)
	$(CODO) $(ODIR)/$(OBJS)
	-$(DEL) $(ODIR)/$(OBJS)
	$(CODO) $(ODIR)/STB/$(OBJS).stb
	-$(DEL) $(ODIR)/STB/$(OBJS).stb
	$(CODO) $(ODIR)/STB/$(OBJS).dbg
	-$(DEL) $(ODIR)/STB/$(OBJS).dbg
	$(CODO) $(ODIR)/STB/$(OBJS).map
	-$(DEL) $(ODIR)/STB/$(OBJS).map
	$(LINK) $(P2_START) $(CRELS) \
			-fd=$(ODIR)/$(TRGTS) -ols=$(ODIR)/STB/$(TRGTS).map

#                                                                           #
#############################################################################

