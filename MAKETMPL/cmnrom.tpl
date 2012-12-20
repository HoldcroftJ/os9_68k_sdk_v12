#############################################################################
#                                                                           #
#              Copyright 1996-2000 by Microware Systems Corporation         #
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
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #  
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 02 98/10/29 MHQco02981 - Changed -ill lib references to use -y.      mgh  #
# 03 99/07/30 change MWOS_DFTLIB to MWOS_LIBDIR                        lrh  #
# 04 99/12/06 Change ROM_LIBDIR back to MWOS_DFTLIB.                   djc  #
# 05 00/01/24 Added NETDB here, it used to be in processor templates   rds  #
# 06 00/02/08 Added MIPS conditional here around the NETDB.			   dwj	#
#############################################################################
#                                                                           # 
# Template for all common ROM definitions

include $(MWOS)/MAKETMPL/make.com

ROM_DDIR		=	$(MWOS_DDIR)/ROM
ROM_OSDDIR		=	$(MWOS_OSDDIR)/ROM

ROM_ODIR		=	$(MWOS_ODIR)/BOOTOBJS/ROM

ROM_LIBDIR		=	$(MWOS_DFTLIB)/ROM
ROM_ILIBDIR		=	$(ROM_LIBDIR)/$(HOSTTYPE)
ROM_PICDIR		=	$(ROM_LIBDIR)/PIC

ROMLIBN			=	$(ROM_LIBDIR)/romsys.l
ROMILIBN		=	$(ROM_ILIBDIR)/romsys.il
ROMLIB			=	-l=$(ROMLIBN)
ROMLIBS			=	$(ROMLIB) -y=$(ROMILIBN)

P2_LIBN			=	$(ROM_LIBDIR)/p2lib.l
P2_ILIBN		=	$(ROM_ILIBDIR)/p2lib.il
P2_LIB			=	-l=$(P2_LIBN)
P2_LIBS			=	$(P2_LIB) -y=$(P2_ILIBN)

P2_PRIVATEN		=	$(ROM_LIBDIR)/p2privte.l
P2_IPRIVATEN		=	$(ROM_ILIBDIR)/p2privte.il
P2_PRIVATE		=	-l=$(P2_PRIVATEN)
P2_PRIVATES		=	$(P2_PRIVATE) -y=$(P2_IPRIVATEN)

# We shouldn't use a LAN Comm library in the low level rom, but it is
# a convenient place to get the endian functions (ntohl, htonl, etc.)
# A better solution would be to have them in an endian.l or some
# existing compiler library.
if $(TARGET) == "mips" || $(TARGET) == "mip3" || $(TARGET) == "mip4" || $(TARGET) == "mip32"
NETDB			=	-l=$(MWOS_BASLIB)/netdb.l
else
NETDB			=	-l=$(MWOS_LIBDIR)/netdb.l
endif
NETDBS			=	$(NETDB) -y=$(MWOS_ILIBDIR)/netdb.il

P2_START		=	$(ROM_LIBDIR)/p2start.r
P2_STARTP		=	$(ROM_LIBDIR)/p2prvstr.r
DATASTRT		=	$(ROM_LIBDIR)/datastrt.r

ROM_DEFS		=	-v=$(ROM_DDIR) -v=$(ROM_OSDDIR)

ROMH			=	$(ROM_DDIR)/rom.h
P2LIBH			=	$(ROM_DDIR)/p2lib.h
P2PRIVTEH		=	$(ROM_DDIR)/p2privte.h
TRAPANCHH		=	$(ROM_DDIR)/trapanch.h
CNFGDATAH		=	$(ROM_DDIR)/cnfgdata.h
