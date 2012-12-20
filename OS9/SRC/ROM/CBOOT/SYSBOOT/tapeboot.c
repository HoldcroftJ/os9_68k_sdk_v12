/*--------------------------------------------------------------------------!
! tapeboot.c: read an OS-9 bootfile from a tape drive						!
+---------------------------------------------------------------------------+
!	Edition History:														!
!	#   Date    Comments												By	!
!	-- -------- ------------------------------------------------------- ---	!
!	00 88/11/03 Hacked into being from the ether						jal	!
!	01 89/01/12 Made compatible with 'C' multibooting "sysboot"			jal	!
!	02 89/08/04 1) fixed "tapeboot" to exit with the error if the call	jal	!
!				   to "inizdriver" fails									!
!				2) moved buffer allocation to after the "inizdriver" call	!
!				3) made memory return use the correct size and check for	!
!				   valid parameters											!
!               ---- OS-9/68K V2.4 released ----                            !
!	03 92/08/07 fixed problems from new defs.							wwb	!
!               ---- OS-9/68K V3.0 released ----                            !
!               ---- OS-9/68K V3.1 released ----                            !
!---------------------------------------------------------------------------!
!                                                                           !
!           Copyright 1990, 1992 by Microware Systems Corporation           !
!                          Reproduced Under License                         !
!                                                                           !
!  This source code is the proprietary confidential property of Microware   !
!  Systems Corporation, and is provided to licensee for documentation and   !
!  educational purposes only. Reproduction, publication, or distribution    !
!  in any form to any party other than the licensee is strictly prohibited. !
!                                                                           !
!--------------------------------------------------------------------------*/

#include <sysboot.h>
#include <tapehead.h>

extern	error_code	checkhead();

/*------------------------------------------------------------------!
! tapeboot: Get an OS-9 bootfile off of a legitimate OS-9 boot tape	!
!------------------------------------------------------------------*/
error_code tapeboot(blocksize)
register u_int32			blocksize;
{
	register u_int32		blockcount,
							bootposn;
	register Tape_header	tpheader;
	char					*msgptr = NULL;
	u_int32					headersize;
	register error_code		error;
	
	/*--------------------------!
	! Initialize the tape drive	!
	!--------------------------*/
	if ((error = (*inizdriver)()) != SUCCESS) {
		outstr("tapeboot: Can't initialize the tape drive.\n");
		return error;
	}

	/*------------------------------!
	! allocate tape header buffer	!
	!------------------------------*/
	hbuff = NULL;		
	headersize = blocksize;
	if ((error = extract(&headersize, &hbuff)) != SUCCESS)
		return error;
	blocksize = powerof2(blocksize);	/* convert block size to power of 2	*/

	/*----------------------------------!
	! Rewind the tape to the beginning	!
	!----------------------------------*/
	outstr("Now rewinding to the beginning of the tape.\n");
	if ((error = (*rwnddriver)()) == SUCCESS) {
	
		/*----------------------------------------------------------!
		! read the header file from the tape into the header buffer	!
		!----------------------------------------------------------*/
		tpheader = (Tape_header) hbuff;
		outstr("Now reading the tape header.\n");
		if ((error = (*readdriver)(1, 1, tpheader)) == SUCCESS) {

			/*----------------------------------!
			! Check integrity of tape header	!
			!----------------------------------*/
			if ((error = checkhead(tpheader)) == SUCCESS) {

				/*----------------------------------------------!
				! get bootfile size and position, and make sure	!
				! there is one									!
				!----------------------------------------------*/
				if (((bootsize = tpheader->th_bootentry.fe_size) != 0)
						&& ((bootposn = tpheader->th_bootentry.fe_posn) != 0)) {

					blockcount = bootsize >> blocksize;
					if ((blockcount << blocksize) != bootsize)
						blockcount++;
						
					/*-----------------------------------!
					! allocate RAM needed (if necessary) !
					!-----------------------------------*/
					if ((error = getbootmem(blockcount << blocksize)) == SUCCESS) {

						/*----------------------------------------------!
						! read OS-9 bootfile into memory				!
						!----------------------------------------------*/
						outstr("Now reading the OS-9 bootfile into memory.\n");
						if ((error = (*readdriver)(bootposn, blockcount,
													  bootram)) != SUCCESS)
							msgptr = "tapeboot: Can't read the bootfile.\n";
								
					} else msgptr = "tapeboot: Can't allocate memory for the bootfile.\n";
				} else {
					msgptr = "tapeboot: There is no OS-9 bootfile on this tape.\n";
					error = E_BTYP;
				}
			} else msgptr = "tapeboot: The tape has a bad header.\n";
		} else msgptr = "tapeboot: Can't read the tape header.\n";
	} else msgptr = "tapeboot: Can't rewind the tape.\n";
	
	/*----------------------------------!
	! print the error message (if any)	!
	!----------------------------------*/
	if (msgptr != NULL)
		outstr(msgptr);

	/*------------------------------------------!
	! Rewind tape to beginning (ignore error)	!
	!------------------------------------------*/
	(*rwnddriver)();

	/*------------------------------------------!
	! De-initialize tape drive (ignore error)	!
	!------------------------------------------*/
	(*termdriver)();

	/*------------------------------------------------------!
	! If an error occurred and memory was allocated for a	!
	! bootfile, return the bootfile memory to the system	!
	!------------------------------------------------------*/
	if ((error != SUCCESS) && (bootram != NULL) && (memsize != 0)) {
		insert(memsize, bootram);
		bootram = NULL;
		memsize = 0;
	}
	
	/*----------------------------------------------!
	! Return the tape header buffer to the system	!
	!----------------------------------------------*/
	if ((hbuff != NULL) && (headersize != 0)) {
		insert(headersize, hbuff);
		hbuff = NULL;
	}
	
	return error;
}


/*------------------------------------------!
! checkhead: Check tape header integrity	!
!------------------------------------------*/
error_code checkhead(tpheader)
register Tape_header	tpheader;
{
	/*------------------------------!
	! for sync code in the header	!
	!------------------------------*/
	if (tpheader->th_sync != SYNCCODE)
		return E_BADHEADER;
	return SUCCESS;
}
