/*--------------------------------------------------------------------------,
|                                                                           |
|            Copyright 1995-1998 by Microware Systems Corporation           |
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
| NCR 53C710 prototypes                                                     |
|                                                                           |
| Edit History:                                                             |
| Date       Who   Reason                                                   |
| --------   ----  --------------------------------------------------       |
| 7 Mar 95   GbG   Created                                                  |
| 23May 96   GbG   Removed in/out proto for OS9000                          |
| 30May 96   GbG   Work around added for 386 icode compiler bug.            |
|                  ---- OS-9/68K V3.0.3 Released ----                       |
|                  ---- OS-9000/x86 V2.1 Released ----                      |
|                  ---- OS-9000/PPC V2.1.1 Released ----                    |
|                  ---- OS-9000/ARMv3 V2.2 Released ----                    |
| 21Apr 97   GbG   Added Wide Support and support for PCI memory model.     |
| 21May 97   GbG   Systype.h only included for NCR53C8xx series.            |
|                  ---- OS-9000/PPC V2.1.2 Released ----                    |
|                  ---- OS-9000/ARMv3 V2.2.1 Released ----                  |
| 24Jul 97   GbG   386 requires prototype for inc when compiled             |
|                  for I/O mode.                                            |
| 17Nov 97   gdb   systype.h now always included for OS-9000.               |
| 10Dec 97   ljs   Removed prototype of "inc()" conditionally defined       |
|                  for x86 targets.                                         |
| Jan29 98   GbG   Fixed compiler warnings.                          GbG    |
|                  ---- OS-9000/SH3 V2.2.6 Released ----                    |
|                  ---- SCSI Common Sub-Component v1.0 Released ----        |
|                  $$               <RELEASE_INFO>                 $$       |
| Sep20 01  pengf  Improve the fariness of access to disk (CF10781)        	|
|--------------------------------------------------------------------------*/

#if defined (_OSK)
#define inc(p) ((u_char)*((volatile u_char*)p))
#define inw(p) ((u_int16)*((volatile u_int16*)p))
#define inl(p) ((u_int32)*((volatile u_int32*)p))
#define outc(p,c) (*((volatile u_char*)p) = (u_char)c)
#define outw(p,w) (*((volatile u_int16*)p) = (u_int16)w)
#define outl(p,l) (*((volatile u_int32*)p) = (u_int32)l)

#define SUPSTAT 0x2000              /* supervisor state (68xxx SR)   */
#define LEVEL7  SUPSTAT | 0x700     /* level bits for SR (highest mask) */
 
#endif

#if defined(_OS9000) && !defined SCSI_SYNCHR
#define SCSI_SYNCHR SCSI_SYNC
#endif

#if !defined(SWAPLONG) 
#if defined(SIOP_LIL_END)
#define SWAPLONG(a) \
	(((a&0xff000000)>>24)|((a & 0xff0000)>>8)|((a & 0xff00)<<8)|((a&0xff)<<24)) 
#else 
#define SWAPLONG
#endif
#endif

#if !defined(SWAPWORD) 
#if defined(SIOP_LIL_END)
#define SWAPWORD(a) (((a & 0xff00)>>8)|((a & 0xff)<<8))
#else
#define SWAPWORD
#endif
#endif

/*
 * We need to define macros that translate CPU to PCI and PCI to CPU
 * memory references.
 * 
*/

#if defined(_OS9000)
#include <systype.h>
#if defined(INCLUDE_PIC_CODE)
#include <piclib.h>
#endif
#endif

#if !defined(SCRIPT_ADDRESS) 
#if defined(SIOP_LIL_END)
#define PCI_IO_PAGE (PCI_IO_BASE >> 24)
#define PCI_MEM_PAGE (PCI_MEM_BASE >> 24)
#define SCRIPT_ADDRESS(a) ((a) == 0 ? 0 : (a)|PCI_IO_PAGE)
#define SCRIPT_MADDRESS(a) ((a)&~PCI_IO_BASE)
#else
#define SCRIPT_ADDRESS
#define SCRIPT_MADDRESS
#endif
#endif

#if defined(NCR53C8XX)

#if !defined(NO_WIDE_SUPPORT)
#define WIDE_SUPPORT
#endif

#if defined(PCI_IO_MAPPED)

#include <inout.h>

#define SCSI_OUTC(p,c)  OUT( ((void *)(p)),c)
#define SCSI_OUTW(p,c)  OUTW( ((void *)(p)),c)
#define SCSI_OUTL(p,c)  OUTL( ((void *)(p)),c)
#define SCSI_INC(p)  IN( ((void *)(p)))
#define SCSI_INW(p)  INW( ((void *)(p)))
#define SCSI_INL(p)  INL( ((void *)(p)))

#else

