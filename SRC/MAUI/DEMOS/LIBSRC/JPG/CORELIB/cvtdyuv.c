/*****************************************************************************
*
* CvtDYUV: RGB888 to DYUV conversion routines.
* Written By: Philips Research Labs.
* Date Written: 10/03/87
*
*/

/*****************************************************************************
*
* Definitions (assembly language) for Conversion Routines
*
*/
#asm
ystart		set		0x10				* Start values used for DYUV coding.
ustart		set		0x80
vstart		set		0x80

			vsect
LUTs		ds.b	(6*256)				* room for 6 lookuptables of 256 bytes each
			align
delabs		dc.w	0,1,4,9,16,27,44,79,128
			dc.w	177,212,229,240,247,252,255
bounds		dc.b	3,7,13,22,36,62,100
			align
absdel		ds.b	256
			align
			ends
			
			org		0					* indices:
YRED		do.b	256					* lookup table F0299
YGRN		do.b	256					*              F0587
YBLU		do.b	256					*              F0114
YcLUT		do.b	256					* lookup table Y to Yc
UcLUT		do.b	256					* 			   Y&B to Uc
VcLUT		do.b	256					*			   Y&R to Vc
*		
* IEEE floats:
*
F0299		set		0x3e991687			* red contribution to Y = 0.299
F0587		set		0x3f1645a2			* green   "             = 0.587
F0114		set		0x3de978d5			* blue    "             = 0.114
F0858		set		0x3f5bdbdb			* Yc = 16 + Y * (219/255)
F0495		set		0x3efda9b2			* Uc = (B-Y) * 0.564 * 224/255 + 128
F0626		set		0x3f20569d			* Vc = (R-Y) * 0.713 * 224/255 + 128
#endasm
			
/*****************************************************************************
*
* Ini_YUV		Initialize tables for RGB888 to YUV conversion.
*
* Passed:		No arguments.
* Returned:		No arguments.
*
*/
#asm
ini_yuv:	movem.l	d0-d3/d7/a0,-(sp)
			lea.l	LUTs(a6),a0			* a0 points to start of lookup table
*										  area, it is incremented by 256 in 'maketable'
			move.l	#F0299,d7			* multiplication factor 0.299
			bsr.s	maketable
			move.l	#F0587,d7			* multiplication factor 0.587
			bsr.s	maketable
			move.l	#F0114,d7			* multiplication factor 0.114
			bsr.s	maketable
			move.l	#F0858,d7			* multiplication factor 219/255
			bsr.s	maketable
			move.l	#F0495,d7			* multiplication factor 224/255*0.564
			bsr.s	maketable
			move.l	#F0626,d7			* multiplication factor 224/255*0.713
			bsr.s	maketable
			movem.l	(sp)+,d0-d3/d7/a0
			rts
			
maketable	clr.l	d2					* d7.l contains multiplier (IEEE float)
tab_lp		move.l	d2,d0
			tcall	T$Math1,T$LtoF		* signed long integer to float d0->d0
			move.l	d7,d1				* restore multiplier
			tcall	T$Math1,T$FMul		* os9 trap, float multiplication
			tcall	T$Math1,T$FInt		* os9 trap, round float to integer
			tcall	T$Math1,T$FtoL		* float to signed integer d0->d0
			move.b	d0,(a0)+			* store result in lookup table
			addq	#1,d2				* table-index 0..255
			cmpi.w	#0x100,d2			* all tables have 256 entries
			bne.s	tab_lp
			rts
#endasm			

/*****************************************************************************
*
* RGBTOYUV		Convert from RGB to absolute YUV conversion.
*
* Passed:		IBuf		= Input buffer (RGB888).
*				YBuff		= absolute Y values.
*				UVBuff		= absolute UV values.
*				LineSize	= Number of pixels to be converted.
* Returned:		YUV values are returned in the YBuff and UVBuff arrays.
*
*/
#asm
rgbtoyuv:	movem.l	d0-d7/a2-a5,-(sp)	* Y = .299R + .587G + .114B
			lea.l	LUTs(a6),a5			* a5 points to start of lookuptable area
			movea.l d0,a2				* red input buffer
			movea.l	d1,a3				* where Y is written to. (y_dest)
			movea.l	52(sp),a4			* where UV is written to.(uv_dest)
			move.l	56(sp),d2			* Nr. of pixels per line - 1
			subq.w	#1,d2
