#############################################################################
#                                                                           #
#              Copyright 2001 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
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
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date     Comments                                                By   #
# --- -------- ------------------------------------------------------- ---- #
#  01 01/08/28 Added edition history.                                  mgh  #
#############################################################################
# Makefile a 16450 Serial I\O Module Drivers

-bo
-x

MWOS            = ../../../../../..
ROOT            = ../../../../..		# base of dir system

# base of dir system
BASEROOT	= $(ROOT)/68000				# dir system for LIB, etc
SRCROOT		= $(ROOT)/SRC				# dir system for source

SDIR		= $(SRCROOT)/IO/SCF/DRVR	# source dir
RDIR		= RELS
ODIR		= ../../CMDS/BOOTOBJS/SC16450

OSDEFS		= $(SRCROOT)/DEFS			# std OS defs
MACDIR		= $(SRCROOT)/MACROS			# std macros

SYSRELS		= $(BASEROOT)/LIB
SLIB		= $(SYSRELS)/sys.l
SYSDEFS		= $(OSDEFS)/oskdefs.d ../../systype.d
SCFSTAT		= $(SYSRELS)/scfstat.l
MACROS		= $(MACDIR)/os9svc.m $(MACDIR)/longio.m

MAKER		= ./scf_drivers.make		# this file

SPEC_RFLAGS	= -m0

# serial ports
#
SCFDRV		= sc16450					# note new driver name
SCFDRVR		= $(SCFDRV).r
SCFSRC		= $(SCFDRV).a				# serial driver source

RC			= r68						# assembler
RFLAGS		= -qb -u=. -u=$(OSDEFS) -u=$(MACDIR) $(SPEC_RFLAGS)

LC			= l68						# linker
LFLAGS		= -g -l=$(SLIB)

include     $(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything

_build: makdir $(SCFDRV)
	$(ECHO)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link SCF drivers

$(SCFDRV): $(SLIB) $(SCFDRVR)
	-$(CODO) $(ODIR)/$*
	-$(DEL)  $(ODIR)/$*
	-$(CODO) $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.stb
	-$(DEL)  $(ODIR)/STB/$*.dbg
	$(LC) $(LFLAGS) $(SCFSTAT) $(RDIR)/$(SCFDRVR) -O=$(ODIR)/$*


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly

$(SCFDRVR): $(SDIR)/$(SCFSRC) $(SYSDEFS) $(MACROS) $(MAKER)
	$(RC) $(RFLAGS) $(SDIR)/$(SCFSRC) -o=$(RDIR)/$*.r

makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB
		
clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(ODIR)/$(SCFDRV)
	-$(DEL)  $(ODIR)/$(SCFDRV) 
	-$(CODO) $(ODIR)/STB/$(SCFDRV).stb
	-$(DEL)  $(ODIR)/STB/$(SCFDRV).stb
	-$(DEL)  $(ODIR)/STB/$(SCFDRV).dbg
								  
								  
# end of file

