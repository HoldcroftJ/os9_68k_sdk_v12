/*
 * definitions for scsi interface DMA device for VME147
 *
 * Copyright 1988 by Microware Systems Corporation
 * Reproduced Under License
 *
 * This source code is the proprietary confidential property of
 * Microware Systems Corporation, and is provided to licensee
 * solely for documentation and educational purposes. Reproduction,
 * publication, or distribution in any form to any party other than
 * the licensee is strictly prohibited.
 *
 * history:
 *
 * ed.  date   reason.                                         who.
 * -- -------  ----------------------------------------------  ---
 * 00 88/11/10 split from "wd33c93.h".                         wwb
 * 01 88/11/14 added VME147 dma irq definitions.               wwb
 * 02 88/12/08 made into local file: dmapcc.h.                 wwb
 *             (this file is now ALWAYS VME147 specific).
 * 03 88/12/20 added DMAIRQ for dma device causing irqs.       wwb
 *             ---- OS-9/68K V2.4 released ----      
 *             ---- OS-9/68K V3.0 released ----      
 *             ---- OS-9/68K V3.1 released ----      
 */


/* hardware structure for the dma controller on the MVME147 */
 
typedef struct dmahardware {
	u_char		*tabladdr;		/* table address register */
	u_char		*dataaddr;		/* data address register */
	unsigned	link	: 1,	/* link flag */
				reserv1	: 4,	/* reserved (always 0) */
				dfc		: 3,	/* data function codes */
				datacount: 24;	/* data count register */
	u_int		datahold;		/* data holding register */
	u_int		reserv2[4];		/* padding */
	u_char		dmaicr,			/* dma interrupt control register */
				dmactrl,		/* dma control and status register */
				reserv3,		/* padding */
				dmastat,		/* dma status register */
				reserv4,		/* padding */
				dmatabl;		/* table function code register */
} *Dmahardware;

/* VME147 dma definitions */

#define	DMA_ADDR			0xfffe1000	/* base address of dma hardware */

/* dma interrupt control register */

#define	DMA_IRQ				(1<<7)		/* dma irq pending */
#define	DMA_IRQ_ENAB		(1<<3)		/* enable dma interrupts */
#define	DMA_IRQ_MASK		0x7			/* irq level mask */

#define	DMA_VECT			70			/* vector #70 */

/*
 * this definition tells whether the dma device should be
 * added to the system irq polling table.
 */
#define	DMAIRQ				1			/* define it *

/* dma control/status register */

#define	DMA_DONE			(1<<7)		/* dma completed */
#define	DMA_ERR_8BIT		(1<<6)		/* port is 8-bit */
#define	DMA_ERR_TBL			(1<<5)		/* table memory not 32-bit */
#define	DMA_ERR_BUS			(1<<4)		/* dma transfer bus error */
#define	DMA_ERR_TBLBE		(1<<3)		/* bus error in table access */
#define	DMA_MS				(1<<2)		/* transfer memory to scsi */
#define	DMA_TW				(1<<1)		/* enable table walk */
#define	DMA_ENAB			(1<<0)		/* enable dma */

#define	DMA_READ			DMA_ENAB	/* dma read mask */
#define	DMA_WRITE			DMA_ENAB|DMA_MS		/* dma write mask */
#define DMA_ERRMASK			0x78		/* error status mask */

/* data address register */

#define	DMA_DFC				0x05		/* supervisor data accesses */

/*
 * scsi port interrupt register on the MVME147:
 * bit 3 - enables interrupts from the wd33c93.
 * bits 0, 1 & 2 supply the interrupt level
 */
#define	SCSI_ICR			0xfffe102a	/* address of irq control reg */
#define SCSI_ICR_ENAB		0x08		/* irq enable flag */
#define	SCSI_ICR_MASK		0x07		/* irq level mask */

