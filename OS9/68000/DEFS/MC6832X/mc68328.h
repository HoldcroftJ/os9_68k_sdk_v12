/*****************************************************************************
**  MC68328 (Dragonball) Integrated Processor Register Map                  **
******************************************************************************
** Copyright 1995-1997 by Microware Systems Corporation                     **
** Reproduced Under License                                                 **
**                                                                          **
** This source code is the proprietary confidential property of             **
** Microware Systems Corporation, and is provided to licensee               **
** solely for documentation and educational purposes. Reproduction,         **
** publication, or distribution in any form to any party other than         **
** the licensee is strictly prohibited.                                     **
******************************************************************************
** Edition History:                                                         **
** #   Date     Comments                                          By        **
** --- -------- ------------------------------------------------- --------- **
**   1 10/27/95 creation                                          rws       **
**   2 08/01/96 General cleanup.                                  ats       **
**              --- OS-9/68K MC68328 Support Beta2 Release ---              **
**   3 12/09/96 Fixed watchdog-timer register definitions         bat       **
**   4 01/20/97 added options for setting UART                    pbt/rwa   **
**   5 06/25/97 added MTMR2 define for interrupt control block regs  nz     **
*****************************************************************************/

#ifndef _MC68328_H_
#define  _MC68328_H_

#include <types.h>
/*
 *           MC68328 (Dragonball) Integrated Processor
 *                          Register Map
 */

/*
 * Bit Operations
 */
#define Bit_Mask_Set(reg, mask)         (*(reg) |=  (mask))
#define Bit_Mask_Clear(reg, mask)       (*(reg) &= ~(mask))
#define Bit_Mask_Test(reg, mask)        (reg & (mask))


/*
 * System Integration Module Registers
 * (minus the Interrupt Controller Block Registers)
 */
typedef struct m68328_sim_regs_struct {

     volatile u_int8 scr;     /* System Control Register */

#define SCR_BETO  0x80     /* bus-error timeout status bit */
#define SCR_WPV   0x40     /* write-protect violation bit */
#define SCR_PRV   0x20     /* privelege violation bit */
#define SCR_BETEN 0x10     /* bus-error timeout enable bit */
#define SCR_SO       0x08     /* supervisor only control bit */
#define SCR_DMAP  0x04     /* double map control bit */
#define SCR_WDTH8 0x01     /* 8-bit data bus width select bit */

     u_int8 resv1[255];

     /* Group-Base Address Registers */
     volatile u_int16 grpbasea,
                      grpbaseb,
                      grpbasec,
                      grpbased,

#define b_GBADDRESS 4     /* group base address bits 31:20 start at bit 4 */
#define b_GBVALID   0     /* valid bit is bit 0 */

     /* Group-Base Address Mask Registers */
                      grpmaska,
                      grpmaskb,
                      grpmaskc,
                      grpmaskd;

#define b_GMADDRESS 4     /* group mask address bits 31:20 start at bit 4 */

     /* Chip-Select Registers */
     volatile u_int32 csa0,
                      csa1,
                      csa2,
                      csa3,

                      csb0,
                      csb1,
                      csb2,
                      csb3,

                      csc0,
                      csc1,
                      csc2,
                      csc3,

                      csd0,
                      csd1,
                      csd2,
                      csd3;

#define b_AB_AC 24     /* A and B address compare bits 23:16 start at bit 24 */
#define b_AB_AM 8     /* A and B address mask bits 23:16 start at bit 8 */
#define b_CD_AC 20     /* C and D address compare bits 23:12 start at bit 20 */
#define b_CD_AM 4     /* C and D address mask bits 23:12 start at bit 4 */
#define CS_BSW  0x00010000     /* data bus width control bit */
#define CS_RO     0x00000008     /* read-only control bit */
#define b_WAIT     0     /* wait state bits start at bit 0 (3 bits) */

     u_int8 resv2[176];

} m68328_sim_regs;

/*
 * Phase-Locked Loop and Power Control Registers
 */
