
/* bootfteac.c prototypes */

#ifdef CBOOT 
#if defined (__STDC__) || defined (_ANSI_EXT)
int bootfteac(void);
#else
int bootfteac();
#endif
#endif

/* init.c prototypes */

#ifndef CBOOT 
#if defined (__STDC__) || defined (_ANSI_EXT)
int init(mod_dev *);
#else
int init();
#endif
#else /* CBOOT */

#if defined (__STDC__) || defined (_ANSI_EXT)
error_code init_fteac(void);
#else
error_code init_fteac();
#endif

#endif /* end of CBOOT conditional */

/* misc.c prototypes */

#if defined (__STDC__) || defined (_ANSI_EXT)
int ftinitdrv (Rbfdrive, int);
int execute (u_int, u_int, u_int, u_int, u_char *, u_int, u_int, int);
int execnoxfer (int, int, int, int, int, int);
int scsistat (int);
int handlerr (void);
void copyblock( u_char *, u_char *, int );
u_int fttrkoffs(u_int, int);
void lxtoa(char *, long);
int hexchar(int);
void strcpy(char *, char *);
int strncmp(char *, char *, int );
int set3l(char *, int);
int c4tol(char *);
int chkfdtime( Rbfdrive, int);
int setfdtime(int);
int validate_descriptor(struct rbf_opt *);
char *get_fc1str(void);
char *get_gsstr(void);
char *get_hsstr(void);
char *get_jsstr(void);
int get_drive_kind( char * );
#else
int ftinitdrv ();
int execute ();
int execnoxfer ();
int handlerr ();
void copyblock();
u_int fttrkoffs();
void lxtoa();
int hexchar();
void strcpy();
int strncmp();
int set3l();
int c4tol();
int chkfdtime();
int setfdtime();
int validate_descriptor();
char *get_fc1str();
char *get_gsstr();
char *get_hsstr();
char *get_jsstr();
int get_drive_kind( char *);
#endif

/* read.c prototypes */

#ifdef CBOOT
#if defined (__STDC__) || defined (_ANSI_EXT)
error_code read_fteac( int, u_int );
#else
error_code read_fteac();
#endif
#else /* not CBOOT */
#if defined (__STDC__) || defined (_ANSI_EXT)
int read (int, u_int);
#else
int read ();
#endif
#endif

/* stat.c prototypes */

#ifdef CBOOT 
#if defined (__STDC__) || defined (_ANSI_EXT)
int getstat (u_short, REGISTERS *);
int putstat (u_short, REGISTERS *);
int direct_command( REGISTERS *, struct rbf_opt *, Rbfdrive);
#else
int getstat ();
int putstat ();
int direct_command();			
#endif
#endif

/* term.c prototypes */

#ifdef CBOOT 
#if defined (__STDC__) || defined (_ANSI_EXT) 
int term (mod_dev *);
#else
int term ();
#endif
#endif

/* write.c prototypes */

#ifdef CBOOT 
#if defined (__STDC__) || defined (_ANSI_EXT) 
int write (int, u_int);
#else
int write ();
#endif
#endif

