**************************************************
* Copyright 1993, 1995 Microware Systems Corporation.
* All Rights Reserved
* Reproduced Under License
*
*   This source code is the proprietary confidential property of
* Microware Systems Corporation, and is provided to the licensee
* for documentation and educational purposes only.  Reproduction,
* publication, or any form of distribution to any party other than
* the licensee is strictly prohibited.

********************************
* History
*    Date      Comments                                      by
* --------- ------------------------------------------------ ---
* 93/08/13  Created.                                         ats
* 95/09/27  Added IP2 registers/values.                      ats
*           ---- OS-9/68K V3.0.2 Release ----
* 01/10/08	Added Petra/IP2 Rev number						 mgh
*

*
* IPIC definitions - MVME162
*

IPIC_ID			equ		$00			Chip ID Register

v_IPIC_ID		equ		$23			Actual IPIC ID value

IPIC_Rev		equ		$01			Chip Revision Register

v_IPIC_Rev		equ		$00			Revision value of IPIC
*
* WARNING: v_IP2_Rev is inaccurate in early IP2 parts!
*   Motorola's suggested workaround is to check for existance
*   of an MC2 chip (second generation mcchip).
v_IP2_Rev		equ		$01			Revision value of IP2
v_PetraIP2_Rev 	equ		$03			Petra Revision value
*

IP_VBR			equ		$03			Vector Base Register (IP2 only)

IP_a_MemBase	equ		$04			IP_a Memory Base Register
IP_b_MemBase	equ		$06			IP_b   "     "     "
IP_c_MemBase	equ		$08			IP_c   "     "     "
IP_d_MemBase	equ		$0a			IP_d   "     "     "

IP_a_MemSize	equ		$0c			IP_a Memory Size Register
IP_b_MemSize	equ		$0d			IP_b   "     "     "
IP_c_MemSize	equ		$0e			IP_c   "     "     "
IP_d_MemSize	equ		$0f			IP_d   "     "     "

c_IP_64K		equ		%00000000	 64KB Memory Size
c_IP_128K		equ		%00000001	128KB   "     "
c_IP_256K		equ		%00000011	256KB   "     "
c_IP_512K		equ		%00000111	512KB   "     "
c_IP_1M			equ		%00001111	  1MB   "     "
c_IP_2M			equ		%00011111	  2MB   "     "
c_IP_4M			equ		%00111111	  4MB   "     "
c_IP_8M			equ		%01111111	  8MB   "     "
c_IP_16M		equ		$11111111	 16MB   "     "	

IP_a_INT0_CTRL	equ		$10			IP_a Interrupt 0 Control Register
IP_a_INT1_CTRL	equ		$11			IP_a     "     1    "       "
IP_b_INT0_CTRL	equ		$12			IP_b     "     0    "       "
IP_b_INT1_CTRL	equ		$13			IP_b     "     1    "       "
IP_c_INT0_CTRL	equ		$14			IP_c     "     0    "       "
IP_c_INT1_CTRL	equ		$15			IP_c     "     1    "       "
IP_d_INT0_CTRL	equ		$16			IP_d     "     0    "       "
IP_d_INT1_CTRL	equ		$17			IP_d     "     1    "       "

b_IP_ICR_IL		equ		0			Interrupt level field bit position
m_IP_ICR_IL		equ		7			    "       "     "    "  mask
b_IP_ICLR		equ		3			Interrupt clear bit (edge sensitive only)
b_IP_IEN		equ		4			Interrupt enable bit
b_IP_INT		equ		5			Interrupt status bit
b_IP_EL			equ		6			Interrupt edge/level sensitivity bit
b_IP_PLTY		equ		7			Interrupt polarity bit

IP_a_GCR		equ		$18			IP_a General Control Register
IP_b_GCR		equ		$19			IP_b    "       "       "
IP_c_GCR		equ		$1a			IP_c    "       "       "
IP_d_GCR		equ		$1b			IP_d    "       "       "

b_IP_MEN		equ		0			Memory enable bit
b_IP_BTD		equ		1			Bus Turn-around Delay bit (IP2 only)
b_IP_WIDTH		equ		2			Memory space bus width field bit position
m_IP_WIDTH		equ		3			   "     "    "    "     "    "  mask
c_IP_32			equ		0			32-bit bus width
c_IP_8			equ		1			 8-bit  "    "
c_IP_16			equ		2			16-bit  "    "

b_IP_RT			equ		4			Recovery timer field bit position
m_IP_RT			equ		3				"      "     "    "  mask
c_IP_0us		equ		0			Zero microseconds
c_IP_2us		equ		1			Two       "
c_IP_4us		equ		2			Four      "
c_IP_8us		equ		3			Eight     "

b_IP_ERR		equ		7			Error* assertion status bit

IP_DMA_ARB		equ		$1e			DMA Arbitration control reg (IP2 only)

b_IP_RROBIN		equ		0			Enable round-robin arbitration
b_IP_PRI		equ		1			Fixed priority arbitration field bit position
m_IP_PRI		equ		3			  "      "         "         "    "  mask
* One DMA channels per IP values:
c_IP_abcd		equ		0
c_IP_bcda		equ		1
c_IP_cdab		equ		2
c_IP_dabc		equ		3
* Two DMA channels per IP a & c values:
c_IP_a0a1c0c1	equ		0
c_IP_a1c0c1a0	equ		1
c_IP_c0c1a0a1	equ		2
c_IP_c1a0a1c0	equ		3

IP_Reset		equ		$1f			Reset Register (for all 4 IPs)

b_IP_RES		equ		0			Reset bit position
b_IP_32mhz		equ		1			32Mhz clock synch enable when set (IP2 only)
b_IP_IP1_RES	equ		1			Petra IP1_Reset mode 0=IP2 mode, 1=compat
*
* The following DMA control registers are present only in the IP2.
*

