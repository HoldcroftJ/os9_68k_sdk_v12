#############################################################################
#                                                                           #
#            Copyright 1996-2001 by Microware Systems Corporation           #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This software is confidential property of Microware Systems Corporation, #
#  and is provided under license for internal development purposes only.    #
#  Reproduction, publication, distribution, or creation of derivative works #
#  in any form to any party other than the licensee is strictly prohibited, #
#  unless expressly authorized in writing by Microware Systems Corporation. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
# 01 96/08/03 Added edition history.                                  rak   #
#             ---- OS-9000/x86 V2.1 Released ----                           #
# 02 96/08/27 Added support for ARM, SH, and MIPS.                    gdb   #
# 03 96/08/28 Changed pmips to mips.                                  gdb   #
# 04 96/10/15 Changed parm to arm and sh to sh. Added arm3 and arm4.  gdb   #
#             ---- OS-9000/PPC V2.1.1 Released ----                         #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
# 05 96/12/11 Changed mips to mip3, added mip4                        sep   #
# 06 97/04/01 Made three MIPS targets: mips, mip3 and mip4.           cdg   #
#             ---- OS-9000/ARMv3 V2.2 Released ----                         #
# 07 97/04/28 Add SPARC support.                                      gdb   #
#             ---- OS-9000/ARMv3 V2.2.1 Released ----                       #
#             ---- OS-9000/SH3 V2.2.6 Released ----                         #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
#             ---- OS-9000/SPARC V2.2.7 Released ----                       #
#             ---- Makefile Support Sub-component v1.1.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 08 98/10/29 Reworked most macros and dependencies to properly       mgh   #
#             recurse without building for _alltrgts anywhere except        #
#             at the layer where make is first called.  Minor changes       #
#             are also required at each subdirectory level.                 #
# 09 98/11/23 More tweeking on build/clean issues. Add conditional    mgh   #
#               set of _ALLTRGTS (see comments below)                       #
#             ---- Makefile Support Sub-component v1.3   Released ----      #
# 10 98/12/01 Move FMAKENAME macro to make.tpl, change macros using   mgh   #
#               MAKEIT since it once again adds the -f makename as needed   #
# 11 98/12/02 Remove build, build.all as possible targets ..          mgh   #
#                conflict with utility build                                #
# 12 99/03/02 Added support for SH4 processor.                        dwj   #
# 13 99/08/25 Corrected quoting in help text.                         ajk   #
# 14 99/10/06 Added sprc8 and sprclite support.                       rmf   #
# 15 99/12/14 Added 8240 (PowerPC) support.                           djc   #
# 16 00/01/26 Added 8260 (PowerPC) support.                           djc   #
# 17 00/08/08 Added mips32 support.									  dwj	#
# 18 01/02/12 add arm5 rules.                                         gdw   #
# 19 01/06/11 Added support for SH5M processor.                       mgh   #
# 20 01/11/28 Added PowerPC 405 and 555 support.                      mdu   #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
# MWOS make rule file for for all target.
#############################################################################
# Target specific rules are defined in trgtrule.tpl and trgtxxx.tpl
# This file defines startup rulls and general recursion rules that match for
#   each processor, though specified as a seperate rule per processor.
#############################################################################

#
# This conditional code allows make xxxx (where xxxx is a processor) to work
#   the same as all.xxxx.  When clean.xxxx, purge.xxxx, etc are called, SUBTRGT
#   must be an empty string to work properly.  but calling any make with an
#   and empty target use to force a recursion with _alltrgts.  This code looks
#   to see if we've already recursed at least once and keep the recursion from
#   having an empty target calling the default behavior of "all" below.
#
if $(TARGET) != "base"
_ALLTRGTS = _build TARGET=$(TARGET)
else
_ALLTRGTS = _alltrgts
endif

#
# If no commanline target is specified, OS9Make executes the first dependency
#   it finds.  "all" MUST be the first dependency encountered for the templates
#   to function properly as described.
#   
# Common Targets to make all OS/CPUs
# Note: $(SUBTRGT) set to _build in trgtbase.tpl to start default build 
#        behavior 
# All of these dependencies should cause a recursive build for each CPU
#  specified by the ALL_TRGTS Macro.   ALL_TRGTS is not passed down thru
#  the recursion so behavior should be similar to V1.2.x if ALL_TRGTS is
#  specified in a makefile.  The Main place this is useful is in a PORTS
#  directory for a specific processor.
#
all _all.all all.all:  _nulltrg     # default target: "all"
	$(MAKEIT) SUBTRGT=_build  $(_ALLTRGTS)
    
    
