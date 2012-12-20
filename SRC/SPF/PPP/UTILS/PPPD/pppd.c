/****************************************************************************
 *           Copyright 1989-1998 by Microware Systems Corporation           *
 *                           All Rights Reserved                            *
 *                         Reproduced Under License                         *
 *                                                                          *
 * This software is confidential property of Microware Systems Corporation, *
 * and is provided under license for internal development purposes only.    *
 * Reproduction, publication, distribution, or creation of derivative works *
 * in any form to any party other than the licensee is strictly prohibited, *
 * unless expressly authorized in writing by Microware Systems Corporation. *
 *                                                                          *
 ****************************************************************************
 *
 * Edition History:
 *
 * #   Date     Comments                                                By
 * --- -------- -----------------------------------------------------  ---
 *  01 01/10/99 Created                                                rds
 *  30 01/10/99 Bump the edition number above the old pppd (21)        rds
 *  31 12/15/00 Added more options to terminate, set options etc.      sr    *
 *  32 12/26/00 Fixed the -p option.                                   sr    *
 *     01/09/01 Modified usage function.                               sr    *
 *  33 01/15/01 Fixed bug with parsing -c & -d options                 sr    *
 *     02/12/01 Fix usage.                                             sr    *
 *  34 04/18/01 Modify to allow multiple debug levels using the -g     sr    *
 *              param many times.                                            *
 ****************************************************************************/

_asm("_sysedit: equ 34");

/*
** Header Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <SPF/ppplib.h>
#include "pppd.h"

/*
** Definitions
*/
#define MAX_ENTRIES 10
#define MAX_ERRORS 10
#define SIG_BASE		0x1000
#define SIG_LCP_UP		(SIG_BASE + 1)
#define SIG_LCP_DOWN	(SIG_BASE + 2)
#define SIG_IPCP_UP		(SIG_BASE + 3)
#define SIG_IPCP_DOWN	(SIG_BASE + 4)
#define SIG_LCP_FINISH	(SIG_BASE + 5)
#define SIG_IPCP_FINISH	(SIG_BASE + 6)
#define SIG_PPPD_KILL	(SIG_BASE + 7)
#define SIG_PPPD_HANGUP	(SIG_BASE + 8)

#define DEF_STACK "/hdlc0/lcp0/ipcp0"
#define DEF_CHATDEV "/hdlc0"
#define DEBUG_LEVEL1 1
#define DEBUG_LEVEL2 2

/*
** Function Prototypes
*/
void sighandler(int sig);
void intercept();
int getopt();
void usage(void);
error_code connection(void);
error_code display_options(ppp_option_block *);
error_code display_parameters(ppp_param_block *);
error_code display_asynch_params(ppp_modem_p *);
error_code get_n_set_options(path_id);

error_code disconnect(void);
void wait_for_exit(void);
void dump_conninfo(Ppp_conninfo);

/*
** Global Variables
*/
auth_handle amod;
char StackName[TMP_BUF_SIZE] = "";
char *ChatFile = NULL;
char *ChatDev = DEF_CHATDEV;
char *PeerName = NULL;
path_id stack_path;
ppp_conninfo conn_info={0};
ppp_error errors[MAX_ERRORS];
int RunChatScript = 0;
int SigReceived;

void sighandler(int sig)
{
	/*
	** Just save the signal and do all required processing outside
	** of the signal handler.
	*/
	SigReceived = sig;
}

void parse_args( int argc, char* argv[] )
{
	char*	arg;

	while( --argc )
	{
		arg = *++argv;

		if( *arg == '-' )
		{
			while( arg && *++arg != '\0' )
			{
				switch( tolower(*arg) )
				{
				case '?':
					usage();
					exit(SUCCESS);

				case 'c':
					if( *(arg+1) == '=' )
					{
						ChatFile = arg + 2;
						printf("ChatFile [%s]\n", ChatFile);
						arg = NULL;
					}
					break;
				case 'd':
					if( *(arg+1) == '=' )
					{
						ChatDev = arg + 2;
						printf("ChatDev [%s]\n", ChatDev);
						arg = NULL;
					}
					break;

				case 'g':
					/* Increment debug to allow multiple debug levels. */
					debug++;
					break;

				case 'h':
					show_set_cmds( arg );
					break;

				case 'i':
					if( *(arg+1) == '=' )
					{
						if( dmn_index )
						{
							printf("-i can only be specified once. \n");
							usage();
						}

						dmn_index = atoi( arg+2 );
						arg = NULL;
					}
					break;

				case 'k':
					if( killd )
					{
						printf("-k and -x are mutually exclusive options. \n");
						usage();
						exit(E_PARAM);
					}

					killd = 1;
					break;

				case 'l':
					listd = 1;
					break;

				case 'p':
					if( *(arg+1) == '=' )
					{
						PeerName = arg+2;
						printf("PeerName [%s]\n", PeerName);
						arg = NULL;
					}
					break;

				case 't':
					hangup = 1;
					break;

				case 'v':
					verbose = 1;
					break;

				case 'x':
					if( killd )
					{
						printf("-k and -x are mutually exclusive options. \n");
						usage();
						exit(E_PARAM);
					}

					killd = 2;
					break;

				case 'z':
					if( *(arg+1) == '=' )
					{
						check_cmds();
						cmd_file = arg+2;
						arg = NULL;
					}
					else
					{
						check_cmds();
						cmd_path = stdin;
					}
					break;

				default:
					usage();
					exit(E_PARAM);
				}
			}
		}

		else	/* name of protocol stack */
		{
			if (strpbrk(arg, "/")) {
				strcpy(StackName, arg);
			} else { /* Append default */
				strcat(StackName, "/");
				strcat(StackName, arg);
				strcat(StackName, DEF_STACK);
			}
			stack_name = StackName;
		}
	}
}

void initialize( void )
{
	u_int16		prior, age, grp, usr;
	int32		sched;
	error_code	err;

	/* Get our process ID */

	err = _os_id( &proc_id, &prior, &age, &sched, &grp, &usr );
	show_error( "_os_id", err );

	/* Set up the signal handler. */

	intercept(sighandler);

	/* mask signals until we sleep */
	_os_sigmask(1);
}

