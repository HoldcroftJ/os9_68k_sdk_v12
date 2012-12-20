#*****************************************************************************
#* Makefile for the MC68328ADS Init Module                                  **
#*****************************************************************************
#* Copyright 1996 by Microware Systems Corporation                          **
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
#*   1 04/30/96 creation                                          bat       **
#*   2 05/13/96 Adopted proper usage of port templates.           ats       **
#*              ---- OS-9/68K MC68328 Support Beta1 Release ----            **
#*   3 08/01/96 General cleanup.                                  ats       **
#*   4 08/02/96 Added flag to activate SysMbuf.                   ats       **
#*              ---- OS-9/68K MC68328 Support Beta2 Release ----            **
#*   5 12/05/96 changed TRGTS to dd_disk , included -asaSYSMBUF   bat       **
#*   6 02/11/97 added -asaPWRMAN define                           ecm       **
#*****************************************************************************

PORT    = ../..
TRGTS   = dd_disk                 # Init module name

include $(PORT)/../make.com

# Define for activating SysMbuf during system startup
SYSMBUF   = -asaSYSMBUF
PWRMAN  = -asaPWRMAN

INIT_OPTS = $(SYSMBUF) $(PWRMAN)
include $(PORT)/INIT/initmake.com

#                                                                           #
#############################################################################