#   
# Make for each TARGET processor specified in ALL_TRGTS or alltrgts.tpl
# 
_alltrgts:  $(ALL_TRGTS)
	$(COMMENT)

#
# Dependency rule to cause targets defined in makfile by TRGTS =  to be made.
# 
_all _build: _trgts
   $(COMMENT)
	        
# 
# Command line calling methods for the  "each processor" Action
#		    
k68k:  _nulltrg
	$(MAKE.k68k) SUBTRGT=$(SUBTRGT)
kc32:  _nulltrg
	$(MAKE.kc32) SUBTRGT=$(SUBTRGT)
k020:  _nulltrg
	$(MAKE.k020) SUBTRGT=$(SUBTRGT)
k040:  _nulltrg
	$(MAKE.k040) SUBTRGT=$(SUBTRGT)
k060:  _nulltrg
	$(MAKE.k060) SUBTRGT=$(SUBTRGT)
o386:  _nulltrg
	$(MAKE.o386) SUBTRGT=$(SUBTRGT)
p386:  _nulltrg
	$(MAKE.p386) SUBTRGT=$(SUBTRGT)
p020:  _nulltrg
	$(MAKE.p020) SUBTRGT=$(SUBTRGT)
pppc:  _nulltrg
	$(MAKE.pppc) SUBTRGT=$(SUBTRGT)
p403:  _nulltrg
	$(MAKE.p403) SUBTRGT=$(SUBTRGT)
p405:  _nulltrg
	$(MAKE.p405) SUBTRGT=$(SUBTRGT)
p505:  _nulltrg
	$(MAKE.p505) SUBTRGT=$(SUBTRGT)
p555:  _nulltrg
	$(MAKE.p555) SUBTRGT=$(SUBTRGT)
p601:  _nulltrg
	$(MAKE.p601) SUBTRGT=$(SUBTRGT)
p602:  _nulltrg
	$(MAKE.p602) SUBTRGT=$(SUBTRGT)
p603:  _nulltrg
	$(MAKE.p603) SUBTRGT=$(SUBTRGT)
p604:  _nulltrg
	$(MAKE.p604) SUBTRGT=$(SUBTRGT)
p821:  _nulltrg
	$(MAKE.p821) SUBTRGT=$(SUBTRGT)
8240:  _nulltrg
	$(MAKE.8240) SUBTRGT=$(SUBTRGT)
8260:  _nulltrg
	$(MAKE.8260) SUBTRGT=$(SUBTRGT)
arm:   _nulltrg
	$(MAKE.arm)  SUBTRGT=$(SUBTRGT)
arm3:   _nulltrg
	$(MAKE.arm3) SUBTRGT=$(SUBTRGT)
arm4:   _nulltrg
	$(MAKE.arm4) SUBTRGT=$(SUBTRGT)
arm5:   _nulltrg
	$(MAKE.arm5) SUBTRGT=$(SUBTRGT)
sh:     _nulltrg
	$(MAKE.sh)   SUBTRGT=$(SUBTRGT)
sh4:   _nulltrg
	$(MAKE.sh4)  SUBTRGT=$(SUBTRGT)
sh5m:  _nulltrg
	$(MAKE.sh5m) SUBTRGT=$(SUBTRGT)
mips:  _nulltrg
	$(MAKE.mips) SUBTRGT=$(SUBTRGT)
mip3:  _nulltrg
	$(MAKE.mip3) SUBTRGT=$(SUBTRGT)
mip32:  _nulltrg
	$(MAKE.mip32) SUBTRGT=$(SUBTRGT)
mip4:  _nulltrg
	$(MAKE.mip4) SUBTRGT=$(SUBTRGT)
sprc:  _nulltrg
	$(MAKE.sprc) SUBTRGT=$(SUBTRGT)
sprc8:  _nulltrg
	$(MAKE.sprc8) SUBTRGT=$(SUBTRGT)
sprclite:  _nulltrg
	$(MAKE.sprclite) SUBTRGT=$(SUBTRGT)


# 
# Command line calling methods for the  "make/build" Action
#		    
all.k68k _all.k68k:  _nulltrg
	$(MAKE.k68k) SUBTRGT=_build _build
all.kc32 _all.kc32:  _nulltrg
	$(MAKE.kc32) SUBTRGT=_build _build
all.k020 _all.k020:  _nulltrg
	$(MAKE.k020) SUBTRGT=_build _build