void link_stackmod( char* name )
{
	u_int16		ar = 0, tl = 0;
	error_code	err;

	if( ! name )
	{
		printf( "Bad module name in link_stackmod() \n" );
		exit(1);
	}

	err = _os_link( &name, (mh_com**) &stack_mod, (void**) &pppd_stacks,
						&tl, &ar );

	if( err == E_MNF )	/* No such module - create it */
	{
		u_int32	size = sizeof(pppd_paths);

		ar = (MA_REENT << 8);
		tl = (MT_DATA << 8);

		if( debug ) printf( "link_stackmod: size = %x \n", size );

		err = _os_datmod( name, size, &ar, &tl, 0x777,
							(void**) &pppd_stacks, (mh_data**) &stack_mod );

		if( err == SUCCESS )
		{
			/* Initialize the data module */

			init_stacks();

		}
		else
		{
			printf( "Can't create module %s \n", name );
			show_error( "_os_datmod", err );
		}
	}
	else if( err != SUCCESS )
	{
		printf( "Can't link to module %s \n", name );
		show_error( "_os_link", err );
	}
}

void check_stackmod( void )
{
	Pppd_stack	stack;

	/* Check for old entry with same process ID.
	 * This would indicate a daemon was killed by
	 * someone other than pppd.
	 */
	
	stack = find_stack( NULL, proc_id );

	if( stack )
	{
		del_stackname( stack );
	}

	/* Now check for an entry with same exact stack name. */

	stack = find_stack(stack_name, 0 );

	if( stack )
	{
		printf( "Stack %s already opened by process %u \n",
				stack->stack_name, stack->proc_id );
			
		exit(1);
	}

	add_stackname();
}

Pppd_stack find_stack( char* name, process_id id )
{
	int			i;
	Pppd_stack	stack;

	for( i = 0; i < MAX_PATHS; i++ )
	{
		stack = &(pppd_stacks->stacks[i]);

		if( stack->flags.in_use )
		{
			if( name && strcmp( stack->stack_name, stack_name ) == 0 )
				return( stack );

			if( id && stack->proc_id == id )
				return( stack );
		}
		else if( name == NULL && id == 0 )
			return( stack );
	}

	return( NULL );
}

Pppd_stack get_stack( void )
{
	Pppd_stack	stack;

	if( dmn_index > 0 )
	{
		u_int32     i = (u_int32) (dmn_index-1);

		if( i < MAX_PATHS )
		{
			stack = &(pppd_stacks->stacks[i]);
			return( stack );
		}
	}

	if( stack_name )
	{
		stack = find_stack( stack_name, 0 );
		return( stack );
	}

	return( NULL );
}

void set_index( void )
{
	int			i;
	Pppd_stack	stack;

	if( stack_name )
	{
		for( i = 0; i < MAX_PATHS; i++ )
		{
			stack = &(pppd_stacks->stacks[i]);

			if( stack->flags.in_use )
			{
				if( strcmp( stack->stack_name, stack_name ) == 0 )
				{
					dmn_index = (i+1);
					return;
				}
			}
		}
	}

	printf( "Invalid stack name (%s) \n", stack_name );
	exit( E_PARAM );
}

void set_stackname( void )
{
	u_int32		i = (u_int32)(dmn_index-1);
	Pppd_stack	stack;

	if( i < MAX_PATHS && pppd_stacks->stacks[i].flags.in_use )
	{
		stack = &(pppd_stacks->stacks[i]);
		stack_name = stack->stack_name;
	}

	printf( "Invalid index (%u) \n", dmn_index );
	exit( E_PARAM );
}

void add_stackname( void )
{
	Pppd_stack	stack;

	if( debug ) printf( "Adding stack %s. \n", stack_name );

	if( pppd_stacks->num_entries >= MAX_PATHS )
	{
		printf( "Maximum number of paths (%u) already opened. \n", MAX_PATHS );
		exit( E_PTHFUL );
	}

	/* Find an empty entry. */

	stack = find_stack( NULL, 0 );

	/* Write stack string into data module */

	if( stack )
	{
		strcpy( stack->stack_name, stack_name );

		stack->proc_id		= proc_id;
		stack->flags.in_use	= 1;

		pppd_stacks->num_entries++;
	}

	if( debug )
	{
		list_stacks();
	}
}

void del_stackname( Pppd_stack stack )
{
	if( stack && stack->flags.in_use )
	{
		memset( stack, 0, sizeof(pppd_stack) );
		pppd_stacks->num_entries--;
	}
}

void del_stackid( process_id proc_id )
{
	Pppd_stack	stack;

	stack = find_stack( NULL, proc_id );
	del_stackname( stack );
}

void init_stacks( void )
{
	Pppd_stack	stack;
	int			i;

	memset( pppd_stacks, 0, sizeof(pppd_paths) );

	for( i = 0; i < MAX_PATHS; i++ )
	{
		stack = &(pppd_stacks->stacks[i]);
		stack->index = (i + 1);
	}
}

void list_stacks( void )
{
	Pppd_stack	stack;
	int			i;

	printf( "Num Stack Name                    Process ID  \n" );
	printf( "--------------------------------------------- \n" );
/*  printf( "00  12345678901234567890123456789012  00      \n" ); */

	for( i = 0; i < MAX_PATHS; i++ )
	{
		stack = &(pppd_stacks->stacks[i]);

		if( stack->flags.in_use )
		{
			int is_pppd = check_proc_desc( stack->proc_id );

			if( is_pppd )
			{
				printf( "%02u  %-32s  %02u \n", stack->index, 
						stack->stack_name, stack->proc_id );
			}
			else
			{
				del_stackname( stack );
			}
		}
	}

	printf ("\n" );
}

int check_proc_desc( process_id pid )
{
	u_int32		count;
	error_code	err;
	
	if( debug ) printf( "check_proc_desc: Proc_id = %u. \n", pid );

	count = sizeof(pr_desc);
	err = _os_gprdsc( pid, &pppd_desc, &count );

	if( err ) printf( "_os_gprdsc() returned %x. \n", err );

	else if( count )
	{
#ifdef _OS9000
		if( pppd_desc.p_pmodul )
		{
			char*	name;

			name = (char*) pppd_desc.p_pmodul;
			name += pppd_desc.p_pmodul->m_name;

			if( strcmp( name, "pppd" ) == 0 )
			{
				return( 1 );
			}
		}
#else	/* OSK */
		return( 1 );
#endif
	}

	else
		printf( "_os_gprdsc() returned zero bytes. \n" );

	return( 0 );
}

