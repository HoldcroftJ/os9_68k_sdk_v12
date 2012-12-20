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
#  01 ??/??/?? Created
#  02 09/27/93 Mods for move to MWOS structure                        gkm
#  03 10/14/93 make rdir dird sticky                                  gkm
#  04 10/21/93 removed current directory from include search list to  gkm
#              resolve difference between #include <dir.h> and
#              #include "dir.h"
#              <--------- OS-9 NFS 1.3 Released ------------------->
#  05 01/31/94 make template reorganization                           bat
#  06 03/03/95 fixed dird link dependency, added o386, pppc            sr
#     03/06/95 Changed make variables to use new macro COPT_CODEADDR   sr
#     05/01/95 Added SELECT_LIBS to List of Libraries                  sr
#  07 01/03/96 Commented o386: not supported by compiler anymore.     pjw
#  08 03/05/96 Changed SELECT_LIBS to UNIXLIBS                        rds
#  09 05/06/96 Increased stack size to 50K due to overflow problems   rds
#     04/22/96 <--------- NFS 1.4 Server Release ------------------>
#     09/05/96 Removed TARGET for k68k                                pjw
#  10 04/10/97 Converted from ISP to SPF                              rds
#  11 05/12/97 Added arm3 to targets, change rpclib.l to rpc.l        rds
#  12 06/09/97 Changed to include BSD4.4 header files                 apw
#  13 12/11/97 Changed RPCDEFSDIR, use new makefile templates         rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  14 11/05/98 Add CODO, Add makedirs, Remove ALL_TRGT etc            mgh
#  15 02/22/99 Took out multiple identical dependencies				  rry
#  16 09/15/00 removed undefined nulltrg                              srm
############################################################################

NFS		=	../..
MWOS		=	$(NFS)/../../..
TRGTS		=	rdir dird

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

RFILES_CLNT	=	$(CSTART) $(RDIR)/dir_clnt.r $(RDIR)/dir_xdr.r $(RDIR)/rdir.r
RFILES_SVR	=	$(CSTART) $(RDIR)/dir_svc.r $(RDIR)/dir_proc.r $(RDIR)/dir_xdr.r

OPTMZ		=	-o=7 -s=1 -t=1
COMPILE		=	$(CC) $(CPARMS) $(COPT_CODEADDR) $(DEFS) $(DEFINES) \
				-eas=$(RDIR) $(OPTMZ) -r $(COPTS)
ASSEMBLE	=	$(ASM) $(APARMS) $(COPT_CODEADDR) $(DEFS) $(OPTMZ) \
				-eas=$(RDIR) -r $(AOPTS)
LINK		=	$(LINKER) $(LPARMS) $(COPT_CODEADDR) -tp=$(CPUC) \
				-olS -olM=50K -k -r $(LIBS) $(OPTMZ) $(LOPTS) \
				$(COPT_GU1)

rdir:  $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/rdir
	$(COMMENT)
dird:  $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/dird
	$(COMMENT)

$(IDIR):
	-$(MAKDIR) $@
$(RDIR):
	-$(MAKDIR) $@
$(ODIR)/STB:
	-$(MAKDIR) $@


$(ODIR)/rdir:  $(RFILES_CLNT) #$(RPCLIBFILE)
	-$(CODO) $(ODIR)/rdir
	-$(DEL)  $(ODIR)/rdir
	-$(CODO) $(ODIR)/STB/rdir.stb
	-$(DEL)  $(ODIR)/STB/rdir.stb
	-$(DEL)  $(ODIR)/STB/rdir.dbg
	$(LINK)  $(RFILES_CLNT) -fd=$(ODIR)/rdir -oln=rdir
	$(ATTRE1) $(ODIR)/rdir
$(ODIR)/dird:  $(RFILES_SVR) #$(RPCLIBFILE)
	-$(CODO) $(ODIR)/dird
	-$(DEL)  $(ODIR)/dird
	-$(CODO) $(ODIR)/STB/dird.stb
	-$(DEL)  $(ODIR)/STB/dird.stb
	-$(DEL)  $(ODIR)/STB/dird.dbg
	$(LINK)  $(RFILES_SVR) -fd=$(ODIR)/dird -oln=dird
	$(ATTRE1) $(ODIR)/dird

$(RDIR)/dir_clnt.r: dir_clnt.c $(DEPENDDEFS) dir.h
	$(COMPILE) dir_clnt.c
$(RDIR)/dir_xdr.r: dir_xdr.c $(DEPENDDEFS) dir.h
	$(COMPILE) dir_xdr.c
$(RDIR)/rdir.r: rdir.c $(DEPENDDEFS) dir.h
	$(COMPILE) rdir.c
$(RDIR)/dir_svc.r: dir_svc.c $(DEPENDDEFS) dir.h
	$(COMPILE) dir_svc.c
$(RDIR)/dir_proc.r: dir_proc.c $(DEPENDDEFS) dir.h
	$(COMPILE) dir_proc.c

_makdir:  $(IDIR) $(RDIR) $(ODIR)/STB

_clean:  .
	-$(DEL) $(RDIR)/*.r
	-$(DEL) $(IDIR)/*.i

_purge:  .
	-$(CODO) $(ODIR)/rdir
	-$(DEL)  $(ODIR)/rdir
	-$(CODO) $(ODIR)/STB/rdir.stb
	-$(DEL)  $(ODIR)/STB/rdir.stb
	-$(DEL)  $(ODIR)/STB/rdir.dbg
	-$(CODO) $(ODIR)/dird
	-$(DEL)  $(ODIR)/dird
	-$(CODO) $(ODIR)/STB/dird.stb
	-$(DEL)  $(ODIR)/STB/dird.stb
	-$(DEL)  $(ODIR)/STB/dird.dbg


#                                                                           #
# End of makefile                                                           #
#                                                                           #
#############################################################################
