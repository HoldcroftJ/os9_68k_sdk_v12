%QEnter utility name:$
%QEnter utility description:$
/*
 * %response1 - %response2
 */

#include	<stdio.h>

/*
 * function prototypes
 */
void	parseopts(int argc, char *argv[]);
void	parseopt(char *argv);
void	help();

/*
 * global variables
 */
char	*progname;

/* 
 * main - main function for %response1
 */
int main(int argc, char *argv[]) {
	
	/* set the program name */
	progname = argv[0];

	/* parse the command line options */
	parseopts(argc, argv);
	
	exit(0);
}

/*
 * parseopts - parse command line arguments
 */
void parseopts(int argc, char *argv[]) {
	while (--argc > 0) {
		if ((*++argv)[0] == '-')
			parseopt(*argv);
	}
}

/* 
 * parseopt - parse a single command line option
 */
void parseopt(char *arg) {
	while (*++arg != '\0') {
		switch (*arg) {
			case	'?':
				help();
				exit(0);
			default:
				fprintf(stderr, "\%s: unkown option \\'\%c\\'\\n", progname, *arg);
				help();
				exit(1);
		}
	}
}

/*
 * help - print usage of this utility
 */
void help() {
	static char *msgs[] = {
		"Syntax:\\t%response1 [<opts>]\\n",
		"Function:\\t%response2\\n",
		"Options:\\n",
		"     -? - print utility use information\\n",
		NULL
	};
	*msgptr;

	for (msgptr = msgs; *msgptr != NULL; msgptr++)
		fputs(*msgptr, stderr);
}