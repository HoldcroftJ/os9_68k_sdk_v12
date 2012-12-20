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
# Makefile for the SC16450 Example SCF Descriptors

-bo
-x

MWOS            = ../../../../../..
ROOT            = ../../../../..        	# base of dir system

BASEROOT        = $(ROOT)/68000				# dir system for LIB, etc
SRCROOT         = $(ROOT)/SRC				# dir system for sources
OSDEFS          = $(SRCROOT)/DEFS			# std OS defs

SDIR            = $(SRCROOT)/IO/SCF/DESC 	# source files
ODIR            = ../../CMDS/BOOTOBJS/SC16450	# output
RDIR            = RELS

SYSRELS         = $(BASEROOT)/LIB
SLIB            = $(SYSRELS)/sys.l
SYSDEFS         = $(OSDEFS)/oskdefs.d ../../systype.d

MAKER           = ./scf_descriptors.make 	# this file

SPEC_RFLAGS     = -aFASTCONS -aFASTCOM  	# special flags

RC				= r68						# assembler
RFLAGS          = -q -u=. -u=$(OSDEFS) $(SPEC_RFLAGS)

LC				= l68						# linker
LFLAGS          = -l=$(SLIB)

# serial ports
#
SCFDSC          = term   t1   t2   t3   p1   p2   p3
SCFDSCR         = term.r t1.r t2.r t3.r p1.r p2.r p3.r

include     $(MWOS)/MAKETMPL/makeosk.tpl

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Link everything
#
_build: makdir $(SCFDSC)
	$(ECHO)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# link serial port descriptors
#
$(SCFDSC) : $(SLIB) $(SCFDSCR)
	$(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR)/$*

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Take care of the assembly
#
$(SCFDSCR): $(SYSDEFS) $(MAKER)

makdir _makdir:
	-$(MAKDIR) $(RDIR)
	-$(MAKDIR) $(ODIR)/STB
	
clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	-$(CODO) $(ODIR)/$(SCFDSC)
	-$(DEL)  $(ODIR)/$(SCFDSC)
					
					
# end of file

