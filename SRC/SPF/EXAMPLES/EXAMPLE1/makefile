# Makefile
############################################################################
#
# Edition History:
#
# #   Date     Comments                                                By
# --- -------- -----------------------------------------------------  ---
#  01 ??/??/?? Created                                                ???
#  02 03/06/00 Changed from makespf.com to makesub.com                rds
############################################################################
# This makefile makes the example 1 send and receive programs for SPF 2.0  #
############################################################################
#                                                                          #

MWOS		=	../../../..
TRGTS		=	ex1_snd ex1_rcv
MAKENAME	=	makefile

include $(MWOS)/MAKETMPL/makesub.com

ex1_snd:  notarget
	$(MAKESUB) -f snd.mak

ex1_rcv:  notarget
	$(MAKESUB) -f rcv.mak

notarget:  .
	$(COMMENT)

#                                                                          #
############################################################################