all.k040 _all.k040:  _nulltrg
	$(MAKE.k040) SUBTRGT=_build _build
all.k060 _all.k060:  _nulltrg
	$(MAKE.k060) SUBTRGT=_build _build
all.o386 _all.o386:  _nulltrg
	$(MAKE.o386) SUBTRGT=_build _build
all.p386 _all.p386:  _nulltrg
	$(MAKE.p386) SUBTRGT=_build _build
all.p020 _all.p020:  _nulltrg
	$(MAKE.p020) SUBTRGT=_build _build
all.pppc _all.pppc:  _nulltrg
	$(MAKE.pppc) SUBTRGT=_build _build
all.p403 _all.p403:  _nulltrg
	$(MAKE.p403) SUBTRGT=_build _build
all.p405 _all.p405:  _nulltrg
	$(MAKE.p405) SUBTRGT=_build _build
all.p505 _all.p505:  _nulltrg
	$(MAKE.p505) SUBTRGT=_build _build
all.p555 _all.p555:  _nulltrg
	$(MAKE.p555) SUBTRGT=_build _build
all.p601 _all.p601:  _nulltrg
	$(MAKE.p601) SUBTRGT=_build _build
all.p602 _all.p602:  _nulltrg
	$(MAKE.p602) SUBTRGT=_build _build
all.p603 _all.p603:  _nulltrg
	$(MAKE.p603) SUBTRGT=_build _build
all.p604 _all.p604:  _nulltrg
	$(MAKE.p604) SUBTRGT=_build _build
all.p821 _all.p821:  _nulltrg
	$(MAKE.p821) SUBTRGT=_build _build
all.8240 _all.8240:  _nulltrg
	$(MAKE.8240) SUBTRGT=_build _build
all.8260 _all.8260:  _nulltrg
	$(MAKE.8260) SUBTRGT=_build _build
all.arm  _all.arm:   _nulltrg
	$(MAKE.arm)  SUBTRGT=_build _build
all.arm3 _all.arm3:   _nulltrg
	$(MAKE.arm3) SUBTRGT=_build _build
all.arm4 _all.arm4:   _nulltrg
	$(MAKE.arm4) SUBTRGT=_build _build
all.arm5 _all.arm5:   _nulltrg
	$(MAKE.arm5) SUBTRGT=_build _build
all.sh  _all.sh:      _nulltrg
	$(MAKE.sh)   SUBTRGT=_build _build
all.sh4  _all.sh4:    _nulltrg
	$(MAKE.sh4)  SUBTRGT=_build _build
all.sh5m _all.sh5m:   _nulltrg
	$(MAKE.sh5m) SUBTRGT=_build _build
all.mips _all.mips:  _nulltrg
	$(MAKE.mips) SUBTRGT=_build _build
all.mip3 _all.mip3:  _nulltrg
	$(MAKE.mip3) SUBTRGT=_build _build
all.mip32 _all.mip32:  _nulltrg
	$(MAKE.mip32) SUBTRGT=_build _build
all.mip4 _all.mip4:  _nulltrg
	$(MAKE.mip4) SUBTRGT=_build _build
all.sprc _all.sprc:  _nulltrg
	$(MAKE.sprc) SUBTRGT=_build _build
all.sprc8 _all.sprc8:  _nulltrg
	$(MAKE.sprc8) SUBTRGT=_build _build
all.sprclite _all.sprclite:  _nulltrg
	$(MAKE.sprclite) SUBTRGT=_build _build


#
# Command line calling methods for the "clean" Action
#
clean clean.all_clean.all:  _nulltrg
	$(MAKEIT) SUBTRGT=_clean _alltrgts

clean.k68k _clean.k68k:  _nulltrg
	$(MAKE.k68k) SUBTRGT=_clean _clean
clean.kc32 _clean.kc32:  _nulltrg
	$(MAKE.kc32) SUBTRGT=_clean _clean
clean.k020 _clean.k020:  _nulltrg
	$(MAKE.k020) SUBTRGT=_clean _clean
clean.k040 _clean.k040:  _nulltrg
	$(MAKE.k040) SUBTRGT=_clean _clean
clean.k060 _clean.k060:  _nulltrg
	$(MAKE.k060) SUBTRGT=_clean _clean
clean.o386 _clean.o386:  _nulltrg
	$(MAKE.o386) SUBTRGT=_clean _clean
clean.p386 _clean.p386:  _nulltrg
	$(MAKE.p386) SUBTRGT=_clean _clean
clean.p020 _clean.p020:  _nulltrg
	$(MAKE.p020) SUBTRGT=_clean _clean
