#############################################################################
#                                                                           #
#              Copyright 1996 by Microware Systems Corporation              #
#                            All Rights Reserved                            #
#                          Reproduced Under License                         #
#                                                                           #
#  This source code is the proprietary confidential property of Microware   #
#  Systems Corporation, and is provided to licensee for documentation and   #
#  educational purposes only. Reproduction, publication, or distribution    #
#  in any form to any party other than the licensee is strictly prohibited. #
#                                                                           #
#############################################################################
#                                                                           #
# Edition History:                                                          #
# #   Date    Comments                                                By    #
# -- -------- ------------------------------------------------------- ---   #
# 01 96/08/03 Added edition history.                                  rak   #
#             ---- OS-9/68K V3.0.3 System-state Debug Support Release ---   #
# 02 98/01/24 MHQco03317 - Removed -as=-bt from APARMS macro.               #
#             MHQco03268 - Added UCC_WARN to all UCC XPARMS macros.         #
#             MHQco02981 - Changed -ill lib references to use -y.           #
#             MHQco03294 - Changed MWOS_CSLLIBS to include i-code           #
#                          versions of standard libraries.            rak   #
#             ---- Makefile Support Sub-component v1.1 Released ----        #
# 03 98/07/15 Added MWOS_LDIR to allow MWOS_IDIR and MWOW_RDIR to       mgh #
#             differ from $TARGET (Sparc was first time problem appeared)   #
#             ---- Makefile Support Sub-component v1.2 Released ----        #
#             ---- Makefile Support Sub-component v1.2.1 Released ----      #
#             ---- Makefile Support Sub-component v1.2.2 Released ----      #
# 04 98/10/29 added OSDIR, COPTs for module owner, attributes, and      mgh #
#             moved includes for baserule.tpl and trgtrule.tpl              #
#             ---- Makefile Support Sub-component v1.3   Released ----      #  
# 05 99/07/06 Added CPUDIR MACRO  (and missing OSDIR)                   mgh #
# 06 99/07/07 Removed superfluous -i option from MWOS_CSLLIBS (CF2653)  ajk #
# 07 00/01/24 Moved ISP macros to isp.tpl                               rds #
# 08 00/08/26 Select Unix.l from cpu32 Libdir (BASLIB)					mgh #
# 09 01/08.01 Change Mbuflib to cpu32 version							mgh #
#             $$                    <RELEASE_INFO>                     $$   #
#############################################################################
#	make template file for all hosts OS-9/CPU32 target 						#
#############################################################################

#############################################################################
#	MWOS must be pre-defined to the location of the MWOS directory by 		#
#	either a full or partial pathlist.										#
#############################################################################
#	HOSTTYPE must be pre-defined to the I-Code Library host type			#
#############################################################################

#############################################################################
#																			#

# Directory Definitions
MWOS_LDIR       =   kc32                # Lower case directory name (Linkers)
MWOS_IDIR		=	RELS/k68k
MWOS_RDIR		=	RELS/kc32
MWOS_OSDIR		=	$(MWOS)/OS9
MWOS_ODIR		=	$(MWOS_OSDIR)/CPU32/CMDS

MWOS_DDIR		=	$(MWOS)/SRC/DEFS
MWOS_CDEF		=	$(TOOLSET)/SRC/DEFS

MWOS_DFTDEF		=	$(MWOS_OSDIR)/CPU32/DEFS/CPU32
MWOS_OSDDIR		=	$(MWOS_OSDIR)/SRC/DEFS

MWOS_DFTLIB		=	$(MWOS_OSDIR)/68000/LIB		# Last chance to find LIB
MWOS_DFTCLIB	=	$(TOOLSET)/OS9/68000/LIB

MWOS_BASLIB		=	$(MWOS_OSDIR)/CPU32/LIB		# First Fallback to find LIB
MWOS_BASCLIB	=	$(TOOLSET)/OS9/CPU32/LIB

MWOS_LIBDIR		=	$(MWOS_OSDIR)/CPU32/LIB		# Processor specific libs
MWOS_CLIBDIR	=	$(TOOLSET)/OS9/CPU32/LIB

MWOS_ILIBDIR	=	$(MWOS_OSDIR)/68000/LIB/$(HOSTTYPE)
MWOS_ICLIBDIR	=	$(TOOLSET)/OS9/68000/LIB/$(HOSTTYPE)

EDITMOD_DDIR	=	$(MWOS)/SRC/DESC
EDITMOD_ODIR	=	$(MWOS_OSDIR)/SRC/DESC

# Host/Target Macros
OS				=	OSK
OSDIR			=	OS9
CPU				=	CPU32
CPUDIR			=	CPU32
CPUC			=	CPU32c

# Definition Directory Search
MWOS_DEFS		=	-v=$(MWOS_CDEF) -v=$(MWOS_DFTDEF) -v=$(MWOS_OSDDIR)
EDITMOD_DEFS	=	-v=$(EDITMOD_DDIR) -v=$(EDITMOD_ODIR)

