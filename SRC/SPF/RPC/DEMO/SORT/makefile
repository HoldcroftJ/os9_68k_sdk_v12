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
#  00 ??/??/?? Created
#  01 10/01/93 Mods for mmove to MWOS structure                       gkm
#  02 10/14/93 make rsort & sortd sticky                              gkm
#  03 10/21/93 <--------- OS-9 NFS 1.3 Released ------------------->
#  05 01/31/94 make template reorganization                           bat
#     03/06/95 Changed make variables to use new macro COPT_CODEADDR   sr
#     05/01/95 Added SELECT_LIBS to List of Libraries                  sr
#  06 05/16/95 Added Targets for o386 and pppc.                        sr
#  07 01/03/96 Commented o386: not supported by compiler anymore.     pjw
#  08 03/05/96 Changed SELECT_LIBS to UNIXLIBS                        rds
#     05/10/96 Temporarily added PUNIXLIB to get 68020 version this   rds
#              should be removed when make templates are fixed
#     05/10/96 <--------- NFS 1.4 Server Release ------------------>
#     09/05/96 Removed TARGET for k68k                                pjw
#  09 04/10/97 Converted from ISP to SPF                              rds
#              Removed PUNIXLIB                                       rds
#  10 05/12/97 Added arm3 to targets, change rpclib.l to rpc.l        rds
#  11 10/08/97 Added BSDDEFS                                          pjw
#  12 12/11/97 Changed RPCDEFSDIR, use new makefile templates         rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  13 11/05/98 Add CODO, Add makedirs, Remove ALL_TRGT etc            mgh
#  14 02/22/99 Took out multiple dependency line for same .r		  rry
#  15 09/15/00 removed undefined nulltrg                              srm
############################################################################
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!                 #
#                                                                          #

NFS		=	../..
MWOS		=	$(NFS)/../../..
TRGTS		=	rsort sortd

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

START		=	$(CSTART)

RFILES_CLNT	= $(RDIR)/sort_clnt.r $(RDIR)/sort_xdr.r $(RDIR)/rsort.r
RFILES_SVR	= $(RDIR)/sort_svc.r $(RDIR)/sort_proc.r $(RDIR)/sort_xdr.r

OPTMZ		=	-o=7 -s=1 -t=1

COMPILE		=	$(CC) $(CPARMS) $(COPT_CODEADDR) $(DEFS) $(DEFINES) \
				-eas=$(RDIR) $(OPTMZ) -r $(COPTS)
ASSEMBLE	=	$(ASM) $(APARMS) $(COPT_CODEADDR) $(DEFS) $(OPTMZ) \
				-eas=$(RDIR) -r $(AOPTS)
LINK		=	$(LINKER) $(LPARMS) $(COPT_CODEADDR) -tp=$(CPUC) \
				-olS -k -r $(LIBS) $(OPTMZ) $(LOPTS) $(COPT_GU1)

rsort:	$(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/rsort
	$(COMMENT)
sortd:	$(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/sortd
	$(COMMENT)

$(IDIR):
	-$(MAKDIR) $@
$(RDIR):
	-$(MAKDIR) $@
$(ODIR)/STB:
	-$(MAKDIR) $@


$(ODIR)/rsort:		$(RFILES_CLNT) #$(RPCLIBFILE)
	-$(CODO) $(ODIR)/rsort
	-$(DEL)  $(ODIR)/rsort
	-$(CODO) $(ODIR)/STB/rsort.stb
	-$(DEL)  $(ODIR)/STB/rsort.stb
	-$(DEL)  $(ODIR)/STB/rsort.dbg
	$(LINK)  $(START) $(RFILES_CLNT) -fd=$(ODIR)/rsort -oln=rsort
	$(ATTRE1) $(ODIR)/rsort
$(ODIR)/sortd:		$(RFILES_SVR) #$(RPCLIBFILE)
	-$(CODO) $(ODIR)/sortd
	-$(DEL)  $(ODIR)/sortd
	-$(CODO) $(ODIR)/STB/sortd.stb
	-$(DEL)  $(ODIR)/STB/sortd.stb
	-$(DEL)  $(ODIR)/STB/sortd.dbg
	$(LINK)  $(START) $(RFILES_SVR) -fd=$(ODIR)/sortd -oln=sortd
	$(ATTRE1) $(ODIR)/sortd

$(RDIR)/sort_clnt.r: sort_clnt.c $(DEPENDDEFS) sort.h
	$(COMPILE) sort_clnt.c
$(RDIR)/sort_xdr.r: sort_xdr.c $(DEPENDDEFS) sort.h
	$(COMPILE) sort_xdr.c
$(RDIR)/rsort.r: rsort.c $(DEPENDDEFS) sort.h
	$(COMPILE) rsort.c
$(RDIR)/sort_svc.r: sort_svc.c $(DEPENDDEFS) sort.h
	$(COMPILE) sort_svc.c
$(RDIR)/sort_proc.r: sort_proc.c $(DEPENDDEFS) sort.h
	$(COMPILE) sort_proc.c

_makdir:  $(IDIR) $(RDIR) $(ODIR)/STB

_clean: .
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(IDIR)/*.i

_purge: .
	-$(CODO) $(ODIR)/rsort
	-$(DEL)  $(ODIR)/rsort
	-$(CODO) $(ODIR)/STB/rsort.stb
	-$(DEL)  $(ODIR)/STB/rsort.stb
	-$(DEL)  $(ODIR)/STB/rsort.dbg
	-$(CODO) $(ODIR)/sortd
	-$(DEL)  $(ODIR)/sortd
	-$(CODO) $(ODIR)/STB/sortd.stb
	-$(DEL)  $(ODIR)/STB/sortd.stb
	-$(DEL)  $(ODIR)/STB/sortd.dbg

#                                                                           #
# End of makefile                                                           #
#                                                                           #
#############################################################################

