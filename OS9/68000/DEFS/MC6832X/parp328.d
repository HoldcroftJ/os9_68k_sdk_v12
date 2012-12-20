 opt -l
 nam parp328.d
 ttl Definitions for the MC68328 Parallel Ports Module

**********************************************************************
* Copyright 1995, 1996 by Microware Systems Corporation              *
* Reproduced Under License                                           *
*                                                                    *
* This source code is the proprietary confidential property of       *
* Microware Systems Corporation, and is provided to licensee         *
* solely for documentation and educational purposes. Reproduction,   *
* publication, or distribution in any form to any party other than   *
* the licensee is strictly prohibited.                               *
**********************************************************************
* Definitions for the MC68328 Parallel Ports Module
*
* Edition History
*
* Ed.   Date.     Reason.                                        Who.
* --  --------  -----------------------------------------------  ---
* 01  95/08/31  history starts.                                  rws
*               --- OS-9/68K MC68328 Support Beta2 Release ---
**********************************************************************


 ifndef ModuleIO
 ifndef StdAddr
ModuleIO equ $FFFFF000  Base address of chip's internal regs (32 bits)
 else
ModuleIO equ $FFF000    Base address of chip's internal regs (24 bits)
 endc
 endc


PPBase equ $400        Offset of Parallel Port Module with System module map
PADIRMap equ ModuleIO+PPBase default location of PADIR

********************************************************************
* The MC68328 provides 10 8-pin parallel I/O ports (A, B, C, D, E, F,
* G, J, K, M).
* Ports A, B, C, E, F, G, J, and K multiplex two functions onto
* one pin.
* Port M is multiplexes two functions onto one pin but also adds
* a switchable pullup resistor to the pin.
* Port D is like port M but also provides interrupt capabilities.

*
* All registers are 8 bits and are referenced from PADIRMap.
*

* Port A is multiplexed with address lines A23:16
* Power on default function is A23:16
PADIR  equ $00 Port A Direction Register
PADATA equ $01 Port A Data Register
PASEL  equ $03 Port A Select Register

* Port B is multiplexed with data lines D7:0
* Power on default function is D7:0
PBDIR  equ $08 Port B Direction Register
PBDATA equ $09 Port B Data Register
PBSEL  equ $0B Port B Select Register

* Port C is multiplexed with M68000 bus-control signals:
* _bit_ _PCx_ _68000 control signal_
*   7   none   none
*   6    PC6   WEn (PCMCIA)
*   5    PC5   DTACKn
*   4    PC4   NMIn
*   3   none   none
*   2    PC2   LDS
*   1    PC1   UDS
*   0    PC0   MOCLK
* Power on default function is 68000 control signals
PCDIR  equ $10 Port C Direction Register
PCDATA equ $11 Port C Data Register
PCSEL  equ $13 Port C Select Register

* Port D has features intended for use as a keyboard input port
* Power on default function is for associated interrupts disabled
PDDIR    equ $18 Port D Direction Register
PDDATA    equ $19 Port D Data Register
PDPUEN    equ $1A Port D Pullup Enable Register
PDPOL     equ $1C Port D Polarity Register
PDIRQEN   equ $1D Port D IRQ Enable Register
PDIRQEDGE equ $1F Port D IRQ Edge Register

* Port E is mulitplexed with 7 chip-select signals:
* _bit_ _PEx_ _chip select signal_
*   7    PE7   CSB3n
*   6    PE6   CSB2n
*   5    PE5   CSB1n
*   4    PE4   CSB0n
*   3    PE3   CSA3n
*   2    PE2   CSA2n
*   1    PE1   CSA1n
*   0   none   none
* Power on default function is PE7 and rest for chip selects
PEDIR  equ $20 Port E Direction Register
PEDATA equ $21 Port E Data Register
PEPUEN equ $22 Port E Pullup Enable Register
PESEL  equ $23 Port E Select Register

* Port F is mulitplexed with address lines A31:23
* Power on default function is for Port I/O, inputs
PFDIR  equ $28 Port F Direction Register
PFDATA equ $29 Port F Data Register
PFPUEN equ $2A Port F Pullup Enable Register
PFSEL  equ $2B Port F Select Register