typedef struct m68328_pll_regs_struct {

     volatile u_int16 pllcr,     /* PLL Control Register */
                      pllfsr;     /* PLL Frequency Select Register */

     u_int8 resv1[3];

     volatile u_int8 pctlr;          /* Power Control Register */

     u_int8 resv2[248];

} m68328_pll_regs;

/*
 * Interrupt Control Block Registers
 */
typedef struct m68328_irq_regs_struct {

     volatile u_int8 ivr;          /* Interrupt Vector Register */

     u_int8 resv1;

     volatile u_int16 icr;          /* Interrupt Control Register */
     volatile u_int32 imr,          /* Interrupt Mask Register */
                      iwr,          /* Interrupt Wakeup-Enable Register */
                      isr,          /* Interrupt Status Register */
                      ipr;          /* Interrupt Pending Register */
#define MINT0to7                (u_int32)0x0000FF00
#define MTMR2			(u_int32)0x00000002
#define MUART			(u_int32)0x00000004
#define MKB                     (u_int32)0x00000040
#define MINT0                   (u_int32)0x00000100
#define MINT1                   (u_int32)0x00000200
#define MINT2                   (u_int32)0x00000400
#define MINT3                   (u_int32)0x00000800
#define MINT4                   (u_int32)0x00001000
#define MINT5                   (u_int32)0x00002000
#define MINT6                   (u_int32)0x00004000
#define MINT7                   (u_int32)0x00008000

#define MaskInt328(reg, mask)     Bit_Mask_Set(reg, mask)
#define UnmaskInt328(reg, mask)   Bit_Mask_Clear(reg, mask)

     u_int8 resv2[236];

} m68328_irq_regs;

/*
 * Parallel Ports Registers map
 *     NOTE: not all ports have all registers
 */
typedef struct m68328_pio_struct {

     volatile u_int8                 dir;         /* direction register */
#define Direction_Input_All          (u_int8)0x00
#define Direction_Output_All         (u_int8)0xFF
#define Direction_Input(reg, mask)   Bit_Mask_Clear(&((reg)->dir),mask)
#define Direction_Output(reg, mask)  Bit_Mask_Set(&((reg)->dir),mask)

     volatile u_int8                 data;        /* data register */
     volatile u_int8                 puen;        /* pull-up resistor enable */
#define Pullup_Disabled_All          (u_int8)0x00
#define Pullup_Enabled_All           (u_int8)0xFF
#define Pullup_Disable(reg, mask)    Bit_Mask_Clear(&((reg)->puen),mask)
#define Pullup_Enable(reg, mask)     Bit_Mask_Set(&((reg)->puen),mask)

     volatile u_int8                 sel;         /* select register */
#define Select_Address_All           (u_int8)0x00
#define Select_GPIO_All              (u_int8)0xFF
#define Select_Address(reg, mask)    Bit_Mask_Clear(&((reg)->sel),mask)
#define Select_GPIO(reg, mask)       Bit_Mask_Set(&((reg)->sel),mask)

     volatile u_int8                 pol;         /* polarity register */
#define Polarity_Rising_All          (u_int8)0x00
#define Polarity_Falling_All         (u_int8)0xFF
#define Polarity_Rising(reg, mask)   Bit_Mask_Clear(&((reg)->pol),mask)
#define Polarity_Falling(reg, mask)  Bit_Mask_Set(&((reg)->pol),mask)
#define Polarity_High_All            Polarity_Rising_All
#define Polarity_Low_All             Polarity_Falling_All
#define Polarity_High(reg, bit)      Polarity_Rising(reg, bit)
#define Polarity_Low(reg, bit)       Polarity_Falling(reg, bit)

     volatile u_int8                 irqen;       /* interrupt enable */
#define IRQ_Disable_All              (u_int8)0x00
#define IRQ_Enable_All               (u_int8)0xFF
#define IRQ_Disable(reg, mask)       Bit_Mask_Clear(&((reg)->irqen),mask)
#define IRQ_Enable(reg, mask)        Bit_Mask_Set(&((reg)->irqen),mask)

     volatile u_int8                 resv;
     volatile u_int8                 irqedge;    /* IRQ edge register */
#define Level_Sense_All              (u_int8)0x00
#define Edge_Sense_All               (u_int8)0xFF
#define Level_Sense(reg, mask)       Bit_Mask_Clear(&((reg)->irqedge),mask)
#define Edge_Sense(reg, mask)        Bit_Mask_Set(&((reg)->irqedge),mask)
} m68328_pio;

