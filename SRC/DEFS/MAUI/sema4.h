#if !defined(_SEMA4_H)
#define _SEMA4_H

/*--------------------------------------------------------------------------,
|                                                                           |
|        Copyright 1991, 1992, 1993 by Microware Systems Corporation        |
|                          Reproduced Under License                         |
|                                                                           |
|  This source code is the proprietary confidential property of Microware   |
|  Systems Corporation, and is provided to licensee for documentation and   |
|  educational purposes only. Reproduction, publication, or distribution    |
|  in any form to any party other than the licensee is strictly prohibited. |
|                                                                           |
`--------------------------------------------------------------------------*/

#include <semaphore.h>

/* Semaphore structure definition */
typedef semaphore sema4, *Sema4;

/*******************************************************************************
* PROTOTYPES
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

error_code sema_init(sema4 *);
error_code sema_p(sema4 *);   
error_code sema_term(sema4 *); 
error_code sema_v(sema4 *);   

#ifdef __cplusplus
}
#endif

#endif /* _SEMA4_H */
