/*--------------------------------------------------------------------------!
! qedvr.h for 68360 scc ethernet contoller using internet BOOTP             !
+---------------------------------------------------------------------------+
!   Edition History:                                                        !
!   #   Date    Comments                                                By  !
!   -- -------- ------------------------------------------------------- --- !
!   00 94/xx/xx Cloned from other Cboot stuff                           arb !
!   01 95/01/30 changed reference of ethernet.h to enet360.h            mgh !
!               ---- OS-9 V3.0.1 Released ----                              !
!---------------------------------------------------------------------------*/
/* This part was if.h */
#include <types.h>
#include <const.h>
#include <errno.h>

#include <MACHINE/reg.h>
#include <modes.h>
#include <procid.h>
#include <sg_codes.h>
#include <sysio.h>
#include <path.h>

#include <INET/socket.h>
#include <INET/in.h>
#include <INET/mbuf.h>
#include <INET/ifman.h>


#define busy_driver(ifstat) (ifstat->v_sysio.v_busy++)
#define free_driver(ifstat) { register int i; if (--ifstat->v_sysio.v_busy \
 == 0) if (i = ifstat->v_sysio.v_dumm1[7]) f_aproc(i); }

 
/*********** FOR BOOT-CODE USE ONLY *****************************************
 * the bpt_ timer code is responsible for the next two items.				*
 * bpt_rseed should be initialized to something random.						*
 * bpt_curtick should increase by one for each elasped 100ms.				*
 ****************************************************************************/
extern                  int bpt_rseed;      /* RNG seed for tbackoff */
extern /*volatile*/ u_int32 bpt_curtick;    /* 100ms tick from timer irq */
    
/* since use of if.h has been discontinued for backplane driver and booter,
 * "extern bpt_curtick" has been explicitly declared in recv.c of the booter.
 * if you change the declaration here, check if it needs to be changed there
 * too. 93/09/10.
 */
/* #include <if.h> */
#include <arp.h>
#include <ether.h>
#include <quicc.h>
#include <enet360.h>

struct qe_desc {
	u_int8		*port;
	u_int16		max_rbufs;	/* max. receive mbufs to use */
	u_int8		my_ether_addr[6];
};


typedef struct ifs {
	void 			*v_port;
	u_int16			v_running;		/* driver initialized and running */
	u_int16			v_irqlvl;		/* irq mask level */
	u_int8			*v_addr;
	Mbuf			v_inq;
	struct qe_desc	v_desc;			/* copy of device descriptor goodies */
} *Ifs;