/*
 * Parallel Ports Registers (10 Ports)
 */
typedef struct m68328_pio_regs_struct {

     m68328_pio     porta,
                portb,
                portc,
                portd,
                porte,
                portf,
                portg,
#define PORT_G_UART			0xFC		/* enables UART TXD and RXD */
                portj,
                portk,
                portm;

     u_int8 resv[176];

} m68328_pio_regs;

/*
 * Pulse-Width Modulator Registers
 */
typedef struct m68328_pwm_regs_struct {

     volatile u_int16 pwmc,          /* PWM Control Register */
#define PWMC_PWMIRQ			0x8000		/* period-compare interrupt */
#define PWMC_PWMIRQ_ROLL	0x8000		/* pwm period rolled over */
#define PWMC_PWMIRQ_NOROLL	0x0000		/* no pwm period rollover */
#define PWMC_IRQEN			0x4000		/* pwm interrupt enable */
#define PWMC_IRQEN_ON		0x4000		/* pwm interrupt enabled */
#define PWMC_IRQEN_OFF		0x0000		/* pwm interrupt disabled */
#define PWMC_LOAD			0x0100		/* force new period (force load) */
#define PWMC_PIN			0x0080		/* current pwm output pin status */
#define PWMC_PIN_LO			0x0000		/* pwm output is low */
#define PWMC_PIN_HI			0x0080		/* pwm output is high */
#define PWMC_POL			0x0020		/* pwm output pin polarity */
#define PWMC_POL_NORMAL		0x0000		/* normal polarity */
#define PWMC_POL_INVERT		0x0020		/* inverted polarity */
#define PWMC_PWMEN			0x0010		/* pwm enable */
#define PWMC_PWMEN_ON		0x0010		/* pwm enabled */
#define PWMC_PWMEN_OFF		0x0000		/* pwm disabled */
#define PWMC_CLKSEL			0x0007		/* clock divider chain output select */
#define PWMC_CLKSEL_2		0x0000		/* divide clock by 2 */
#define PWMC_CLKSEL_4		0x0001		/* divide clock by 4 */
#define PWMC_CLKSEL_8		0x0002		/* divide clock by 8 */
#define PWMC_CLKSEL_16		0x0003		/* divide clock by 16 */
#define PWMC_CLKSEL_32		0x0004		/* divide clock by 32 */
#define PWMC_CLKSEL_64		0x0005		/* divide clock by 64 */
#define PWMC_CLKSEL_128		0x0006		/* divide clock by 128 */
#define PWMC_CLKSEL_256		0x0007		/* divide clock by 256 */
                      pwmp,          /* PWM Period Register */
                      pwmw,          /* PWM Width Register */
                      pwmcnt;     /* PWM Counter Register */

     u_int8 resv[248];

} m68328_pwm_regs;

/*
 * Timer Module Register definitions
 */
