# makefile to produce "syscache" modules.
# the "syscache" module provides the mechanism for the
# OS-9 kernel to enable/flush any cache hardware for
# the system.

# This makefile (and the syscache.a source) is used to produce
# standard "on-chip cache" control for 68020/68030/68040/68349 mpus.
#  For consistancy, a 68000 version is also made, but this is
# essentially a "no-op" routine.  Custom versions made be made
# to control any EXTERNAL (i.e. not on-chip) hardware caches
# by providing a "SYSCACHE" macro and re-making the module.

# Caching is enabled for the system by including the appropriate
# syscache module in your bootfile, and including the module name
# "syscache" in the Extens list of the init module.

MWOS			= ../../../..			# base of MWOS structure

SDIR			= .
RDIR			= RELS

ROOT			= ../../..				# path to base of tree

CPUROOT_000		= 68000					# sub dir for output (000)
CPUROOT_020		= 68020					# sub dir for output (020/030)
CPUROOT_040		= 68040					# sub dir for output (040
CPUROOT_060		= 68060					# sub dir for output (060)
CPUROOT_CPU32	= CPU32					# sub dir for output (68349)
BASEROOT		= 68000					# sub dir for LIB, etc
DEFSROOT		= SRC/DEFS				# sub dir to std defs

ODIR_000		= $(ROOT)/$(CPUROOT_000)/CMDS/BOOTOBJS
ODIR_020		= $(ROOT)/$(CPUROOT_020)/CMDS/BOOTOBJS
ODIR_040		= $(ROOT)/$(CPUROOT_040)/CMDS/BOOTOBJS
ODIR_060		= $(ROOT)/$(CPUROOT_060)/CMDS/BOOTOBJS
ODIR_CPU32		= $(ROOT)/$(CPUROOT_CPU32)/CMDS/BOOTOBJS

SYSLIB			= $(ROOT)/$(BASEROOT)/LIB/sys.l

DEFS_CPU32		= $(ROOT)/$(CPUROOT_CPU32)/DEFS/CPU32	# 68349 cache defs dir
DEFS_349		= $(DEFS_CPU32)/cic349.d


SRCFILE			= $(SDIR)/syscache.a

FILE			= cache
FILE020			= cache020
FILE030			= cache030
FILE040			= cache040
FILE060			= cache060
FILE349			= cache349
RFILE			= $(FILE).r
RFILE020		= $(FILE020).r
RFILE030		= $(FILE030).r
RFILE040		= $(FILE040).r
RFILE060		= $(FILE060).r
RFILE349		= $(FILE349).r

MODNAME			= syscache				# the actual module name

RFLAGS_COM		= -u=. -u=$(ROOT)/$(DEFSROOT)
RFLAGS			= -q $(RFLAGS_COM)
FLAGS020		= -aMPU020
FLAGS030		= -aMPU030
FLAGS040		= -aMPU040
FLAGS060		= -aMPU060
FLAGS349		= -aMPU349 -u=$(DEFS_CPU32)
LISTFLAGS		= -lxfd96 $(RFLAGS_COM)

RC				= r68					# assembler to use, default mode
RC020			= $(RC) -m2				# 68020 mode
RC030			= $(RC) -m3				# 68030 mode
RC040			= $(RC) -m4				# 68040 mode
RC060			= $(RC) -m6				# 68060 mode
RC349			= $(RC) -m2				# 68020 mode for CPU32

LC				= l68
LFLAGS			= -n=$(MODNAME) -l=$(SYSLIB) -g -gu=0.0

TOUCH			= touch					# touch utility
SPL				= spl					# spooler
SPLOPT_000		= "-jn=$(FILE) 000"
SPLOPT_020		= "-jn=$(FILE) 020"
SPLOPT_030		= "-jn=$(FILE) 030"
SPLOPT_040		= "-jn=$(FILE) 040"
SPLOPT_060		= "-jn=$(FILE) 060"
SPLOPT_349		= "-jn=$(FILE) 349"

include		$(MWOS)/MAKETMPL/makeosk.tpl
-x
-bo

syscache.date _build : makdir $(ODIR_000)/$(FILE) $(ODIR_020)/$(FILE020) \
				$(ODIR_020)/$(FILE030) $(ODIR_040)/$(FILE040) \
				$(ODIR_060)/$(FILE060) $(ODIR_CPU32)/$(FILE349)
				$(TOUCH) syscache.date

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make 68000/68010 cache module
#
$(ODIR_000)/$(FILE): $(SYSLIB) $(RFILE)
	-$(CODO) $(ODIR_000)/$*
	-$(CODO) $(ODIR_000)/STB/$(FILE).stb
    $(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR_000)/$*

$(RFILE): $(SRCFILE)
    $(RC) $(SRCFILE) $(RFLAGS) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make 68020 cache module
#
$(ODIR_020)/$(FILE020): $(SYSLIB) $(RFILE020)
	-$(CODO) $(ODIR_020)/$*
	-$(CODO) $(ODIR_020)/STB/$(FILE020).stb
    $(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR_020)/$*

$(RFILE020) : $(SRCFILE)
    $(RC020) $(SRCFILE) $(RFLAGS) $(FLAGS020) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make 68030 cache module
#
$(ODIR_020)/$(FILE030): $(SYSLIB) $(RFILE030)
	-$(CODO) $(ODIR_020)/$*
	-$(CODO) $(ODIR_020)/STB/$(FILE030).stb
    $(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR_020)/$*

$(RFILE030) : $(SRCFILE)
    $(RC030) $(SRCFILE) $(RFLAGS) $(FLAGS030) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make 68040 cache module
