/*--------------------------------------------------------------------------!
! diskboot.c: read an OS-9 bootfile from a disk drive						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 88/11/03 Hacked into being from the ether						jal	!
!   01 89/08/04 1) Fixed error exit to not "insert" memory that wasn't	jal	!
!				extracted													!
!				2) fixed "tryboot" to exit with the error if the call	jal	!
!				   to "inizdriver" fails									!
!				3) fixed floppy formats loop to exit the first time		jal	!
!				   if it gets an "E_NOTRDY" error from "tryboot"		jal	!
!   02 90/03/20 Modified to handle both FD booting and deblocking of        !
!               of disks which have logical sectors sizes other than        !
!               the physical sector size                                    !
!   02 90/04/19 Found variable init problem in getboot                  Rwb !
!	03 90/06/20 Removed ambiguous setting of allocation size.			wwb	!
!   04 90/06/20 Modified FD boot to rely properly on FD termination     Rwb !
!   05 90/07/25 Added deblocking code                                   mjd !
!               ---- OS-9/68K V2.4 released ----							!
!   06 90/09/19 tryboot() now correctly updates total cylinders in          !
!                 the options section of the path descriptor            Rwb !
!               ---- OS-9/68K V2.4 DevPak released ----						!
!               ---- OS-9/68K V2.4.3 released ----							!
!	07 92/08/07 fixed problems from new defs.							wwb	!
!               ---- OS-9/68K V3.0 released ----							!
!   08 96/07/11 Added variable sector size support to ram disk booter.  ats !
!               Also added error returns for early tests in getboot().      !
!               Added non-prompting bootramdisk() service.                  !
!               ---- OS-9/68K V3.0.3 released ----							!
!               ---- OS-9/68K V3.1 released ----							!
!---------------------------------------------------------------------------!
!                                                                           !
!        Copyright 1990, 1992, 1996 by Microware Systems Corporation        !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
#include <rbf.h>

extern	char		getdefmat();
extern	error_code	tryboot();
extern	error_code	getboot();

#define SPINCOUNT	4000		/* Hard drive spin-up delay value	*/
#define SIXTYFOURK	65536		/* number of bytes in 64k           */

/*--------------------------------------!
! Static storage variable declarations	!
!--------------------------------------*/
Sect_zero		sectzero;		/* sector zero pointer					*/
u_char			*pathbuf;		/* read buffer pointer					*/
int				drivenum;		/* logical drive number					*/
struct rbf_opt	pathopts;		/* pseudo path descriptor options		*/
struct rbf_opt	*defopts;		/* default path descriptor options ptr	*/
struct rbfdrive	drivetable;		/* the pseudo drive table				*/
struct cmdblk	cmdblk;			/* high/low level SCSI command block	*/
								/* communications structure				*/
u_char          *fdptr,			/* pointer to the FD buffer             */
                *dbptr;			/* pointer to the deblock buffer        */

#define MAXFMATS	7	/* max number of floppy drive formats supported	*/

fmat_id	format[MAXFMATS] = {
	{ "Universal", 0x20, 3, 79, 16, 16, 1, 1 }, /* 79 trk M'ware Universal	*/
	{ "58W7", 0x20, 3, 80, 16, 16, 0, 1 },	/* 80 track Heurikon/UW/PEP		*/
	{ "5803", 0x00, 3, 80, 16, 10, 0, 0 },	/* 80 track Microware Standard	*/
	{ "5807", 0x20, 3, 80, 16, 16, 0, 0 },	/* 80 track Mizar/TLM/Hallock	*/
	{ "5403", 0x00, 1, 40, 16, 10, 0, 0 },	/* 40 track Microware Standard	*/
	{ "5407", 0x20, 1, 40, 16, 16, 0, 0 },	/* 40 track Mizar/TLM/Hallock	*/
	{ "8773", 0x01, 1, 77, 28, 16, 0, 0 }	/* 8 inch Microware Standard	*/
};

char dskbtjicpad[3];			/* pad out storage to be long aligned */
	