typedef struct m68328_timer_struct {

     volatile u_int16 ctl,     /* control register */
#define TIMER_CTL_CEDGE		0x00C0		/* capture edge bits */
#define TIMER_CTL_CEDGE_OFF	0x0000		/* disable capture edge int */
#define TIMER_CTL_CEDGE_RIS	0x0040		/* int on rising edge capture */
#define TIMER_CTL_CEDGE_FAL	0x0080		/* int on falling edge capture */
#define TIMER_CTL_CEDGE_EDG	0x00C0		/* int on any edge capture */
#define TIMER_CTL_FRR		0x0100		/* free-run / restart bit */
#define TIMER_CTL_FRR_RSTRT	0x0000		/* restart mode */
#define TIMER_CTL_FRR_FREE	0x0100		/* free-run mode */
#define TIMER_CTL_OM		0x0020		/* output mode */
#define TIMER_CTL_OM_LOW	0x0000		/* active-low pulse output */
#define TIMER_CTL_OM_TGLE	0x0020		/* toggle output */
#define TIMER_CTL_IRQEN		0x0010		/* reference event interrupt-enable */
#define TIMER_CTL_IRQEN_ON	0x0010		/* irq disabled */
#define TIMER_CTL_IRQEN_OFF	0x0000		/* irq enabled */
#define TIMER_CTL_CKSRC		0x000E		/* clock source */
#define TIMER_CTL_CKSRC_OFF	0x0000		/* stop count (clock disabled) */
#define TIMER_CTL_CKSRC_SYS	0x0002		/* system clock to timer */
#define TIMER_CTL_CKSRC_S16	0x0004		/* system clock / 16 to timer */
#define TIMER_CTL_CKSRC_TIN	0x0006		/* TIN pin is clock source */
#define TIMER_CTL_CkSRC_32K	0x0008		/* 32kHz clock source */
#define TIMER_CTL_TEN		0x0001		/* timer enable */
#define TIMER_CTL_TEN_ON	0x0001		/* timer enabled */
#define TIMER_CTL_TEN_OFF	0x0000		/* timer disabled */
                      prer,     /* prescale register */
                      cmp,     /* compare register */
                      cr,     /* capture register */
                      cn,     /* counter register */
                      stat;     /* status register */
#define TIMER_STAT_CAPT          0x0002     /* capture event occured */
#define TIMER_STAT_COMP          0x0001     /* compare event occured */

} m68328_timer;

/*
 * Timer Module Registers (2 Timers)
 */
typedef struct m68328_tmr_regs_struct {

     m68328_timer timer1,
                 timer2;

} m68328_tmr_regs;

/*
 * Software Watchdog Timer Registers
 */
typedef struct m68328_wdog_regs_struct {

     volatile u_int16 wcntrl,	/* WD Control/Status Register */
					  wcmp,		/* WD Compare Register */
                      wcntr;	/* WD Counter Register */

     u_int8 resv[226];

} m68328_wdog_regs;

/*
 * Slave Serial Peripheral Interface Registers
 */
typedef struct m68328_spis_regs_struct {

     volatile u_int16 spisr;          /* SPI Slave Register */

     u_int8 resv[254];

} m68328_spis_regs;

/*
 * Master Serial Peripheral Interface Registers
 */
typedef struct m68328_spim_regs_struct {

     volatile u_int16 data,     /* SPIM Data Register */
                      cont;     /* SPIM Control/Status Register */

/* Baud rate control, 3 bits */
#define b_DATA_RATE 13
/* shift following values up to b_DATA_RATE in cont to select baud rate */
#define DIV_SYSCLK_4   0     /* baud = SYSCLK divided by 4 */
#define DIV_SYSCLK_8   1     /* baud = SYSCLK divided by 8 */
#define DIV_SYSCLK_16  2     /* baud = SYSCLK divided by 16 */
#define DIV_SYSCLK_32  3     /* baud = SYSCLK divided by 32 */
#define DIV_SYSCLK_64  4     /* baud = SYSCLK divided by 64 */
#define DIV_SYSCLK_128 5     /* baud = SYSCLK divided by 128 */
#define DIV_SYSCLK_256 6     /* baud = SYSCLK divided by 256 */
#define DIV_SYSCLK_512 7     /* baud = SYSCLK divided by 512 */

#define SPIMEN     0x0200     /* OR into cont to enable SPIM */
#define SPIM_XCH   0x0100     /* OR into cont to initbiate exchange */
#define SPIM_IRQST 0x0080     /* SPIM IRQ status bit - cleared by writing 1 to bit */
#define SPIM_IRQEN 0x0040     /* OR into cont to enable SPIM IRQ */
#define SPIM_PHA   0x0020     /* OR into cont to shift advance to opposite phase*/
#define SPIM_POL   0x0010     /* OR into cont to invert polarity */

/* transfer length control, 4 bits starting at bit 0 in cont:
 * 0000 = 1 bit transfer ... 1111 = 16 bit transfer
 */
#define b_SPIM_BITCNT 0

     u_int8 resv[252];

} m68328_spim_regs;

/*
 * UART Registers
 */