error_code main(int argc, char *argv[])
{
	path_id ChatPath;
	error_code err;

	/*
	** initialize connection information
	*/
	memset(errors, 0, sizeof(errors));
	conn_info.max_errors = MAX_ERRORS;
	conn_info.error_array = errors;
	conn_info.sig_lcp_up = SIG_LCP_UP;
	conn_info.sig_lcp_down = SIG_LCP_DOWN;
	conn_info.sig_ipcp_up = SIG_IPCP_UP;
	conn_info.sig_ipcp_down = SIG_IPCP_DOWN;
	conn_info.sig_lcp_finish = SIG_LCP_FINISH;
	conn_info.sig_ipcp_finish = SIG_IPCP_FINISH;

	initialize();

	parse_args(argc, argv);

	/* Link to our data module (internal use only!) */

	link_stackmod( "pppd_stacks" );

	if( listd )		/* User wants list of current pppd's running */
	{
		list_stacks();
		exit(1);
	}

	if (killd == 2) { /* Terminate all pppds running */
		int			i;
		Pppd_stack	stack;

		for( i = 0; i < MAX_PATHS; i++ )
		{
			stack = &(pppd_stacks->stacks[i]);

			if( stack->flags.in_use )
			{
				int is_pppd = check_proc_desc( stack->proc_id );

				if( is_pppd )
				{
					fprintf(stderr, "Killing daemon process %u. \n", stack->proc_id );
					kill( stack->proc_id, SIG_PPPD_KILL );
					del_stackname( stack );
				}
			}
		}
		exit(SUCCESS);
	}

	if ((stack_name == NULL) && (dmn_index == 0)) {
		fprintf(stderr, "Error - no index or descriptor/stack name specified.\n");
		usage();
		exit(E_BNAM);
	}

	if( killd || hangup )	/* User wants to kill or hangup specific pppd */
	{
		Pppd_stack	stack;

		do
		{
			stack = get_stack();

			if( stack && stack->flags.in_use )
			{
				if( hangup )
				{
					fprintf(stderr,  "Sending hangup signal to daemon process %u. \n",
							stack->proc_id );
					kill( stack->proc_id, SIG_PPPD_HANGUP );
					exit( SUCCESS );
				}

				if( killd )
				{
					int is_pppd = check_proc_desc( stack->proc_id );

					if( is_pppd )
					{
						fprintf(stderr, "Killing daemon process %u. \n", stack->proc_id );
						kill( stack->proc_id, SIG_PPPD_KILL);
						del_stackname( stack );
						exit( SUCCESS );
					}

				}
			}

		} while( stack );

		fprintf(stderr,  "Unable to locate original daemon for %s. \n",
		                                                      stack_name );
		exit( E_MNF );

	}


	if (stack_name == NULL) {
		fprintf(stderr, "Error - Descriptor/stack name specified.\n");
		usage();
		exit(E_BNAM);
	}

	if (verbose) {
		fprintf(stdout, "PPP stack is [%s]\n", stack_name);
	}

	if (ChatFile) {
		if (err = _os_open(ChatFile,FAM_READ,&ChatPath)) {
			fprintf(stderr,"Chat script [%s] not found\n", ChatFile);
			exit(err);
		} else {
			_os_close(ChatPath);
		}
	}

	if (err = ppp_init(NULL)) {
		fprintf(stderr,"ERROR: ppp_init = %s\n", strerror(err));
		exit(err);
	}

	/* make sure this stack not already opened */

	check_stackmod();

	set_index();

	if ((err = connection()) != SUCCESS) {
		ppp_term(NULL);

		/* other cleanup? */
		exit(err);
	}

	wait_for_exit();
	disconnect();
	return(SUCCESS);
}

void usage(void)
{
#define FP fprintf(stderr,
	FP "Syntax: pppd [<options>] <stack_name> [<parameters>]\n");
	FP "Function: Setup point-to-point connection\n");
	FP "Options\n");
	FP "   -c=<name>  Run the chat script located in <name>\n");
	FP "              <name> may either be a disk file or data module\n");
	FP "   -d=<dev>   Use <dev> as the chat device (Default: %s)\n", DEF_CHATDEV);
	FP "              Requires the -c option\n");
	FP "   -i=<index> The ppp stack index number \n");
	FP "   -k         Terminate pppd session specified by the -i option\n");
	FP "   -p <name>  Select <name> in ppp_auth to be used for authentication\n");
	FP "              Equivalent to pppauth -h <name>\n");
	FP "   -v         Turn on verbose mode\n");
	FP "   -x         Terminate all pppd sesssions\n");
	FP "   -z         Read commands from stdin \n");
	FP "   -z=<name>  Read commands from file or data module \n");
	FP "  stack_name  Name of stack to open (If no / is specified,\n");
	FP "                  %s is automatically appended)\n", DEF_STACK);
	FP "  parameters  PPP stack configuration parameters\n");
#undef FP
}

