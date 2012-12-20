#*****************************************************************************
#* Makefile for romcore module in the MC68328ADS ROM                        **
#*****************************************************************************
#* Copyright 1995, 1996 by Microware Systems Corporation                    **
#* Reproduced Under License                                                 **
#*                                                                          **
#* This source code is the proprietary confidential property of             **
#* Microware Systems Corporation, and is provided to licensee               **
#* solely for documentation and educational purposes. Reproduction,         **
#* publication, or distribution in any form to any party other than         **
#* the licensee is strictly prohibited.                                     **
#*****************************************************************************
#* Edition History:                                                         **
#* #   Date     Comments                                          By        **
#* --- -------- ------------------------------------------------- --------- **
#*   1 10/02/95 creation                                          bat       **
#*   2 05/14/96 Adopted proper usage of port and base templates.  ats       **
#*              Updated output filename.                                    **
#*   3 06/20/96 Build with ROMBug by default.                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   4 08/01/96 Improved portability. General cleanup.            ats       **
#*   5 08/01/96 More cleanup.                                     ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   6 04/14/99 support MAKDIR and CODO                           srm       **
#*   7 08.05.99 Change ODIR to NOBUG to match other CBOOT ports   mgh       **
#*****************************************************************************

PORT		=	../..
TRGTS		=	romcore

include $(PORT)/../make.com

# Reference (origination) Address [in hex]
REF_ADDR    =	400000

BOOTROOT	=	$(OS_SDIR)/ROM/LIB
LIBROOT		=	$(PORT)/LIB/ROM

ODIR		=	$(PORT_OBJDIR)/NOBUG

ROMDBG		=	$(BOOTROOT)/rombug.l
ROMIO		=	$(BOOTROOT)/romio.l
#ROMDBG		=	$(LIBROOT)/rombug.l
#ROMIO		=	$(LIBROOT)/romio.l

FILES		=	$(LIBROOT)/common.l \
				$(LIBROOT)/port.l \
				$(LIBROOT)/serial.l \
				$(ROMDBG) $(ROMIO)

SYSBOOT		=	$(BOOTROOT)/sysboot.l
LSYSBOOT	=	-l=$(SYSBOOT)

# generic cache flush library routine
CACHEFL		=	$(BOOTROOT)/flushcache.l
LCACHEFL	=	-l=$(CACHEFL)

EXTRA_MEM	=	#-olM=3k

LIBS		=	$(LSYSBOOT) $(LCACHEFL) $(CLIB) $(SYS_CLIB) $(OS_LIB) $(SYS)
LIBDEPS		=	$(SYSBOOT) $(CACHEFL)

LINK		=	$(LINKER) $(LPARMS) -k -r -olswam -olb=4 $(EXTRA_MEM) \
				$(LIBS) -olr=$(REF_ADDR) $(LOPTS)

$(TRGTS): _makdir $(ODIR)/$(TRGTS)
	$(COMMENT)

$(ODIR)/$(TRGTS): $(FILES) $(LIBDEPS) ./makefile
	$(CODO) $@
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	$(LINK) $(FILES) -fd=$@ $(REDIR_OUT)$(ODIR)/STB/$(TRGTS).map

_makdir: .
	$(MAKDIR) $(ODIR)/STB

_clean _purge:  .
	$(CODO) $(ODIR)/$(TRGTS)
	-$(DEL) $(ODIR)/$(TRGTS)
	$(CODO) $(ODIR)/STB/$(TRGTS).map
	-$(DEL) $(ODIR)/STB/$(TRGTS).map
	$(CODO) $(ODIR)/STB/$(TRGTS).stb
	-$(DEL) $(ODIR)/STB/$(TRGTS).stb

_lint:  .
	$(COMMENT)

#																			#
#############################################################################

