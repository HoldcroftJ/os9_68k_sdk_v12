# make file to update OS-9/68K System libraries
#
-bo

MWOS		= ../../..

TRGTS	= sys.l
RDIR    = RELS

LDIR    = ../../68000/LIB		# global library directory (MWOS)
#LDIR	= LIB					# local version

RC		= r68					# assembler to use
RFLAGS  = -q

include		$(MWOS)/MAKETMPL/makeosk.tpl

SYSL    = sysio.r sysglob.r
USRL    = funcs.r io.r module.r process.r event.r

# Make sys.l system value library
#
_build sys.l: $(RDIR) $(LDIR)/sys.l
	$(COMMENT)

$(LDIR)/sys.l: $(USRL) $(SYSL) $(LDIR)/scfstat.l $(LDIR)/drvs1.l \
		$(LDIR)/ucstat.l $(LDIR)/cdidrvs1.l
	-$(CODO) $(LDIR)/sys.l
	-$(DEL)  $(LDIR)/sys.l
	-$(CODO) $(LDIR)/usr.l
	-$(DEL)  $(LDIR)/usr.l
	$(CHD) $(RDIR); $(MERGE) $(USRL) $(REDIR_OUT)../$(LDIR)/usr.l
	$(CHD) $(RDIR); $(MERGE) $(SYSL) ../$(LDIR)/usr.l $(REDIR_OUT)../$(LDIR)/sys.l

sysio.r:   sysio.a iodev.a rbfdev.a scfdev.a cdfmdev.a ucmdev.a
sysglob.r: sysglob.a ioglob.a

scfstat.r: scfstat.a iodev.a scfdev.a

# make SCF static storage library
#
$(LDIR)/scfstat.l: scfstat.r
	-$(CODO) $(LDIR)/scfstat.l
	-$(DEL)  $(LDIR)/scfstat.l
    $(MERGE) $(RDIR)/scfstat.r $(REDIR_OUT)$(LDIR)/scfstat.l

# make RBF static storage/drive-table libraries
#
$(LDIR)/drvs1.l: rbfstat.r drvstat.r
	-$(CODO) $(LDIR)/drvs1.l
	-$(DEL)  $(LDIR)/drvs1.l
	-$(CODO) $(LDIR)/drvs2.l
	-$(DEL)  $(LDIR)/drvs2.l
	-$(CODO) $(LDIR)/drvs3.l
	-$(DEL)  $(LDIR)/drvs3.l
	-$(CODO) $(LDIR)/drvs4.l
	-$(DEL)  $(LDIR)/drvs4.l
	-$(CODO) $(LDIR)/drvs8.l
	-$(DEL)  $(LDIR)/drvs8.l
	-$(CODO) $(LDIR)/drvs12.l
	-$(DEL)  $(LDIR)/drvs12.l
	-$(CODO) $(LDIR)/drvs20.l
	-$(DEL)  $(LDIR)/drvs20.l
    $(MERGE) $(RDIR)/rbfstat.r $(RDIR)/drvstat.r $(REDIR_OUT)$(LDIR)/drvs1.l
    $(MERGE) $(LDIR)/drvs1.l $(RDIR)/drvstat.r $(REDIR_OUT)$(LDIR)/drvs2.l
    $(MERGE) $(LDIR)/drvs2.l $(RDIR)/drvstat.r $(REDIR_OUT)$(LDIR)/drvs3.l
    $(MERGE) $(LDIR)/drvs3.l $(RDIR)/drvstat.r $(REDIR_OUT)$(LDIR)/drvs4.l
    $(MERGE) $(LDIR)/drvs4.l $(RDIR)/drvstat.r $(RDIR)/drvstat.r \
    	$(RDIR)/drvstat.r $(RDIR)/drvstat.r $(REDIR_OUT)$(LDIR)/drvs8.l
    $(MERGE) $(LDIR)/drvs8.l $(RDIR)/drvstat.r $(RDIR)/drvstat.r \
    	$(RDIR)/drvstat.r $(RDIR)/drvstat.r $(REDIR_OUT)$(LDIR)/drvs12.l
    $(MERGE) $(LDIR)/drvs12.l $(RDIR)/drvstat.r $(RDIR)/drvstat.r \
    	$(RDIR)/drvstat.r $(RDIR)/drvstat.r $(RDIR)/drvstat.r \
		$(RDIR)/drvstat.r $(RDIR)/drvstat.r $(RDIR)/drvstat.r \
		$(REDIR_OUT)$(LDIR)/drvs20.l