error_code connection(void)
{
	error_code	err;
	path_id		ChatPath;

	if (PeerName) {
		if (err = ppp_auth_link_mod(&amod)) {
			fprintf(stderr, "Can't link to ppp_auth module\n");
			return(err);
		}

		if (err = ppp_auth_set_peer_name(PeerName, &amod)) {
			fprintf(stderr, "Can't select %s in ppp_auth module\n");
			return(err);
		}
	}

	if (err = ppp_open(stack_name, &stack_path)) {
		fprintf(stderr, "Can't open %s\n", stack_name);
		return(err);
	}

	if (err = get_n_set_options(stack_path)) {
		return (err);
	}

	if (ChatFile) {
		if (err = ppp_chat_open(ChatDev, &ChatPath)) {
			fprintf(stderr,"ERROR: ppp_chat_open = %s\n", strerror(err));
			return(err);
		}

		if (err = ppp_chat_script(ChatPath, PPP_NOLOG, PPP_CHAT_TYPE_FILE,
								ChatFile, &conn_info)) {
			fprintf(stderr,"ERROR: ppp_chat_script = %s\n", strerror(err));
		}

		if (debug > DEBUG_LEVEL1) {
			dump_conninfo(&conn_info);
			fprintf(stdout,"ppp_chat_close\n"); fflush(stdout);
		}

		ppp_chat_close(ChatPath);

		if (err) {
			if (verbose)
				fprintf(stderr, "Chat script %s failed\n", ChatFile);
			return(err);
		}
	}

	if (debug > DEBUG_LEVEL1) {
		dump_conninfo(&conn_info);
	}

	if (err = ppp_start(stack_path, &conn_info)) {
		fprintf(stderr,"ERROR: ppp_start = %s\n", strerror(err));
		return(err);
	}


	return(SUCCESS);
}

error_code get_n_set_options(path_id stack_path)
{
	error_code err;

	if (err = ppp_get_options(stack_path, &o_ppp_options)) {
		fprintf(stderr,"ERROR: ppp_get_options = %s\n", strerror(err));
		return(err);
	}

	memcpy( &n_ppp_options, &o_ppp_options, sizeof(ppp_option_block) );

	/* If this is an asynch link, get the asynch params.  For now we assume
	 * all connection are asynch
	 */

	if (err = ppp_get_asynch_params(stack_path, &o_asynch_params)) {
		fprintf(stderr,"ERROR: ppp_get_asynch_params = %s\n", strerror(err));
		return(err);
	}

	memcpy( &n_asynch_params, &o_asynch_params, sizeof(ppp_modem_p) );

	if (verbose) {
		fprintf(stdout, "\nRequested negotiable options\n");
		display_options(&o_ppp_options);
		fprintf(stdout, "\nAsynchronous Parameters\n");
		display_asynch_params(&o_asynch_params);
	}

	/* Parse the command file */

	if( cmd_path )
	{
		parse_stream( cmd_path );
	}

	else if( cmd_file )
	{
		if( use_datamod )
		{
			parse_datamod( cmd_file );
		}
		else
		{
			parse_file( cmd_file );
		}
	}

	/* See if the options/asynch params have to be updated in the stack. */

	update_drivers(stack_path);

	return (SUCCESS);
}


char *priorString[4] = { "UNKNOWN", "DEFAULT", "DESIRED", "REQUIRE" };
#define DO_OPT(parm) parm.priority < 4 ? priorString[parm.priority] : "?"

error_code display_parameters(ppp_param_block *opts)
{
	fprintf(stdout, "PPP Mode: 0x%x\n", opts->ppp_mode);

	fprintf(stdout, "IPCP Options:\n");
	fprintf(stdout, "  Incoming packets compressed: %s\n",
		opts->rx_ip_cproto? "Yes": "No");
	fprintf(stdout, "  Send compressed packets: %s\n",
		opts->rx_ip_cproto? "Yes": "No");
	fprintf(stdout, "  Incoming slot identifiers compressed: %s\n",
		opts->rx_ip_cproto? "Yes": "No");
	fprintf(stdout, "  Compress outgoing slot identifiers: %s\n",
		opts->rx_ip_cproto? "Yes": "No");
	fprintf(stdout, "  Number of incoming slot identifiers: %d\n",
		opts->rx_ipcp_mslot);
	fprintf(stdout, "  Number of outgoing slot identifiers: %d\n",
		opts->tx_ipcp_mslot);

	fprintf(stdout, "LCP Options:\n");
	fprintf(stdout, "  Receive async control character map: 0x%x\n",
		opts->rx_accm);
	fprintf(stdout, "  Transmit async control character map: 0x%x\n",
		opts->tx_accm[0]);
	fprintf(stdout, "  Incoming address/control fields compressed: %s\n",
		opts->rx_acfc? "Yes": "No");
	fprintf(stdout, "  Compress outgoing address/control fields: %s\n",
		opts->tx_acfc? "Yes": "No");
	fprintf(stdout, "  Incoming protocol fields compressed: %s\n",
		opts->rx_pfc? "Yes": "No");
	fprintf(stdout, "  Compress outgoing protocol fields: %s\n",
		opts->tx_pfc? "Yes": "No");
	fprintf(stdout, "  Maximum recieve unit (MRU): %d\n", opts->rx_mru);
	fprintf(stdout, "  Maximum transmit unit (MTU): %d\n", opts->tx_mru);


	fprintf(stdout, "Misc. Options:\n");
	fprintf(stdout, "  Local magic number: 0x%x\n", opts->local_magic);
	fprintf(stdout, "  Remote magic number: 0x%x\n", opts->remote_magic);
	fprintf(stdout, "  HDLC I/O enabled: %s\n", opts->hdlc_io_enabled? "Yes": "No");
	fprintf(stdout, "  LCP I/O enabled: %s\n", opts->lcp_io_enabled? "Yes": "No");
	fprintf(stdout, "  IPCP I/O enabled: %s\n", opts->ipcp_io_enabled? "Yes": "No");

	fprintf(stdout,"\n");
	fflush(stdout);

	return(SUCCESS);
}

