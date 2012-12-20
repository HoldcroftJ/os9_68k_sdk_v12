/*******************************************************************************
* FILENAME : mfm.h
*
* DESCRIPTION :
*
*   This file contains definitions for the interface to the MAUI graphics
*   driver. This file should not be used by applications.
*
* COPYRIGHT:
*
*   This source code is the proprietary confidential property of Microware
*   Systems Corporation, and is provided to licensee solely for documentation
*   and educational purposes. Reproduction, publication, or distribution in
*   form to any party other than the licensee is strictly prohibited.
*
* EDITION  DATE      WHO  DETAIL
*       1  12/21/94  TJE  First version
*       2  12/18/00  SRM  CF2120 Add extended device capabilities
*/
#ifndef _MFM_H
#define _MFM_H

#include <sg_codes.h>

/*******************************************************************************
* FUNCTION CODES FOR PARAMETER BLOCKS
*
* The function codes below are 16 bit values and should fall into the following
* ranges:
*
*     0 - 32767 --> Functions defined by MAUI as part of the official interface
*                   between the application/APIs and the driver.
* 32768 - 49151 --> Functions defined within the template code for the MAUI
*                   drivers.
* 49152 - 57343 --> Driver specific (non-standard) functions defined by
*                   Microware.
* 57344 - 65535 --> Driver specific (non-standard) functions defined by
*                   3rd parties.
*******************************************************************************/

#ifndef SS_MAUI 
#define SS_MAUI 0x95
#endif

#define FC_BLT_CREATEBC      1
#define FC_BLT_DESTROYBC     2
#define FC_GFX_BKCOL         3
#define FC_GFX_OPENDEV       4
#define FC_GFX_CREATEVP      5
#define FC_GFX_CLOSEDEV      6
#define FC_GFX_DESTROYVP     7
#define FC_GFX_DEVCAP        8
#define FC_GFX_DEVRES        9
#define FC_GFX_EXTVID       10
#define FC_GFX_TRANSCOL     11
#define FC_GFX_VPMIX        12
#define FC_GFX_COMPAT       13
#define FC_GFX_ALLOC        14
#define FC_GFX_DEALLOC      15
#define FC_GFX_RESTACKDEV   16
#define FC_GFX_DECODEDST    17
#define FC_SND_DEVCAP       18
#define FC_SND_DEV_STATUS   19
#define FC_SND_PLAY         20
#define FC_SND_RECORD       21
#define FC_SND_ABORT        22
#define FC_SND_GAIN         23
#define FC_SND_PAUSE        24
#define FC_SND_CONT         25
#define FC_GFX_CLONEDEV     26
#define FC_GFX_CLONEVP      27
#define FC_SND_COMPAT       28
#define FC_GFX_CURCREATE    29
#define FC_GFX_CURDESTROY   30
#define FC_GFX_CURCAP       31
#define FC_GFX_ATTRIBUTE    32
#define FC_GFX_DEVCAPEXTEN  33

#endif /* _MFM_H */