typedef struct m68328_uart_regs_struct {

     volatile u_int16 ustcnt,		/* Status/Control Register */
#define UART_ENABLE		0x8000		/* Enable UART */
#define RX_ENABLE		0x4000		/* Enable Receiver */
#define	TX_ENABLE		0x2000		/* Enable Transmitter */
#define RX_CLK_CONT		0x1000		/* Receiver Clock Control */
#define	PARITY_EN		0x0800		/* Parity Enable */
#define ODD_EVEN		0x0400		/* Odd/Even Parity */
#define STOP_BITS		0x0200		/* Stop Bits */
#define CHAR_LEN		0x0100		/* 8/7 Bit Character Length */
#define GPIO_DELTA_EN	0x0080		/* Enable GPIO delta interrupt */
#define CTS_DELTA_EN	0x0040		/* Enable CTS delta interrupt */
#define RX_FULL_EN 		0x0020		/* Enable interrupt when rx FIFO full */
#define RX_HALF_EN		0x0010		/* Enable interrupt when rx FIFO > 1/2 full */
#define RX_READY_EN		0x0008		/* Enable interrupt when rx FIFO >= 1 byte */
#define TX_EMPTY_EN 	0x0004		/* Enable interrupt when tx FIFO is empty */
#define TX_HALF_EN		0x0002		/* Enable interrupt when tx FIFO < 1/2 full */
#define TX_AVAIL_EN 	0x0001		/* Enable interrupt when tx FIFO has slot avail */

                      ubaud,		/* Baud Control Register */
/* The baud rate control register sets the baud rate according to the following
** relationship:
**			16x clock = clock source / [(65-PRESCALAR) * DIVIDE]
**			baud rate = {clock source / [(65-PRESCALAR)* DIVIDE]}/16
**
** To obtain the following baud rates, the settings illustrated below are used:
**
**			baud rate		DIVIDE		PRESCALAR
**			  (Hz)			(dec)		(dec)
**			115200			DIVIDE_1	PRESC_56
**			 57600          DIVIDE_2    PRESC_56
**			 38400			DIVIDE_1	PRESC_38
**			 19200			DIVIDE_2	PRESC_38
**			  9600			DIVIDE_4	PRESC_38
**			  4800			DIVIDE_8	PRESC_38
**			  2400			DIVIDE_16	PRESC_38
**			  1200			DIVIDE_32	PRESC_38
**			   600			DIVIDE_64	PRESC_38
**			   300			DIVIDE_128	PRESC_38
*/

#define GPIO_DELTA		0x8000		/* GPIO change occurred */
#define GPIO			0x4000		/* Status of GPIO pin */
#define GPIO_DIR		0x2000		/* GPIO pin direction, input/output */
#define GPIO_SRC		0x1000		/* GPIO pin source */
#define BAUD_SRC		0x0800		/* Baud rate generator clock source */
#define DIVIDE_128		0x0700		/* Divider=128 in Baud rate generator */
#define DIVIDE_64		0x0600		/* Divider=064 in Baud rate generator */
#define DIVIDE_32		0x0500		/* Divider=032 in Baud rate generator */
#define	DIVIDE_16		0x0400		/* Divider=016 in Baud rate generator */
#define DIVIDE_8		0x0300		/* Divider=008 in Baud rate generator */
#define DIVIDE_4		0x0200		/* Divider=004 in Baud rate generator */
#define DIVIDE_2		0x0100		/* Divider=002 in Baud rate generator */
#define DIVIDE_1		0x0000		/* Divider=001 in Baud rate generator */
#define PRESC_38		0x0026		/* PRESCALAR=38 in Baud rate generator */
#define PRESC_56		0x0038		/* PRESCALAR=56 in Baud rate generator */

/* Baud Rate Control Register Settings for the given Baud Rate */
#define BAUD_1152	DIVIDE_1 | PRESC_56		/* 115.2 kbps */
#define BAUD_576	DIVIDE_2 | PRESC_56		/* 57.6 kbps */
#define BAUD_384	DIVIDE_1 | PRESC_38		/* 38.4 kbps */
#define BAUD_192	DIVIDE_2 | PRESC_38		/* 19.2 kbps */
#define BAUD_96		DIVIDE_4 | PRESC_38		/* 9.6 kbps */
#define BAUD_48		DIVIDE_8 | PRESC_38		/* 4.8 kbps */
#define BAUD_24		DIVIDE_16 | PRESC_38	/* 2.4 kbps */
#define BAUD_12		DIVIDE_32 | PRESC_38	/* 1.2 kbps */

                      urx,			/* Receive Register */
/* The following apply to the rx status byte */
#define RX_FIFO_FULL	0x8000		/* Rx FIFO is full */
#define RX_FIFO_HALF	0x4000		/* Rx FIFO > 1/2 full */
#define DATA_READY		0x2000		/* Rx FIFO >= 1 byte */
#define OVRUN			0x0800		/* Rx FIFO overrun */
#define FRAME_ERROR		0x0400		/* Frame error detected */
#define BREAK			0x0200		/* BREAK character detected */
#define PARITY_ERROR	0x0100		/* Parity error detected */
                      
                      utx,			/* Transmit Register */
/* The following apply to the tx status byte */
#define TX_FIFO_EMPTY	0x8000		/* Tx FIFO is empty */
#define TX_FIFO_HALF	0x4000		/* Tx FIFO < 1/2 full */
#define	TX_AVAIL		0x2000		/* Tx FIFO has slot available */
#define SEND_BREAK		0x1000		/* Send Break, continuous 0's */
#define	IGNORE_CTS		0x0800		/* Ignore CTS bit (ignore flow control) */
#define CTS_STATUS		0x0200		/* CTS Pin status */
#define CTS_DELTA		0x0100		/* CTS pin changed state */

                      umisc;		/* Miscellaneous Register */
#define CLK_SRC			0x4000		/* Clock Source */
#define FORCE_PERR		0x2000		/* Force parity error */
#define LOOP_EN			0x1000		/* Enable internal loopback */
#define RTS_CONT		0x0080		/* RTS Control */
#define RTS_PIN			0x0040		/* RTS pin */
#define IRDA_EN			0x0020		/* Enable IrDA interface */
#define LOOP_IR_EN		0x0010		/* Enable IR loopback */

     u_int8 resv[246];

} m68328_uart_regs;

