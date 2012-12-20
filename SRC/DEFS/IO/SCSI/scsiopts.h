#if !defined(_RBF_H)
#define _RBF_H

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1996 by Microware Systems Corporation              |
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
! SCSI specific definitions for use by SCSI oriented drivers/descriptors    |
|                                                                           |
!  Edition History:                                                         !
!   #   Date    Comments                                                By  !
!  -- -------- -------------------------------------------------------- --- !
! 	1 96/11/06  created from rbf.h                                      ebm !
|               ---- SCSI Common Sub-Component v1.0 Released ----           |
|               $$               <RELEASE_INFO>                          $$ |
`--------------------------------------------------------------------------*/

/* 
 * SCSI Options bit defintions - these are applicable to SCSI based hardware
 * only.
 */
#if !defined (_SCSI_H)
#define _SCSI_H

#if !defined(_SCSI_OPTS)

#ifdef _OS9000
#define SCSI_ATN     0x01    /* attention supported -- drive supports
                              * SCSI attention and standard messageing.
                              * (required for disconnect support.)
                              */

#define SCSI_PARITY  0x02	/* enable parity checking on data transfers.
                             * Drivers should set the up controllers to
                             * generate PARITY.  This flag indicates that
                             * parity should be checked on incomming data.
                             */

#define SCSI_SYNC    0x04   /* drive supports synchronous data transfer
                             * This would cause the driver to negotiate
                             * for synchronous transfers.
                             */

#define SCSI_WIDE    0x08   /* drive may support SCSI-II "wide" bus
                             * (16 bit transfers.)  This would cause
                             * the driver to negotiate for wide bus
                             * transfers. ( implies ATN is also set )
                             */

#define SCSI_TARGET  0x10   /* This unit should function as a target
                             * device on the SCSI bus.  We currently
                             * have no drivers that can do this but...
                             */
#else 
/* OS-9'S DEFS ARE DIFFERENT !!!!!!!! */
#define     SCSI_ATN        (1<<0)  /* assert ATN supported             */
#define     SCSI_TARGET     (1<<1)  /* target mode supported            */
#define     SCSI_SYNCHR     (1<<2)  /* synchronous transfers supported  */
#define     SCSI_PARITY     (1<<3)  /* enable SCSI parity checking      */
#define     SCSI_WIDE       (1<<4)  /* enable SCSI wide operation       */

#endif /* _OS9000 */

#define     _SCSI_OPTS

#endif /* _SCSI_OPTS */

#endif /* _SCSI_H */
