#include <stdio.h>
#include <errno.h>
#include <sgstat.h>


typedef int	error_code;

#define SUCCESS	0
#define ERROR	-1

#define STDIN	0
#define STDOUT	1
#define STDERR	2

#define FALSE	0
#define TRUE	1

char	*outfile;
int		devpath = STDOUT;

struct sgbuf	orgopts, hackopts;

/*--------------------------------------------------------------!
! "Sload" takes two arguments:									!
!	argv[0] = "sload" - the name of this utility.				!
!	argv[1] = the pathlist of the file to be sent (mandatory)	!
!	argv[2] = the pathlist of the output port (optional)		!
!			  if no output path is specified, STDOUT is used.	!
!--------------------------------------------------------------*/
main (argc, argv)
register int argc;
register char *argv[];
{
	register char **argvect = argv;
	register char *ptr;

	argvect++;
    while (--argc > 0) {
    	if ((*(ptr = *argvect++)) == '-') {
     		optparse(ptr);
		}
	}
	
	/*--------------------------------------------------!
	! Open a path to the output device (if specified)	!
	!--------------------------------------------------*/
	if (argv[2] != NULL) {
		if ((devpath = open(argv[2], _READ+_WRITE)) == ERROR)
			return senderr(errno);
	}
		
	if ((outfile = argv[1]) != NULL) {
		readfile();
	} else {
		printuse();
		exit(1, _errmsg("A pathlist MUST be specified.\n"));
	}
}	

readfile()
{
	register char	*buffer;
	int				fsize;
	int				dummy;
	int				fpath;
	int				iosize;
	
	/*----------------------------------------------------------!
	! reset the options on the I/O path to the target system	!
	!----------------------------------------------------------*/
	chgopts();

	/*------------------------------!
	! Open a path to the bootfile	!
	!------------------------------*/
	if ((fpath = open(outfile, _READ)) == ERROR)
		return senderr(errno);

	/*------------------------------!
	! Get the size of the bootfile	!
	!------------------------------*/
	if ((fsize = _gs_size(fpath)) == ERROR)
		return senderr(errno);

	/*--------------------------!
	! Allocate a buffer for I/O	!
	!--------------------------*/
	iosize = fsize;
	while ((buffer = malloc(iosize)) == NULL) {
		if ((iosize >> 1) == 0)
			return senderr(E_MEMFUL);
	}
	
	/*----------------------------------------------!
	! Send file size to target system and wait for	!
	! the target to acknowledge receipt of the size	!
	!----------------------------------------------*/
	write(devpath, &fsize, 4);
	read(devpath, &dummy, 1);

	/*------------------------------------------!
	! Send the bootfile to the target system	!
	!------------------------------------------*/		
	while (((dummy = read(fpath, buffer, iosize)) != 0) && (dummy != ERROR))
		write(devpath, buffer, dummy);
		
	/*------------------------------------------!
	! restore the options on the I/O path to	!
	! the target system and exit this program	!
	!------------------------------------------*/	
	_ss_opt(devpath, &orgopts);
	exit(0);
}


/*------------------------------------------------------------------!
! senderr: Send an error flag and error code to the target system	!
!------------------------------------------------------------------*/
senderr(error)
error_code	error;
{
	int	dummy = ERROR;

	/*--------------------------!
	! Send error flag to target	!
	!--------------------------*/
	write(devpath, &dummy, 4);

	/*------------------------------------------------------!
	! Wait for target to acknowledge receipt of the error	!
	!------------------------------------------------------*/
	read(devpath, &dummy, 1);
	
	/*--------------------------!
	! Send error flag to target	!
	!--------------------------*/
	write(devpath, &error, 4);

	/*------------------------------------------!
	! restore the options on the I/O path to	!
	! the target system and exit this program	!
	!------------------------------------------*/	
	_ss_opt(devpath, &orgopts);
	exit(0);
}


/*--------------------------------------------------!
! chgopts: Change the path options on the standard	!
!		   output path to facilitate downloading	!
!--------------------------------------------------*/
chgopts()
{	
	_gs_opt(devpath, &orgopts);
	hackopts = orgopts;
	hackopts.sg_echo = 0;
	hackopts.sg_alf = 0;
	hackopts.sg_pause = 0;
	hackopts.sg_eorch = 0;
	hackopts.sg_eofch = 0;
	hackopts.sg_psch = 0;
	hackopts.sg_kbich = 0;
	hackopts.sg_kbach = 0;
	hackopts.sg_xon	= 0;
	hackopts.sg_xoff = 0;
	_ss_opt(devpath, &hackopts);
}


optparse(ptr)
register char *ptr;
{
	while (*++ptr) {
		switch (*ptr) {
			case '?':
				printuse();
				exit(0);
				
			default:
				printuse();
				exit(*ptr == '?' ? 0:_errmsg(1,"unknown option '%c'\n",*ptr));
		}
	}
}

static char *cmds[] = {
     "Syntax: sload <filename> [<output path>]\n",
     "Function: dump a file out a serial port\n",
     "Options: none\n"
};

printuse()
{
	register char	**p = cmds,
					**cmdsend = cmds + sizeof cmds / sizeof (char *);
 
	while (p < cmdsend) fputs(*p++,stderr);
}

