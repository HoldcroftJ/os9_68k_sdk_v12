#if !defined(_CURSES_H)
#define _CURSES_H

/*		header defs for curses.l */

/*--------------------------------------------------------------------------,
|                                                                           |
|              Copyright 1998 by Microware Systems Corporation              |
|                            All Rights Reserved                            |
|                          Reproduced Under License                         |
|                                                                           |
|  This software is confidential property of Microware Systems Corporation, |
|  and is provided under license for internal development purposes only.    |
|  Reproduction, publication, modification, distribution, or creation of    |
|  derivative works in any form to any party other than the Licensee is     |
|  strictly prohibited, unless expressly provided for under the terms of a  |
|  mutually executed written license agreement for this software between    |
|  the end-user and Microware Systems Corporation, or otherwise expressly   |
|  authorized in writing by Microware Systems Corporation.  Licensee shall  |
|  reproduce this copyright notice exactly as provided herein on any copies |
|  of the software that Licensee is authorized to distribute.               |
|---------------------------------------------------------------------------|
|                                                                           |
| Edition History:                                                          |
| #   Date     Comments                                                By   |
| --- -------- ------------------------------------------------------- ---- |
|  01 98/07/29 Added edition history.                                  wwb  |
|				---- OS-9/68K V3.1 Release ----								|
|  02 99/05/11 Fix warnings                                            srm  |
|              $$                    <RELEASE_INFO>                     $$  |
`--------------------------------------------------------------------------*/

#if !defined(_STDIO_H)
#include <stdio.h>
#endif /* _STDIO_H */

#if !defined(_SGSTAT_H)
#include <sgstat.h>
#endif /* _SGSTAT_H */

#if !defined(_ERRNO_H)
#include <errno.h>
#endif /* _ERRNO_H */

/* private stuff */
extern FILE *_stdin, *_stdout;

#define ECHOON 1
#define ECHOOFF 0
#define	TRUE	(1)
#define	FALSE	(0)
#define	ERR	(0)
#define	OK	(1)
#define SCFDEV		0
#define OTHERDEV	1
#define TCAPSLEN 300
#define TTYNAMESIZE 50

#define bool char
#define reg register

/*	Termcap defs */

char *tgetstr(), *tgoto();
char *getenv();


extern bool		AM, BS, CA, DA, DB, EO, GT, HZ, IN, MI, MS, NC, OS, UL, XN;

extern char		*AL, *BC, *BT, *CD, *CE, *CL, *CM, *CR, *DC, *DL, *DM, *DO,
		*ED, *EI, *HO, *IC, *IM, *IP, *LL, *MA, *ND, *NL, *SE, *SF,
		*SO, *SR_, *TA, *TE, *TI, *UC, *UE, *UP, *US, *VB, *VE, *VS;

extern char PC_;

extern int SG;
extern int LINES, COLS;  /* lines and columns on physical terminal screen */


extern short ospeed;
extern char termbuf[1024], tcapbuf[TCAPSLEN];


/* physical terminal defs */


extern int NONL, UPPERCASE;
extern int _devtype;

extern int ttcol, ttrow;  /* cursor location */
extern int tt_tabsiz;

int tt_mv(), tt_clrln(), tt_clrscr(), tt_clrpag(), tt_so(), tt_se();
int tt_init(), tt_putc(), tt_puts(), tt_flush();

#define tt_mvputc(r,c,ch) (tt_mv(r,c)==ERR ? ERR : tt_putc(ch))
#define tt_mvputs(r,c,str) (tt_mv(r,c) == ERR ? ERR : tt_puts(str))
#define tt_mvclrln(r,c) (tt_mv(r,c) == ERR : ERR : tt_clrln())
#define tt_mvclrpag(r,c) (tt_mv(r,c) == ERR ? ERR : tt_clrpag())
#define tt_mvso(r,c) (tt_mv(r,c) == ERR ? ERR : tt_so())
#define tt_mvse(r,c) (tt_mv(r,c) == ERR ? ERR : tt_se())

extern struct sgbuf _pdbuf, _savepdbuf;  /* the path descriptor buffers */

/* curses defs */

