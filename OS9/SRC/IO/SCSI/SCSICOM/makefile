# makefile to produce SCSI System Common Modules
#
# These modules provide the interfaces between the File Managers,
# C Drivers and Low-Level module.  Changing these files can have
# SERIOUS repercussions, so be carefull !

MWOS		= ../../../../..

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

SDIR		= .
RDIR		= RELS

ROOT		= ../../../..				# path to root of dir system
DEFSROOT	= $(ROOT)/SRC/DEFS			# std defs dir

SYSDEFS		= $(DEFSROOT)/oskdefs.d		# included by defsfile

RC			= r68
RFLAGS		= -qb -u=$(DEFSROOT)

# these are the scsi-system 'common' modules.
# they will be linked when the actual driver is made,
# and they MUST be linked first (they contain the root psect)

RBFIF		= rbglue.r					# RBF --> C-Driver interface
SBFIF		= sbglue.r					# SBF --> C-Driver interface
OS9IF		= syscalls.r				# C-Driver --> OS-9 System Calls interface
LLIF		= scsiglue.r				# High-Level --> Low-Level interface

RFILES		= $(RBFIF) $(SBFIF) $(OS9IF) $(LLIF)

TOUCH		= touch						# touch utility name

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make the interfaces
#
scsicom.date:  makdir $(RFILES)
	$(TOUCH) $@

$(RBFIF):	rbglue.a $(SYSDEFS)

$(SBFIF):	sbglue.a $(SYSDEFS)

$(OS9IF):	syscalls.a

$(LLIF):	scsiglue.a $(SYSDEFS)

makdir _makdir:
    -$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/*.r

purge _purge:
	$(COMMENT)

# end of file

