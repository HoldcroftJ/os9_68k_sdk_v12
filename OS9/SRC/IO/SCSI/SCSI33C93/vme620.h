/*
 * definitions for scsi interface for VME620
 *
 * Copyright 1988, 1989, 1990 by Microware Systems Corporation
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
 * ed.  date   reason											who
 * -- -------  ------------------------------------------------ ---
 * 00 88/12/20 created.											rwb
 * 01 89/02/27 updated for use with merged 33c93 source code	jal
 * 02 90/08/30 added MAXDMA for checking dma byte-counts.       wwb
 *             ---- OS-9/68K V2.4 released ----
 *             ---- OS-9/68K V3.0 released ----
 *             ---- OS-9/68K V3.1 released ----
 */


/* dma definitions for the VME620 */

#define	DMA_DMAOFF		0x44		/* disable dma chip */
#define	DMA_DMAON		0x40		/* enable dma chip */
#define	DMA_READ		0x44		/* transfer device to memory */
#define DMA_WRITE		0x48		/* transfer memory to device */

#define	DMAINIT			1			/* initialize dma chip */

#define	SCSI_CHAN		3			/* scsi uses channel 3 */

/* irq enable definitions */

#define	VMEIRQ_DIS		0			/* disable vme irqs */
#define	VMEIRQ_EN		0x4			/* enable vme irqs */

/* maximum transferable byte-count by the dma device */
#define MAXDMA			(1<<16)		/* can handle 2^16 - 1 */

