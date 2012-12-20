********************************
* System Definitions for ram disk descriptors.
*
********************************
* Edition History
*   date    comments                                            by
* -------- ---------------------------------------------------- ---
* 88/08/01 created.                                             wwb
* 88/10/03 added ifndef cases for DiskR0, DiskR1 being defined  wwb
*          in local systype.d
*           ---- OS-9/68000 V2.3 released ----
*           ---- OS-9/68000 V2.4 released ----
* 93/07/02 added more comment info.  Made R1 example use a		wwb
*          port address "less likely" to clash with normal
*          system ram.
*			---- OS-9/68k V3.0 released ----
* 96/07/18 updated r1 example for variable sector size.			wwb
*          added r2 example for huge ramdisk.					wwb
*			---- OS-9/68k V3.0.3 released ----
*			---- OS-9/68k V3.1 released ----
*

********************************
* RBF device descriptor definitions
*    used only by rbf descriptor modules
*
* RBFDesc Port,Vector,IRQLevel,Priority,DriverName,DriveOptions
*

* volatile ram disk
*
* This is a ram disk which will be allocated from the system's
* free memory pool..  A Port address from 0 to 0x3ff will determine
* that the free memory pool is used.  This allows up to 1024
* individual ram disks to be created (using the free memory pool).
*

 ifndef DiskR0

DiskR0 macro

 RBFDesc 0,0,0,0,ram,ramdisk

* note that sizes here are assuming that the default sector size
* of 256 is being used.

*SectTrk set 128 sectors/track (32K of sectors)
*SectTrk set 256 sectors/track (64K of sectors)
*SectTrk set 512 sectors/track (128K of sectors)
*SectTrk set 1024 sectors/track (256k of sectors)
*SectTrk set 2048 sectors/track (512k of sectors)
SectTrk set 3072 sectors/track (768k of sectors)
*SectTrk set 4096 sectors/track (1024k of sectors)
*SectTrk set 8192 sectors/track (2048k of sectors)

 endm
 endc

* non-volatile ram disk (NOTE: WARNING this is an example!)
*
* This is a ram disk which will be NOT be allocated from the
* system's free memory pool.  Instead, a Port address of 0x400
* or greater causes the ram disk driver to assume that memory
* is available at the address specified (and continuing thru the
* size given) for use as a ram disk.  The ram disk may be
* "volatile" or "non-volatile".  The allows, for eg, a battery
* backed ram card to be used as the systems ram disk.
*
* The V3.0.3 release os OS-9/68K provides a ramdisk driver (ed. 24)
* that allows variable sector sizes (256 - 32K sectors).  The
* media can also be "reformatted" (using Format) and/or OS9Gen'd.
* To do these operations on a "nvramdisk", you must use the format
* enabled version of the descriptor (e.g. r1fmt) [this is just like
* formatting a hard disk]
* 
 ifndef DiskR1

DiskR1 macro

 RBFDesc $80000000,0,0,0,ram,nvramdisk

* the default sector size is 256-byte sectors.  This can be
* over-ridden by defining SectSize in the macro.

* this next line would create a 2M ram disk, using 256-byte sectors.
*SectTrk set 8192 sectors/track (2048k of sectors)

* these two lines create a 2M ram disk, using 512-byte sectors.
SectSize set 512 sector size
SectTrk set 2048*2 2M ramdisk

 endm
 endc


* example of a huge non-volatile ramdisk.  Note the usage of the
* DevCon flag to set the "concatenate" sct/trk fields mode
* of the ramdisk driver, so that more than 64K sectors can be
* defined for the media

 ifndef DiskR2

DiskR2 macro

 RBFDesc $80000000,0,0,0,ram,nvramdisk

* these lines create a 256M ram disk, using 1024-byte sectors.
SectSize set 1024 sector size

MediaSize set 0x10000000 256MB total size

* the media size (in sectors) is determined by the formula:
*    (SectTrk0 << 16 ) + (SectTrk)
*
SectTrk0 set (MediaSize&0xffff0000)>>16
SectTrk set MediaSize&0x0000ffff

DevCon dc.b RamConcat concatenate sect/trk fields for size
 
 endm
 endc

* end of file