#define SCSI_OUTC(p,c) (*((volatile u_int8*)p) = (u_int8)c)
#define SCSI_OUTW(p,c) (*((volatile u_int16*)p) = (u_int16)c)
#define SCSI_OUTL(p,c) (*((volatile u_int32*)p) = (u_int32)c)
#define SCSI_INC(p) ((u_int8)*((volatile u_int8*)p))
#define SCSI_INW(p) ((u_int16)*((volatile u_int16*)p))
#define SCSI_INL(p) ((u_int32)*((volatile u_int32*)p))

#endif

#endif


/*
 * proto.h - prototype definitions 
*/
 
/* devxxx.c proto */

#if defined (__STDC__) || defined (_ANSI_EXT)
u_int8 get_siop_clock( void );
error_code chipreset( Lstat );
error_code start_siop( Lstat );
void stop_siop( Lstat );
error_code buscondition( void );
void SCSI_Reset( Lstat );
error_code seltoirq( Lstat );
error_code initsto( Lstat );
void freesto( Lstat );
void _timer( int32 );
void start_sto( int32 );
void stop_sto( void );
#if defined(NCR53C8XX)
u_int32 get_port_address(u_int32, Lstat);
#endif
#ifdef CBOOT
error_code _f_sleep( int32 );
#endif

#else /* K&R proto */

u_int8 get_siop_clock();
error_code chipreset();
error_code start_siop();
void stop_siop();
error_code buscondition();
void SCSI_Reset();
error_code seltoirq();
error_code initsto();
void freesto();
void _timer();
void start_sto();
void stop_sto();
#if defined(NCR53C8XX)
u_int32 get_port_address();
#endif
#ifdef CBOOT
error_code _f_sleep();
#endif 
#endif

/* script.c proto */

/* hw53c710.c proto */

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code relocate( Lstat );
void act_scsi_select( Lstat, Lthread );
void scsi_rejectmsg( Lstat, Lthread  );
void act_scsi_rejectmsg( Lstat, Lthread  );
void scsi_idle( Lstat, Lthread  );
void act_scsi_idle( Lstat, Lthread  );
void scsi_free( Lstat, Lthread  );
void act_scsi_free( Lstat, Lthread  );
void scsi_pemsg( Lstat, Lthread  );
void scsi_smsg( Lstat, Lthread  );
void act_scsi_waitrsel( Lstat, Lthread  );
void nthread( Lstat, Lthread );
error_code ncr_reset( Lstat );
void ncr_start( Lstat );
void ncr_stop( Lstat );
void ncr_SCSI_reset( Lstat );
void ncr_kick( Lstat );
void save_state( Lstat );
error_code ncr_irq_sort( Lstat );
void routeon_dsp( Lstat );
void check_synchmsg( Lstat, Lthread );
void check_widemsg( Lstat, Lthread );
error_code ncr_ph_select( Lstat );
error_code ncr_ph_messd( Lstat );
error_code ncr_ph_idata( Lstat );
error_code ncr_ph_odata( Lstat );
error_code ncr_ph_mess( Lstat );
void ncr_flush_fifo( Lstat );
void ncr_scsi_reset( Lstat );
void ncr_setpar( Lstat, Lthread );
void ncr_setsync( Lstat, Lthread ); 
void ncr_rethread( Lstat, Lthread, int32 );
void ncr_clear_sxfr_mask( Lthread );
void ncr_init_sxfr_mask( Lthread );
void ncr_async_msg( Lstat, Lthread );
error_code set_msg( Lstat, Lthread, u_int32 );
#if defined(NCR53C8XX)
u_int8 ncr_scntl3( Lstat );
#endif

#else /* K&R proto */
error_code relocate();
void act_scsi_select();
void scsi_rejectmsg();
void act_scsi_rejectmsg();
void scsi_idle();
void act_scsi_idle();
void scsi_free();
void act_scsi_free();
void scsi_pemsg();
void scsi_smsg();
void act_scsi_waitrsel();
void nthread();
error_code ncr_reset();
void ncr_start();
void ncr_stop();
void ncr_SCSI_reset();
void ncr_kick();
void save_state();
error_code ncr_irq_sort();
void routeon_dsp();
void check_synchmsg();
void check_widemsg();
error_code ncr_ph_select();
error_code ncr_ph_messd();
error_code ncr_ph_idata();
error_code ncr_ph_odata();
error_code ncr_ph_mess();
void ncr_flush_fifo();
void ncr_scsi_reset();
void ncr_setpar();
void ncr_setsync();
void ncr_rethread();
void ncr_clear_sxfr_mask();
void ncr_init_sxfr_mask();
void ncr_async_msg();
error_code set_msg();
#if defined(NCR53C8XX)
u_int8 ncr_scntl3( Lstat );
#endif
#endif

