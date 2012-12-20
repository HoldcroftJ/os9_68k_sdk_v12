/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 2000 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 00/12/22 Added edition history.                                  djc  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#if !defined(_FCNTL_H)
#define _FCNTL_H

/* Flag values accessible to open(2) and fcntl(2) */
/*  (The first three can only be set by open) */
#define	O_RDONLY 1	/* xxxxxx01	-- read only	*/
#define	O_WRONLY 2	/* xxxxxx10	-- write only	*/	
#define	O_RDWR	 3	/* xxxxxx11	-- read/write	*/
#define O_ACCMODE (O_RDONLY | O_WRONLY)

#define	O_APPEND 16	/* xxx1xxxx -- append (writes guaranteed at the end) */

/* Flag values accessible only to open(2) */
#define	O_CREAT	8	/* xxxx1xxx -- open with file create (uses third open arg)*/
#define	O_EXCL	4	/* xxxxx1xx -- exclusive open */
#define O_NOCTTY 32	/* xx1xxxxx -- not control terminal open */
#define O_NONBLOCK 64 	/* x1xxxxxx -- non-blocking I/O	*/
#define	O_TRUNC	128	/* 1xxxxxxx -- open with truncation */

#define	O_NDELAY 64	/* Non-blocking I/O */
#define O_SYNC	 256	/* synchronous write option */

#define O_TEXT		0x8000
#define O_BINARY	0x4000

/* fcntl(2) requests */
#define	F_DUPFD	0	/* Duplicate fildes */
#define	F_GETFD	1	/* Get fildes flags */
#define	F_SETFD	2	/* Set fildes flags */
#define	F_GETFL	3	/* Get file flags */
#define	F_SETFL	4	/* Set file flags */
#define	F_GETLK	5	/* Get file lock */
#define	F_SETLK	6	/* Set file lock */
#define	F_SETLKW	7	/* Set file lock and wait */

#endif  /* _FCNTL_H */