/*--------------------------------------------------------------!
! diskboot: Read a bootfile from a hard drive or floppy disk	!
!--------------------------------------------------------------*/
error_code diskboot()
{
	register char		*msgptr = NULL;
	register Fmat_id	furmat;
	register char		defmat,			/* default format code	*/
						fmcode;
	register error_code	error;

	/*------------------------------------------!
	! Check for a null boot descriptor pointer	!
	!------------------------------------------*/
	if (defopts == NULL) {
		outstr("diskboot: No boot descriptor pointer available.\n");
		return E_BPNUM; /* no boot descriptor pointer	*/
	}

	/*------------------------------------------!
	! Copy boot descriptor to "pathopts" buffer	!
	!------------------------------------------*/
	if (defopts != &pathopts)
		pathopts = *defopts;
		
	/*--------------------------------------------------------------!
	! If a controller ID was passed on the command line, insert the	!
	! new I.D. into the "pathopts" copy of the boot descriptor		!
	!--------------------------------------------------------------*/
	if (ctrlr_id != 0xFF)
		pathopts.pd_ctrlrid = ctrlr_id;
		
	/*--------------------------------------------------!
	! If device is a floppy disk, try ALL the formats	!
	!--------------------------------------------------*/
	if ((pathopts.pd_typ & TYP_HARD) == 0) {
		outstr("Now trying default floppy format \(");
		if ((defmat = getdefmat()) == MAXFMATS)
			outstr("unknown");
		else outstr(format[defmat].fcode);
		outstr("\)...\n");
		if (((error = tryboot()) != SUCCESS) && (error != E_NOTRDY)) {
			furmat = &format[0];
			for (fmcode = 0; fmcode < MAXFMATS; fmcode++) {
				if (fmcode == defmat) {
					furmat++;
					if (fmcode++ == MAXFMATS)
						break;
				}
				pathopts = *defopts;
				outstr("Now trying floppy format \(");
				outstr(furmat->fcode);
				outstr("\)...\n");
				pathopts.pd_typ = furmat->type;
				pathopts.pd_dns = furmat->density;
				pathopts.pd_cyl = furmat->cyls;
				pathopts.pd_sct = furmat->sectors;
				pathopts.pd_t0s = furmat->sectors_t0;
				pathopts.pd_toffs = furmat->toffs;
				pathopts.pd_soffs = furmat->soffs;
				error = tryboot();
				if ((error == SUCCESS) || (error == E_NOTRDY))
					break;
				furmat++;
			}
		}
		
	/*----------------------------------------------------------!
	! If device is a hard drive or RAM disk, try it only once	!
	!----------------------------------------------------------*/
	} else return tryboot();
	return error;
}


/*----------------------------------------------------------------------!
! getdefmat: Determine what is the default format and return its code	!
!----------------------------------------------------------------------*/
char getdefmat()
{
	register char		fcount;
	register Fmat_id	furmat;

	furmat = &format[0];
	for (fcount = 0; fcount < MAXFMATS; fcount++) {
		if ((pathopts.pd_typ == furmat->type) &&
				(pathopts.pd_dns == furmat->density) &&
				(pathopts.pd_cyl == furmat->cyls) &&
				(pathopts.pd_sct == furmat->sectors) &&
				(pathopts.pd_t0s == furmat->sectors_t0) &&
				(pathopts.pd_toffs == furmat->toffs) &&
				(pathopts.pd_soffs == furmat->soffs))
			break;
		furmat++;
	}
	return fcount;
}


