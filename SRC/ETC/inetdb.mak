############################################################################
#                                                                          #
#           Copyright 1989-2001 by Microware Systems Corporation           #
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
#  02 98/11/10 Remove ALL_TRGTS, Add CODO, add copyrigh, add history  mgh
#  03 00/09/11 Added _clean to _purge.                                djc
#  04 00/11/20 Fix dependencies.                                      djc
#  05 01/11/29 Created from original makefile.                        djc
############################################################################
# This makefile will make the LAN inetdb and rpcdb data modules	           #
############################################################################
#                                                                          #

MWOS		=	../..
TRGTS		=	inetdb rpcdb
MAKENAME	=	inetdb.mak

include $(MWOS)/MAKETMPL/make.com

ODIR		=	$(MWOS_ODIR)/BOOTOBJS/SPF
SDIR		=	.
RDIR		=	$(MWOS_RDIR)


SFILES		=	$(SDIR)/hosts $(SDIR)/hosts.equiv $(SDIR)/protocols \
			$(SDIR)/networks $(SDIR)/services $(SDIR)/inetd.conf \
			$(SDIR)/resolv.conf $(SDIR)/host.conf \
			$(SDIR)/interfaces.conf $(SDIR)/routes.conf $(SDIR)/rpc
RFILES    	=    	$(SDIR)/nfs.map  $(SDIR)/nfsd.map
RPCOPT      	=  # -s -c -d


inetdb:	_makdir $(ODIR)/inetdb2
	$(COMMENT)

rpcdb:	_makdir $(ODIR)/rpcdb
	$(COMMENT)

$(ODIR)/inetdb2:	$(ODIR)/inetdb
$(ODIR)/inetdb:	$(RDIR)/inetdb.date
	-$(CODO) $@
	-$(DEL)  $@
	-$(CODO) $@2
	-$(DEL)  $@2
	$(IDBGEN) -to=$(OS) -tp=$(CPU) -d=$(SDIR) $@
	$(ATTRE0) $@
	$(ATTRE0) $@2

$(ODIR)/rpcdb:	$(RDIR)/rpcdb.date
	-$(CODO) $@
	-$(DEL)  $@
	$(RPCDBGEN) -to=$(OS) -tp=$(CPU) -w=$(SDIR) -o=$@ $(RPCOPT)
	$(ATTRE0) $@

$(RDIR)/inetdb.date: $(SFILES)
	$(TOUCH) $@

$(RDIR)/rpcdb.date: $(RFILES)
	$(TOUCH) $@


_makdir: $(RDIR)
$(RDIR):
	-$(MAKDIR) $@

_clean: .
for NAME in $(TRGTS)
	-$(DEL) $(RDIR)/$(NAME).date
endfor
	$(COMMENT)

_purge: .
	-$(CODO) $(ODIR)/inetdb
	-$(DEL)  $(ODIR)/inetdb
	-$(CODO) $(ODIR)/inetdb2
	-$(DEL)  $(ODIR)/inetdb2
	-$(CODO) $(ODIR)/rpcdb
	-$(DEL)  $(ODIR)/rpcdb
	$(COMMENT)

#									    #
#############################################################################
