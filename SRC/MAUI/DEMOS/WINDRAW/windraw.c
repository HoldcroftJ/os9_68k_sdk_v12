/******************************************************************************
*
* DESCRIPTION:  Window drawing demo. The windowing device must already be
*               created before you run this program.
*
* CREATED ON:   07/30/96
*
* COPYRIGHT:    Copyright 1995 Microware Systems Corporation. All Rights
*               Reserved. Use and/or distribution subject to terms and
*               conditions of applicable Microware license agreement(s).
*
*/
#include <MAUI/mauidemo.h>
#include <MAUI/maui.h>
#include <signal.h>
#include <process.h>

volatile u_int32 done=0;            /* done flag */

void demo_rectangles(WIN_ID win, DRW_CONTEXT_ID drwctx, GFX_PIXEL pix[], u_int32 num_colors);
void demo_lines(WIN_ID win, DRW_CONTEXT_ID drwctx, GFX_PIXEL pix[], u_int32 num_colors);
void signal_handler(int sig);
void my_sleep(u_int32 ticks);

void main(int argc, char *argv[])
{
    GFX_DEV_ID gfxdev;              /* Graphics device ID */
    MSG_MBOX_ID mbox;               /* Mailbox ID */
    WIN_DEV_ID windev;              /* Windowing device ID */
    WIN_ID root;                    /* Root window */
    WIN_ID win;                     /* Drawing window */
    WIN_STATUS rootstat;            /* Window status for root window */
    DRW_CONTEXT_ID drwctx;          /* Drawing context */
    GFX_POS win_x = 0, win_y = 0;   /* Window position */
    GFX_DIMEN win_w = 0, win_h = 0; /* Window size */
    u_int8 demo = 0;                /* Demo number */
    char *mbox_name = NULL;         /* Mailbox name */
    WIN_CMAP_ID cmap;               /* Default colormap */
    GFX_COLOR_VALUE color;          /* Color value */
    GFX_PIXEL pix[DEMO_NUM_COLORS]; /* Pixel values for default colors */
    u_int32 i;                      /* Loop counter */
    error_code err;
    u_int32 num_colors = DEMO_NUM_COLORS;/* number of demo colors */

    /* Check command line parameters */
    if (argc == 7) {
        win_x = atoi(argv[1]);
        win_y = atoi(argv[2]);
        win_w = atoi(argv[3]);
        win_h = atoi(argv[4]);
        demo = atoi(argv[5]);
        mbox_name = argv[6];
    }
    if ((argc != 7) || (demo > 2)) {
        fprintf(stderr, "Syntax: %s <x> <y> <w> <h> <demo> <mbox>\n", argv[0]);
        fprintf(stderr, "Demo 1: random rectangles\n");
        fprintf(stderr, "Demo 2: random lines\n");
        exit(1);
    }

    /* setup signal handler */
    signal(SIGQUIT,signal_handler);
    signal(SIGINT,signal_handler);

    /* Initialize MAUI APIs used by this program */
    msg_init();
    win_init();
    drw_init();

    /* Create the application mailbox */
    msg_create_mbox(&mbox, mbox_name, 50, sizeof(MAUI_MSG), MEM_ANY);

    /* Open the windowing device */
    err = win_open_dev(&windev, &gfxdev, &root, "/win", mbox);
    if (err != SUCCESS) {
      msg_close_mbox(mbox);
      win_term();
      drw_term();
      msg_term();
      _os_exit(err);
    }

    /* Allocate colors from the default colormap */
    win_get_win_status(&rootstat, root);
    cmap = rootstat.cmap;
    for (i = 0; i < num_colors; i++) {
        color.rgb = demo_colors[i];
        err = win_alloc_cmap_color(&pix[i], cmap, color);
        if (err != SUCCESS) {
          fprintf(stderr,"Could only allocate %d colors\n",i);
          num_colors = i;
        }
    }

    /* Create and setup the drawing context */
    drw_create_context(&drwctx, gfxdev);
    drw_set_context_fm(drwctx, DRW_FM_SOLID);

    /* Create the window and activate it */
    win_create_win(&win, root, win_x, win_y, win_w, win_h, WIN_MASK_NONE,
        WIN_BACK);
    win_set_state(win, TRUE);

    maui_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT);

    /* Switch based on the demo requested */
    switch (demo) {
        case 1:
            demo_rectangles(win, drwctx, pix, num_colors);
            break;
        case 2:
            demo_lines(win, drwctx, pix, num_colors);
            break;
    }

    /* Shut down */
    drw_destroy_context(drwctx);
    msg_close_mbox(mbox);
    win_destroy_win(win);
    for (i = 0; i < num_colors; i++) {
        win_free_cmap_cells(cmap, pix[i], 1);
    }
    win_destroy_dev(windev);
    win_term();
    drw_term();
    msg_term();

    _os_exit(SUCCESS);
}