/* term.c proto */

#if defined (_OS9000) && !defined(BOOTNCR)
error_code term (Dev_list, u_int32, u_int32, u_int32, Datmod);

#elif defined (_OSK) /* must be OSK */

#ifndef CBOOT

#if defined (__STDC__) || defined (_ANSI_EXT)
int term (mod_dev *, Datmod );
#else /* K&R proto */
int term ();
#endif

#else /* must be CBOOT */

#if defined (__STDC__) || defined (_ANSI_EXT)
int llterm (mod_dev *, Datmod );
#else /* K&R proto */
int llterm ();
#endif

#endif /* CBOOT case */

#endif /* _OS9000 or OSK case */

/* exec.c proto */

#if defined (_OS9000) && !defined(BOOTNCR)
error_code exec (Datmod, Cmdblk, u_int32 , u_int32);

#elif defined (_OSK)

#ifndef CBOOT
#if defined (__STDC__) || defined (_ANSI_EXT)
int exec (procid *, Datmod, Cmdblk, u_int32, u_int32 );
#else
int exec ();
#endif 

#else /* _OSK and CBOOT */

#if defined (__STDC__) || defined (_ANSI_EXT)
int llexec (Cmdblk, u_int32, u_int32 );
#else /* K&R proto */
int llexec ();
#endif

#endif /* CBOOT case */

#endif /* _OS9000 or OSK case */

/* init.c proto */

#if defined(_OS9000) && !defined(BOOTNCR)

error_code init(Dev_list, u_int32, u_int32, u_int32, Datmod * );

#elif defined (_OSK)

#ifndef CBOOT
#if defined (__STDC__) || defined (_ANSI_EXT)
int init (mod_dev *, Datmod * );
#else /* K&R proto */
int init ();
#endif

#else /* _OSK and CBOOT */

#if defined (__STDC__) || defined (_ANSI_EXT)
int llinit( void );
#else
int llinit();
#endif
#endif /* CBOOT case */

#endif /* _OS9000 or OSK case */

/* irq.c proto */

#if defined (__STDC__) || defined (_ANSI_EXT)
void scripti_msgi( Lstat, Lthread );
void scripti_badmsg( Lstat, Lthread );
void scripti_reject( Lstat, Lthread );
void scripti_idisc( Lstat, Lthread );
void scripti_done( Lstat, Lthread );
void scripti_resel( Lstat, Lthread );
void scripti_sigp( Lstat );
void dma_buserror( Lstat );
void dma_watchdog( Lstat );
void dma_illscript( Lstat );
void siop_par( Lstat );
void siop_rrst( Lstat );
void siop_udc( Lstat );
void siop_sge( Lstat );
void siop_sto( Lstat );
void siop_pm( Lstat );
void rethread( Lstat, Lthread, int32 );

#else /* K&R proto */

void scripti_msgi();
void scripti_badmsg();
void scripti_reject();
void scripti_idisc();
void scripti_done();
void scripti_resel();
void scripti_sigp();
void dma_buserror();
void dma_watchdog();
void dma_illscript();
void siop_par();
void siop_rrst();
void siop_udc();
void siop_sge();
void siop_sto();
void siop_pm();
void rethread();
#endif

#if defined (_OS9000)
error_code irqsvc ( Datmod );
void coherent(void);
#elif defined (_OSK)
#if defined (__STDC__) || defined (_ANSI_EXT) 
error_code irq (Lstat);
int irqsvc( void );
#else /* K&R proto */
error_code irq ();
int irqsvc();
#endif
#endif /* _OS9000 or _OSK case */

/* irqsup.c proto */

#if defined (__STDC__) || defined (_ANSI_EXT)
void reschedule( Lstat );
error_code setasyn( Lstat, Lthread, int32, int32 );
void bomblog( Lstat, Lthread, int32 );

#else /* K&R proto */

void reschedule();
error_code setasyn();
void bomblog();
#endif


/* main.c proto */


#if defined (__STDC__) || defined (_ANSI_EXT)
int ncrchip ( void );
#else
int ncrchip ();
#endif

/* misc.c proto */

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code grabsem( u_int8 * , u_int32 *);
void initthreads( Lstat );
void enqueue( Lstat, Lthread );
void clearmem( char *, int32 );
int8 hexchar( int8 );
void lxtoa(char *, int32 );
void resetdblog( Lstat );

#ifdef STATS
void dblog( Lstat, int32 );
#endif

#ifndef CBOOT
void strcpy( char *, char *);
#endif

#else /* K&R proto */

error_code grabsem();
void initthreads();
void enqueue();
int clearmem();
int8 hexchar();
void lxtoa();
void resetdblog();

#ifdef STATS
void dblog();
#endif

#ifndef CBOOT
void strcpy();
#endif

#endif 