#
$(ODIR_040)/$(FILE040): $(SYSLIB) $(RFILE040)
	-$(CODO) $(ODIR_040)/$*
	-$(CODO) $(ODIR_040)/STB/$(FILE040).stb
    $(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR_040)/$*

$(RFILE040) : $(SRCFILE)
    $(RC040) $(SRCFILE) $(RFLAGS) $(FLAGS040) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make 68060 cache module
#
$(ODIR_060)/$(FILE060): $(SYSLIB) $(RFILE060)
	-$(CODO) $(ODIR_060)/$*
	-$(CODO) $(ODIR_060)/STB/$(FILE060).stb
    $(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR_060)/$*

$(RFILE060) : $(SRCFILE)
    $(RC060) $(SRCFILE) $(RFLAGS) $(FLAGS060) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make 68349 cache module
#
$(ODIR_CPU32)/$(FILE349): $(SYSLIB) $(RFILE349)
	-$(CODO) $(ODIR_CPU32)/$*
	-$(CODO) $(ODIR_CPU32)/STB/$(FILE349).stb
    $(LC) $(LFLAGS) $(RDIR)/$*.r -O=$(ODIR_CPU32)/$*

$(RFILE349) : $(SRCFILE) $(DEFS_349)
    $(RC349) $(SRCFILE) $(RFLAGS) $(FLAGS349) -o=$(RDIR)/$*.r

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# make listings
#
list.date:  $(SRCFILE)
    $(RC) $(SRCFILE) $(LISTFLAGS) ! $(SPL) $(SPLOPT_000)
    $(RC020) $(SRCFILE) $(FLAGS020) $(LISTFLAGS) ! $(SPL) $(SPLOPT_020)
    $(RC030) $(SRCFILE) $(FLAGS030) $(LISTFLAGS) ! $(SPL) $(SPLOPT_030)
    $(RC040) $(SRCFILE) $(FLAGS040) $(LISTFLAGS) ! $(SPL) $(SPLOPT_040)
    $(RC060) $(SRCFILE) $(FLAGS060) $(LISTFLAGS) ! $(SPL) $(SPLOPT_060)
    $(RC349) $(SRCFILE) $(FLAGS349) $(LISTFLAGS) ! $(SPL) $(SPLOPT_349)
    $(TOUCH) list.date

makdir _makdir:
    -$(MAKDIR) $(RDIR)

clean _clean:
	-$(DEL) $(RDIR)/*

purge _purge:
	-$(CODO) $(ODIR_000)/$(FILE)
	-$(CODO) $(ODIR_000)/STB/$(FILE).stb
	-$(DEL) $(ODIR_000)/$(FILE)
	-$(DEL) $(ODIR_000)/STB/$(FILE).stb
	-$(DEL) $(ODIR_000)/STB/$(FILE).dbg

	-$(CODO) $(ODIR_020)/$(FILE020)
	-$(CODO) $(ODIR_020)/$(FILE030)
	-$(CODO) $(ODIR_020)/STB/$(FILE020).stb
	-$(CODO) $(ODIR_020)/STB/$(FILE030).stb
	-$(DEL) $(ODIR_020)/$(FILE020)
	-$(DEL) $(ODIR_020)/$(FILE030)
	-$(DEL) $(ODIR_020)/STB/$(FILE020).stb
	-$(DEL) $(ODIR_020)/STB/$(FILE030).stb
	-$(DEL) $(ODIR_020)/STB/$(FILE020).dbg
	-$(DEL) $(ODIR_020)/STB/$(FILE030).dbg

	-$(CODO) $(ODIR_040)/$(FILE040)
	-$(CODO) $(ODIR_040)/STB/$(FILE040).stb
	-$(DEL) $(ODIR_040)/$(FILE040)
	-$(DEL) $(ODIR_040)/STB/$(FILE040).stb
	-$(DEL) $(ODIR_040)/STB/$(FILE040).dbg

	-$(CODO) $(ODIR_060)/$(FILE060)
	-$(CODO) $(ODIR_060)/STB/$(FILE060).stb
	-$(DEL) $(ODIR_060)/$(FILE060)
	-$(DEL) $(ODIR_060)/STB/$(FILE060).stb
	-$(DEL) $(ODIR_060)/STB/$(FILE060).dbg

	-$(CODO) $(ODIR_CPU32)/$(FILE349)
	-$(CODO) $(ODIR_CPU32)/STB/$(FILE349).stb
	-$(DEL) $(ODIR_CPU32)/$(FILE349)
	-$(DEL) $(ODIR_CPU32)/STB/$(FILE349).stb
	-$(DEL) $(ODIR_CPU32)/STB/$(FILE349).dbg

#checkin _checkin:
#	-$(CIDO) $(ODIR_000)/$(FILE)
#	-$(CIDO) $(ODIR_000)/STB/$(FILE).stb
#	-$(CIDO) $(ODIR_020)/$(FILE020)
#	-$(CIDO) $(ODIR_020)/$(FILE030)
#	-$(CIDO) $(ODIR_020)/STB/$(FILE020).stb
#	-$(CIDO) $(ODIR_020)/STB/$(FILE030).stb
#	-$(CIDO) $(ODIR_040)/$(FILE040)
#	-$(CIDO) $(ODIR_040)/STB/$(FILE040).stb
#	-$(CIDO) $(ODIR_060)/$(FILE060)
#	-$(CIDO) $(ODIR_060)/STB/$(FILE060).stb
#	-$(CIDO) $(ODIR_CPU32)/$(FILE349)
#	-$(CIDO) $(ODIR_CPU32)/STB/$(FILE349).stb


# end of file

