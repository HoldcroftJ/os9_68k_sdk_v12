# Makefile to ensure all SCSI related items are up to date - MVME172
#
-b
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
ALL_TRGTS	= k020

SRCROOT		= $(ROOT)/SRC/IO			# dir system for source

SCSILOWLEV	= $(SRCROOT)/SCSI			# low-level SCSI driver(s)
SCSIRBF		= $(SRCROOT)/RBF/DRVR/SCSI	# RBF SCSI drivers
SCSISBF		= $(SRCROOT)/SBF/DRVR/SCSI	# SBF SCSI drivers

# SCSI low-level (SCSI Interface) subroutine directories
#
LLDIRS		= SCSI53C710
LL_MAKE		= -f=scsi172.make

# SCSI RBF (Disk) driver directories
#
RBFDIRS		= RBVCCS RBTEAC

# SCSI SBF (Tape) directories
#
SBFDIRS		= SBVIPER SBTEAC

TRGTS		= $(LLDIRS) $(RBFDIRS) $(SBFDIRS)

include		$(MWOS)/MAKETMPL/makesub.com


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# check that modules are up to date
#

$(LLDIRS):
	$(CHD) $(SCSILOWLEV)/$@; $(MAKESUB) $(LL_MAKE)

$(RBFDIRS):
	$(CHD) $(SCSIRBF)/$@; $(MAKESUB)

$(SBFDIRS):
	$(CHD) $(SCSISBF)/$@; $(MAKESUB)


# end of file