/*----------------------------------------------------------------------!
! tryboot: Attempt to read a bootfile from a hard drive or floppy disk	!
!----------------------------------------------------------------------*/
error_code tryboot()
{
	register struct rbf_opt	*sect0opts;		/* sector zero pd options ptr	*/
	register error_code 	(*funcptr)();
	register char			*msgptr = NULL;
	register error_code		error;
	register int16			spincount;
	u_int32					sectorsize;
	u_int32                 defsectsize = DEFSECTSIZE;

	hbuff = NULL;				/* make sure buffer pointer points nowhere	*/
	sectorsize = 0;				/* make sure sector buffer size is zero		*/

	if (inizdriver == (error_code (*)()) NULL) {
		outstr("No drive initialization function.\n");
		return E_BTYP;
	} else if ((error = (*inizdriver)()) != SUCCESS) {
		outstr("Can't initialize the drive.\n");
		return error;
	}

	drivetable.v_init = FALSE;
	drivetable.v_zerord = FALSE;
	if ((pathopts.pd_typ & TYP_HARD) == 0)
		drivetable.v_0.dd_fmt = (pathopts.pd_sid - 1) | (pathopts.pd_dns << 1);
	
	/*--------------------------------------------------------------!
	! read sector zero from the disk into the sector zero buffer	!
	!--------------------------------------------------------------*/
	sectorsize = pathopts.pd_ssize;		/* get possible sector zero size	*/
	/* grab enough memory to cover a single block read from any drive */
	if ((error = extract(&defsectsize, &hbuff)) != SUCCESS)
		return error;
	pathbuf = hbuff;

	/* If this is a hard disk, give it several tries to get up to
	 *  speed, but if it is a floppy, only give it one shot.
	 */			
	if ((pathopts.pd_typ & TYP_HARD) != 0) {
		for (spincount = SPINCOUNT; --spincount >= 0;) {
			if ((error = (*readdriver)(1, 0)) != E_NOTRDY)
				break;
		}
	} else error = (*readdriver)(1, 0);

	if (error == SUCCESS) {
		sectzero = (Sect_zero) pathbuf;
		if ((pathopts.pd_toffs == 0) || 
						(*((u_int32 *) sectzero->dd_sync) == SYNC_CRUZ)) {
				
			sect0opts = (struct rbf_opt *) &sectzero->dd_opt[0];
			if ((pathopts.pd_typ & TYP_HARD) != 0) {
				pathopts.pd_sid = sect0opts->pd_sid;
				pathopts.pd_cyl = ((*((u_char *) &sect0opts->pd_cyl)) << 8) +
									*((u_char *) &sect0opts->pd_cyl + 1);
				pathopts.pd_totcyls = pathopts.pd_cyl + 
									*((u_char *) &sect0opts->pd_toffs + 1);
				pathopts.pd_sct = (sectzero->dd_spt[0] << 8) 
									+ sectzero->dd_spt[1];
			} else {
				if (*((u_char *) &sect0opts->pd_sct + 1) != 0) {
					pathopts.pd_sct = *((u_char *) &sect0opts->pd_sct + 1);
					pathopts.pd_t0s = *((u_char *) &sect0opts->pd_t0s + 1);
				} else {
					pathopts.pd_sct = *(u_char *) &sect0opts->pd_sct;
					pathopts.pd_t0s = *(u_char *) &sect0opts->pd_t0s;
				}
			}	
			drivetable.v_init = FALSE;			/* flag drive uninitialized	*/
			
			/*----------------------------------!
			! Attempt to read in the bootfile	!
			!----------------------------------*/
			error = getboot();
		} else {
			if (*((u_int32 *) sectzero->dd_sync) == 0) {
				msgptr = "No sync code in sector zero.\n";
			} else msgptr = "Illegal sync code in sector zero.\n";
			error = E_BTYP;
		}
	} else msgptr = "Can't read sector zero.\n";

	/*----------------------------------!
	! print the error message (if any)	!
	!----------------------------------*/
	if (msgptr != NULL)
		outstr(msgptr);

	/*------------------------------------------!
	! De-initialize disk drive (ignore error)	!
	!------------------------------------------*/
	if (termdriver != (error_code (*)()) NULL)
		(*termdriver)();	
	
	/*------------------------------!
	! return the sector zero buffer	!
	!------------------------------*/
	if ((hbuff != NULL) && ( sectorsize != 0)) {
		insert(defsectsize, hbuff);
		hbuff = NULL;
	}

	return error;
}