/*
 * LCD Controller Module Registers
 */
typedef struct m68328_lcdc_regs_struct {

     volatile void* lssa;          /* Screen Starting Address Register */

     u_int8 resv1;

     volatile u_int8 lvpw;          /* Virtual Page Width Register */

     u_int8 resv2[2];

     volatile u_int16 lxmax,          /* Screen Width Register */
                      lymax;          /* Screen Height Register */

     u_int8 resv3[12];

     volatile u_int16     lcxp;            /* Cursor X Position */
#define CURSOR_DISABLED   0x0000       /* cursor disabled */
#define CURSOR_BLACK      0x4000       /* black, full-density cursor */
#define CURSOR_REVERSED   0x8000       /* reversed video cursor */
#define CURSOR_XPOS(x)    ((x)&0x3FFF) /* X position of Cursor */

     volatile u_int16 lcyp;            /* Cursor Y Position */
#define CURSOR_YPOS(y)    (y)          /* Y position of Cursor */

     volatile u_int16 lcwch;           /* Cursor Width & Height Register */
#define CURSOR_WIDTH(w)   ((w) << 8)
#define CURSOR_HEIGHT(h)  ((h) & 0xFF)

     u_int8 resv4;

     volatile u_int8      lblkc;       /* Blink Control Register */
#define BLKC_DISABLE      0x00         /* non-blinking cursor */
#define BLKC_ENABLE       0x80         /* blinking cursor */
#define BLKC_DIVISOR(d)   ((d)&0x7F)   /* Blink Rate */

     volatile u_int8      lpicf;       /* Panel Interface Config Register */
#define PICF_PBSIZ1       0x00         /* 1-bit LCD panel bus size */
#define PICF_PBSIZ2       0x02         /* 2-bit LCD panel bus size */
#define PICF_PBSIZ4       0x04         /* 4-bit LCD panel bus size */
#define PICF_GSON         0x01         /* gray-scale on */
#define PICF_GSOFF        0x00         /* gray-scale off */

     volatile u_int8      lpolcf;      /* Polarity Config Register */
#define POLCF_SCKPOLNEG   0x00         /* negative-edge shift-clock polarity */
#define POLCF_SCKPOLPOS   0x08         /* positive-edge shift-clock polarity */
#define POLCF_FLMHI       0x00         /* high first-line marker polarity */
#define POLCF_FLMLO       0x04         /* low first-line marker polarity */
#define POLCF_LPHI        0x00         /* high line-pulse polarity */
#define POLCF_LPHO        0x02         /* low  line-pulse polarity */
#define POLCF_PIXHI       0x00         /* high pixel polarity */
#define POLCF_PIXLO       0x01         /* low pixel polarity */

     volatile u_int8      resv5,
                          lacdrc,      /* ACD (M) Rate Control Register */
                          resv6,
                          lpxcd,       /* Pixel Clock Divider Register */
                          resv7,
                          lckcon;      /* Clocking Control Register */
#define CKCON_LCDCON      0x80          /* LCD controller on */
#define CKCON_LCDCOFF     0x00         /* LCD controller off */
#define CKCON_DMA8        0x00         /* 8 word DMA burst */
#define CKCON_DMA16       0x40         /* 16 word DMA burst */
#define CKCON_WS1         0x00         /* 1-clock DMA transfer */
#define CKCON_WS2         0x10         /* 2-clock DMA transfer */
#define CKCON_WS3         0x20         /* 3-clock DMA transfer */
#define CKCON_WS4         0x30         /* 4-clock DMA transfer */
#define CKCON_DWIDTH8     0x02         /* 8-bit memory-data bus width */
#define CKCON_DWIDTH16    0x00         /* 16-bit memory-data bus width */
#define CKCON_PCDSSYS     0x00         /* SYS CLK pixel clock divider */
#define CKCON_PCDSPIX     0x01         /* PIX CLK pixel clock divider */

     volatile u_int8      resv8,
                          llbar,       /* Last Buffer Address Register */
                          resv9,
                          lotcr,       /* Octet Terminal Count Register */
                          resv10,
                          lposr;       /* Panning Offset Register */
#define LPOSR_BOS_FIRST   0x00         /* Retrieve First Byte for display */
#define LPOSR_BOS_SECOND  0x08         /* Retrieve Second Byte for display */
#define LPOSR_POS(c)      ((c) & 0x07) /* Pixel Offset Code */

     volatile u_int8      resv11[3],
                          lfrcm;       /* Frame Rate Control Modulation */
#define LFRCM_XMOD(c)     ((c) << 4)   /* X Mod Value */
#define LFRCM_YMOD(c)     ((c) & 0x0F) /* Y Mod Value */

     volatile u_int16     lgpmr;       /* Gray Pallete Mapping Register */

     u_int8               resv12[204];

} m68328_lcdc_regs;

/*
 * Real-Time Clock Module Registers
 */
typedef struct m68328_rtc_regs_struct {

     volatile u_int32 hmsr,          /* Hours Minutes Seconds Register */
                      alarm;          /* Alarm Register */

     u_int8 resv1[5];

     volatile u_int8 ctl,          /* Control Register */
                     resv2,
                     isr,          /* Interrupt Status Register */
                     resv3,
                     ienr,          /* Interrupt Enable Register */
                     resv4,
                     stpwch;          /* Stopwatch Minutes */

} m68328_rtc_regs;

/*
 * All of the MC68328 Registers
 */
typedef struct m68328_regs_struct {

     m68328_sim_regs  sim_regs;
     m68328_pll_regs  pll_regs;
     m68328_irq_regs  irq_regs;
     m68328_pio_regs  pio_regs;
     m68328_pwm_regs  pwm_regs;
     m68328_tmr_regs  tmr_regs;
     m68328_wdog_regs wdog_regs;
     m68328_spis_regs spis_regs;
     m68328_spim_regs spim_regs;
     m68328_uart_regs uart_regs;
     m68328_lcdc_regs lcdc_regs;
     m68328_rtc_regs  rtc_regs;

} m68328_regs, *M68328_regs;


#endif
