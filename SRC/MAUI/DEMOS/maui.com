
MWOS        =   ../../../.. # relative from DEMO subdirectories

include $(MWOS)/MAKETMPL/make.com

SDIR        =   .
RDIR        =   RELS/$(TARGET)
IDIR        =   $(RDIR)/$(HOSTTYPE)
ODIR        =   $(MWOS_ODIR)/MAUIDEMO

DEFINES     =   
DEFS        =   -v=. $(MWOS_DEFS)

LIBS_DEP    =   $(MWOS_LIBDIR)/mauidemo.l $(MWOS_ILIBDIR)/mauidemo.il \
                $(MWOS_LIBDIR)/iff.l $(MWOS_ILIBDIR)/iff.il \
                $(MWOS_DFTLIB)/maui.l $(MWOS_ILIBDIR)/maui.il \
                $(MWOS_DFTLIB)/mauilib.l $(MWOS_ILIBDIR)/mauilib.il \
                $(MWOS_DFTLIB)/mfm.l $(MWOS_ILIBDIR)/mfm.il

# standard shared-libraries used to compile the MAUI demos
SHARLIBS        =   -l=$(MWOS_LIBDIR)/mauidemo.l -y=$(MWOS_ILIBDIR)/mauidemo.il \
                -l=$(MWOS_LIBDIR)/iff.l -y=$(MWOS_ILIBDIR)/iff.il \
                -l=$(MWOS_DFTLIB)/maui.l -y=$(MWOS_ILIBDIR)/maui.il \
                $(MWOS_LIBS)

# alternative libraries, mainly used when debugging
HARDLIBS    =   -l=$(MWOS_LIBDIR)/mauidemo.l -y=$(MWOS_ILIBDIR)/mauidemo.il \
                -l=$(MWOS_LIBDIR)/iff.l -y=$(MWOS_ILIBDIR)/iff.il \
                -l=$(MWOS_DFTLIB)/mauilib.l -y=$(MWOS_ILIBDIR)/mauilib.il \
                -l=$(MWOS_DFTLIB)/mfm.l -y=$(MWOS_ILIBDIR)/mfm.il \
                $(MWOS_LIBS)

MAUI_COM_OPTS =  # used for override in maui.com make include
LOCAL_MAKEOPTS = # useful for override in initial makefile
DEBUG       = # envoke debug mode with "os9make MOPTS=DEBUG=-g"

if ($(DEBUG) == "")
LIBS	    =	$(SHARLIBS)
else
LIBS	    =	$(HARDLIBS)
endif

OPTMZ       =   $(DEBUG)
LOCAL_COPTS =   $(DEFS) $(OPTMZ) $(DEFINES) $(COPTS) $(COPT_CODEADDR) \
                $(MAUI_COM_OPTS) $(LOCAL_MAKEOPTS)

CC_COMMON	=  	$(CC) $(CPARMSNC)$(COPT_LD)$(COPT_LC)$(COPT_LCD) $(LOCAL_COPTS) -r
COMPILE		=	$(CC_COMMON) -eil=$(IDIR) 
COMPILE_MT	=	$(CC_COMMON) -mt -eil=$(IDIR)/MT
LINK     	=   $(CC_COMMON) $(LIBS) -olg
ILINK		=	$(LINK) -eas=$(RDIR) 
ILINK_MT		=	$(LINK) -mt -eas=$(RDIR)/MT

MKLIB       =   $(LIBGEN)
MKLIB_MT    =   $(MKLIB) -mts
MKILIB      =   $(CC) $(CPARMS) $(COPT_CODEADDR) $(COPT_LONGCODE) \
	            $(COPT_LONGDATA) -ilm -eil
MKILIB_MT   =   $(MKILIB) -ilmts