clean.pppc _clean.pppc:  _nulltrg
	$(MAKE.pppc) SUBTRGT=_clean _clean
clean.p403 _clean.p403:  _nulltrg
	$(MAKE.p403) SUBTRGT=_clean _clean
clean.p405 _clean.p405:  _nulltrg
	$(MAKE.p403) SUBTRGT=_clean _clean
clean.p505 _clean.p505:  _nulltrg
	$(MAKE.p505) SUBTRGT=_clean _clean
clean.p555 _clean.p555:  _nulltrg
	$(MAKE.p555) SUBTRGT=_clean _clean
clean.p601 _clean.p601:  _nulltrg
	$(MAKE.p601) SUBTRGT=_clean _clean
clean.p602 _clean.p602:  _nulltrg
	$(MAKE.p602) SUBTRGT=_clean _clean
clean.p603 _clean.p603:  _nulltrg
	$(MAKE.p603) SUBTRGT=_clean _clean
clean.p604 _clean.p604:  _nulltrg
	$(MAKE.p604) SUBTRGT=_clean _clean
clean.p821 _clean.p821:  _nulltrg
	$(MAKE.p821) SUBTRGT=_clean _clean
clean.8240 _clean.8240:  _nulltrg
	$(MAKE.8240) SUBTRGT=_clean _clean
clean.8260 _clean.8260:  _nulltrg
	$(MAKE.8260) SUBTRGT=_clean _clean
clean.arm  _clean.arm:   _nulltrg
	$(MAKE.arm)  SUBTRGT=_clean _clean
clean.arm3 _clean.arm3:  _nulltrg
	$(MAKE.arm3) SUBTRGT=_clean _clean
clean.arm4 _clean.arm4:  _nulltrg
	$(MAKE.arm4) SUBTRGT=_clean _clean
clean.arm5 _clean.arm5:  _nulltrg
	$(MAKE.arm5) SUBTRGT=_clean _clean
clean.sh   _clean.sh:    _nulltrg
	$(MAKE.sh)   SUBTRGT=_clean _clean
clean.sh4  _clean.sh4:   _nulltrg
	$(MAKE.sh4)  SUBTRGT=_clean _clean
clean.sh5m _clean.sh5m:  _nulltrg
	$(MAKE.sh5m) SUBTRGT=_clean _clean
clean.mips _clean.mips:  _nulltrg
	$(MAKE.mips) SUBTRGT=_clean _clean
clean.mip3 _clean.mip3:  _nulltrg
	$(MAKE.mip3) SUBTRGT=_clean _clean
clean.mip32 _clean.mip32:  _nulltrg
	$(MAKE.mip32) SUBTRGT=_clean _clean
clean.mip4 _clean.mip4:  _nulltrg
	$(MAKE.mip4) SUBTRGT=_clean _clean
clean.sprc _clean.sprc:  _nulltrg
	$(MAKE.sprc) SUBTRGT=_clean _clean
clean.sprc8 _clean.sprc8:  _nulltrg
	$(MAKE.sprc8) SUBTRGT=_clean _clean
clean.sprclite _clean.sprclite:  _nulltrg
	$(MAKE.sprclite) SUBTRGT=_clean _clean


#
# Command line calling methods for the "purge" Action
#
purge purge.all _purge.all:  _nulltrg
	$(MAKEIT) SUBTRGT=_purge _alltrgts

purge.k68k _purge.k68k:  _nulltrg
	$(MAKE.k68k) SUBTRGT=_purge _purge
purge.kc32 _purge.kc32:  _nulltrg
	$(MAKE.kc32) SUBTRGT=_purge _purge
purge.k020 _purge.k020:  _nulltrg
	$(MAKE.k020) SUBTRGT=_purge _purge
purge.k040 _purge.k040:  _nulltrg
	$(MAKE.k040) SUBTRGT=_purge _purge
purge.k060 _purge.k060:  _nulltrg
	$(MAKE.k060) SUBTRGT=_purge _purge
purge.o386 _purge.o386:  _nulltrg
	$(MAKE.o386) SUBTRGT=_purge _purge
purge.p386 _purge.p386:  _nulltrg
	$(MAKE.p386) SUBTRGT=_purge _purge
purge.p020 _purge.p020:  _nulltrg
	$(MAKE.p020) SUBTRGT=_purge _purge
purge.pppc _purge.pppc:  _nulltrg
	$(MAKE.pppc) SUBTRGT=_purge _purge