yuvcoll
			clr.l	d6
			clr.w	d7
			move.b	(a2)+,d3			* get next red pixel
			andi.w	#0xff,d3

			move.b	d3,d7
			add.w	#YRED,d7			* make d7 point to right entry in YRED-table

			move.b	(a5,d7.w),d6		* move red * 0.299 to Y
			move.b	(a2)+,d4			* get next green pixel
			andi.w	#0xff,d4			* mask one of them
			add.w	#YGRN,d4			* make d4 point to right entry in YGRN table
			move.b	(a5,d4.w),d4		* green * 0.587
			add.b	d4,d6				* Y = 0.299R + 0.587G
			move.b	(a2)+,d5			* get next blue pixel
			andi.w	#0xff,d5			* mask one of them
			move.w	d5,d7				*
			add.w	#YBLU,d7			* d7 points to right entry in YBLU table
			move.b	(a5,d7.w),d7		* blue * 0.114
			add.b	d7,d6				* Y = 0.299R + 0.587G + 0.114B
			clr.w	d7					*
			move.b	d6,d7				* 
			add.w	#YcLUT,d7			* d7 points to right entry in YcLUT
			move.b	(a5,d7.w),d7		* Yc = Y*(219/255)
			add.b	#16,d7				* Yc = Yc + 16
* previous line (add 16) should be removed when calc. unlimit. YUV!
			move.b	d7,(a3)+			* write Yc to y_dest
			btst.l	#0,d2				* if odd pixel, then do not make U and V
			beq.s	no_uv1
u_part		sub.w	d6,d5				* d5 := B-Y
			bpl.s	uplus				* B-Y positive?
umin		neg.b	d5					* no, make positive, so one half of the symmetrical
*										  UcLUT is sufficient.
			andi.w	#0xff,d5			*
			add.w	#UcLUT,d5			* d5 points to right entry in UcLUT
			move.b	(a5,d5.w),d5		* d5 = - (B-Y)*0.564*224/255
			neg.b	d5					* d5 = + (B-Y)*0.564*224/255
			bra.s	v_part				* now process the V
uplus		add.w	#UcLUT,d5			* B-Y positive, d5 points to UcLUT
			move.b	(a5,d5.w),d5		* (B-Y)*0.564*224/255
			
v_part		sub.w	d6,d3				* d3 = (R-Y)
			bpl.s	vplus				* positive?, one half of the VcLUT is sufficient
vmin		neg.b	d3					* negative: make it temporary positive
			andi.w	#0xff,d3
			add.w	#VcLUT,d3			* point to right entry in VcLUT
			move.b	(a5,d3.w),d3		* (R-Y)*0.713*224/255
			neg.b	d3					* restore the sign
			bra.s	put_uv				* and go write the UV pixels
vplus		add.w	#VcLUT,d3			* (R-Y) positive.
			move.b	(a5,d3.w),d3		* d3 = (R-Y)*0.713*224/255
			
put_uv
			add.b	#128,d5
			add.b	#128,d3
			move.b	d5,(a4)+			* write to uv_dest (u)
			move.b	d3,(a4)+			* write to uv_dest (v)

no_uv1		dbf		d2,yuvcoll			* check for last pixel on line
		
			movem.l	(sp)+,d0-d7/a2-a5	* READY!!!
			rts
#endasm			
			
/*****************************************************************************
*
* IniDelta		Initialize tables for YUV to DYUV conversion.
*
* Passed:		No arguments.
* Returned:		No arguments.
*
*/
#asm
inidelta:
			movem.l	d0-d3/a0-a2,-(sp)
			lea.l	absdel(a6),a0			* get start address and
			lea.l	absdel+256(a6),a1		* .. end of dyuv-code table
			lea.l	bounds(a6),a2			* The absolute value where you change area
			move.w	#128,d0					* size/2 - 1;
			clr.b	(a0)+					* first code = 0
			moveq	#1,d1					* next code to start with
			moveq	#(16-1),d3				* and its counterpart
			moveq	#1,d2					* pointer in table (1..128)
