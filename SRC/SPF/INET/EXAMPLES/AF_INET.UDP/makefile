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
#  03 03/21/97 Changed to SPF BSD 4.4									rds
#     05/05/97 <--------- SPF LAN Comm Pack v3.1 Release ---------->
#  04 12/13/97 Use new makefile templates								rds
#     04/03/98 <--------- SPF LAN Comm Pack v3.2 Release ---------->
#  05 11/04/98 Add CODO, Add makedirs, Remove ALL_TRGT, add COPT_GU0	mgh
#  06 05/03/00 Fixed PURGE to purge correct files.  Added .map files.	dwj
############################################################################

MWOS		=	../../../../..
TRGTS		=	beam target
MAKENAME	=	makefile

include $(MWOS)/MAKETMPL/make.com
include $(MWOS)/MAKETMPL/lancom.tpl

IDIR		=	$(MWOS_IDIR)
RDIR		=	$(MWOS_RDIR)
ODIR		=	$(MWOS_ODIR)

BRELS		=	$(CSTART) $(RDIR)/beam.r
BIRELS		=	$(IDIR)/beam.i
TRELS		=	$(CSTART) $(RDIR)/target.r
TIRELS		=	$(IDIR)/target.i

DEFINES		=	#-dUSE_CONNECT
DEFS		=	$(BSDDEFS) $(MWOS_DEFS)
LIBS		=	$(LAN_LIBS) $(MWOS_CSLLIBS)

OPTMZ		=

COMPILE		=	$(CC) $(CPARMS) $(OPTMZ) -efe=$(IDIR) $(DEFS) $(DEFINES) \
				$(COPTS)
ILINK		=	$(CC) $(CPARMS) $(OPTMZ) -k -eas=$(RDIR) $(DEFS) $(LIBS) \
				$(COPTS)
LINK		=	$(LINKER) $(LPARMS) $(OPTMZ) -tp=$(CPUC) -k -olS  $(LIBS) \
				$(LOPTS) $(COPT_GU0)


beam:  $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/beam
	$(COMMENT)
target:  $(IDIR) $(RDIR) $(ODIR)/STB $(ODIR)/target
	$(COMMENT)

$(IDIR): nulltrg
	-$(MAKDIR) $@
$(RDIR): nulltrg
	-$(MAKDIR) $@
$(ODIR)/STB: nulltrg
	-$(MAKDIR) $@

$(ODIR)/beam:  $(BRELS)
	-$(CODO) $(ODIR)/beam
	-$(DEL)  $(ODIR)/beam
	-$(CODO) $(ODIR)/STB/beam.stb
	-$(DEL)  $(ODIR)/STB/beam.stb
	-$(CODO) $(ODIR)/STB/beam.map
	-$(DEL)  $(ODIR)/STB/beam.map
	-$(DEL)  $(ODIR)/STB/beam.dbg
	$(LINK)  $(BRELS) -fd=$(ODIR)/beam -ols=$(ODIR)/STB/beam.map

$(ODIR)/target:  $(TRELS)
	-$(CODO) $(ODIR)/target
	-$(DEL)  $(ODIR)/target
	-$(CODO) $(ODIR)/STB/target.stb
	-$(DEL)  $(ODIR)/STB/target.stb
	-$(CODO) $(ODIR)/STB/target.map
	-$(DEL)  $(ODIR)/STB/target.map
	-$(DEL)  $(ODIR)/STB/target.dbg
	$(LINK)  $(TRELS) -fd=$(ODIR)/target -ols=$(ODIR)/STB/target.map

$(RDIR)/beam.r:  $(BIRELS)
	$(ILINK) $(BIRELS) -fd=beam.r
$(RDIR)/target.r:  $(TIRELS)
	$(ILINK) $(TIRELS) -fd=target.r

$(IDIR)/beam.i:  beam.c
	$(COMPILE) beam.c
$(IDIR)/target.i:  target.c
	$(COMPILE) target.c

_makdir:  nulltrg
	-$(MAKDIR) $(IDIR)
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB

_clean:  nulltrg
	-$(DEL) $(IDIR)/*.i
	-$(DEL) $(RDIR)/*.r
	$(COMMENT)

_purge:  nulltrg
	$(CODO) $(ODIR)/beam
	-$(DEL) $(ODIR)/beam
	$(CODO) $(ODIR)/STB/beam.stb
	-$(DEL) $(ODIR)/STB/beam.stb
	$(CODO) $(ODIR)/STB/beam.map
	-$(DEL) $(ODIR)/STB/beam.map
	$(CODO) $(ODIR)/target
	-$(DEL) $(ODIR)/target
	$(CODO) $(ODIR)/STB/target.stb
	-$(DEL) $(ODIR)/STB/target.stb
	$(CODO) $(ODIR)/STB/target.map
	-$(DEL) $(ODIR)/STB/target.map
	$(COMMENT)

nulltrg:  .
	$(COMMENT)


#                                                                          #
############################################################################
