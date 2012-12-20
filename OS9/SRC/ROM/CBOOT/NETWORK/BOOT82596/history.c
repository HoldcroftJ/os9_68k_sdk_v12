/*-------------------------------------------------------------------\
| Copyright 1989,1993 by Microware Systems Corporation
| Reproduced Under License
|
| This source code is the proprietary confidential property of
| Microware Systems Corporation, and is provided to licensee
| solely for documentation and educational purposes. Reproduction,
| publication, or distribution in any form to any party other than
| the licensee is strictly prohibited.
|
|		internet bootp driver for i82596 Intel 32-bit ethernet
|
| Edition History
|  #    Date      Comments                                      by
| -- --------- ------------------------------------------------ ---
| 01 91/12/13   made into bootp driver from isp driver.         djl
| 02 93/06/17   "Ultrification"                                 bat
| 03 93/08/11  Two changes to makefiles in IE162 and IE167:                 gkm
|              1. ie162.h and ie167.h were merged into ie162167.h for both
|                 the booter and the driver (4 files into 1).
|              2. LCLMEM was undefined in the driver makefiles - it was
|                 changed to PVTRAM for the driver long ago (see note #11)
|                 ago BUT the booter still uses LCLMEM!!!  Now that i82596.h    
|                 is being merged for the two we better not confuse their
|                 usage.  Both (LCLMEM and PVTRAM) are used in the i82596.h 
|                 file because the booter and driver seem to be using 
|                 differnt structures when they have private memory (or 
|                 local ram, what ever you may want to call it).
|				-dCBOOT is now being used to distinguish between booter 
|				and driver in the newly merged i82596.h
|				ie.h was also merged for booter & driver & moved to
|				mwos/os9/src/defs/inet
|               ---- OS-9 V3.0 Released ----                         
| 04 98/01/25   Removed 5 tick add to timeout value              mgh
|               enclosed rprintf's in "if (msgprint=xx)" control 
| 				added #include <inetboot_p.h> to files using msgprint
|               changed for Edition 12 of inetboot.l
\-------------------------------------------------------------------*/