typedef struct WINDOW {
	struct WINDOW	*w_next,  /* next window in list */
					*w_prev;  /* previous window */
	struct WINBUF	*w_buf;  /* text buffer for this window */
	int				w_hrow,  /* home row on screen */
					w_hcol,  /* home column on screen */
					w_orow,  /* offset row in buffer */
					w_ocol,  /* offset column in buffer */
					w_rows,  /* number of lines of window on screen */
					w_cols,  /* number of columns of window on screen */
					w_wrow,  /* cursor row location in the window */
					w_wcol,  /* cursor column */
					w_sflag,  /* flags used by the windowing system */
					w_aflag;  /* flags for use by the application */
	bool			w_clear,  /* clear screen flag */
					w_scroll,  /* scroll flag */
					w_leave;  /* leave cursor flag */
	struct WINLIN	*w_bline;  /* a pointer to the buffer line the cursor is on*/
} WINDOW;

#define W_HIDE	0x01  /* flag that says window should not be displayed */
#define W_SCREEN	0x02  /* window is a screen */
#define W_TOUCH		0x04  /* ignore output optimization */
#define W_NODELAY	0x08  /* if set, getch will return if no data */
#define W_SCROLL	0x10  /* window has very recently been scrolled */
						  /* i.e., no writes have occurred since scroll */
#define W_STANDOUT	0x0100  /* output to window should be in standout mode */


typedef struct WINBUF {
	struct WINBUF	*b_next,  /* next buffer in list */
					*b_prev;  /* previous buffer */
	struct WINLIN	*b_text;  /* the first line of text in this buffer */
	int				b_wcnt,  /* number of windows looking at this buffer */
					b_lines,  /* number of lines in this buffer */
					b_lsize,  /* default size to allocate for new lines */
					b_sflag,  /* flags used by the windowing system */
					b_aflag;  /* flags for use by the application */
} WINBUF;

typedef struct WINLIN {
	struct WINLIN	*l_next,  /* next line of buffer */
					*l_prev;  /* previous line */
	int				l_size,  /* space allocated for line */
					l_sflag,  /* flags used by windowing system */
					l_aflag;  /* flags for use by the application */
	char			l_text[1];  /* the characters in the line */
} WINLIN;

#define L_CLEAR	0x01  /* line contains all blanks */

extern WINDOW *winhead;  /* the root of the window list */
extern WINBUF *bufhead;  /* the root of the buffer list */

extern WINDOW *curscr, *virscr, *stdscr;  /* standard windows */

extern FILE *bufin;

extern bool _echo, _crmode, _rawmode, _typeahead, _nlmode, My_term;

extern char *Def_term, ttytype[TTYNAMESIZE];

WINDOW *newwin(), *subwin(), *w_open(), *w_linkbuf();

WINBUF *w_bufopen(), *newbuf();

char *w_addline();


/* macro functions */
#define clearok(win,b) (win->w_clear = b)
#define leaveok(win,b) (win->w_leave = b)
#define scrollok(win,b) (win->w_scroll = b)
#define getyx(win,r,c) (r = win->w_wrow, c = win->w_wcol)
#define winch(win) (win->w_bline->l_text[win->w_ocol + win->w_wcol] & 0x7f)
#define wnoutrefresh(win) paint(win)
#define doupdate() update(virscr)
#define hide(win) (win->w_sflag |= W_HIDE)
#define unhide(win) (win->w_sflag &= ~W_HIDE)

/* stdscr functions */

#define	addch(ch)	(waddch(stdscr, ch))
#define	getch()		(wgetch(stdscr))
#define	addstr(str)	(waddstr(stdscr, str))
#define	getstr(str)	(wgetstr(stdscr, str))
#define	move(y, x)	(wmove(stdscr, y, x))
#define	clear()		(wclear(stdscr))
#define	erase()		(werase(stdscr))
#define	clrtobot()	(wclrtobot(stdscr))
#define	clrtoeol()	(wclrtoeol(stdscr))
#define	insertln()	(winsertln(stdscr))
#define	deleteln()	(wdeleteln(stdscr))
#define	refresh()	(wrefresh(stdscr))
#define	inch()		(winch(stdscr))
#define	insch(c)	(winsch(stdscr,c))
#define	delch()		(wdelch(stdscr))
#define	standout()	(wattron(stdscr, W_STANDOUT))
#define	standend()	(wattrset(stdscr,0))
#define	wstandout(win)	(wattron(win, W_STANDOUT))
#define	wstandend(win)	(wattrset(win, 0))

/* mv function defines */