error_code display_options(ppp_option_block *opts)
{
	fprintf(stdout, "PPP Mode: 0x%x\n", opts->ppp_mode);

	fprintf(stdout, "IPCP Options:\n");
	fprintf(stdout, "  Timeout: %5.2f seconds\n", opts->ipcp_timeout/1000.0);
	fprintf(stdout, "  Maximum configure requests to send: %d\n",
			opts->ipcp_max_configure);
	fprintf(stdout, "  Maximum terminate requests to send: %d\n",
			opts->ipcp_max_terminate);
	fprintf(stdout, "  Maximum configure nacks to send: %d\n",
			opts->ipcp_max_failure);
	fprintf(stdout, "  Accept IP address from peer: %s (%d)\n",
			DO_OPT(opts->ipcp_accept_local), opts->ipcp_accept_local.value);
	fprintf(stdout, "  Allow peer to pick its own IP address: %s (%d)\n",
			DO_OPT(opts->ipcp_accept_remote), opts->ipcp_accept_remote.value);
	fprintf(stdout, "  Receive compressed IP packets: %s (%d)\n",
			DO_OPT(opts->rx_ip_cproto), opts->rx_ip_cproto.value);
	fprintf(stdout, "  Compress slot identifiers: %s (%s)\n",
			DO_OPT(opts->rx_ipcp_cslot),
			opts->rx_ipcp_cslot.value? "Compression": "No Compression");
	fprintf(stdout, "  Number of requested slots: %s (%d)\n",
			DO_OPT(opts->rx_ipcp_mslot), opts->rx_ipcp_mslot.value);

	fprintf(stdout, "LCP Options:\n");
	fprintf(stdout, "  Timeout: %5.2f seconds\n", opts->lcp_timeout/1000.0);
	fprintf(stdout, "  Maximum configure requests to send: %d\n",
			opts->lcp_max_configure);
	fprintf(stdout, "  Maximum terminate requests to send: %d\n",
			opts->lcp_max_terminate);
	fprintf(stdout, "  Maximum configure nacks to send: %d\n",
			opts->lcp_max_failure);
	fprintf(stdout, "  Desired receive async control character map: 0x%x\n",
			opts->rx_accm.value);
	fprintf(stdout, "  Desired transmit async Control character map: 0x%x\n",
			opts->tx_accm.value[0]);
	fprintf(stdout, "  Recieve compressed address/control fields: %s (%s)\n",
			DO_OPT(opts->rx_acfc),
			opts->rx_acfc.value? "Compression": "No Compression");
	fprintf(stdout, "  Recieve compressed protocol fields: %s (%s)\n",
			DO_OPT(opts->rx_pfc),
			opts->rx_pfc.value? "Compression": "No Compression");
	fprintf(stdout, "  Maximum size packet to recieve: %s (%d bytes)\n",
			DO_OPT(opts->rx_mru), opts->rx_mru.value);
	fprintf(stdout, "  Challenge server for authentication: (Not Supported)\n");


	fprintf(stdout,"\n");
	fflush(stdout);

	return(SUCCESS);
}

error_code display_asynch_params(ppp_modem_p *opts)
{
	fprintf(stdout, "  Baud Rate: %d Baud\n", opts->baud_rate);
	fprintf(stdout, "  Parity: %d \n", opts->parity);
	fprintf(stdout, "  Word Size: %d \n", opts->word_size);
	fprintf(stdout, "  Stop Bits: %d \n", opts->stop_bits);
	fprintf(stdout, "  RTS Enable: %d \n", opts->rts_enable);

	fflush(stdout);

	return(SUCCESS);
}

void wait_for_exit(void)
{
	int done = 0;
	error_code err;
	u_int32 ticks;
	signal_code	sig;

	while(! done) {
		ticks = 0; sig = 0;
		_os_sleep(&ticks,&sig);

		switch(SigReceived) {
			case SIGHUP:
				/*
				** In this case we should drop the current connection and open
				** a new one (only if persist is set?), but for now just exit.
				**                                        RDS
				*/
			case SIGINT:
			case SIGTERM:
				if (debug)
					fprintf(stdout, "Received exit signal\n");
				done = 1;
				return;

			case SIG_LCP_UP:
				if (verbose)
					fprintf(stdout,"LCP layer is up\n");
				break;

			case SIG_LCP_DOWN:
				if (verbose)
					fprintf(stdout,"LCP layer is down\n");
				break;

			case SIG_IPCP_UP:
				if (verbose)
					fprintf(stdout,"IPCP layer is up\n");

				if (debug) {
					ppp_param_block params;

					if (err = ppp_get_params(stack_path, &params)) {
						fprintf(stderr,"ERROR: ppp_get_params = %s\n", strerror(err));
					} else {
						display_parameters(&params);
					}
				}
				break;

			case SIG_IPCP_DOWN:
				if (verbose)
					fprintf(stdout,"IPCP layer is down\n");
				break;

			case SIG_LCP_FINISH:
				if (verbose)
					fprintf(stdout,"LCP layer failed to [re]connect\n");
				break;

			case SIG_IPCP_FINISH:
				if (verbose)
					fprintf(stdout,"IPCP layer failed to [re]connect\n");
				break;

			case SIG_PPPD_KILL:
				if (verbose)
					fprintf(stdout,"Terminate Requested.  Exiting...\n");
				done = 1;
				return;

#if 0
			case SIG_PPPD_HANGUP:
				if (verbose)
					fprintf(stdout,"Hangup Requested.  Exiting...\n");
				done = 1;
				return;
#endif
			default :
				if (debug)
					fprintf(stdout,"Received unexpected signal number 0x%x\n", sig);
				break;
		}

		if (debug > DEBUG_LEVEL1)
			dump_conninfo(&conn_info);
	}
}


error_code disconnect(void)
{
	error_code	err;

	if (err = ppp_disconnect(stack_path)) {
		fprintf(stderr,"ERROR: ppp_disconnect = %s\n", strerror(err));
	}

	if (err = ppp_close(stack_path)) {
		fprintf(stderr,"ERROR: ppp_close = %s\n", strerror(err));
	}

	if (PeerName) {
		if (err = ppp_auth_unlink_mod(&amod)) {
			fprintf(stderr,"Can't unlink from ppp_auth module\n");
		}
	}

	if (err = ppp_term(NULL)) {
		fprintf(stderr,"ERROR: ppp_term = %s\n", strerror(err));
	}

	return(SUCCESS);
}

char *
layerText(u_int32 layer)
{
	if (layer == 0)
		return "NONE";
	else if (layer == PPP_LAYER_IPCP)
		return "IPCP";
	else if (layer == PPP_LAYER_LCP)
		return "LCP";
	else if (layer == PPP_LAYER_HDLC)
		return "HDLC";
	else if (layer == PPP_LAYER_CHAT)
		return "CHAT";
	return "????";
}

