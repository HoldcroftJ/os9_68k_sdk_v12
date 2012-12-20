 opt -l
*****************
* Definitions for MCchip device - VME162
*
* Edition History
*
* Ed.   Date.     Reason.                                            Who
* --  --------  ---------------------------------------------------  ---
* 01  93/01/08  Initial edition.                                     ats
* 02  93/03/12  Added FLASH device type to ROR                       ats
* 03  94/03/09  Renamed MCDCR to MCDRCR to resolve conflict with     ats
*               mcecc.d.
* 04  94/04/28  Renamed MCID to MCCHID and MCBCR to MCCHBCR to       ats
*               resolve conflict with memc040.d.
* 05  95/05/11  Replaced 33MHz defs with 32MHz defs.                 ats
* 06  95/09/27  Added revision number for MC2.                       ats
* 07  95/09/29  Added field updates for MC2.                         ats
*               ---- OS-9/68K V3.0.2 Release ----
* 08  98/01/26  Added 30MHz definitions.                             gdw
*


*
*  Conventions used in this file:
*
*   b_XXXX  denotes a bit number definition (note access size quoted)
*   m_XXXX  denotes a bit mask (again, note access size quoted)
*

 ifndef MCchBase
MCchBase	set		$FFF42000
 endc

*
*  Note:  All accesses to the Tick Timer Compare and Counter Registers,
*         and the Prescaler Count Register must be LONG word accesses.
*         All other registers may be Byte, Word or Long word accesses
*         (on appropriate boundaries, of course :-).
*

MCCHID		equ		$00		Identification Register
MCdefID		equ		$84		MCchip ID number

MCREV		equ		$01		Revision Register
MCchipRev	equ		$00		Revision of MCchip
MC2Rev		equ		$01		Revision of MC2
**
* NOTE: Current revision below is eliminated. Both chip revisions exist
*       simultaneously on differing model boards.
*MCcurr		equ		$00		Current revision

MCGCR		equ		$02		General Control Register
b_FAST		equ		0		Set speed of BBRAM control
m_FAST		equ		($1<<b_FAST)
b_MIEN		equ		1		Master Interrupt Enable
m_MIEN		equ		($1<<b_MIEN)
b_PPC		equ		2		PowerPC Interrupt mode (MC2 only)
m_PPC		equ		($1<<b_PPC)
b_SCCIT		equ		3		SCC IACK daisy chain time (MC2 only)
m_SCCIT		equ		($1<<b_SCCIT)

MCIVBR		equ		$03		Interrupt Vector Base Register
MCrstIV		equ		$0F		Value after Reset, before s/w init.
MCdefIV		equ		$50		Default value, established by Mbug.
MCvecT4		equ		$03		Vector offset, Timer 4
MCvecT3		equ		$04		Vector offset, Timer 3
MCvecSCSI	equ		$05		Vector offset, SCSI controller
MCvecELAN	equ		$06		Vector offset, LANC Error
MCvecILAN	equ		$07		Vector offset, LANC IRQ
MCvecT2		equ		$08		Vector offset, Timer 2
MCvecT1		equ		$09		Vector offset, Timer 1
MCvecPE		equ		$0B		Vector offset, DRAM parity error
MCvecABT	equ		$0E		Vector offset, Abort switch

MCT1CMP		equ		$04		Tick Timer 1 Compare Register
MCT1CNT		equ		$08		Tick Timer 1 Counter

MCT2CMP		equ		$0C		Tick Timer 2 Compare Register
MCT2CNT		equ		$10		Tick Timer 2 Counter

MCLPCR		equ		$14		LSB Prescaler Count Register

MCPCAR		equ		$15		Prescalar Clock Adjust Register
MCpca32		equ		$E0		adjust for 32Mhz
MCpca30		equ		$E2		adjust for 30Mhz
MCpca25		equ		$E7		adjust for 25Mhz

MCT2CR		equ		$16		Tick Timer 2 Control Register
MCT1CR		equ		$17		Tick Timer 1 Control Register
b_CEN		equ		0		counter enable
m_CEN		equ		($1<<b_CEN)
b_COC		equ		1		clear on compare
m_COC		equ		($1<<b_COC)
b_COVF		equ		2		clear overflow
m_COVF		equ		($1<<b_COVF)
b_OVF		equ		4		overflow counter
m_OVF		equ		($4<<b_OVF)