purge.p403 _purge.p403:  _nulltrg
	$(MAKE.p403) SUBTRGT=_purge _purge
purge.p405 _purge.p405:  _nulltrg
	$(MAKE.p405) SUBTRGT=_purge _purge
purge.p505 _purge.p505:  _nulltrg
	$(MAKE.p505) SUBTRGT=_purge _purge
purge.p555 _purge.p555:  _nulltrg
	$(MAKE.p555) SUBTRGT=_purge _purge
purge.p601 _purge.p601:  _nulltrg
	$(MAKE.p601) SUBTRGT=_purge _purge
purge.p602 _purge.p602:  _nulltrg
	$(MAKE.p602) SUBTRGT=_purge _purge
purge.p603 _purge.p603:  _nulltrg
	$(MAKE.p603) SUBTRGT=_purge _purge
purge.p604 _purge.p604:  _nulltrg
	$(MAKE.p604) SUBTRGT=_purge _purge
purge.p821 _purge.p821:  _nulltrg
	$(MAKE.p821) SUBTRGT=_purge _purge
purge.8240 _purge.8240:  _nulltrg
	$(MAKE.8240) SUBTRGT=_purge _purge
purge.8260 _purge.8260:  _nulltrg
	$(MAKE.8260) SUBTRGT=_purge _purge
purge.arm  _purge.arm:   _nulltrg
	$(MAKE.arm)  SUBTRGT=_purge _purge
purge.arm3 _purge.arm3:  _nulltrg
	$(MAKE.arm3) SUBTRGT=_purge _purge
purge.arm4 _purge.arm4:  _nulltrg
	$(MAKE.arm4) SUBTRGT=_purge _purge
purge.arm5 _purge.arm5:  _nulltrg
	$(MAKE.arm5) SUBTRGT=_purge _purge
purge.sh   _purge.sh:    _nulltrg
	$(MAKE.sh)   SUBTRGT=_purge _purge
purge.sh4  _purge.sh4:   _nulltrg
	$(MAKE.sh4)  SUBTRGT=_purge _purge
purge.sh5m _purge.sh5m:  _nulltrg
	$(MAKE.sh5m)  SUBTRGT=_purge _purge
purge.mips _purge.mips:  _nulltrg
	$(MAKE.mips) SUBTRGT=_purge _purge
purge.mip3 _purge.mip3:  _nulltrg
	$(MAKE.mip3) SUBTRGT=_purge _purge
purge.mip32 _purge.mip32:  _nulltrg
	$(MAKE.mip32) SUBTRGT=_purge _purge
purge.mip4 _purge.mip4:  _nulltrg
	$(MAKE.mip4) SUBTRGT=_purge _purge
purge.sprc _purge.sprc:  _nulltrg
	$(MAKE.sprc) SUBTRGT=_purge _purge
purge.sprc8 _purge.sprc8:  _nulltrg
	$(MAKE.sprc8) SUBTRGT=_purge _purge
purge.sprclite _purge.sprclite:  _nulltrg
	$(MAKE.sprclite) SUBTRGT=_purge _purge


# 
# Command line calling methods for the "lint" Action
#
lint lint.all _lint.all:  _nulltrg
	$(ECHO) "*** Please Specify a Target OS/Processor for LINT ***"
	$(ECHO) "***     ie, [lint.k68k]                           ***"
	$(COMMENT)

lint.k68k _lint.k68k:  _nulltrg
	$(MAKE.k68k) SUBTRGT=_lint _lint
lint.kc32 _lint.kc32:  _nulltrg
	$(MAKE.kc32) SUBTRGT=_lint _lint
lint.k020 _lint.k020:  _nulltrg
	$(MAKE.k020) SUBTRGT=_lint _lint
lint.k040 _lint.k040:  _nulltrg
	$(MAKE.k040) SUBTRGT=_lint _lint
lint.k060 _lint.k060:  _nulltrg
	$(MAKE.k060) SUBTRGT=_lint _lint
lint.o386 _lint.o386:  _nulltrg
	$(MAKE.o386) SUBTRGT=_lint _lint
lint.p386 _lint.p386:  _nulltrg
	$(MAKE.p386) SUBTRGT=_lint _lint
lint.p020 _lint.p020:  _nulltrg
	$(MAKE.p020) SUBTRGT=_lint _lint
lint.pppc _lint.pppc:  _nulltrg
	$(MAKE.pppc) SUBTRGT=_lint _lint
lint.p403 _lint.p403:  _nulltrg
	$(MAKE.p403) SUBTRGT=_lint _lint
