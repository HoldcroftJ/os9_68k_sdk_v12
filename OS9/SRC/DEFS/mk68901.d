*
* definitions file for "sc68901.a" serial driver
*
********************
* Definition for MK68901
***********
*          MK68901 register layout
Prt_GPIP   equ      $00*BW_68901
Prt_AER    equ      $01*BW_68901
Prt_DDR    equ      $02*BW_68901
Prt_IERA   equ      $03*BW_68901
Prt_IERB   equ      $04*BW_68901
Prt_IPRA   equ      $05*BW_68901
Prt_IPRB   equ      $06*BW_68901
Prt_ISRA   equ      $07*BW_68901
Prt_ISRB   equ      $08*BW_68901
Prt_IMRA   equ      $09*BW_68901
Prt_IMRB   equ      $0a*BW_68901
Prt_VR     equ      $0b*BW_68901
Prt_TACR   equ      $0c*BW_68901
Prt_TBCR   equ      $0d*BW_68901
Prt_TCDCR  equ      $0e*BW_68901
Prt_TCCR   equ      Prt_TCDCR
Prt_TDCR   equ      Prt_TCDCR
Prt_TADR   equ      $0f*BW_68901
Prt_TBDR   equ      $10*BW_68901
Prt_TCDR   equ      $11*BW_68901
Prt_TDDR   equ      $12*BW_68901
Prt_SCR    equ      $13*BW_68901
Prt_UCR    equ      $14*BW_68901
Prt_RSR    equ      $15*BW_68901
Prt_TSR    equ      $16*BW_68901
Prt_UDR    equ      $17*BW_68901
***********
*          Channel #
Chnl_G7    equ      %00001111
Chnl_G6    equ      %00001110
Chnl_TA    equ      %00001101
Chnl_RB    equ      %00001100
Chnl_RE    equ      %00001011
Chnl_XB    equ      %00001010
Chnl_XE    equ      %00001001
Chnl_TB    equ      %00001000
Chnl_G5    equ      %00000111
Chnl_G4    equ      %00000110
Chnl_TC    equ      %00000101
Chnl_TD    equ      %00000100
Chnl_G3    equ      %00000011
Chnl_G2    equ      %00000010
Chnl_G1    equ      %00000001
Chnl_G0    equ      %00000000
***********
*          Mask for GPIP
GPIP_G7    equ      %10000000
GPIP_G6    equ      %01000000
GPIP_G5    equ      %00100000
GPIP_G4    equ      %00010000
GPIP_G3    equ      %00001000
GPIP_G2    equ      %00000100
GPIP_G1    equ      %00000010
GPIP_G0    equ      %00000001
***********
*          Mask for GPIP in bit position
GPIP_G7_b  equ      7
GPIP_G6_b  equ      6
GPIP_G5_b  equ      5
GPIP_G4_b  equ      4
GPIP_G3_b  equ      3
GPIP_G2_b  equ      2
GPIP_G1_b  equ      1
GPIP_G0_b  equ      0
***********
*          Mask for AER
AER_G7     equ      %10000000
AER_G6     equ      %01000000
AER_G5     equ      %00100000
AER_G4     equ      %00010000
AER_G3     equ      %00001000
AER_G2     equ      %00000100
AER_G1     equ      %00000010
AER_G0     equ      %00000001
***********
*          Mask for AER in bit position
AER_G7_b   equ      7
AER_G6_b   equ      6
AER_G5_b   equ      5
AER_G4_b   equ      4
AER_G3_b   equ      3
AER_G2_b   equ      2
AER_G1_b   equ      1
AER_G0_b   equ      0
***********
*          Mask for DDR
DDR_G7     equ      %10000000
DDR_G6     equ      %01000000
DDR_G5     equ      %00100000
DDR_G4     equ      %00010000
DDR_G3     equ      %00001000
DDR_G2     equ      %00000100
DDR_G1     equ      %00000010
DDR_G0     equ      %00000001
***********
*          Mask for DDR in bit position
DDR_G7_b   equ      7
DDR_G6_b   equ      6
DDR_G5_b   equ      5
DDR_G4_b   equ      4
DDR_G3_b   equ      3
DDR_G2_b   equ      2
DDR_G1_b   equ      1
DDR_G0_b   equ      0
***********
*          Mask for IERA
IERA_G7    equ      %10000000
IERA_G6    equ      %01000000
IERA_TA    equ      %00100000
IERA_RB    equ      %00010000
IERA_RE    equ      %00001000
IERA_XB    equ      %00000100
IERA_XE    equ      %00000010
IERA_TB    equ      %00000001
***********
*          Mask for IERA in bit position
IERA_G7_b  equ      7
IERA_G6_b  equ      6
IERA_TA_b  equ      5
IERA_RB_b  equ      4
IERA_RE_b  equ      3
IERA_XB_b  equ      2
IERA_XE_b  equ      1
IERA_TB_b  equ      0
***********
*          Mask for IERB
IERB_G5    equ      %10000000
IERB_G4    equ      %01000000
IERB_TC    equ      %00100000
IERB_TD    equ      %00010000
IERB_G3    equ      %00001000
IERB_G2    equ      %00000100
IERB_G1    equ      %00000010
IERB_G0    equ      %00000001
***********
*          Mask for IERB in bit position
IERB_G5_b  equ      7
IERB_G4_b  equ      6
IERB_TC_b  equ      5
IERB_TD_b  equ      4
IERB_G3_b  equ      3
IERB_G2_b  equ      2
IERB_G1_b  equ      1
IERB_G0_b  equ      0
***********
*          Mask for IPRA
IPRA_G7    equ      %10000000
IPRA_G6    equ      %01000000
IPRA_TA    equ      %00100000
IPRA_RB    equ      %00010000
IPRA_RE    equ      %00001000
IPRA_XB    equ      %00000100
IPRA_XE    equ      %00000010
IPRA_TB    equ      %00000001
***********
*          Mask for IPRA in bit position
IPRA_G7_b  equ      7
IPRA_G6_b  equ      6
IPRA_TA_b  equ      5
IPRA_RB_b  equ      4
IPRA_RE_b  equ      3
IPRA_XB_b  equ      2
IPRA_XE_b  equ      1
IPRA_TB_b  equ      0
***********
*          Mask for IPRB
IPRB_G5    equ      %10000000
IPRB_G4    equ      %01000000
IPRB_TC    equ      %00100000
IPRB_TD    equ      %00010000
IPRB_G3    equ      %00001000
IPRB_G2    equ      %00000100
IPRB_G1    equ      %00000010
IPRB_G0    equ      %00000001
***********
*          Mask for IPRB in bit position
IPRB_G5_b  equ      7
IPRB_G4_b  equ      6
IPRB_TC_b  equ      5
IPRB_TD_b  equ      4
IPRB_G3_b  equ      3
IPRB_G2_b  equ      2
IPRB_G1_b  equ      1
IPRB_G0_b  equ      0
***********
*          Mask for ISRA
ISRA_G7    equ      %10000000
ISRA_G6    equ      %01000000
ISRA_TA    equ      %00100000
ISRA_RB    equ      %00010000
ISRA_RE    equ      %00001000
ISRA_XB    equ      %00000100
ISRA_XE    equ      %00000010
ISRA_TB    equ      %00000001
***********
*          Mask for ISRA in bit position
ISRA_G7_b  equ      7
ISRA_G6_b  equ      6
ISRA_TA_b  equ      5
ISRA_RB_b  equ      4
ISRA_RE_b  equ      3
ISRA_XB_b  equ      2
ISRA_XE_b  equ      1
ISRA_TB_b  equ      0
***********
*          Mask for ISRB
ISRB_G5    equ      %10000000
ISRB_G4    equ      %01000000
ISRB_TC    equ      %00100000
ISRB_TD    equ      %00010000
ISRB_G3    equ      %00001000
ISRB_G2    equ      %00000100
ISRB_G1    equ      %00000010
ISRB_G0    equ      %00000001
***********
*          Mask for ISRB in bit position
ISRB_G5_b  equ      7
ISRB_G4_b  equ      6
ISRB_TC_b  equ      5
ISRB_TD_b  equ      4
ISRB_G3_b  equ      3
ISRB_G2_b  equ      2
ISRB_G1_b  equ      1
ISRB_G0_b  equ      0
***********
*          Mask for IMRA
IMRA_G7    equ      %10000000
IMRA_G6    equ      %01000000
IMRA_TA    equ      %00100000
IMRA_RB    equ      %00010000
IMRA_RE    equ      %00001000
IMRA_XB    equ      %00000100
IMRA_XE    equ      %00000010
IMRA_TB    equ      %00000001
***********
*          Mask for IMRA in bit position
IMRA_G7_b  equ      7
IMRA_G6_b  equ      6
IMRA_TA_b  equ      5
IMRA_RB_b  equ      4
IMRA_RE_b  equ      3
IMRA_XB_b  equ      2
IMRA_XE_b  equ      1
IMRA_TB_b  equ      0
***********
*          Mask for IMRB
IMRB_G5    equ      %10000000
IMRB_G4    equ      %01000000
IMRB_TC    equ      %00100000
IMRB_TD    equ      %00010000
IMRB_G3    equ      %00001000
IMRB_G2    equ      %00000100
IMRB_G1    equ      %00000010
IMRB_G0    equ      %00000001
***********
*          Mask for IMRB in bit position
IMRB_G5_b  equ      7
IMRB_G4_b  equ      6
IMRB_TC_b  equ      5
IMRB_TD_b  equ      4
IMRB_G3_b  equ      3
IMRB_G2_b  equ      2
IMRB_G1_b  equ      1
IMRB_G0_b  equ      0
***********
*          Mask for VR
VR_V       equ      %11110000
VR_S       equ      %00001000
***********
*          Mask for VR in bit position
VR_S_b     equ      3
***********
*          Mask for TACR
TACR_RS    equ      %00010000
TACR_C3    equ      %00001000
TACR_C2    equ      %00000100
TACR_C1    equ      %00000010
TACR_C0    equ      %00000001
***********
*          Mask for TACR in bit position
TACR_RS_b  equ      4
TACR_C3_b  equ      3
TACR_C2_b  equ      2
TACR_C1_b  equ      1
TACR_C0_b  equ      0
***********
*          Mask for TBCR
TBCR_RS    equ      %00010000
TBCR_C3    equ      %00001000
TBCR_C2    equ      %00000100
TBCR_C1    equ      %00000010
TBCR_C0    equ      %00000001
***********
*          Mask for TBCR in bit position
TBCR_RS_b  equ      4
TBCR_C3_b  equ      3
TBCR_C2_b  equ      2
TBCR_C1_b  equ      1
TBCR_C0_b  equ      0
***********
*          Mask for TCCR
TCCR_C2    equ      %01000000
TCCR_C1    equ      %00100000
TCCR_C0    equ      %00010000
***********
*          Mask for TCCR in bit position
TCCR_C2_b  equ      6
TCCR_C1_b  equ      5
TCCR_C0_b  equ      4
***********
*          Mask for TDCR
TDCR_C2    equ      %00000100
TDCR_C1    equ      %00000010
TDCR_C0    equ      %00000001
***********
*          Mask for TDCR in bit position
TDCR_C2_b  equ      2
TDCR_C1_b  equ      1
TDCR_C0_b  equ      0
***********
*          Mask for UCR
UCR_d16    equ      %10000000
UCR_WL1    equ      %01000000
UCR_WL0    equ      %00100000
UCR_SB1    equ      %00010000
UCR_SB0    equ      %00001000
UCR_PON    equ      %00000100
UCR_PEV    equ      %00000010
***********
*          Mask for UCR in bit position
UCR_d16_b  equ      7
UCR_WL1_b  equ      6
UCR_WL0_b  equ      5
UCR_SB1_b  equ      4
UCR_SB0_b  equ      3
UCR_PON_b  equ      2
UCR_PEV_b  equ      1
***********
*          Mask for RSR
RSR_BF     equ      %10000000
RSR_OE     equ      %01000000
RSR_PE     equ      %00100000
RSR_FE     equ      %00010000
RSR_BD     equ      %00001000
RSR_CP     equ      %00000100
RSR_SS     equ      %00000010
RSR_RE     equ      %00000001
***********
*          Mask for RSR in bit position
RSR_BF_b   equ      7
RSR_OE_b   equ      6
RSR_PE_b   equ      5
RSR_FE_b   equ      4
RSR_BD_b   equ      3
RSR_CP_b   equ      2
RSR_SS_b   equ      1
RSR_RE_b   equ      0
***********
*          Mask for TSR
TSR_BE     equ      %10000000
TSR_UE     equ      %01000000
TSR_AT     equ      %00100000
TSR_ET     equ      %00010000
TSR_BK     equ      %00001000
TSR_HI     equ      %00000100
TSR_LO     equ      %00000010
TSR_XE     equ      %00000001
***********
*          Mask for TSR in bit position
TSR_BE_b   equ      7
TSR_UE_b   equ      6
TSR_AT_b   equ      5
TSR_ET_b   equ      4
TSR_BK_b   equ      3
TSR_HI_b   equ      2
TSR_LO_b   equ      1
TSR_XE_b   equ      0
***********