IP_DMACa		equ		$20			DMACa base register
IP_DMACb		equ		$38			DMACb base register
IP_DMACc		equ		$50			DMACc base register
IP_DMACd		equ		$68			DMACd base register

IP_DMAC_STAT	equ		$00			DMA status register offset

b_IP_DONE		equ		0			DMA done status bit
b_IP_IPTO		equ		1			DMA IP timed out status bit
b_IP_TBL		equ		2			DMA local bus read error status bit
b_IP_CHANI		equ		3			DMA channel processing terminated status bit
b_IP_IPEND		equ		4			DMA transfer ended by IP status bit

b_IP_DLBE		equ		6			DMA transfer ended from local bus TEA status bit

IP_DMAC_ICR		equ		$01			DMA interrupt control reg offset

b_IP_DICR_DIL	equ		0			DMA interrupt level field bit position
m_IP_DICR_DIL	equ		7			 "      "       "     "    "  mask
b_IP_DICR_DICLR equ		3			DMA interrupt clear bit
b_IP_DICR_DIEN  equ		4			DMA interrupt enable bit
b_IP_DICR_DINT	equ		5			DMA interrupt status bit

IP_DMAC_BTO		equ		$02			DMA Bus time-out reg offset

c_IP_BTO_none	equ		0			Bus time-out disabled
c_IP_BTO_16us	equ		1			16 microsecond bus time-out
c_IP_BTO_32us	equ		2			32      "       "   "    "
c_IP_BTO_64us	equ		3			64      "       "   "    "
c_IP_BTO_128us	equ		4			128     "       "   "    "
c_IP_BTO_256us	equ		5			256     "       "   "    "
c_IP_BTO_512us	equ		6			512     "       "   "    "
c_IP_BTO_1024us	equ		7			1024    "       "   "    "

IP_DMAC_CR1		equ		$04			DMA Control register 1 offset

b_IP_DCR1_FLUSH	equ		0			Flush DMA data path
b_IP_DCR1_ch1	equ		1			Select DMA request 1 assoc (DMACb, DMACd only)
b_IP_DCR1_WIDTH	equ		2			Width of IP interface bit field position
m_IP_DCR1_WIDTH	equ		3			  "   "  "      "      "    "   mask
c_IP_DW32		equ		0			32-bits
c_IP_DW8		equ		1			 8-bits
c_IP_DW16		equ		2			16-bits
b_IP_DCR1_ADMA	equ		4			Address mode operation enable
b_IP_DCR1_DTBL	equ		5			Table (chaining) mode enable
b_IP_DCR1_DEN	equ		6			DMA enable
b_IP_DCR1_DHALT	equ		7			Halt DMA in table (chaining) mode

IP_DMAC_CR2		equ		$05			DMA Control register 2 offset

b_IP_DCR2_SC0	equ		0			Snoop control bit (SC0)
b_IP_DCR2_SC1	equ		1			Snoop control bit (SC1)
b_IP_DCR2_TOIP	equ		2			DMA direction bit

b_IP_DCR2_DMAEO	equ		4			Drive DMAEND out
b_IP_DCR2_DMAEI equ		5			Recognize DMAEND in
b_IP_DCR2_DMAEC	equ		6			Force DMAEND by software
b_IP_DCR2_INTE	equ		7			End of DMA packet interrupt enable

IP_DMAC_LBA		equ		$08			DMA Local Bus Address counter reg offset

IP_DMAC_IPA		equ		$0c			DMA IP Address counter reg offset

m_IP_DMAC_IPA	equ		$00ffffff	24-bit mask for IP address counter

IP_DMAC_BC		equ		$10			DMA Byte counter reg offset

m_IP_DMAC_BC	equ		$00ffffff	24-bit mask for Byte counter

IP_DMAC_TBLA	equ		$14			DMA Table address counter reg offset

*
* The following Pacer clock control registers are present only in the IP2.
*

IP_PACER_ICR	equ		$88			Pacer Interrupt control register

b_IP_PICR_IL	equ		0			Pacer interrupt level field bit position
m_IP_PICR_IL	equ		7			  "       "       "     "    "  mask
b_IP_PICR_ICLR	equ		3			Pacer interrupt clear bit
b_IP_PICR_IEN	equ		4			Pacer interrupt enable bit
b_IP_PICR_INT	equ		5			Pacer interrupt status bit
b_IP_PICR_IRE	equ		6			Pacer interrupt edge select bit

IP_PACER_GCR	equ		$89			Pacer General control register

b_IP_PGCR_PS	equ		0			Pacer pre-scale frequency field bit position
m_IP_PGCR_PS	equ		7			  "    "    "       "       "    "  mask
c_IP_PGCR_32M	equ		0			32Mhz output frequency
c_IP_PGCR_16M	equ		1			16Mhz   "       "
c_IP_PGCR_8M	equ		2			 8Mhz   "       "
c_IP_PGCR_4M	equ		3			 4Mhz   "       "
c_IP_PGCR_2M	equ		4			 2Mhz   "       "
c_IP_PGCR_1M	equ		5			 1Mhz   "       "
c_IP_PGCR_500K	equ		6			500Khz  "       "
c_IP_PGCR_250K	equ		7			250Khz  "       "
b_IP_PGCR_CLR	equ		3			Clear pacer clock's internal registers
b_IP_PGCR_EN	equ		4			Enable pacer clock

b_IP_PGCR_PLS	equ		6			Pacer clock is 31ns pulse
b_IP_PGCR_PLTY	equ		7			Pacer clock output polarity

IP_PACER_TIMER	equ		$8a			Pacer Timer register