lint.p405 _lint.p405:  _nulltrg
	$(MAKE.p405) SUBTRGT=_lint _lint
lint.p505 _lint.p505:  _nulltrg
	$(MAKE.p505) SUBTRGT=_lint _lint
lint.p555 _lint.p555:  _nulltrg
	$(MAKE.p555) SUBTRGT=_lint _lint
lint.p601 _lint.p601:  _nulltrg
	$(MAKE.p601) SUBTRGT=_lint _lint
lint.p602 _lint.p602:  _nulltrg
	$(MAKE.p602) SUBTRGT=_lint _lint 
lint.p603 _lint.p603:  _nulltrg
	$(MAKE.p603) SUBTRGT=_lint _lint
lint.p604 _lint.p604:  _nulltrg
	$(MAKE.p604) SUBTRGT=_lint _lint
lint.p821 _lint.p821:  _nulltrg
	$(MAKE.p821) SUBTRGT=_lint _lint
lint.8240 _lint.8240:  _nulltrg
	$(MAKE.8240) SUBTRGT=_lint _lint
lint.8260 _lint.8260:  _nulltrg
	$(MAKE.8260) SUBTRGT=_lint _lint
lint.arm  _lint.arm:   _nulltrg
	$(MAKE.arm)  SUBTRGT=_lint _lint
lint.arm3 _lint.arm3:  _nulltrg
	$(MAKE.arm3) SUBTRGT=_lint _lint
lint.arm4 _lint.arm4:  _nulltrg
	$(MAKE.arm4) SUBTRGT=_lint _lint
lint.arm5 _lint.arm5:  _nulltrg
	$(MAKE.arm5) SUBTRGT=_lint _lint
lint.sh   _lint.sh:    _nulltrg
	$(MAKE.sh)   SUBTRGT=_lint _lint
lint.sh4  _lint.sh4:   _nulltrg
	$(MAKE.sh4)  SUBTRGT=_lint _lint
lint.sh5m _lint.sh5m:  _nulltrg
	$(MAKE.sh5m) SUBTRGT=_lint _lint
lint.mips _lint.mips: _nulltrg
	$(MAKE.mips) SUBTRGT=_lint _lint
lint.mip3 _lint.mip3: _nulltrg
	$(MAKE.mip3) SUBTRGT=_lint _lint
lint.mip32 _lint.mip32: _nulltrg
	$(MAKE.mip32) SUBTRGT=_lint _lint
lint.mip4 _lint.mip4: _nulltrg
	$(MAKE.mip4) SUBTRGT=_lint _lint
lint.sprc _lint.sprc: _nulltrg
	$(MAKE.sprc) SUBTRGT=_lint _lint
lint.sprc8 _lint.sprc8: _nulltrg
	$(MAKE.sprc8) SUBTRGT=_lint _lint
lint.sprclite _lint.sprclite: _nulltrg
	$(MAKE.sprclite) SUBTRGT=_lint _lint


#
# Command line calling methods for the "depend" Action
#
depend depend.all _depend.all:  _nulltrg
	$(MAKEIT) SUBTRGT=_depend _alltrgts

depend.k68k _depend.k68k:  _nulltrg
	$(MAKE.k68k) SUBTRGT=_depend _depend
depend.kc32 _depend.kc32:  _nulltrg
	$(MAKE.kc32) SUBTRGT=_depend _depend
depend.k020 _depend.k020:  _nulltrg
	$(MAKE.k020) SUBTRGT=_depend _depend
depend.k040 _depend.k040:  _nulltrg
	$(MAKE.k040) SUBTRGT=_depend _depend
depend.k060 _depend.k060:  _nulltrg
	$(MAKE.k060) SUBTRGT=_depend _depend
depend.o386 _depend.o386:  _nulltrg
	$(MAKE.o386) SUBTRGT=_depend _depend
depend.p386 _depend.p386:  _nulltrg
	$(MAKE.p386) SUBTRGT=_depend _depend
depend.p020 _depend.p020:  _nulltrg
	$(MAKE.p020) SUBTRGT=_depend _depend
depend.pppc _depend.pppc:  _nulltrg
	$(MAKE.pppc) SUBTRGT=_depend _depend
depend.p403 _depend.p403:  _nulltrg
	$(MAKE.p403) SUBTRGT=_depend _depend
depend.p405 _depend.p405:  _nulltrg
	$(MAKE.p403) SUBTRGT=_depend _depend
depend.p505 _depend.p505:  _nulltrg
	$(MAKE.p505) SUBTRGT=_depend _depend
