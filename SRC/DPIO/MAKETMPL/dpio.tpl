#############################################################################
#   $Workfile:   dpio.tpl  $
#   $Revision:   1.1  $
#   $Modtime:   10 Oct 1994 12:15:00  $
#   $Date:   10 Oct 1994 13:45:34  $
#############################################################################
#	DPIO make template file for system-wide maro definitions.				#
#############################################################################

#############################################################################
#	[DPIO] must be predefined to the location of the DPIO directory, by		#
#	either full or partial pathlist.										#
#############################################################################
#	[TRGTS] must be predefined to the default target(s) which should be 	#
#	made by this makefile.													#
#############################################################################
#	The following targets must be defined in the makefile:					#
#		$(TRGTS):	(see above)												#
#		_clean:		remove all intermediate (.r/.i) files					#
#		_purge:		remove all target (object) files						#
#############################################################################
#	[TARGET] can be overriden on the command line if an alternate targe is 	#
#	to be made.																#
#############################################################################

#############################################################################
#																			#

# Get DPIO Target Specific Definitions
include $(DPIO)/MAKETMPL/trgt$(OS).tpl

#																			#
#############################################################################