/*******************************************************************************
* NAME:     demo_rectangles()
*
* USAGE:    Draw random rectangles to a window forever.
*/
void demo_rectangles(WIN_ID win, DRW_CONTEXT_ID drwctx, GFX_PIXEL pix[], u_int32 num_colors)
{
    WIN_STATUS winstat;
    GFX_RECT rect;
    GFX_DIMEN w,h;

    /* Set the drawing context and get the dimensions of the window */
    win_set_drw_context(win, drwctx);
    win_get_win_status(&winstat, win);
    w = winstat.width;
    h = winstat.height;

    /* Draw random rectangles */
    while (!done) {
        /* be nice, give up some time for other things to run */
        my_sleep(1); 

        /* gets some random coordinates */
        demo_random_rect(&rect, 0, 0, 1, 1, w, h);
        /* set the drawing clut/color */
        drw_set_context_pix(drwctx, pix[demo_random(1, num_colors - 1)]);

        /* clear the cursor and reserve some screen area */
        win_lock_region(win, rect.x, rect.y, rect.width, rect.height);
        /* do all our drawing */
        drw_rectangle(drwctx, rect.x, rect.y, rect.width, rect.height);
        /* restore the cursor and give up the screen area */
        win_unlock_region(win);
    }
}

/*******************************************************************************
* NAME:     demo_lines()
*
* USAGE:    Draw random lines to a window forever.
*/
void demo_lines(WIN_ID win, DRW_CONTEXT_ID drwctx, GFX_PIXEL pix[], u_int32 num_colors)
{
    WIN_STATUS winstat;
    GFX_RECT rect;
    GFX_DIMEN w,h;

    /* Set the drawing context and get the dimensions of the window */
    win_set_drw_context(win, drwctx);
    win_get_win_status(&winstat, win);
    w = winstat.width;
    h = winstat.height;

    /* Draw random rectangles */
    while (!done) {
        /* be nice, give up some time for other things to run */
        my_sleep(1);

        /* gets some random coordinates */
        demo_random_rect(&rect, 0, 0, 0, 0, w, h);
        /* set the drawing clut/color */
        drw_set_context_pix(drwctx, pix[demo_random(1, num_colors - 1)]);

        /* clear the cursor and reserve some screen area */
        win_lock_region(win, rect.x, rect.y, rect.width, rect.height);
        /* do all our drawing */
        drw_line(drwctx, rect.x, rect.y, rect.x + rect.width - 1,
            rect.y + rect.height - 1);
        /* restore the cursor and give up the screen area */
        win_unlock_region(win);
    }
}


void signal_handler(int sig)
{
  if ((sig == SIGINT) ||
      (sig == SIGQUIT))
    done = TRUE;
}


void my_sleep(u_int32 ticks)
{
  u_int32 t = ticks;
#ifdef _OS9000
  signal_code sig;
  _os_sleep(&t, &sig);
#endif
#ifdef _OSK
  _os9_sleep(&t);
#endif
}