depend.p555 _depend.p555:  _nulltrg
	$(MAKE.p555) SUBTRGT=_depend _depend
depend.p601 _depend.p601:  _nulltrg
	$(MAKE.p601) SUBTRGT=_depend _depend
depend.p602 _depend.p602:  _nulltrg
	$(MAKE.p602) SUBTRGT=_depend _depend
depend.p603 _depend.p603:  _nulltrg
	$(MAKE.p603) SUBTRGT=_depend _depend
depend.p604 _depend.p604:  _nulltrg
	$(MAKE.p604) SUBTRGT=_depend _depend
depend.p821 _depend.p821:  _nulltrg
	$(MAKE.p821) SUBTRGT=_depend _depend
depend.8240 _depend.8240:  _nulltrg
	$(MAKE.8240) SUBTRGT=_depend _depend
depend.8260 _depend.8260:  _nulltrg
	$(MAKE.8260) SUBTRGT=_depend _depend
depend.arm  _depend.arm:   _nulltrg
	$(MAKE.arm)  SUBTRGT=_depend _depend
depend.arm3 _depend.arm3:  _nulltrg
	$(MAKE.arm3) SUBTRGT=_depend _depend
depend.arm4 _depend.arm4:  _nulltrg
	$(MAKE.arm4) SUBTRGT=_depend _depend
depend.arm5 _depend.arm5:  _nulltrg
	$(MAKE.arm5) SUBTRGT=_depend _depend
depend.sh   _depend.sh:    _nulltrg
	$(MAKE.sh)   SUBTRGT=_depend _depend
depend.sh4  _depend.sh4:   _nulltrg
	$(MAKE.sh4)  SUBTRGT=_depend _depend
depend.sh5m _depend.sh5m:  _nulltrg
	$(MAKE.sh5m) SUBTRGT=_depend _depend
depend.mips _depend.mips:  _nulltrg
	$(MAKE.mips) SUBTRGT=_depend _depend
depend.mip3 _depend.mip3:  _nulltrg
	$(MAKE.mip3) SUBTRGT=_depend _depend
depend.mip32 _depend.mip32:  _nulltrg
	$(MAKE.mip32) SUBTRGT=_depend _depend
depend.mip4 _depend.mip4:  _nulltrg
	$(MAKE.mip4) SUBTRGT=_depend _depend
depend.sprc _depend.sprc:  _nulltrg
	$(MAKE.sprc) SUBTRGT=_depend _depend
depend.sprc8 _depend.sprc8:  _nulltrg
	$(MAKE.sprc8) SUBTRGT=_depend _depend
depend.sprclite _depend.sprclite:  _nulltrg
	$(MAKE.sprclite) SUBTRGT=_depend _depend


