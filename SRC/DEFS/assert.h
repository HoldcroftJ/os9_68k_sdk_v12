/* ****NOTE: The ANSI defined behavior of assert and NDEBUG dictate that no
			 wrapper be used to guard against multiple inclusion in this
			 header file.
*/

/*--------------------------------------------------------------------------,
!                                                                           !
!               Copyright 1998 by Microware Systems Corporation             !
!                           Reproduced Under License                        !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!---------------------------------------------------------------------------!
!                                                                           !
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 92-03-17 Created													rcb !
!	2 92-05-07 Changed proto for __assert so that line is an int		rcb !
!	3 95-02-06 Added conditionals for C++								rcb !
!	4 98-02-20 Added this header										nan !
!                                                                           !
`--------------------------------------------------------------------------*/


#undef assert

#if defined(NDEBUG)
#define assert(ignore)	((void)0)
#else

#if !defined(__STDC__) && !defined(_ANSI_EXT) && !defined(__cplusplus)
extern void	__assert();
#define assert(expression) ((expression) ? (void)0 : \
		__assert(#expression,__FILE__,__LINE__))
#else
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void	__assert(char *expression, char *file, int line);
#define assert(expression) ((expression) ? (void)0 : \
		__assert(#expression,__FILE__,__LINE__))
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* __STDC__ */

#endif /* NDEBUG */