/*----------------------------------------------!
! getboot: Get the bootfile from any OS-9 disk	!
!----------------------------------------------*/
/*--->  This routine is entirely rewritten for ed.2.                   <---*/
/*--->     We now determine a deblocking factor and call a deblocking  <---*/
/*--->     read call.  We also allow for the boot to be defined by an  <---*/
/*--->     FD rather than block 0.                                     <---*/
error_code getboot()
{
	register u_int32	sectorno,		/* base sector number to read		*/
						sectcount;		/* bootfile size in sectors			*/
	register Fdsl_entry fd_seg;			/* file segment entry pointer		*/
	register char		*msgptr = NULL;	/* error message pointer			*/
	register int32      nread;			/* number of bytes read             */
	u_int32				sectsize,		/* physical sector size (power of 2)*/
	      				sectorsize,		/* physical sector size (actual)	*/
	                    lsnsize,		/* logical sector size              */
	                    btsect,			/* logical sector start of boot     */
	                    btsize,         /* boot size                        */
	                    allocsize;      /* size of the buffers to be alloced*/
    char                *allocedmem;	/* memory allocated for buffers     */
	int                 cvf;			/* convertion factor log -> phys    */
	int16				segcounter;		/* file segment counter				*/
	register error_code	error;			/* error code						*/
	extern error_code   callread();
	extern error_code	handle_read();
	extern u_int32      getvalue();

	allocsize = cvf = 0;			/* assume 1:1 l:p mapping, size = 0 */
	allocedmem  = NULL;				/* no memory allocated as yet */
	sectorsize = pathopts.pd_ssize;	/* physical sector size */

	do {
		if( (lsnsize = getvalue( sizeof(u_short), &sectzero->dd_lsnsize) )) {
			/* logical sector size is recorded create a convertion factor */
			if( lsnsize > sectorsize ) {
				if (*((u_int32 *) sectzero->dd_sync) == 0) {
					/* if not a 'CRUZ' disk, we're in trouble */
					msgptr = "Sector zero corrupted!\n";
					error = E_BTYP;
					break;
				} else
					sectorsize = lsnsize;
			}
		} else lsnsize = sectorsize;	/* default to physical sector size */
		
		/* now create a deblocking factor if nessesary */
		if( lsnsize != sectorsize ) {
			cvf = sectorsize / lsnsize;
			cvf = powerof2(cvf);
		}

		/* get the sector zero information about the boot */
		bootsize = btsize = getvalue( 2, &sectzero->dd_bsz );
		btsect = getvalue( 3, &sectzero->dd_bt[0] );
		if( (btsize == 0) && (btsect == 0) ) {
			/* no boot on this disk!!! */
			msgptr = "The bootfile is missing from this disk.\n";
			error = E_UNIT;
			break;
		}
			
		sectsize = powerof2(lsnsize);  /* convert sector size to power of 2 */

		/* try to allocate the buffers that are needed */
		/* get space for FD and deblocking buffers if later required */
		allocsize = sectorsize << 1;
		if( error = extract(&allocsize, &allocedmem) ) {
			msgptr = "Cannot allocate additional memory for bootstrap\n";
			break;
		}

		/* allocation worked, now set the pointers */
		fdptr = (u_char *)allocedmem;
		dbptr = (u_char *)allocedmem + sectorsize;

		if( btsize ) {	/* this is an old style boot, sector zero defines
			* both the start and the length.  It is also going to be
			* contiguous on the disk.  A single read call will be made to
			* get the entire bootfile into memory.
			*/
			sectcount = btsize >> sectsize;		/* integral logical sectors */
			if( (sectcount << sectsize) != btsize )
				sectcount++;		/* not an integral number of sectors */
			
			/* allocate the memory nessesary for the bootfile */
			if( error = getbootmem( sectcount << sectsize ) ) {
				msgptr = "Cannot allocate memory for the bootfile.\n";
				break;
			}
			
			pathbuf = bootram;		/* set pointer for the read */
			
			/* now read the bootfile into memory */
			if( error = handle_read( sectcount, btsect, cvf, lsnsize ) )
				msgptr = "Cannot read bootfile.\n";

		} else {	/* this is a new style boot.  In this case the 
			*  sector zero information is pointing to a file descriptor,
			*  which we will read in and then get the bootfile in parts.
			*/

			pathbuf = fdptr;		/* point to the fd buffer */
			if( error = handle_read( 1, btsect, cvf, lsnsize ) ) {
				msgptr = "Could not read bootfile FD.\n";
				break;
			}
			
			/* fd segment is now present in the buffer, allocate the
			 * nessesary memory for the bootfile.
			 */
			if( !(btsize = getvalue( 4, &((Rbf_fd)fdptr)->fd_siz[0] ))) {
				msgptr = "Bootfile found to have 0 size.\n";
				error = E_IBA;  /* return illegal block address */
			}
			
			bootsize = btsize;		/* preserve exact size for the system */

			sectcount = btsize >> sectsize;				/* logical sectors */
			if( (sectcount << sectsize) != btsize )
				sectcount++;		/* was not an integral of logical sectors */

			if( error = getbootmem( sectcount << sectsize ) ) {
				msgptr = "Could not allocate memory for the bootfile\n";
				break;
			}
			
			/* now read in the bootfile from the info in the bootfile's
			 *  FD sector.
			 */
			fd_seg = (Fdsl_entry) &((Rbf_fd)fdptr)->fd_seg;
			pathbuf = bootram;

			while( (sectcount = getvalue( 2, &fd_seg->fdsl_b[0]))) {
				sectorno = getvalue( 3, &fd_seg->fdsl_a[0] );
					/* if we are on the last segment, and must be sure that
					 *  we only read in the rounded up file size, and not
					 *  the full cluster size.
					 */
				if( (sectcount * lsnsize) > btsize ) {
					sectcount = btsize >> sectsize;		/* logical sectors */
					if( (sectcount << sectsize) != btsize )
						sectcount++;		/* was not an integral of logical sectors */
				}
				if( (error = handle_read(sectcount, sectorno, cvf, lsnsize)) ) {
						msgptr = "An error occurred reading the bootfile.\n";
						break;
				}
				nread = sectcount << sectsize;
				pathbuf += nread;
				btsize -= nread;
				fd_seg++;
			}
		}	/* end FD oriented boot */
	} while( 0 );
		
	if( msgptr )
		outstr( msgptr );		/* some kind of error message present */

	if( (int)allocedmem )
		insert(allocsize, allocedmem);	/* return memory to system */

	return error;
}

