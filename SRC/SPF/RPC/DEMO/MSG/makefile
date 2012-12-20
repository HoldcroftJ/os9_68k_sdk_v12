# Makefile
############################################################################
#           Copyright 1989-1998 by Microware Systems Corporation           #
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
#  01 11/22/33 Created                                                rds
#  01 ??/??/?? Created
#  01 10/01/93 Mods for move to MWOS structure                        gkm
#  02 10/14/93 Make rmsg & msgd sticky                                gkm
#  03 10/21/93 <--------- OS-9 NFS 1.3 Released ------------------->
#  05 01/31/94 make template reorganization                           bat
#     03/06/95 Changed make variables to use new macro COPT_CODEADDR   sr
#     05/01/95 Added SELECT_LIBS to List of Libraries                  sr
#  06 05/16/95 Added Target for o386 and pppc.                         sr
#  07 01/03/96 Commented o386: not supported by compiler anymore.     pjw
#  08 03/05/96 Changed SELECT_LIBS to UNIXLIBS                        rds
#  09 05/14/96 Fixed typo where msgd was spelled as msgds             rds
#     05/14/96 <--------- NFS 1.4 Server Release ------------------>
#     09/05/96 Removed TARGET for k68k                                pjw
#  10 04/10/97 Converted from ISP to SPF                              rds
#  11 05/12/97 Added arm3 to targets, change rpclib.l to rpc.l        rds
#  12 10/08/97 Added BSDDEFS                                          pjw
#  13 12/11/97 Changed RPCDEFSDIR, use new makefile templates         rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  14 11/05/98 Add CODO, Add makedirs, Remove ALL_TRGT etc            mgh
#  15 12/03/98 Remove space from front of target _clean.              sr 
#  16 02/22/99 Removed multiple declarations of same .r				  rry
#  17 09/15/00 removed undefined nulltrg                              srm
############################################################################
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!                 #
#                                                                          #

NFS		=	../..
MWOS		=	$(NFS)/../../..
TRGTS		=	rmsg msgd

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

DEFINES		=
DEFS		=	-v=$(NFS)/DEFS $(BSDDEFS) $(SPFDEFS) $(MWOS_DEFS)

LIBS		=	$(RPCS) $(UNIXLIBS) $(LAN_LIBS) $(MWOS_CSLLIBS)

RPCDEFSDIR	=	$(MWOS)/SRC/DEFS/SPF/RPC
DEPENDDEFS	=	$(RPCDEFSDIR)/rpc.h $(RPCDEFSDIR)/types.h \
				$(RPCDEFSDIR)/xdr.h $(RPCDEFSDIR)/auth.h \
				$(RPCDEFSDIR)/clnt.h $(RPCDEFSDIR)/rpc_msg.h \
        		$(RPCDEFSDIR)/auth_unix.h $(RPCDEFSDIR)/svc.h \
        		$(RPCDEFSDIR)/svc_auth.h

START	=	$(CSTART)

RFILES_CLNT	= $(RDIR)/msg_clnt.r $(RDIR)/msg_xdr.r $(RDIR)/rmsg.r
RFILES_SVR	= $(RDIR)/msg_svc.r $(RDIR)/msg_proc.r $(RDIR)/msg_xdr.r

OPTMZ		=	-o=7 -s=1 -t=1

COMPILE		=	$(CC) $(CPARMS) $(COPT_CODEADDR) $(DEFS) $(DEFINES) \
				-eas=$(RDIR) $(OPTMZ) -r $(COPTS)
ASSEMBLE	=	$(ASM) $(APARMS) $(COPT_CODEADDR) $(DEFS) $(OPTMZ) \
				-eas=$(RDIR) -r $(AOPTS)
LINK		=	$(LINKER) $(LPARMS) $(COPT_CODEADDR) -tp=$(CPUC) \
				-olS -k -r $(LIBS) $(OPTMZ) $(LOPTS) $(COPT_GU1)

rmsg:	$(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/rmsg
	$(COMMENT)
msgd:	$(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/msgd
	$(COMMENT)

$(IDIR):
	-$(MAKDIR) $@
$(RDIR):
	-$(MAKDIR) $@
$(ODIR)/STB:
	-$(MAKDIR) $@

$(ODIR)/rmsg:	$(RFILES_CLNT) #$(RPCLIBFILE)
	-$(CODO) $(ODIR)/rmsg
	-$(DEL)  $(ODIR)/rmsg
	-$(CODO) $(ODIR)/STB/rmsg.stb
	-$(DEL)  $(ODIR)/STB/rmsg.stb
	-$(DEL)  $(ODIR)/STB/rmsg.dbg
	$(LINK)  $(START) $(RFILES_CLNT) -fd=$(ODIR)/rmsg -oln=rmsg
	$(ATTRE1) $(ODIR)/rmsg
$(ODIR)/msgd:	$(RFILES_SVR) #$(RPCLIBFILE)
	-$(CODO) $(ODIR)/msgd
	-$(DEL)  $(ODIR)/msgd
	-$(CODO) $(ODIR)/STB/msgd.stb
	-$(DEL)  $(ODIR)/STB/msgd.stb
	-$(DEL)  $(ODIR)/STB/msgd.dbg
	$(LINK)  $(START) $(RFILES_SVR) -fd=$(ODIR)/msgd -oln=msgd
	$(ATTRE1) $(ODIR)/msgd

$(RDIR)/msg_clnt.r: msg_clnt.c $(DEPENDDEFS) msg.h
	$(COMPILE) msg_clnt.c
$(RDIR)/msg_xdr.r: msg_xdr.c $(DEPENDDEFS) msg.h
	$(COMPILE) msg_xdr.c
$(RDIR)/rmsg.r: rmsg.c $(DEPENDDEFS) msg.h
	$(COMPILE) rmsg.c
$(RDIR)/msg_svc.r: msg_svc.c $(DEPENDDEFS) msg.h
	$(COMPILE) msg_svc.c
$(RDIR)/msg_proc.r: msg_proc.c $(DEPENDDEFS) msg.h
	$(COMPILE) msg_proc.c

_makdir:  $(IDIR) $(RDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(IDIR)/*.i

_purge:  .
	-$(CODO) $(ODIR)/rmsg
	-$(DEL)  $(ODIR)/rmsg
	-$(CODO) $(ODIR)/STB/rmsg.stb
	-$(DEL)  $(ODIR)/STB/rmsg.stb
	-$(DEL)  $(ODIR)/STB/rmsg.dbg
	-$(CODO) $(ODIR)/msgd
	-$(DEL)  $(ODIR)/msgd
	-$(CODO) $(ODIR)/STB/msgd.stb
	-$(DEL)  $(ODIR)/STB/msgd.stb
	-$(DEL)  $(ODIR)/STB/msgd.dbg

#                                                                           #
# End of makefile                                                           #
#                                                                           #
#############################################################################