help _help:  _nulltrg
	$(ECHO) '                                                             '
	$(ECHO) '============================================================='
	$(ECHO) '                                                             '
	$(ECHO) 'This is a host and target independent makefile. This means   '
	$(ECHO) 'that it will work regardless of the host system you are  and '
	$(ECHO) 'using, and that you are able to specify the target you are   '
	$(ECHO) 'making code for.  The current host is "$(HOST)".'
	$(ECHO) '                                                             '
	$(ECHO) '-------------------------------------------------------------'
	$(ECHO) '                                                             '
	$(ECHO) 'This makefile supports the following built-in products to    '
	$(ECHO) 'provide several housekeeping features:                       '
	$(ECHO) '                                                             '
	$(ECHO) '    help:       Prints this help message                     '
	$(ECHO) '    all:        Make all products                            '
	$(ECHO) '    clean:      Deletes all intermediate files (e.g. .r)     '
	$(ECHO) '    purge:      Deletes all object files                     '
	$(ECHO) '                                                             '
	$(ECHO) 'This makefile supports the following built-in target OS/CPU  '
	$(ECHO) 'combinations:                                                '
	$(ECHO) '                                                             '
	$(ECHO) '    k68k:       Performs a make for the OS-9/68000 target    '
	$(ECHO) '    kc32:       Performs a make for the OS-9/CPU32 target    '
	$(ECHO) '    k020:       Performs a make for the OS-9/68020 target    '
	$(ECHO) '    k040:       Performs a make for the OS-9/68040 target    '
	$(ECHO) '    k060:       Performs a make for the OS-9/68060 target    '
	$(ECHO) '    p386:       Performs a make for the OS-9000/80386 target '
	$(ECHO) '    p020:       Performs a make for the OS-9000/68020 target '
	$(ECHO) '    pppc:       Performs a make for the OS-9000/PowerPC target'
	$(ECHO) '    p403:       Performs a make for the OS-9000/PPC403 target'
	$(ECHO) '    p405:       Performs a make for the OS-9000/PPC405 target'
	$(ECHO) '    p505:       Performs a make for the OS-9000/PPC505 target'
	$(ECHO) '    p555:       Performs a make for the OS-9000/PPC555 target'
	$(ECHO) '    p601:       Performs a make for the OS-9000/PPC601 target'
	$(ECHO) '    p602:       Performs a make for the OS-9000/PPC602 target'
	$(ECHO) '    p603:       Performs a make for the OS-9000/PPC603 target'
	$(ECHO) '    p604:       Performs a make for the OS-9000/PPC604 target'
	$(ECHO) '    p821:       Performs a make for the OS-9000/PPC821 target'
	$(ECHO) '    8240:       Performs a make for the OS-9000/PPC8240 target'
	$(ECHO) '    8260:       Performs a make for the OS-9000/PPC8260 target'
	$(ECHO) '    arm:        Performs a make for the OS-9000/ARM target'
	$(ECHO) '    arm3:       Performs a make for the OS-9000/ARM3 target'
	$(ECHO) '    arm4:       Performs a make for the OS-9000/ARM4 target'
	$(ECHO) '    arm5:       Performs a make for the OS-9000/ARM5 target'
	$(ECHO) '    sh:         Performs a make for the OS-9000/SH target'
	$(ECHO) '    sh4:        Performs a make for the OS-9000/SH4 target'
	$(ECHO) '    sh5m:       Performs a make for the OS-9000/SH5M target'
	$(ECHO) '    mips:       Performs a make for the OS-9000/MIPS target'
	$(ECHO) '    mip3:       Performs a make for the OS-9000/MIPS3000 target'
	$(ECHO) '    mip32:      Performs a make for the OS-9000/MIPS32 target'
	$(ECHO) '    mip4:       Performs a make for the OS-9000/MIPS64 target'
	$(ECHO) '    sprc:       Performs a make for the OS-9000/SPARC target'
	$(ECHO) '    sprc8:      Performs a make for the OS-9000/SPARC8 target'
	$(ECHO) '    sprclite:   Performs a make for the OS-9000/SPARCLITE target'
	$(ECHO) '                                                             '
	$(ECHO) 'Targets can be made by specifying a product.target           '
	$(ECHO) 'combination.  For example, to delete all intermediate files  '
	$(ECHO) 'for OS-9/68020 targets, type:                                '
	$(ECHO) '    SHELL: make clean.k020                                   '
	$(ECHO) '                                                             '
	$(ECHO) 'To build all components for a processor type:                '
	$(ECHO) '   SHELL: make all.k020  or SHELL: make k020                 '
	$(ECHO) '                                                             '
	$(ECHO) 'If not specified, the target will be the processors named in '
	$(ECHO) 'the file: MWOS/MAKETMPL/alltrgts.tpl or an ALL_TRGTS=xxxx    '
	$(ECHO) 'entry included in your MWMAKEOPTS enviroment variable.       '
	$(ECHO) 'The alltrgts.tpl method may include multiple target OS/CPUs. '
	$(ECHO) 'Enviroment variables overide makefile and includefile Macros '
	$(ECHO) '                                                             '
	$(ECHO) 'Delete all intermediate files for all the "alltrgts" CPUs:   '
	$(ECHO) '   SHELL: make clean  or SHELL: make clean.all               '
	$(ECHO) '                                                             '
	$(ECHO) 'Build all components for all the "alltrgts" CPUs:            '
	$(ECHO) '  SHELL: make  or  SHELL: make all.all  or  SHELL: make all  '
	$(ECHO) '                                                             '
	$(ECHO) '-------------------------------------------------------------'
	$(ECHO) '                                                             '
	$(ECHO) 'This  makefile also supports the following macros defined on '
	$(ECHO) 'the command line.                                            '
	$(ECHO) '                                                             '
	$(ECHO) 'MOPTS = Makefile options. These options are passed to any    '
	$(ECHO) '        sub-makefile that is called.                         '
	$(ECHO) 'COPTS = Compiler options.                                    '
	$(ECHO) 'AOPTS = Assembler options.                                   '
	$(ECHO) 'LOPTS = Linker options.                                      '
	$(ECHO) '                                                             '
	$(ECHO) '============================================================='
	$(ECHO) '                                                             '



_nulltrg:  .
	$(COMMENT)