error_code handle_read(nsect, lsect, cvf, lssize )
u_int32 nsect;				/* number of logical sectors to read */
u_int32 lsect;				/* starting logical sector number */
register u_int32 cvf;		/* power of 2 convertion factor */
u_int32 lssize;				/* logical sector size */
{
	error_code error;
	u_int32 max_lsects;			/* maximum number of logical sectors */
	u_char *savbufp;			/* place to save pathbuf across subroutine */	
	
	/* save pathbuf throughout subroutine */
	savbufp = pathbuf;
	
	/* calculate max number of logical sectors to be sent at a time */
	max_lsects = (SIXTYFOURK - (lssize << cvf)) / lssize;
	
	while(1) {
		if (nsect > max_lsects) {
			if (error = callread(max_lsects, lsect, cvf, lssize))
				break;
			nsect -= max_lsects;  /* adjust remaining number of sectors */
			lsect += max_lsects;  /* adjust logical sector number */
			pathbuf += (max_lsects * lssize); /* update global pointer */
		} else {
			error = callread(nsect, lsect, cvf, lssize);
			break;
		}
	}
	pathbuf = savbufp;		/* restore buffer pointer */
	return error;
}

error_code callread(nsect, lsect, cvf, lssize )
u_int32 nsect;				/* number of logical sectors to read */
u_int32 lsect;				/* starting logical sector number */
register u_int32 cvf;		/* power of 2 convertion factor */
u_int32 lssize;				/* logical sector size */
{
	error_code error;
	u_int32 lsect_per_phys,		/* number of logical sectors/physical sector */
	        physect_to_read,	/* number of physical sectors requested */
	        physect,			/* physical sector number */
	        nread;				/* number of sectors read */
	u_char *buffptr;			/* place to save pathbuf across subroutine */
	register u_char *lbufp,*sptr;	/* local buffer and deblocking xfer pointer */

	/* if there is a 1:1 relationship between logical and physical sectors
	 *  we only need to make the read call and it will be done.
	 */
	if( !cvf )
		return (*readdriver)(nsect, lsect );
		
	/* the relationship is not 1:1, some deblocking will be nessesary */
	lsect_per_phys = 2 * cvf;		/* number of logical sectors/physical sector */
    lbufp = buffptr = pathbuf;			/* preserve for later use */
	
	/* this then is the loop that will read the requested sectors and
	 *  deblock them.
	 */
	while( nsect > 0 ) {
		physect = lsect >> cvf;			/* get starting physical sector */
		physect_to_read = nsect >> cvf;	/* estimated # of physical sectors */

		/* if logical sector is co-incident with a physical sector start and
		 *   at least one physical sector is to be read, then read directly
		 *   into the user's buffer.
		 */
		if( !(lsect % lsect_per_phys) && (physect_to_read > 1) ) {

			if( nsect % lsect_per_phys )
				physect_to_read--;	/* will be some left to read at end */
				
			pathbuf = lbufp;		/* set buffer to user's buffer for read */
			
			if( (error = (*readdriver)(physect_to_read, physect)) )
				break;				/* read failed */
				
			nread = physect_to_read << cvf;		/* number of logical sectors */
			lsect += nread;						/* adjust starting lsn */
			nsect -= nread;						/* adjust number to read */
			lbufp += physect_to_read * (lssize << cvf);
		} else {
			/* the logical sector start does not coincide with a physical
			 *   sector start, or something less than a full physical sectors
			 *   worth of data is to be read.
			 */
			register int xfercnt, cnt;		/* counters for buffer transfer */
			register int usable;			/* number of usable blocks read */
			u_int32 multip;

			pathbuf = dbptr;		/* set pointer to private buffer */

			if( (error = (*readdriver)(1,physect)) )
			break;				/* failed read */
			
			/* determine the number of usable sectors that have been read */
			multip = lsect - ((lsect >> cvf) << cvf);  /* lsn - lsn % ps */
			usable = lsect_per_phys - multip;
			
			/* set the deblocking pointer */
			sptr = dbptr + (u_int32)(lssize * multip);
			
			/* how many sectors to pull out of the buffer */
			if( usable > nsect )
				xfercnt = nsect;	/* less that full amount */
			else
				xfercnt = usable;	/* transfer all that is usable */

			lsect += xfercnt;		/* adjust the logical sector */
			nsect -= xfercnt;		/* adjust the number of sectors */
			
			/* transfer from buffer to the user's buffer.
			 *   NOTE:  local copy of buffer pointer is updated.
			 */
			while( --xfercnt >= 0 ) {
				cnt = lssize;		/* size to transfer */
				while( --cnt >= 0 )		
					*lbufp++ = *sptr++;
			}
		}	/* end of less than full sector to read */
	}	/* end of while sectors remain to be read */
	
	pathbuf = buffptr;		/* restore original buffer pointer */
	return error;			/* any error code is thus returned */
}

			
extern Sect_zero	rdiskbase;
extern error_code	readramdisk();
error_code			bootramdisk();
/*----------------------------------------------------------!
! rdiskboot: Read a bootfile from a non-volatile ramdisk	!
!----------------------------------------------------------*/
error_code rdiskboot()
{
	outstr("\n           NON-VOLATILE RAM DISK BOOT\n\n");
	outstr("1\) Enter \"Q\" to get back to the boot driver menu.\n");
	outstr("2\) Hit <RETURN> to use the default address. \($");
	Out4Hex(rdiskbase);
	outstr("\)\n");
	outstr("3\) Enter \(in hexadecimal\) the absolute address of your non-volatile RAM disk.\n");
	outstr("   ---> $");
	sectzero = gethexaddr();
	if (sectzero == (Sect_zero) ERROR2)
		return E_NOTRDY;
	if (sectzero == (Sect_zero) ERROR)
		sectzero = rdiskbase;
	return bootramdisk();
}