inilp		cmp.b	(a2),d2					* reached boundary?
			bne.s	samecode				* no.
			adda.l	#1,a2					* yes, increment boundary pointer and
			addq	#1,d1					* increment 'code'.
			subq	#1,d3
samecode	move.b	d1,(a0)+				* write code to first half
			move.b	d3,-(a1)				* and 16-code to second half
			addq	#1,d2					* increment pointer in table
			dbf		d0,inilp				* do this 128 times
			
			movem.l	(sp)+,d0-d3/a0-a2
			rts
#endasm

/*****************************************************************************
*
* MakeDelta		Convert from YUV to DYUV. This procedures converts the arrays,
*			    y_addr and uv_addr into the output array y_addr. The algorithm
*				used to convert absolute to delta YUV is according to the
*				recommendation by C. Luijks, HIS-TC Eindhoven.
*
* Layout of the input-arrays:
*
*  Y: pix.0  | pix.1  | pix.2  | .....  | pix.linelength-1 |
*
* UV: Upix.0 | Vpix.0 | Upix.2 | Vpix.2 | Upix.4 | Vpix.4 | 
*     .....| Upix.linelength-2 | Vpix.linelenght-2
*
* Passed:		Y_Addr		= Address of Y values.
*				UV_Addr		= Address of UV values.
*				LineLength	= Line length in pixels.
* Returned:		Y_Addr		= Address of DYUV values.
*
*/
#asm
todelta:
			movem.l	d0-d7/a0-a7,-(sp)
			bclr	#0,d0					* force to even address Y
			bclr	#0,d1					* dito for UV
			movea.l	d0,a0					* a0 points to Yinput / output array
			movea.l	d1,a1					* a1 points to UVinput
			move.l	68(sp),d0				* d0 is number of pixels
			lea.l	delabs(a6),a2			* tables nec. to encode | initialized
			lea.l	absdel(a6),a3			*    "    "    "    "   | by inidelta
			move.l	#ystart,d5				* d5 is predicted Y
			move.l	#ustart,d6				* d6 is predicted U
			move.l	#vstart,d7				* d7 is predicted V
			lsr.l	#1,d0					* 2 pixels per loop, so divide by 2
			subq	#1,d0					* adjust count for dbf operation
			
deltaline	clr.w	d2						* temporary variable
Y1pix		move.l	d5,d1					* predicted value
			move.b	(a0),d2					* get input Y
			sub.w	d1,d2					* d2 = delta
			move.w	d2,d4					* save delta
			andi.w	#0xff,d2
			move.b	(a3,d2.w),d2			* d2 = code[delta]
			move.w	d2,d3
			lsl.w	#1,d2					* d2 is 16bit index
			add.w	(a2,d2.w),d1			* d1 = temp			
			or.w	d4,d4					* was delta pos. or neg.?
			bmi.s	y1neg					* it was negative.
			cmpi.w	#255,d1					* test for byteoverflow
			ble.s	endy1					* no overflow, ok jump.
			subq	#1,d3					* yes overflow, decrement code, so no ovfl
			bra.s	endy1
			
y1neg		cmpi.w	#256,d1					* test for byteunderflow
			bge.s	endy1					* no underflow, ok jump.
			addq	#1,d3					* yes underflow, increment code, so no ovfl
						
endy1		move.b	d3,(a0)					* write y_part of delta (lower nibble)
			lsl.w	#1,d3					* d3 will serve as word-index
			add.w	(a2,d3.w),d5			* new predictor
			andi.w	#0xff,d5				* modulo 255.
			
*********************************************

