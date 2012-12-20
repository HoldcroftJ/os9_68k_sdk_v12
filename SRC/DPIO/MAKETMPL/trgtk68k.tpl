#############################################################################
#   $Workfile:   trgtk68k.tpl  $
#   $Revision:   1.5  $
#   $Modtime:   01 Nov 1994 11:23:00  $
#   $Date:   01 Nov 1994 14:51:10  $
#############################################################################
#   DPIO make template file for all hosts and  OS-9/68000 target            #
#   macro definitions.                                                      #
#############################################################################
#                                                                           #

# DPIO Defs
DPIO_DEFS		=	-v=$(DPIO)/DEFS $(MWOS_DEFS)

# DPIO Libs
DPIO_LIBDIR		=	$(MWOS)/OS9/68000/LIB/DPIO

DPIO_DESCSTART	=	$(DPIO_LIBDIR)/descstart.r
DPIO_SYSTART	=	$(DPIO_LIBDIR)/systart.r
DPIO_DRVSTART	=	$(DPIO_LIBDIR)/drvstart.r
DPIO_FMSTART	=	$(DPIO_LIBDIR)/fmstart.r
DPIO_MPFMSTART	=	$(DPIO_LIBDIR)/mpfmstart.r
DPIO_MPFM2START	=	$(DPIO_LIBDIR)/mpfm2start.r

# DPIO Commands
CHTYPE_DESC     =   $(DPIO_CHTYPE)MT_DEVDESC
CHTYPE_DRVR     =   $(DPIO_CHTYPE)MT_DEVDRVR
CHTYPE_FM       =   $(DPIO_CHTYPE)MT_FILEMAN
CHTYPE_PROG     =   $(DPIO_CHTYPE)MT_PROGRAM
CHTYPE_SYS      =   $(DPIO_CHTYPE)MT_SYSTEM 

#                                                                           #
#############################################################################
