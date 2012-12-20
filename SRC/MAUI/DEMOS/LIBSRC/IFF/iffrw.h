
/*----------------------------------------------------------------------\
!                                                                       !
!   Name: IffRW.h                                                       !
!                                                                       !
!   Function: Internal definitions for reader and writer code.          !
!                                                                       !
!   Revision History:                                                   !
!    #  Reason for Change                               By  Date        !
!  ---- ----------------------------------------------- --- --------    !
!   1   Creat                                           tod 88/06/22    !
!   2   Mods for AIFF and IMAG - Version 0.90           srm 88/07/27    !
!   3   Mods for Version 0.99                           srm 89/01/09    !
\----------------------------------------------------------------------*/

/****************************************************************************
*                                                                           *
*              Copyright 1988 by Microware Systems Corporation              *
*                         Reproduced Under License.                         *
*                                                                           *
* This  source  code is  the proprietary confidential property of Microware *
* Systems Corporation, and is  provided  to licensee  for documentation and *
* educational purposes only. Reproduction, publication, or  distribution in *
* any form to any party other than the licensee is strictly prohibited.     *
*                                                                           *
****************************************************************************/

#ifndef _IFFRW_H
#define _IFFRW_H

#include <MAUI/iff.h>

/*****************************************************************************
*
* IFF Macros
*
*/
#define IFF_IS_ODD(a) ( (a)&1 )         /* return true if odd value */
#define IFF_MAKE_EVEN(a) ( ((a)&1) ? ((a)+1) : (a) ) /* make number even */

/*****************************************************************************
*
* IFF Constants
*
*/
#define IFF_ID_CAT  IFF_MAKEID('C','A','T',' ') /* type for CAT group */
#define IFF_ID_FORM IFF_MAKEID('F','O','R','M') /* type for FORM group */
#define IFF_RBF     0                           /* device type is RBF */
#define IFF_PIPE    1                           /* device type is PIPE */

/*****************************************************************************
*
* IFF Group Structure. This is for CAT and FORM headers. Note that the size
* includes all data that follows. This includes the group id.
*
*/
typedef struct {
    IFF_U_32 type;                          /* group type (CAT or FORM) */
    IFF_U_32 size;                          /* group size */
    IFF_U_32 id;                                /* group ID */
} IFF_GROUP;
#define SIZEOF_IFF_GROUP (sizeof(IFF_U_32)*3)

/*****************************************************************************
*
* IFF Chunk Structure. This is for DATA chunks.
*
*/
typedef struct {
    IFF_U_32 id;                                /* chunk type */
    IFF_U_32 size;                          /* chunk size (size of data) */
} IFF_CHUNK;
#define SIZEOF_IFF_CHUNK (sizeof(IFF_U_32)*2)

/*****************************************************************************
*
* Prototypes for swap/alignment functions 
*
*/
IFF_U_32 iff_get32(u_char *s);
void iff_set32(u_char *d, IFF_U_32 s);
IFF_U_16 iff_get16(u_char *s);
void iff_set16(u_char *d, IFF_U_16 s);

#endif 