error_code bootramdisk()
{
	u_int32	sectorsize;
	
	readdriver = readramdisk;
	sectorsize = sectzero->dd_lsnsize;

	defopts = &pathopts;
	if (!(defopts->pd_ssize = sectorsize))
		defopts->pd_ssize = OSKSECTSIZE;
	else if ((sectorsize < 256) || (sectorsize > 32768))
		return E_SECTSIZE;
	else {
		u_int32 ssmask = 0x8000; /* sector size mask */
		/* Validate the sector size is a power of two. */
		do {
			if (!(sectorsize & ssmask))
				ssmask >>= 1;
			else if (sectorsize & ~ssmask)
				return E_SECTSIZE;
			else
				break;
		} while (ssmask);
		/* We should never really just fall out of the loop since
		   the other cases have already been caught. */
	}
	return getboot();
}

/*------------------------------------------------------!
! getvalue:  collect value from mis-aligned field       !
!------------------------------------------------------*/
u_int32 getvalue( cnt, cpt )
register int32 cnt;		/* number of characters to use to form value */
register u_char *cpt;	/* pointer to the field to be analyzed */
{
	register u_int32 value = 0;
	
	while( --cnt >= 0 ) {
		value <<= 8;
		value |= *cpt++;
	}
	return value;
}
		


/*------------------------------------------------------!
! readramdisk: read ramdisk OS-9 bootfile into memory	!
!------------------------------------------------------*/
error_code readramdisk(sectcount, basesect)
register u_int32		sectcount,
						basesect;
{
	register u_int32	*memaddr;
	register u_int32	*diskaddr;
	register u_int32	blocksize;
	register u_int32	sectorsize;
	char				wordflag = 0;

	sectorsize = pathopts.pd_ssize;
	memaddr = (u_int32 *) pathbuf;
	diskaddr = (u_int32 *) (((u_int32) sectzero) + (basesect * sectorsize));
	blocksize = (sectcount * sectorsize) / 4;
	if ((blocksize * 4) != sectcount * sectorsize)
		wordflag = 1;
	for  (; blocksize > 0; --blocksize)
		*memaddr++ = *diskaddr++;
	if (wordflag != 0)
		*(u_int16 *)memaddr++ = *(u_int16 *)diskaddr++;
	return SUCCESS;
}