rbfstat.r: rbfstat.a iodev.a rbfdev.a
drvstat.r: drvstat.a iodev.a rbfdev.a

ucmstat.r: ucmstat.a iodev.a ucmdev.a

# make UCM static storage library
#
$(LDIR)/ucstat.l: ucmstat.r
	-$(CODO) $(LDIR)/ucstat.l
	-$(DEL)  $(LDIR)/ucstat.l
    $(MERGE) $(RDIR)/ucmstat.r $(REDIR_OUT)$(LDIR)/ucstat.l

# make CDFM static storage/drive-table libarys
#
$(LDIR)/cdidrvs1.l: cdfmstat.r cdrvstat.r
	-$(CODO) $(LDIR)/cdidrvs1.l
	-$(DEL)  $(LDIR)/cdidrvs1.l
	-$(CODO) $(LDIR)/cdidrvs2.l
	-$(DEL)  $(LDIR)/cdidrvs2.l
    $(MERGE) $(RDIR)/cdfmstat.r $(RDIR)/cdrvstat.r $(REDIR_OUT)$(LDIR)/cdidrvs1.l
    $(MERGE) $(LDIR)/cdidrvs1.l $(RDIR)/cdrvstat.r $(REDIR_OUT)$(LDIR)/cdidrvs2.l

$(RDIR)/cdfmstat.r: cdfmstat.a cdfmdev.a
$(RDIR)/cdrvstat.r: cdrvstat.a


$(RDIR):
	-$(MAKDIR) $@
 
clean _clean: .
	-$(DEL) $(RDIR)/sysio.r
	-$(DEL) $(RDIR)/sysglob.r
	-$(DEL) $(RDIR)/funcs.r
	-$(DEL) $(RDIR)/io.r
	-$(DEL) $(RDIR)/module.r
	-$(DEL) $(RDIR)/process.r
	-$(DEL) $(RDIR)/event.r
	-$(DEL) $(RDIR)/scfstat.r
	-$(DEL) $(RDIR)/rbfstat.r
	-$(DEL) $(RDIR)/drvstat.r
	-$(DEL) $(RDIR)/ucmstat.r
	-$(DEL) $(RDIR)/cdfmstat.r
	-$(DEL) $(RDIR)/cdrvstat.r

purge _purge: .
	-$(CODO) $(LDIR)/sys.l
	-$(DEL)  $(LDIR)/sys.l
	-$(CODO) $(LDIR)/usr.l
	-$(DEL)  $(LDIR)/usr.l
	-$(CODO) $(LDIR)/scfstat.l
	-$(DEL)  $(LDIR)/scfstat.l
	-$(CODO) $(LDIR)/drvs1.l
	-$(DEL)  $(LDIR)/drvs1.l
	-$(CODO) $(LDIR)/drvs2.l
	-$(DEL)  $(LDIR)/drvs2.l
	-$(CODO) $(LDIR)/drvs3.l
	-$(DEL)  $(LDIR)/drvs3.l
	-$(CODO) $(LDIR)/drvs4.l
	-$(DEL)  $(LDIR)/drvs4.l
	-$(CODO) $(LDIR)/drvs8.l
	-$(DEL)  $(LDIR)/drvs8.l
	-$(CODO) $(LDIR)/drvs12.l
	-$(DEL)  $(LDIR)/drvs12.l
	-$(CODO) $(LDIR)/drvs20.l
	-$(DEL)  $(LDIR)/drvs20.l
	-$(CODO) $(LDIR)/ucstat.l
	-$(DEL)  $(LDIR)/ucstat.l
	-$(CODO) $(LDIR)/cdidrvs1.l
	-$(DEL)  $(LDIR)/cdidrvs1.l
	-$(CODO) $(LDIR)/cdidrvs2.l
	-$(DEL)  $(LDIR)/cdidrvs2.l

# end of file

