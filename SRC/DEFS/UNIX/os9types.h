/*--------------------------------------------------------------------------,
! edition history                                                           !
!   #   date     comments                                               by  !
!  -- -------- -------------------------------------------------------- --- !
!   1 95-09-18  Created                                                 rry !
!	2 96-05-13	Optional prototypes added								rry !
!	3 97-06-06	New support for >= 32 paths in OS-9000					tsb !
!	4 97-10-06	Changed "unix" reference to "UNIX"						tsb !
!   5 98-02-20  Added this header                                       nan !
!                                                                           !
`--------------------------------------------------------------------------*/

#if !defined(_OS9TYPES_H)
#define _OS9TYPES_H

#if !defined(_OS9TIME_H)
#include <UNIX/os9time.h>
#endif

#ifndef FD_SETSIZE
#ifdef _OSK
#define FD_SETSIZE	32
#elif defined(_OS9000)
#define FD_SETSIZE	256
#else
#error Unknown OS
#endif
#endif


#define FD_MASKSIZE 32  
typedef unsigned int fd_mask;

#define FD_SIZE 	(((FD_SETSIZE) + ((FD_MASKSIZE)-1)) / (FD_MASKSIZE))

typedef struct fd_set {
	fd_mask fds_bits[FD_SIZE];
} fd_set;

/*	select() bit-field manipulation macros:
**  
**	FD_SET(int fd, fd_set *fdset);  
**	FD_CLR(int fd, fd_set *fdset);  
**	FD_ISSET(int fd, fd_set *fdset);
**	FD_ZERO(fd_set *fdset);
*/
#if (FD_SIZE > 1) /* Use an array of u_int32's */
#	define	FD_CHOOSEWORD(bit_num)	((bit_num)/FD_MASKSIZE)
#	define	FD_CHOOSEBIT(bit_num)	((bit_num)%FD_MASKSIZE)
#	define FD_SET(bit_num,bits)	((bits)->fds_bits[FD_CHOOSEWORD(bit_num)] |=  \
							(1 << FD_CHOOSEBIT(bit_num)))
#	define FD_CLR(bit_num,bits)	((bits)->fds_bits[FD_CHOOSEWORD(bit_num)] &=  \
							~(1 << FD_CHOOSEBIT(bit_num)))
#	define FD_ISSET(bit_num,bits)	((bits)->fds_bits[FD_CHOOSEWORD(bit_num)] &  \
							(1 << FD_CHOOSEBIT(bit_num)))
#	define FD_ZERO(bits)		{ int word;  \
					  for (word=FD_SIZE-1; word>=0; word--)  \
					      ((bits)->fds_bits[word] = 0); }
#elif (FD_SIZE == 1) /* ASSUME: FD_SETSIZE (max open paths) == 32 */
	#define FD_SET(fd,fdset)	(*((fd_mask*)fdset) |= (1 << fd))
	#define FD_CLR(fd,fdset)	(*((fd_mask*)fdset) &= (~(1 << fd)))
	#define FD_ISSET(fd,fdset)	(*((fd_mask*)fdset) & (1 << fd))
	#define FD_ZERO(fdset)		{*((fd_mask*)fdset) = 0;}
#else
#error	FD_SIZE <= 0
#endif

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int select _OP((unsigned int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif	/* _OS9TYPES_H */