* Port G is mulitplexed with timer and serial communication signals:
* _bit_ _PGx_ _other signal_
*   7    PG7   RTCOUTn 
*   6    PG6   TIN1
*   5    PG5   TOUT1
*   4    PG4   TIN2
*   3    PG3   TOUT2
*   2    PG2   PWMOUT
*   1    PG1   UART RXD 
*   0    PG0   UART TXD
* Power on default function is for the various functions
PGDIR  equ $30 Port G Direction Register
PGDATA equ $31 Port G Data Register
PGPUEN equ $32 Port G Pullup Enable Register
PGSEL  equ $33 Port G Select Register

* Port J is mulitplexed with 8 chip-select signals:
* _bit_ _PJx_ _chip select signal_  
*   7    PJ7   CSD3n
*   6    PJ6   CSD2n
*   5    PJ5   CSD1n
*   4    PJ4   CSD0n
*   3    PJ3   CSC3n
*   2    PJ2   CSC2n
*   1    PJ1   CSC1n
*   0    PJ0   CSC0n
* Power on default function is chip select usage
PJDIR  equ $38 Port J Direction Register
PJDATA equ $39 Port J Data Register
PJSEL  equ $3B Port J Select Register

* Port K is mulitplexed with SPI and PCMCIA related signals:
* _bit_ _PKx_ _other signal_
*   7    PK7   PCMCIA CE1
*   6    PK6   PCMCIA CE2
*   5    PK5   SPIS CLK1
*   4    PK4   SPIS RXD
*   3    PK3   SPIS EN
*   2    PK2   SPIM CLK0
*   1    PK1   SPIM RXD
*   0    PK0   SPIM TXD
* Power on default function is Port I/O, inputs
PKDIR  equ $40 Port K Direction Register
PKDATA equ $41 Port K Data Register
PKPUEN equ $42 Port K Pullup Enable Register
PKSEL  equ $43 Port K Select Register

* Port M is mulitplexed with IRQ and UART signals:
* _bit_ _PMx_ _other signal_
*   7    PM7   UART GPIO
*   6    PM6   PEN IRQn
*   5    PM5   IRQ1n
*   4    PM4   IRQ2n
*   3    PM3   IRQ3n
*   2    PM2   IRQ6n
*   1    PM1   RTSn
*   0    PM0   CTSn
* Power on default function is for interrupt usage
PMDIR  equ $48 Port M Direction Register
PMDATA equ $49 Port M Data Register
PMPUEN equ $4A Port M Pullup Enable Register
PMSEL  equ $4B Port M Select Register

 
*****  These are separate masks within the various registers ********

*********************  Direction Registers  *************************
* PADIR, PBDIR, PCDIR, PDDIR, PEDIR, PFDIR, PGDIR, PJDIR, PKDIR, PMDIR
* Initialized to $00 at reset.
* Each bit controls the corresponding pin's data direction, 
* 1=output 0=input.  These bits have no function while the SELECT bits are
* low.
*
DIR7 equ $80 Data Direction control for Px7 pin
DIR6 equ $40 Data Direction control for Px6 pin
DIR5 equ $20 Data Direction control for Px5 pin
DIR4 equ $10 Data Direction control for Px4 pin
DIR3 equ $08 Data Direction control for Px3 pin
DIR2 equ $04 Data Direction control for Px2 pin
DIR1 equ $02 Data Direction control for Px1 pin
DIR0 equ $01 Data Direction control for Px0 pin


*********************  Data Registers  *************************
* PADATA, PBDATA, PCDATA, PDDATA, PEDATA, PFDATA, PGDATA, PJDATA, 
* PKDATA, PMDATA
* Initialized to $00 at reset.
* These bits control or report the data on the pins while the SELECT
* bits are high.
* For SELECT=1 && DIRECTION=1, DATA=data to be output
* for SELECT=1 && DIRECTION=0, DATA=input data
* for SELECT=0, bits reset to 0.
* The actual value on the pin is reported regardless of whether it's
* programmed for input or output.
*