# Library Files
CLIB			=	-l=$(MWOS_CLIBDIR)/clib.l
CLIBS			=	$(CLIB) -y=$(MWOS_ICLIBDIR)/clib.il
CSL				=	-l=$(MWOS_BASCLIB)/csl.l
CSLS			=	$(CSL) -y=$(MWOS_ICLIBDIR)/csl.il $(CSL)
OS_CSL			=	-l=$(MWOS_BASCLIB)/os_csl.l
OS_CSLS			=	$(OS_CSL) -y=$(MWOS_ICLIBDIR)/os_csl.il
OS_LIB			=	-l=$(MWOS_BASCLIB)/os_lib.l
OS_LIBS			=	$(OS_LIB) -y=$(MWOS_ICLIBDIR)/os_lib.il
SCLIB			=	-l=$(MWOS_BASCLIB)/sclib.l
SCLIBS			=	$(SCLIB) -y=$(MWOS_ICLIBDIR)/sclib.il
SYS_CLIB		=	-l=$(MWOS_BASCLIB)/sys_clib.l
SYS_CLIBS		=	$(SYS_CLIB) -y=$(MWOS_ICLIBDIR)/sys_clib.il
SYS_CSL			=	-l=$(MWOS_BASCLIB)/sys_csl.l
SYS_CSLS		=	$(SYS_CSL) -y=$(MWOS_ICLIBDIR)/sys_csl.il
TERMLIB			=	-l=$(MWOS_DFTCLIB)/termlib.l
TERMLIBS		=	$(TERMLIB)

SYS				=	-l=$(MWOS_DFTLIB)/sys.l
USR				=	-l=$(MWOS_DFTLIB)/usr.l

MWOS_LIBS		=	$(SYS_CLIBS) $(CLIBS) $(OS_LIBS) $(SYS)
MWOS_CSLLIBS	=	$(SYS_CSLS) $(SYS_CLIBS) $(CSLS) $(CLIBS) $(OS_CSLS) \
					$(OS_LIBS) $(SYS) 

MBUFLIB			=	-l=$(MWOS_BASLIB)/mbuflib.l
MBUFLIBS		=	$(MBUFLIB) -y=$(MWOS_ILIBDIR)/mbuflib.il

CONV_LIB        =   -l=$(MWOS_BASLIB)/conv_lib.l
CONV_LIBS       =   $(CONV_LIB) -y=$(MWOS_ILIBDIR)/conv_lib.il
CPULIB          =   -l=$(MWOS_DFTLIB)/cpu.l
CPULIBS         =   $(CPULIB) -y=$(MWOS_ILIBDIR)/cpu.il
LOCKLIB         =   -l=$(MWOS_DFTLIB)/lock.l
LOCKLIBS        =   $(LOCKLIB) -y=$(MWOS_ILIBDIR)/lock.il

ISDNLIB			=	-l=$(MWOS_DFTLIB)/isdnlib.l
ISDNLIBS		=	$(ISDNLIB) -y=$(MWOS_ILIBDIR)/isdnlib.il
TESTSUITE    	=   -l=$(MWOS_DFTLIB)/testsuite.l
TESTSUITES  	=   $(TESTSUITE) -y=$(MWOS_ILIBDIR)/testsuite.il
TIMERLIB    	=   -l=$(MWOS_DFTLIB)/timerlib.l
TIMERLIBS   	=   $(TIMERLIB) -y=$(MWOS_ILIBDIR)/timerlib.il
X25LIB			=	-l=$(MWOS_DFTLIB)/x25lib.l
X25LIBS			=	$(X25LIB) -y=$(MWOS_ILIBDIR)/x25lib.il

CDILIB			=	-l=$(MWOS_DFTLIB)/cdi.l
CDBLIB			=	-l=$(MWOS_DFTLIB)/cdb.l

UNIXLIB			=	-l=$(MWOS_BASLIB)/unix.l
UNIXLIBS		=	$(UNIXLIB) -y=$(MWOS_ILIBDIR)/unix.il

# Root PSects
ANSI_CSTART		=	ansi_cstart.r		# allow host type name override
CSTART			=	$(MWOS_DFTLIB)/$(ANSI_CSTART)
SYSANSI_CSTART	=	sysansi_cstart.r	# allow host type name override
SYSCSTART		=	$(MWOS_DFTLIB)/$(ANSI_CSTART) #$(MWOS_DFTLIB)/$(SYSANSI_CSTART)
DESCSTART		=	$(MWOS_DFTLIB)/descstart.r
DRVSTART		=	$(MWOS_DFTLIB)/drvstart.r
FMSTART			=	$(MWOS_DFTLIB)/fmstart.r
SUBSTART		=	$(MWOS_DFTLIB)/substart.r
SYSTART			=	$(MWOS_DFTLIB)/systart.r

# Default Compile/Assemble/Link Options
UCC_WARN		= 	-cw
CPARMS			=	-td=$(TEMP) -to=$(OS) $(UCC_WARN) -tp=$(CPUC)
CPARMSNC		=	-td=$(TEMP) -to=$(OS) $(UCC_WARN) -tp=$(CPU)
APARMS			=	-td=$(TEMP) -to=$(OS) $(UCC_WARN) -tp=$(CPU)
LPARMS			=	-td=$(TEMP) -to=$(OS) $(UCC_WARN) -olg -tp=$(CPU)

COPT_SC         =,sc
COPT_LC         =,lc
COPT_SD         =,sd
COPT_LD         =,ld
COPT_SCD        =
COPT_LCD        =

COPT_CODEADDR	=	
COPT_LONGADDR   =   -be=pl	# obsolete - use LONGDATA/LONGCODE
COPT_LONGDATA   =   -be=pl
COPT_LONGCODE	=

COPT_JMPTBL		=	-ola
COPT_GU0	=   -olgu=0.0 
COPT_GU1	=   -olgu=1.0 
COPT_AM0	=   -olf=oroegrge
COPT_AM1	=   -olf=oroegrgeprpe
COPT_AF0	=   -olf=orgr 
COPT_AF1	=   -olf=orgrpr

# Lint Options
LINTPARMS		=	-d_UCC -dANSI_EXT -d_OSK -d_MPFCPU32 -d_FPF881 -d_BIG_END

#																			#
#############################################################################
