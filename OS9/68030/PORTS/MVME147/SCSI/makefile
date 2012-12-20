# Makefile to ensure all SCSI related items are up to date - MVME147
#
-b
-x

MWOS		= ../../../../..
ROOT		= ../../../..				# base of dir system
PORT		= ..

SRCROOT		= $(ROOT)/SRC/IO			# dir system for source

SCSILOWLEV	= $(SRCROOT)/SCSI			# low-level SCSI driver(s)
SCSIRBF		= $(SRCROOT)/RBF/DRVR/SCSI	# RBF SCSI drivers
SCSISBF		= $(SRCROOT)/SBF/DRVR/SCSI	# SBF SCSI drivers

# SCSI low-level (SCSI Interface) subroutine directories
#
LLDIRS		= SCSI33C93
LL_MAKE		= -f=scsi147.make

# SCSI RBF (Disk) driver directories
#
RBFDIRS		= RBVCCS RBTEAC

# SCSI SBF (Tape) directories
#
SBFDIRS		= SBVIPER SBTEAC

TRGTS		= $(LLDIRS) $(RBFDIRS) $(SBFDIRS)

include		$(PORT)/../makesub.com


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