*********************  Select Registers  *************************
* PASEL, PBSEL, PCSEL, PESEL, PFSEL, PGSEL, PJSEL, PKSEL, PMSEL
* Initialized to $00 at reset, except for PESEL ($80),  PFSEL ($FF),
* and PKSEL ($FF)
* These bits select whether the port is to used as an I/O port
* (SELECT=1) or as its other funtion (SELECT=0)
*
SEL7 equ $80 Port Selection control for Px7 pin
SEL6 equ $40 Port Selection control for Px6 pin
SEL5 equ $20 Port Selection control for Px5 pin
SEL4 equ $10 Port Selection control for Px4 pin
SEL3 equ $08 Port Selection control for Px3 pin
SEL2 equ $04 Port Selection control for Px2 pin                                 
SEL1 equ $02 Port Selection control for Px1 pin
SEL0 equ $01 Port Selection control for Px0 pin


************  Pullup Resistor Enable Registers  *******************
* PDPUEN, PEPUEN, PFPUEN, PGPUEN, PKPUEN, PMPUEN
* Initialized to $00 at reset.
* These bits enable the pullup resisitors on the port, 
* 1=pullup resistors enabled 0=pullup resistors disabled.
*
PU7 equ $80 Pullup Resistor control for Px7 pin
PU6 equ $40 Pullup Resistor control for Px6 pin                                 
PU5 equ $20 Pullup Resistor control for Px5 pin
PU4 equ $10 Pullup Resistor control for Px4 pin
PU3 equ $08 Pullup Resistor control for Px3 pin
PU2 equ $04 Pullup Resistor control for Px2 pin
PU1 equ $02 Pullup Resistor control for Px1 pin
PU0 equ $01 Pullup Resistor control for Px0 pin


************  PDPOL (Port D Polarity Register)  *******************             
* Initialized to $00 at reset.
* These bits select the input signal polarity,
* 1=input data inverted before being presented to holding register/
*   interrupt is active-low or falling edge
* 0=input data unchanged/interrupt is active-high or rising edge
*
POLP7 equ $80 Polarity control for Px7 pin
POLP6 equ $40 Polarity control for Px6 pin
POLP5 equ $20 Polarity control for Px5 pin
POLP4 equ $10 Polarity control for Px4 pin
POLP3 equ $08 Polarity control for Px3 pin
POLP2 equ $04 Polarity control for Px2 pin
POLP1 equ $02 Polarity control for Px1 pin
POLP0 equ $01 Polarity control for Px0 pin                                 


**********  PDIRQEN (Port D IRQ Enable Register)  *****************
* Initialized to $00 at reset.
* These bits allow the interrupts to be presented to the interrupt
* controller block,
* 1=enable interrupts 0=disaable interrupts
*
IQEN7 equ $80 Interrupt Enable control for Px7 pin
IQEN6 equ $40 Interrupt Enable control for Px6 pin
IQEN5 equ $20 Interrupt Enable control for Px5 pin
IQEN4 equ $10 Interrupt Enable control for Px4 pin
IQEN3 equ $08 Interrupt Enable control for Px3 pin
IQEN2 equ $04 Interrupt Enable control for Px2 pin
IQEN1 equ $02 Interrupt Enable control for Px1 pin
IQEN0 equ $01 Interrupt Enable control for Px0 pin


************  PDIRQEDGE (Port D IRQ Edge Register)  *****************
* Initialized to $00 at reset.
* These bits enable edge interrupts,
* 1=edge-triggered interrupts 0=level-sensitive interrupts
*
IQEG7 equ $80 Interrupt Edge control for Px7 pin
IQEG6 equ $40 Interrupt Edge control for Px6 pin
IQEG5 equ $20 Interrupt Edge control for Px5 pin
IQEG4 equ $10 Interrupt Edge control for Px4 pin
IQEG3 equ $08 Interrupt Edge control for Px3 pin
IQEG2 equ $04 Interrupt Edge control for Px2 pin
IQEG1 equ $02 Interrupt Edge control for Px1 pin
IQEG0 equ $01 Interrupt Edge control for Px0 pin

 
 pag
 opt l
