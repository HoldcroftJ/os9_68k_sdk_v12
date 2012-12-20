
* Copyright 1993 by Microware Systems Corporation
* Reproduced Under License

* This source code is the proprietary confidential property of
* Microware Systems Corporation, and is provided to licensee
* solely for documentation and educational purposes. Reproduction,
* publication, or distribution in any form to any party other than
* the licensee is strictly prohibited.


********************
* Edition History
*
* Ed.   Date.    Reason												Who
* --  --------  ---------------------------------------------------	---
* 01  93/07/09	split from boot.a (ed 61).							wwb
*                ---- OS-9/68K V3.0 Release ----
*                ---- OS-9/68K V3.1 Release ----
*

* Bootstrap system global constants.
*
* These constants are kept in this file so that multiple users of
* the data can access them.  Normally these constants are part of
* "boot.a", but others (eg disklink.a) may have a need to access
* the constants also.

********************************
* Absolute Address references and Constants
*
MemList:  MemDefs define the memory list for a given system

ChunkSiz: dc.l $2000 block size for initial RAM search
ConsPort: dc.l Cons_Adr System acia console port address
CommPort: dc.l Comm_Adr System auxilliary port address
DiskPort: dc.l SysDisk System disk port address
DiskVect: dc.b FDsk_Vct Irq Vector # for disk
CallDBug: dc.b 0 0 = call debugger
AbortVect: dc.b ABORTVECT abort switch vector #
 dc.b 0 reserved for alignment
VBRPatch: dc.l VBRBase VBR for system
TransFact: dc.l TRANSLATE dma address translation factor
 align

* end of file (bootglob.d)

