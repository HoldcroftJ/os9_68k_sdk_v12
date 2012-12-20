# Makefile
#############################################################################
#           Copyright 1989-2000 by Microware Systems Corporation            #
#                           All Rights Reserved                             #
#                         Reproduced Under License                          #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, modification, distribution, or creation of    #
#  derivative works in any form to any party other than the Licensee is     #
#  strictly prohibited, unless expressly provided for under the terms of a  #
#  mutually executed written license agreement for this software between    #
#  the end-user and Microware Systems Corporation, or otherwise expressly   #
#  authorized in writing by Microware Systems Corporation.  Licensee shall  #
#  reproduce this copyright notice exactly as provided herein on any copies #
#  of the software that Licensee is authorized to distribute.               #
#############################################################################
#
# Edition History:
#
# #   Date     Comments													By
# --- -------- --------------------------------------------------------	---
#  01 ??/??/?? Created
#  02 10/17/95 Added -olS to make modules sticky						mgh
#  03 01/27/97 Copied for SPF examples									pjw
#  04 03/24/97 Removed ISP support										rds
#  05 05/02/97 Added include directory  for BSDDEFS						pjw
#     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
#  06 12/16/97 Use new makefile templates								rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  07 11/04/98 Add CODO, Add makedirs, Remove ALL_TRGT, add CPU_GU0		mgh
#  08 06/14/99 Added CODO for purge target.								sr
#  09 05/02/00 Fixed PURGE to purge correct files.  Added .map files.	dwj
#  10 08/31/00 Added the missing CODOs in the purge target.             srm
############################################################################

MWOS		=	../../../../..
TRGTS		=	tcpsend tcprecv
MAKENAME	=	makefile

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

BRELS		=	$(CSTART) $(RDIR)/tcpsend.r
BIRELS		=	$(IDIR)/tcpsend.i
TRELS		=	$(CSTART) $(RDIR)/tcprecv.r
TIRELS		=	$(IDIR)/tcprecv.i

DEFS		=	$(BSDDEFS) $(MWOS_DEFS)
LIBS		=	$(LAN_LIBS) $(UNIXLIBS) $(MWOS_CSLLIBS)
OPTMZ		=

COMPILE		=	$(CC) $(CPARMS) $(OPTMZ) -efe=$(IDIR) $(DEFS) $(COPTS)
ILINK		=	$(CC) $(CPARMS) $(OPTMZ) -k -eas=$(RDIR) $(DEFS) $(LIBS) \
				$(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(OPTMZ) -tp=$(CPUC) -k -olS \
				$(LIBS) $(LOPTS) $(COPT_GU0) 


tcpsend: $(IDIR) $(RDIR) $(ODIR)/STB  $(ODIR)/tcpsend
	$(COMMENT)
tcprecv: $(IDIR) $(RDIR) $(ODIR)/STB  $(ODIR)/tcprecv
	$(COMMENT)

$(ODIR)/tcpsend:  $(BRELS)
	-$(CODO) $(ODIR)/tcpsend
	-$(DEL)  $(ODIR)/tcpsend
	-$(CODO) $(ODIR)/STB/tcpsend.stb
	-$(DEL)  $(ODIR)/STB/tcpsend.stb
	-$(CODO) $(ODIR)/STB/tcpsend.map
	-$(DEL)  $(ODIR)/STB/tcpsend.map
	-$(DEL)  $(ODIR)/STB/tcpsend.dbg
	$(LINK)  $(BRELS) -fd=$(ODIR)/tcpsend -oln=tcpsend \
				-ols=$(ODIR)/STB/tcpsend.map
$(ODIR)/tcprecv:  $(TRELS)
	-$(CODO) $(ODIR)/tcprecv
	-$(DEL)  $(ODIR)/tcprecv
	-$(CODO) $(ODIR)/STB/tcprecv.stb
	-$(DEL)  $(ODIR)/STB/tcprecv.stb
	-$(CODO) $(ODIR)/STB/tcprecv.map
	-$(DEL)  $(ODIR)/STB/tcprecv.map
	-$(DEL)  $(ODIR)/STB/tcprecv.dbg
	$(LINK)  $(TRELS) -fd=$(ODIR)/tcprecv -oln=tcprecv \
				-ols=$(ODIR)/STB/tcprecv.map

$(RDIR)/tcpsend.r:  $(BIRELS)
	$(ILINK) $(BIRELS) -fd=tcpsend.r
$(RDIR)/tcprecv.r:  $(TIRELS)
	$(ILINK) $(TIRELS) -fd=tcprecv.r

$(IDIR)/tcpsend.i:  tcpsend.c
	$(COMPILE) tcpsend.c
$(IDIR)/tcprecv.i:  tcprecv.c
	$(COMPILE) tcprecv.c


$(IDIR) $(RDIR) $(ODIR)/STB:
	-$(MAKDIR) $@

_clean:
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r

_purge:
	$(CODO) $(ODIR)/tcpsend
	-$(DEL) $(ODIR)/tcpsend
	$(CODO) $(ODIR)/STB/tcpsend.stb
	-$(DEL) $(ODIR)/STB/tcpsend.stb
	$(CODO) $(ODIR)/STB/tcpsend.map
	-$(DEL) $(ODIR)/STB/tcpsend.map
	$(CODO) $(ODIR)/tcprecv
	-$(DEL) $(ODIR)/tcprecv
	$(CODO) $(ODIR)/STB/tcprecv.stb
	-$(DEL) $(ODIR)/STB/tcprecv.stb
	$(CODO) $(ODIR)/STB/tcprecv.map
	-$(DEL) $(ODIR)/STB/tcprecv.map

#                                                                          #
############################################################################