void
dump_conninfo(Ppp_conninfo ci)
{
	Ppp_error	cerr;
	u_int32 i;

	printf("Conninfo block:\n"
			"signals  lcp up=%d  lcp dwn=%d  lcp fin=%d\n"
			"        ipcp up=%d ipcp dwn=%d ipcp fin=%d\n",
			ci->sig_lcp_up, ci->sig_lcp_down, ci->sig_lcp_finish,
			ci->sig_ipcp_up, ci->sig_ipcp_down, ci->sig_ipcp_finish);
	printf("  errors flags=0x%x(%d) max=%d last=%d\n",
			ci->flags, ci->flags, ci->max_errors, ci->last_err);
	for (cerr = ci->error_array, i = 0; i < ci->max_errors; i++, cerr++) {
		printf("   [%2d]: 0x%2x(%s)	 0x%-8x 0x%-8x 0x%-8x\n",
			i, cerr->layer, layerText(cerr->layer),
			cerr->err_info.lncp.option,
			cerr->err_info.lncp.my_request,
			cerr->err_info.lncp.his_request);
	}
}

void update_drivers(path_id stack_path)
{
	error_code	err;

	if( memcmp( &o_asynch_params, &n_asynch_params, sizeof(ppp_modem_p) ) != 0 )
	{
		err = ppp_set_asynch_params(stack_path, &n_asynch_params );

		if( err )
		{
			printf( "Warning: Unable to write modem parameters. \n" );
			printf( "ppp_set_asynch_params() returned %x. \n", err );
		}

		else
		{
			ppp_get_asynch_params(stack_path, &o_asynch_params );
		}
	}

	if( memcmp( &o_ppp_options, &n_ppp_options, sizeof(ppp_option_block) ) != 0 )
	{
		err = ppp_set_options(stack_path, &n_ppp_options);

		if( err )
		{
			printf( "Warning: Unable to write ppp Options. \n" );
			printf( "ppp_set_options() returned %x. \n", err );
		}

		else
		{
			ppp_get_options(stack_path, &o_ppp_options);
		}
	}

}

void do_set_cmd( void )
{
	int		i = 0;
	char*	word;

	word =   next_token( NULL, " \t.,=\n" );

	while( set_cmds[i].token )
	{
		if( strnnccmp( word, set_cmds[i].token ) == 0 )
		{
			set_cmds[i].fn();
			return;
		}

		i++;
	}

	invalid_cmd();
}

void do_set_baud( void )
{
	char*	arg = next_token( NULL, " \t,=\n" );
	int		index = 0;
	double	value = 0.0;

#if defined(_OS9000)
	if( strnccmp( arg, "midi" ) == 0 )
	{
		n_asynch_params.baud_rate = BAUDRATE_MIDI;
		if( debug ) printf( "baud_rate = %u \n", n_asynch_params.baud_rate );
		return;
	}
#endif
	/* Make sure the argument is a valid baud rate */

	value = get_decnum_only( arg );

	if( value == 0.0 )
		invalid_cmd();

	while( br_lut[index].value > 0.0 )
	{
		if( value == br_lut[index].value )
		{
			n_asynch_params.baud_rate = br_lut[index].baud;
			if( debug ) printf( "baud_rate = %u \n", n_asynch_params.baud_rate );
			return;
		}

		index++;
	}

	invalid_cmd();
}

void do_set_flow( void )
{
	char*	arg;

	arg = next_token( NULL, " \t.,=\n" );

	if( strnnccmp( arg, "xon" ) == 0 )
	{
		n_asynch_params.rts_enable = 0;
	}
	else if( strnnccmp( arg, "rts" ) == 0 )
	{
		n_asynch_params.rts_enable = 1;
	}
	else
	{
		invalid_cmd();
	}

	if( debug ) printf( "rts_enable = %u \n", n_asynch_params.rts_enable );
}

void do_set_ipcp( void )
{
	char*	word = next_token( NULL, " \t.,=\n" );

	if( strnnccmp( word, "cslot" ) == 0 ) {

		n_ppp_options.rx_ipcp_cslot.value = get_integer();
		if( debug ) printf( "ipc_cslot = %u \n", n_ppp_options.rx_ipcp_cslot.value);

	} else if( strnnccmp( word, "mslot" ) == 0 ) {

		n_ppp_options.rx_ipcp_mslot.value = get_integer();
		if( debug ) printf( "ipc_mslot = %u \n", n_ppp_options.rx_ipcp_mslot.value );

	} else if( strnnccmp( word, "proto" ) == 0 ) {

		n_ppp_options.tx_ip_cproto.value = get_integer();
		if( debug ) printf( "ipc_proto = %u \n",
																	n_ppp_options.tx_ip_cproto.value );

	} else if( strnnccmp( word, "defaultroute" ) == 0 ) {

		n_ppp_options.ipcp_default_route = SET_DEFAULT_ROUTE;
		if( debug ) printf( "ipc_def_route = %u \n",
		                                 n_ppp_options.ipcp_default_route );

	} else if ( strnnccmp( word, "accept-remote" ) == 0 ) {

		n_ppp_options.ipcp_accept_remote.value = get_integer();
		if(debug) printf("ipc_accept_remote = %u \n",
			                      n_ppp_options.ipcp_accept_remote.value);

	} else if ( strnnccmp( word, "accept-local" ) == 0 ) {

		n_ppp_options.ipcp_accept_local.value = get_integer();
		if( debug ) printf("ipc_accept_local = %u \n",
		                        n_ppp_options.ipcp_accept_local.value);

	} else if ( strnnccmp( word, "scr" ) == 0 ) {

		n_ppp_options.ipcp_max_configure = get_integer();
		if( debug ) printf( "max_scr = %u \n", n_ppp_options.ipcp_max_configure );

	} else if ( strnnccmp( word, "str" ) == 0 ) {

		n_ppp_options.ipcp_max_terminate = get_integer();
		if( debug ) printf( "max_str = %u \n", n_ppp_options.ipcp_max_terminate );

	} else if ( strnnccmp( word, "timeout" ) == 0 ) {

		n_ppp_options.ipcp_timeout = get_integer();
		if( debug ) printf( "IPCP timeout = %u \n", n_ppp_options.ipcp_timeout );

	} else {
		invalid_cmd();
	}
}