MCT4ICR		equ		$18		Tick Timer 4 Interrupt Control Register
MCT3ICR		equ		$19		Tick Timer 3 Interrupt Control Register
MCT2ICR		equ		$1A		Tick Timer 2 Interrupt Control Register
MCT1ICR		equ		$1B		Tick Timer 1 Interrupt Control Register
b_IL		equ		0		interrupt level
m_IL		equ		($7<<b_IL)
b_ICLR		equ		3		interrupt clear
m_ICLR		equ		($1<<b_ICLR)
b_IEN		equ		4		interrupt enable
m_IEN		equ		($1<<b_IEN)
b_INT		equ		5		interrupt status
m_INT		equ		($1<<b_INT)

MCPEICR		equ		$1C		DRAM Parity Error Interrupt Control Register
MCSCCICR	equ		$1D		SCC Interrupt Control Register

MCT4CR		equ		$1E		Tick Timer 4 Control Register
MCT3CR		equ		$1F		Tick Timer 3 Control Register

MCDSBAR		equ		$20		DRAM Space Base Address Register

MCSSBAR		equ		$22		SRAM Space Base Address Register

MCDSSR		equ		$24		DRAM Space Size Register
b_DZI		equ		0		size bits

MCROR		equ		$25		DRAM/SRAM Options Register
b_DZO		equ		0		DRAM size bits
m_DZO		equ		($7<<b_DZO)
b_SZO		equ		3		SRAM size bits
m_SZO		equ		($3<<b_SZO)
b_FO		equ		5		FLASH device type
m_FO		equ		($1<<b_FO)
b_BEQ1		equ		7		Base board population 1MB (MC2 only)
m_BEQ1		equ		($1<<b_BEQ1)

MCSSSR		equ		$26		SRAM Space Size Register
b_SZI		equ		0		size bits to set
b_SEN		equ		2		SRAM enable
m_SEN		equ		($1<<b_SEN)

MCLESR		equ		$28		LANC Error Status Register
b_SCLR		equ		0		status clear
m_SCLR		equ		($1<<b_SCLR)
b_LTO		equ		1		TEA from local bus timeout
m_LTO		equ		($1<<b_LTO)
b_EXT		equ		2		TEA from external source (VMEbus)
m_EXT		equ		($1<<b_EXT)
b_PRTY		equ		3		TEA from DRAM parity error
m_PRTY		equ		($1<<b_PRTY)

MCLICR		equ		$2A		LANC Interrupt Control Register
b_EL		equ		6		interrupt sensitivity: 0=level, 1=edge
m_EL		equ		($1<<b_EL)
b_PLTY		equ		7		polarity: 0=rising edge/high level,
*									  1=falling edge/low level

MCLBEICR	equ		$2B		LANC Bus Error Interrupt Control Register
b_sc		equ		6		snoop control
m_sc		equ		($3<<b_sc)

MCSESR		equ		$2C		SCSI Error Status Register


MCGPIR		equ		$2d		General Purpose Inputs Register

MC162VR		equ		$2e		MVME162 Version Register
b_V0_32		equ		0		Speed indicator: 0=25Mhz, 1=32Mhz
b_V1_NOVME	equ		1		VMEchip2 presence: 0=present, 1=NOT!
b_V2_NOSCSI	equ		2		SCSI i/f presence: 0=present, 1=NOT!
b_V3_NOEth	equ		3		Ethernet presence: 0=present, 1=NOT!
b_V4_NOLC	equ		4		MC68040 type: 0=MC68LC040, 1=std
b_V5		equ		5		Alternate PROM/Flash mapping
b_V6_IPIC2	equ		6		IPIC #2 presence: 0=NOT! 1=present
b_V7_IPIC1	equ		7		IPIC #1 presence: 0=NOT! 1=present

MCSICR		equ		$2F		SCSI Interrupt Control Register

MCT3CMP		equ		$30		Tick Timer 3 Compare Register
MCT3CNT		equ		$34		Tick Timer 3 Counter

MCT4CMP		equ		$38		Tick Timer 4 Compare Register
MCT4CNT		equ		$3C		Tick Timer 4 Counter

MCCHBCR		equ		$40		Bus Clock Register
MCbc25		equ		25
MCbc30		equ		30
MCbc32		equ		32

