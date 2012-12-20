/*--------------------------------------------------------------------------,
|                                                                           |
|           Copyright 1996-1998 by Microware Systems Corporation            |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, distribution, or creation of derivative works |
|  in any form to any party other than the licensee is strictly prohibited, |
|  unless expressly authorized in writing by Microware Systems Corporation. |
|                                                                           |
|---------------------------------------------------------------------------|
|                                                                           |
| piclib.h - Programmable Interrupt Controller function prototypes.         |
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---  |
|  01 96/12/03 Created.                                                gdb  |
|              ---- OS-9000/ARM V2.2.3 OS Component Released ----           |
|              ---- MBX_BSP Released ----                                   |
|              ---- OS-9000/SH3 V2.2.6 Released ----                        |
|              ---- OS-9000/SPARC V2.2.7 Released ----                      |
|              ---- OS-9000 OS Sub-component v2.2.8 Released ----           |
|              ---- OS-9000 IO Base Sub-component v1.1 Released ----        |
|              ---- SCSI Common Sub-Component v1.0 Released ----            |
|              ---- OS-9000 SCSI Sub-component v1.0 Released ----           |
|              ---- OS-9000 CDFM Sub-component v1.0 Released ----           |
|              ---- PIC v1.1 Sub-component Released ----                    |
|              ---- PIC v1.2 Sub-component Released ----                    |
|              ---- PIC v1.2.1 Sub-component Released ----                  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/
 
#if !defined(_PICLIB_H)
#define _PICLIB_H

#if !defined(_TYPES_H)
#include <types.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

error_code _pic_enable(u_int32);
error_code _pic_disable(u_int32);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _PICLIB_H */

