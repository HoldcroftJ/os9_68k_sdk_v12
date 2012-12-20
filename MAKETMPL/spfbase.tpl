#############################################################################
#                                                                           #
#              Copyright 1997 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This source code is the proprietary confidential property of Microware   #
#  Systems Corporation, and is provided to licensee for documentation and   #
#  educational purposes only. Reproduction, publication, or distribution    #
#  in any form to any party other than the licensee is strictly prohibited. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
# 01 11/10/97 Added edition history.                                        #
#    04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>             #
# 02 10/29/98 MHQco02981 - Changed -ill lib references to use -y.     mgh   #
# 03 05/14/99 Changed libdir reference to correct values for 680x0    mgh   #
#############################################################################
#                                                                           # 
#############################################################################
#	SPF Base Package make template file                                 #
#############################################################################

#############################################################################
#	MWOS must be pre-defined to the location of the MWOS directory by 		#
#	either a full or partial pathlist.										#
#############################################################################
#############################################################################
#																			#
# SPF Directory defines

DPIO			=	$(MWOS)/SRC/DPIO
SPF_DPIO		= 	$(MWOS)/SRC/DPIO/SPF
DRVRDIR			=	$(MWOS)/SRC/DPIO/SPF/DRVR


# Definition Directory Search
SPFDEFS			=	-v=$(MWOS)/SRC/DEFS/SPF

# Library Files
CONV_LIB        =   -l=$(MWOS_DFTLIB)/conv_lib.l
CONV_LIBS       =   $(CONV_LIB) -y=$(MWOS_ILIBDIR)/conv_lib.il
if $(TARGET) == "mip4" || $(TARGET) == "mip3" || $(TARGET) == "mip32"
DBG_MOD         =   -l=$(MWOS_LIBDIR)/dbg_mod.l
LOCKLIB         =   -l=$(MWOS_LIBDIR)/lock.l
MBUFLIB         =   -l=$(MWOS_LIBDIR)/mbuflib.l
PPSTAT          =   -l=$(MWOS_LIBDIR)/ppstat.l
SPTIMER         =   -l=$(MWOS_LIBDIR)/sptimer.l
else
DBG_MOD         =   -l=$(MWOS_BASLIB)/dbg_mod.l
LOCKLIB         =   -l=$(MWOS_BASLIB)/lock.l
MBUFLIB         =   -l=$(MWOS_BASLIB)/mbuflib.l
PPSTAT          =   -l=$(MWOS_BASLIB)/ppstat.l
SPTIMER         =   -l=$(MWOS_BASLIB)/sptimer.l
endif
ITEM	        =   -l=$(MWOS_BASLIB)/item.l
DBG_MODS        =   $(DBG_MOD) -y=$(MWOS_ILIBDIR)/dbg_mod.il
ITEMS           =   $(ITEM) -y=$(MWOS_ILIBDIR)/item.il
LOCKLIBS        =   $(LOCKLIB) -y=$(MWOS_ILIBDIR)/lock.il
MBUFLIBS        =   $(MBUFLIB) -y=$(MWOS_ILIBDIR)/mbuflib.il
PPSTATS         =   $(PPSTAT) -y=$(MWOS_ILIBDIR)/ppstat.il
SPTIMERS        =   $(SPTIMER) -y=$(MWOS_ILIBDIR)/sptimer.il

SPF_LIBS	=   $(MBUFLIBS) $(ITEMS) $(SPTIMERS) $(CONV_LIBS)
#																			#
#############################################################################