MCPATCR		equ		$41		PROM Access Time Control Register
b_ET		equ		0		prom access time
m_ET		equ		($7<<b_ET)
MCet25		equ		$3		180ns for 150ns PROMS
MCet30		equ		$4		180ns for 150ns PROMS
MCet32		equ		$4		160ns for 150ns PROMS
b_ROM0		equ		4		PROM/Flash mapping
m_ROM0		equ		($1<<b_ROM0)

MCFATCR		equ		$42		Flash Access Time Control Register
b_FT		equ		0		flash access time
m_FT		equ		($7<<b_FT)
MCft25		equ		$2		140ns for 120ns Flash
MCft30		equ		$3		140ns for 120ns Flash
MCft32		equ		$3		130ns for 120ns Flash
b_FWEN		equ		3		Flash Write enable (MC2 only)
m_FWEN		equ		($1<<b_FWEN)

MCABTICR	equ		$43		Abort switch Interrupt Control Register
b_ABS		equ		6		Switch status, 1 = switch pressed
m_ABS		equ		($1<<b_ABS)
b_ABTINT	equ		5		Interrupt status
m_ABTINT	equ		($1<<b_ABTINT)
b_ABTIEN	equ		4		Interrupt Enable
m_ABTIEN	equ		($1<<b_ABTIEN)
b_ABTICLR	equ		3		Interrupt Clear
m_ABTICLR	equ		($1<<b_ABTICLR)
b_ABTIL		equ		0		Interrupt Level
m_ABTIL		equ		($7<<b_ABTIL)

MCRSCR		equ		$44		Reset Switch Control Register
b_RSWE		equ		0		reset switch enabled
m_RSWE		equ		($1<<b_RSWE)
b_BDFLO		equ		1		board fail output
m_BDFLO		equ 	($1<<b_BDFLO)
b_CPURS		equ		2		clear power up reset
m_CPURS		equ		($1<<b_CPURS)
b_PURS		equ		3		power up reset status
m_PURS		equ		($1<<b_PURS)
b_BRFLI		equ		4		board fail input
m_BRFLI		equ		($1<<b_BRFLI)

MCWTCR		equ		$45		Watchdog Timer Control Register
b_WDEN		equ		0		watchdog enable
m_WDEN		equ		($1<<b_WDEN)
b_WDRSE		equ		1		watchdog reset enable
m_WDRSE		equ		($1<<b_WDRSE)
b_WDBFE		equ		3		watchdog board fail enable
m_WDBFE		equ		($1<<b_WDBFE)
b_WDTO		equ		4		watchdog timeout status
m_WDTO		equ		($1<<b_WDTO)
b_WDCC		equ		5		watchdog counter clear
m_WDCC		equ		($1<<b_WDCC)
b_WDCS		equ		6		watchdog timeout status clear
m_WDCS		equ		($1<<b_WDCS)

MCAWTBSR	equ		$46		Access and Watchdog Time Base Select Register
b_WDTOP		equ		0		watchdog timeout period
m_WDTOP		equ		($0F<<b_WDTOP)
b_LBTO		equ		4		local bus timeout
m_LBTO		equ		($2<<b_LBTO)

MCDRCR		equ		$48		DRAM Control Register
b_RAMEN		equ		0		DRAM enable
m_RAMEN		equ		($1<<b_RAMEN)
b_RPAREN	equ		1		Enable parity checking - slow cycle
m_RPAREN	equ		($1<<b_RPAREN)
b_PARINT	equ		2		Enable parity interrupt
m_PARINT	equ		($1<<b_PARINT)
b_WWP		equ		3		write wrong parity
m_WWP		equ		($1<<b_WWP)

MCMPUSR		equ		$4a		MPU Status Register
b_MLTO		equ		0		TEA from local bus timeout
m_MLTO		equ		($1<<b_MLTO)
b_MLPE		equ		1		TEA from Dram parity error
m_MLPE		equ		($1<<b_MLPE)
b_MLBE		equ		2		TEA - no detailed status
m_MLBE		equ		($1<<b_MLBE)
b_MCLR		equ		3		clear MPU status
m_MCLR		equ		($1<<b_MCLR)

MCPCR		equ		$4c		32-bit Prescaler Count Register

* end of mcchip.d
 opt l