Upix		clr.w	d2						* temporary variable
			move.l	d6,d1					* d1 predicted U
			move.b	(a1)+,d2				* get U input
			sub.w	d1,d2					* d2 = delta
			move.w	d2,d4					* save for later use
			andi.w	#0xff,d2
			move.b	(a3,d2.w),d2			* d2 = code[delta]
			move.w	d2,d3
			lsl.w	#1,d2					* d2 is 16bit index
			add.w	(a2,d2.w),d1			* d1 = temp			
			or.w	d4,d4					* was delta pos. or neg.?
			bmi.s	uneg					* it was negative.
			cmpi.w	#255,d1					* test for byteoverflow
			ble.s	endu					* no overflow, ok jump.
			subq	#1,d3					* yes overflow, decrement code, so no ovfl
			bra.s	endu
			
uneg		cmpi.w	#257,d1					* test for byteunderflow
			bge.s	endu					* no underflow, ok jump.
			addq	#1,d3					* yes underflow, increment code, so no ovfl
						
endu		lsl.w	#1,d3					* need d3 as wordindex
			add.w	(a2,d3.w),d6			* new predictor
			andi.w	#0xff,d6				* modulo 255.
			lsr.w	#1,d3					* restore d3 to original value
			lsl.w	#4,d3					* shift code to upper nibble
			or.b	d3,(a0)+				* write y_part of delta (lower nibble)
			
*********************************************

Y2pix		clr.w	d2
			move.l	d5,d1					* predicted value
			move.b	(a0),d2					* get second Y input
			sub.w	d1,d2					* d2 = delta
			move.w	d2,d4					* save delta for later use
			andi.w	#0xff,d2
			move.b	(a3,d2.w),d2			* d2 = code[delta]
			move.w	d2,d3
			lsl.w	#1,d2					* d2 is 16bit index
			add.w	(a2,d2.w),d1			* d1 = temp			
			or.w	d4,d4					* was delta pos. or neg.?
			bmi.s	y2neg					* it was negative.
			cmpi.w	#255,d1					* test for byteoverflow
			ble.s	endy2					* no overflow, ok jump.
			subq	#1,d3					* yes overflow, decrement code, so no ovfl
			bra.s	endy2
			
y2neg		cmpi.w	#256,d1					* test for byteunderflow
			bge.s	endy2					* no underflow, ok jump.
			addq	#1,d3					* yes underflow, increment code, so no ovfl
						
endy2		move.b	d3,(a0)					* write y_part of delta (lower nibble)
			lsl.w	#1,d3					* d3 will serve as word-index
			add.w	(a2,d3.w),d5			* new predictor
			andi.w	#0xff,d5				* modulo 255.
			
*********************************************

Vpix		clr.w	d2						* temporary variable
			move.l	d7,d1					* d1 predicted V
			move.b	(a1)+,d2				* get V input
			sub.w	d1,d2					* d2 = delta
			move.w	d2,d4					* save delta or later use
			andi.w	#0xff,d2
			move.b	(a3,d2.w),d2			* d2 = code[delta]
			move.w	d2,d3
			lsl.w	#1,d2					* d2 is 16bit index
			add.w	(a2,d2.w),d1			* d1 = temp			
			or.w	d4,d4					* was delta pos. or neg.?
			bmi.s	vneg					* it was negative.
			cmpi.w	#255,d1					* test for byteoverflow
			ble.s	endv					* no overflow, ok jump.
			subq	#1,d3					* yes overflow, decrement code, so no ovfl
			bra.s	endv
			
vneg		cmpi.w	#257,d1					* test for byteunderflow
			bge.s	endv					* no underflow, ok jump.
			addq	#1,d3					* yes underflow, increment code, so no ovfl
						
endv		lsl.w	#1,d3					* need d3 as wordindex
			add.w	(a2,d3.w),d7			* new predictor
			andi.w	#0xff,d7				* modulo 255.
			lsr.w	#1,d3					* restore d3 to original value
			lsl.w	#4,d3					* shift code to upper nibble
			or.b	d3,(a0)+				* write y_part of delta (lower nibble)
			
			dbf		d0,deltaline			* line completed?
			
			movem.l	(sp)+,d0-d7/a0-a7
			rts
#endasm
