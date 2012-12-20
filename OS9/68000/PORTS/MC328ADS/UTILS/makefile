#############################################################################
#                                                                                                                                                       #
# FILE:  makefile                                                                                                                       #
#                                                                                                                                                       #
# This makefile will make the UTILS objects by invoking the                 #
# makefile in each sub-directory.                                                                                       #
#                                                                                                                                                       #
#############################################################################


#############################################################################
# YOU SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS POINT!                                      #
#                                                                                                                                                       #

PORT            =       ..
TRGTS           =       STARTDBG

include $(PORT)/../makesub.com

$(TRGTS): nulltrg
	-$(CHD) $*; $(MAKEIT) TARGET=$(TARGET) $(SUBTRGT)

nulltrg:  .
	$(COMMENT)

	
#                                                                                                                                                       #
# End of makefile                                                                                                                       #
#                                                                                                                                                       #
#############################################################################
