# Makefile
############################################################################
#           Copyright 1989-2000 by Microware Systems Corporation           #
#                           All Rights Reserved                            #
#                         Reproduced Under License                         #
#                                                                          #
# This software is confidential property of Microware Systems Corporation, #
# and is provided under license for internal development purposes only.    #
# Reproduction, publication, distribution, or creation of derivative works #
# in any form to any party other than the licensee is strictly prohibited, #
# unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                          #
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 ??/??/?? Created
#     04/03/98 <***** SPF LAN Comm Pack v3.2 Release **********>
#  02 11/24/98 Add CODO, etc                                          mgh
#  03 09/20/00 Fix rpcdb dependency.                                  djc
#     10/12/00 Fix invalid dependencies                               srm
#############################################################################
# This makefile will make the LAN inetdb and rpcdb data module              #
#############################################################################

PORT    = ../..
TRGTS		=	inetdb rpcdb

include $(PORT)/../make.com

ODIR		=	$(PORT)/CMDS/BOOTOBJS/SPF
SDIR		=	$(MWOS)/SRC/ETC
SDIR2		=	.

SFILES		=	$(SDIR)/hosts $(SDIR)/hosts.equiv $(SDIR)/protocols \
				$(SDIR)/networks $(SDIR)/services $(SDIR)/inetd.conf \
				$(SDIR)/resolv.conf $(SDIR)/host.conf \
				$(SDIR2)/interfaces.conf $(SDIR)/routes.conf $(SDIR)/rpc
RFILES		=	$(SDIR)/nfs.map $(SDIR)/nfsd.map
RPCOPT		=	# -s -c -d

rpcdb: $(ODIR)/rpcdb
$(ODIR)/rpcdb: $(RFILES)
	-$(CODO) $(ODIR)/rpcdb
	-$(DEL)  $(ODIR)/rpcdb
	$(RPCDBGEN) -to=$(OS) -tp=$(CPU) -w=$(SDIR) -o=$(ODIR)/rpcdb $(RPCOPT)
	$(ATTRE0) $(ODIR)/rpcdb

inetdb: $(ODIR)/inetdb2
$(ODIR)/inetdb2 : $(ODIR)/inetdb
$(ODIR)/inetdb : $(SFILES)
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $@2
	-$(DEL)  $@2
	$(IDBGEN) -to=$(OS) -tp=$(CPU) -d=. -d=$(SDIR) $@
	$(ATTRE0) $@
	$(ATTRE0) $@2

_clean _purge: notarget
	-$(CODO) $(ODIR)/inetdb
	-$(DEL)  $(ODIR)/inetdb
	-$(CODO) $(ODIR)/inetdb2
	-$(DEL)  $(ODIR)/inetdb2
	-$(CODO) $(ODIR)/rpcdb
	-$(DEL)  $(ODIR)/rpcdb

notarget: .
	$(COMMENT)

#                                                                           #
#############################################################################