#define	mvwaddch(win,y,x,ch)	(wmove(win,y,x)==ERR?ERR:waddch(win,ch))
#define	mvwgetch(win,y,x)	(wmove(win,y,x)==ERR?ERR:wgetch(win))
#define	mvwaddstr(win,y,x,str)	(wmove(win,y,x)==ERR?ERR:waddstr(win,str))
#define mvwgetstr(win,y,x,str)  (wmove(win,y,x)==ERR?ERR:wgetstr(win,str))
#define	mvwinch(win,y,x)	(wmove(win,y,x) == ERR? ERR : winch(win))
#define	mvwdelch(win,y,x)	(wmove(win,y,x) == ERR? ERR : wdelch(win))
#define	mvwinsch(win,y,x,c)	(wmove(win,y,x) == ERR? ERR : winsch(win,c))
#define mvwclrtobot(win,r,c) (wmove(win,r,c) == ERR? ERR: wclrtobot(win))
#define mvwclrtoeol(win,r,c) (wmove(win,r,c)== ERR? ERR: wclrtoeol(win))
#define mvwinsertln(win,r,c) (wmove(win,r,c) == ERR? ERR: winsertln(win))
#define mvwdeleteln(win,r,c) (wmove(win,r,c) == ERR? ERR: wdeleteln(win))

#define	mvaddch(y,x,ch)		mvwaddch(stdscr,y,x,ch)
#define	mvgetch(y,x)		mvwgetch(stdscr,y,x)
#define	mvaddstr(y,x,str)	mvwaddstr(stdscr,y,x,str)
#define mvgetstr(y,x,str)       mvwgetstr(stdscr,y,x,str)
#define	mvinch(y,x)		mvwinch(stdscr,y,x)
#define	mvdelch(y,x)		mvwdelch(stdscr,y,x)
#define	mvinsch(y,x,c)		mvwinsch(stdscr,y,x,c)
#define mvclrtobot(r,c) mvwclrtobot(stdscr,r,c)
#define mvclrtoeol(r,c) mvwclrtoeol(stdscr,r,c)
#define mvinsertln(r,c) mvwinsertln(stdscr,r,c)
#define mvdeleteln(r,c) mvwdeleteln(stdscr,r,c)

#define crmode() _crmode = TRUE
#define nocrmode() _crmode = FALSE
#define raw() _rawmode = TRUE
#define noraw() _rawmode = FALSE

#define restoremodes() (_echo ? echo() : noecho())


/* some system five functions */
#define REFRESH 256
#define OUT 257
#define prefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol) \
_pfresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol, REFRESH)
#define pnoutrefresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol) \
_pfresh(pad, pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol, OUT)

#define tbreak() (_typeahead && chkin())

#define cbreak() crmode()
#define nocbreak() nocrmode()

#define A_STANDOUT W_STANDOUT
#define A_REVERSE 0
#define A_BOLD 0
#define A_DIM 0
#define A_BLINK 0
#define A_UNDERLINE 0
#define A_MASK (A_STANDOUT|A_REVERSE|A_BOLD|A_DIM|A_BLINK|A_UNDERLINE)

#define attron(attr) wattron(stdscr, attr)
#define attroff(attr) wattroff(stdscr, attr)
#define attrset(attr) wattrset(stdscr, attr)

#if defined(__cplusplus) || ((defined(_ANSI_EXT) || defined(__STDC__)) && defined(_OPT_PROTOS))
#define _OP(x) x
#else
#define _OP(x) ()
#endif

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int allpaint _OP((void));
void beep _OP((void));
int chkin _OP((void));
int delwin _OP((WINDOW *win));
int echo _OP((void));
int endwin _OP((void));
int initscr _OP((int spec));
int mvprintw(int r, int c, char *fmt, ...);
int mvwprintw(WINDOW *win, int r, int c, char *fmt, ...);
int noecho _OP((void));
int paint _OP((WINDOW *win));
int printw(char *fmt, ...);
int priority _OP((WINDOW *win));
int resetty(void);
int touchwin _OP((WINDOW *win));
int update _OP((WINDOW *win));
int updateln _OP((WINDOW *win, WINLIN *wline, WINLIN *cline, int row));
int waddch _OP((WINDOW *win, char ch));
int waddstr _OP((WINDOW *win, char *str));
int wattroff _OP((WINDOW *win, int flags));
int wattron _OP((WINDOW *win, int flags));
int wattrset _OP((WINDOW *win, int flags));
int wclear _OP((WINDOW *win));
int wclrtobot _OP((WINDOW *win));
int wclrtoeol _OP((WINDOW *win));
int werase _OP((WINDOW *win));
char wgetch _OP((WINDOW *win));
int wgetstr _OP((WINDOW *win, char *str));
int wmove _OP((WINDOW *win, int row, int col));
int wprintw(WINDOW *win, char *fmt, ...);
int wrefresh _OP((WINDOW *win));

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#undef _OP

#endif /* _CURSES_H */
