/*
	$Header:   /h0/MWOS/OS9/SRC/IO/BETA_PCF/DEFS/VCS/typedefs.h_v   1.2   24 Jul 1998 12:13:34   dibble  $
	$Log:   /h0/MWOS/OS9/SRC/IO/BETA_PCF/DEFS/VCS/typedefs.h_v  $
 *  *    Rev 1.2   24 Jul 1998 12:13:34   dibble * Fix errors resulting from 0-length file bug fix. * Sometimes PCF could not find FDs for 0-lenght files, * and locking a zero-length file locked the root directory. *  *  * 
 *    Rev 1.1   28 Dec 1997 19:46:38   dibble
 * 
 *    Rev 1.0   28 Dec 1997 19:43:56   dibble
 * Initial revision.
 * 
 *    Rev 1.0   13 Mar 1991 14:08:16   peter
 * Initial revision.
*/
typedef void *POINTER;
typedef unsigned char uchar;
typedef unsigned char boolean;
typedef unsigned long ulong;

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

