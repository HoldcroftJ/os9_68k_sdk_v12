/*
 * Definitions for non-volatile ram in the on-board 48t02 chip
 */

#define NVR_SYNCCODE	0x6E0DE51C		/* sync code for nv_ram validation	*/
#define MENUSWITCH		0x02	/* menu selection switch	*/
#define BOOTSWITCH		0x04	/* boot selection switch	*/
#define DEBUGSWITCH		0x08	/* debugger enable switch	*/
#define ROMSWITCH   	0x10    /* boot from rom (autoselect) */
#define AUTOSWITCH		0x20	/* autoboot selection switch			*/
#define IPCSWITCH		0x80	/* enable ipc register */

typedef struct boentry {
	u_char		booterno;
	u_char		scsi_id;
	u_char		scsi_lun;
	u_char		dummy;
} bo_entry, *Bo_entry;

/*
 * Non-volatile RAM structure
 */
typedef struct nvram {
	u_int32		nvr_sync;		/* validation sync code ("0x6E0DE51C")	*/
	u_int32		nvr_mailbox1;	/* space to pass "mailbox" info			*/
	u_int16		nvr_aconf;		/* a port baud rate  & parity 			*/
	u_int16		nvr_bconf;		/* b port baud rate & parity			*/
	u_char		nvr_reserv;		/* reseerved							*/
	u_char		nvr_switches;	/* Microware "standard" boot "switches"	*/
	u_char		nvr_cpuid;		/* cpu id number 						*/
	u_char		nvr_vmeirq;		/* vme irqs to be enabled				*/
	u_int32		nvr_vmecr1;		/* vme control reg 1					*/
	u_int32		nvr_vmecr2;		/* vme control reg 2					*/
	bo_entry	nvr_booter[8];	/* boot order array						*/
	u_int32		nvr_svrmem;		/* backplane server memory 				*/
	u_int32		nvr_svrmbox;	/* backplane server mailbox				*/
} nv_ram, *Nv_ram;	

/*
 * This is the number of times needed to press the reset button
 * in order to trigger the boot system reinitialization code.
 */
#define MAXRESETS		5