void do_set_mode( void )
{
	char*	word;

	n_ppp_options.ppp_mode = 0;

	do
	{
		word = next_token( NULL, " \t,=\n" );

		if( word )
		{
			if( strnnccmp( word, "nowait" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_WAIT_ON_OPEN;
			}
			else if( strnnccmp( word, "passive" ) == 0 )
			{
				n_ppp_options.ppp_mode |= PASSIVE_OPEN;
			}
			else if( strnnccmp( word, "notify" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NOTIFY_ON_LINKUP;
			}
			else if( strnnccmp( word, "updata" ) == 0 )
			{
				n_ppp_options.ppp_mode |= XPARENT_UPDATA_OK;
			}
			else if( strnnccmp( word, "modem" ) == 0 )
			{
				n_ppp_options.ppp_mode |= WAIT_FOR_MODEM;
			}
			else if( strnnccmp( word, "loopback" ) == 0 )
			{
				n_ppp_options.ppp_mode |= LOOPBACK_MODE;
			}
			else if( strnnccmp( word, "norxcomp" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_RX_COMPRESS;
			}
			else if( strnnccmp( word, "notxcomp" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_TX_COMPRESS;
			}
			else if( strnnccmp( word, "nopap" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_PAP;
			}
			else if( strnnccmp( word, "nochap" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_CHAP;
			}
			else if( strnnccmp( word, "nopfc" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_PFC;
			}
			else if( strnnccmp( word, "noacfc" ) == 0 )
			{
				n_ppp_options.ppp_mode |= NO_ACFC;
			} else {
				invalid_cmd();
			}
		}

	} while( word );

	if( debug ) printf( "mode = %x \n", n_ppp_options.ppp_mode );
}

void do_set_parity( void )
{
	char*	arg = next_token( NULL, " \t.,=\n" );

		 if( strnccmp( arg, "none"  ) == 0 ) n_asynch_params.parity = NO_PARITY;
	else if( strnccmp( arg, "odd"   ) == 0 ) n_asynch_params.parity = ODD_PARITY;
	else if( strnccmp( arg, "even"  ) == 0 ) n_asynch_params.parity = EVEN_PARITY;
	else if( strnccmp( arg, "mark"  ) == 0 ) n_asynch_params.parity = MARK_PARITY;
	else if( strnccmp( arg, "space" ) == 0 ) n_asynch_params.parity = SPACE_PARITY;
	else invalid_cmd();

	if( debug ) printf( "parity = %u \n", n_asynch_params.parity );
}

void do_set_rx( void )
{
	char*	word = next_token( NULL, " \t.,=\n" );

	if( strnnccmp( word, "device" ) == 0 )
	{
		get_next_word( n_asynch_params.rx_dev_name, MAX_NAME_LEN );
		if( debug ) printf( "rx_dev_name = %s \n", n_asynch_params.rx_dev_name );
	}

	else if( strnnccmp( word, "buffer" ) == 0 )
	{
		n_asynch_params.rx_bufsize = get_integer();
		if( debug ) printf( "rx_bufsize = %u \n", n_asynch_params.rx_bufsize );
	}

	else if( strnnccmp( word, "accm" ) == 0 )
	{
		n_ppp_options.rx_accm.value = get_integer();
		if( debug ) printf( "rx_accm = %u \n", n_ppp_options.rx_accm.value );
	}

	else if( strnnccmp( word, "mru" ) == 0 )
	{
		n_ppp_options.rx_mru.value = get_integer();
		if( debug ) printf( "rx_mru = %u \n", n_ppp_options.rx_mru.value );
	} else {
		invalid_cmd();
	}
}

void do_set_scr( void )
{
	n_ppp_options.lcp_max_configure = get_integer();
	if( debug ) printf( "max_scr = %u \n", n_ppp_options.lcp_max_configure );
}

void do_set_stop( void )
{
	char*	arg = next_token( NULL, " \t,=\n" );
	double	sb = atof( arg );

		 if( sb == 1.0 ) n_asynch_params.stop_bits = STOP_BITS_1;
	else if( sb == 1.5 ) n_asynch_params.stop_bits = STOP_BITS_1P5;
	else if( sb == 2.0 ) n_asynch_params.stop_bits = STOP_BITS_2;
	else invalid_cmd();

	if( debug ) printf( "stop_bits = %u \n", n_asynch_params.stop_bits );
}

void do_set_str( void )
{
	n_ppp_options.lcp_max_terminate = get_integer();
	if( debug ) printf( "max_str = %u \n", n_ppp_options.lcp_max_terminate );
}

void do_set_timeout( void )
{
	n_ppp_options.lcp_timeout = get_integer();
	if( debug ) printf( "timeout = %u \n", n_ppp_options.lcp_timeout );
}

void do_set_tx( void )
{
	char*	word = next_token( NULL, " \t.,=\n" );

	if( strnnccmp( word, "device" ) == 0 )
	{
		get_next_word( n_asynch_params.tx_dev_name, MAX_NAME_LEN );
		if( debug ) printf( "tx_dev_name = %s \n", n_asynch_params.tx_dev_name );
	}

	else if( strnnccmp( word, "block" ) == 0 )
	{
		n_asynch_params.tx_bufsize = get_integer();
		if( debug ) printf( "tx_blocksize = %u \n", n_asynch_params.tx_bufsize );
	}

	else if( strnnccmp( word, "accm" ) == 0 )
	{
		n_ppp_options.tx_accm.value[0] = get_integer();
		if( debug ) printf( "tx_accm = %u \n", n_ppp_options.tx_accm.value[0] );
	}

	else if( strnnccmp( word, "mru" ) == 0 )
	{
		n_ppp_options.tx_mru.value = get_integer();
		if( debug ) printf( "tx_mru = %u \n", n_ppp_options.tx_mru.value );
	} else {
		invalid_cmd();
	}
}

void do_set_word( void )
{
	int	size = get_integer();

		 if( size == 5 ) n_asynch_params.word_size = WORD_SIZE_5;
	else if( size == 6 ) n_asynch_params.word_size = WORD_SIZE_6;
	else if( size == 7 ) n_asynch_params.word_size = WORD_SIZE_7;
	else if( size == 8 ) n_asynch_params.word_size = WORD_SIZE_8;
	else invalid_cmd();

	if( debug ) printf( "word_size = %u \n", n_asynch_params.word_size );
}

/**********************************************************************/
/* This is simply a wrapper function for strtok that allows us to     */
/* print the tokens if we want to.                                    */
/**********************************************************************/
char* next_token( char* src, char* delim )
{
	char*	word = strtok( src, delim );

	if( debug && word ) printf( "Token = '%s' \n", word );
	return( word );
}

/**********************************************************************/
/* This function will get the next word in the sentence, and copy it  */
/* into the caller's buffer.                                          */
/**********************************************************************/
void get_next_word( char* buf, int maxlen )
{
	char*	word;

	word =   next_token( NULL, " \t.,=\n" );

	strncpy( buf, word, maxlen );
	buf[maxlen-1] = '\0';
}

/**********************************************************************/
/* This function will check for the word 'on' or 'off', and return    */
/* one or zero as the result.                                         */
/**********************************************************************/
int get_boolean( void )
{
	char*	word;

	word =   next_token( NULL, " \t.,=\n" );

	if( strnccmp( word, "on" ) == 0 )
		return( 1 );

	else if( strnccmp( word, "off" ) == 0 )
		return( 0 );

	invalid_cmd();

	/* never reached */
	return(0);
}

/**********************************************************************/
/* This function will check to make sure all characters in a word are */
/* decimal digits, and return the decimal value.                      */
/**********************************************************************/
int get_integer( void )
{
	int	valid = 0, value = 0;

	value = check_integer( next_token( NULL, " \t,\n" ), &valid );
	if( ! valid ) invalid_cmd();

	return( value );
}

int check_integer( char* word, int* valid )
{
	int	value = 0;

	do
	{
		if( isdigit(*word) )
		{
			value *= 10;
			value += (*word++ - '0');
		}

		else
		{
			*valid = 0;
			return( 0 );
		}

	} while( *word != '\0' );

	*valid = 1;
	return( value );
}

void check_cmds( void )
{
	if( cmd_path || cmd_file )
	{
		printf( "Cannot specify multiple -z options. \n" );
		usage();
		exit(E_PARAM);
	}
}

void show_error( char* fn, error_code err )
{
	if( err != SUCCESS )
	{
		printf( "Error - %s returned %d (%x).\n", fn, err, err );
		exit( err );
	}
}

void invalid_cmd( void )
{
	printf( "\nInvalid command at line %u \n", line_num );
	printf( "    %s \n\n", cur_line );

	exit(1);
}

void show_set_cmds( char* s )
{
	printf( _set_cmds );
	not_valid = 1;
}

/**********************************************************************/
/* This function will skip over whitespace characters in a string and */
/* return a pointer to the first non-white character in the string    */
/**********************************************************************/
char* skipwhite( char* s )
{
	while( *s != '\0' && isspace( *s ) )
		s++;

	return( s );
}

/**********************************************************************/
/* This function does the same thing as strcmp(), but converts each   */
/* character to lower case prior to comparison. The original strings  */
/* are not altered.                                                   */
/**********************************************************************/
int strnccmp( char* s, char* t )
{
	while( tolower(*s) == tolower(*t) )
	{
		if( *s == '\0' )
			return( 0 );

		s++;
		t++;
	}

	return( *s - *t );
}

/**********************************************************************/
/* This function is like strnccmp() above, but will allow the first   */
/* string to be longer than the second string.                        */
/**********************************************************************/
int strnnccmp( char* s, char* t )
{
	while( tolower(*s) == tolower(*t) )
	{
		if( *s == '\0' )
			return( 0 );

		s++;
		t++;
	}

	if( *t == '\0' )
		return( 0 );

	return( *s - *t );
}

/**********************************************************************/
/* This function checks for decimal digits and decimal point, and     */
/* returns the value. If any invalid characters are found, zero is    */
/* returned.                                                          */
/**********************************************************************/
double get_decnum_only( char* s )
{
	double	value = 0.0, dp = 0.0;

	s = skipwhite( s );

	while( *s != '\0' )
	{
		if( isdigit(*s) )
		{
			value *= 10.0;
			value += (*s - '0');

			if( dp )
				dp *= 10.0;
		}

		else if( ! dp && (*s == '.') )
			dp = 1.0;

		else if( ! isspace(*s) )
			return( 0.0 );

		s++;
	}

	if( dp )
		value /= dp;

	return( value );
}

void parse_datamod( char* name )
{
	mh_com*		mod_hdr;
	u_int16		ar = 0, tl = 0;
	char		*s, *t, *mod_data, module_name[MAX_NAME_LEN];
	error_code	err;

	if( ! name )
		return;

	/* Fix up the module name */

	s = skipwhite( name );	/* Strip leading spaces */
	t = module_name;

	while( *s > ' ' )
		*t++ = *s++;

	*t = '\0';
	t = module_name;

	err = _os_link( &t, (mh_com**) &mod_hdr, (void**) &mod_data, &tl, &ar );

	if( err != SUCCESS )
	{
		printf( "Can't link to module %s \n", module_name );
		show_error( "_os_link", err );
	}

	parse_line( mod_data );

	_os_unlink( mod_hdr );
}

void parse_file( char* name )
{
	FILE*	fp;

	fp = fopen( name, "r" );

	if( ! fp )
	{
		printf( "Can't open file %s \n", name );
		show_error( "fopen", E_FNA );
	}

	parse_stream( fp );
	fclose( fp );
}

void parse_stream( FILE* fp )
{
	char	*s, buf[MAX_LINE_LEN];

	while( (s = fgets( buf, MAX_LINE_LEN, fp )) )
		parse_line( s );
}

void parse_line( char* line )
{
	char*	word;
	int		i = 0;

	if( debug && line ) printf( "Line = %s\n", line );

	line_num++;
	strcpy( cur_line, line );

	if( ! line || strlen(line) < 1 || *line == '#' )
		return;

	word = next_token( line, " \t.,=\n" );

	while( cmds[i].token )
	{
		if( strnccmp( word, cmds[i].token ) == 0 )
		{
			cmds[i].fn();
			return;
		}

		i++;
	}

	invalid_cmd();
}
