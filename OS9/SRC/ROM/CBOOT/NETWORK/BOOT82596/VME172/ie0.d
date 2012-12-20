*
* this stuff should match with 'struct mz8554_desc'
*
*
* if ramaddr==0, the driver will assume the i82596
* and the CPU share the same memory addresses.  In that case, ramsize
* of memory will be allocated for the i82596 use.
*
 dc.l 0x00000000	u_int32		ramaddr;	/* base address for i82596 shared ram */
 dc.l 0x00000000	u_int32		ramoffset;	/* offset in window for i82596 */
 dc.l 0x00000000	u_int32		ramadjust;	/* NOT USED */
 dc.l 128*1024		u_int32		ramsize;	/* size of ram for i82596 */
 dc.l 0xff070000	u_int32		idpromaddr;	/* address of ID prom */
 dc.w 8			u_int16		recv_mbufs;	/* max. receive mbufs to use */
 dc.w 0x00		u_int16		port_off;	/* offset (from device addr) to port access address */
 dc.w 0x04		u_int16		ca_off;		/* offset (from device addr) to channel attention address */
 dc.w 0x00		u_int16		irq_off;	/* offset (from device addr) to clear IRQ address */
 dc.b 4			u_int8		ramwidth;	/* 2=D16 (word) ram, 4=D32 (longword) ram */
 dc.b 4			u_int8		portwidth;	/* i82596 port access width */
 dc.b 1			u_int8		ca_width;	/* i82596 channel access width */
 dc.b %01010100	u_int8		sysbus;		/* i82596 SCP sysbus value */
*       |^^^^^
*       ||||||
*       ||||\-\---- linear mode (never change)
*       |||\------- external/internal bus throttle trigger
*       ||\-------- lock disable/enable
*       \\--------- IRQ pin active low/high
*        ---------- Must be 1 (Intel Erratta)
