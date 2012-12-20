#############################################################################
#                                                                           #
#            Copyright 1997-1998 by Microware Systems Corporation           #
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
# 02 12/08/97 Added RPC library  and SPF libraries to LAN_LIBS        pjw   #
#    04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>             #
# 03 10/29/98 MHQco02981 - Changed -ill lib references to use -y.     mgh   #
# 04 05/14/99 Changed from DFTLIB TO BASLIB for 680x0 compatibility   mgh   #
# 05 11/12/01 Added IPv6 support                                      taka  #
#############################################################################
#                                                                           # 
#############################################################################
#	LAN Communication Package make template file                                 #
#############################################################################

#############################################################################
#	MWOS must be pre-defined to the location of the MWOS directory by 		#
#	either a full or partial pathlist.										#
#############################################################################

#############################################################################
#																			#

# Obtain generic SPF Base Package rules
include $(MWOS)/MAKETMPL/spfbase.tpl


# Definition Directory Search
# LANCOM need to be defined and set to 2 in makefile for IPv6 Support
if defined(LANCOM) && $(LANCOM) == 2
BSDDEFS		=	-v=$(MWOS)/SRC/DEFS/SPF/INET 
else
BSDDEFS		=	-v=$(MWOS)/SRC/DEFS/SPF/BSD 
endif

# Library Files
NETDB	        =   -l=$(MWOS_BASLIB)/netdb.l
NETDBS          =   $(NETDB) -y=$(MWOS_ILIBDIR)/netdb.il
NETDB_LOCAL     =   -l=$(MWOS_BASLIB)/netdb_local.l
NETDB_LOCALS    =   $(NETDB_LOCAL) -y=$(MWOS_ILIBDIR)/netdb_local.il
NETDB_DNS       =   -l=$(MWOS_BASLIB)/netdb_dns.l
NETDB_DNSS      =   $(NETDB_DNS) -y=$(MWOS_ILIBDIR)/netdb_dns.il
SOCKET          =   -l=$(MWOS_BASLIB)/socket.l
SOCKETS         =   $(SOCKET) -y=$(MWOS_ILIBDIR)/socket.il
NDBLIB	        =   -l=$(MWOS_BASLIB)/ndblib.l
NDBLIBS         =   $(NDBLIB) -y=$(MWOS_ILIBDIR)/ndblib.il
RPC             =   -l=$(MWOS_BASLIB)/rpc.l
RPCS            =   $(RPC) -y=$(MWOS_ILIBDIR)/rpc.il


LAN_LIBS        =   $(SOCKETS) $(NETDBS) $(NDBLIBS) $(ITEMS) $(CONV_LIBS)

#																			#
#############################################################################
