/*******************************************************************************
 *
 * DESCRIPTION:	Play Audio files using the MFM snd device
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */
#define MAIN


#include <MAUI/maui_cdb.h>
#include "common.h"
#include "fmt_com.h"
#include <alarm.h>
#include <string.h>


#define READY_SIG 1024
#define ABORT_SIG 1025

char cdbname[CDB_MAX_DNAME];
char *snd_name = &cdbname[0];
volatile int sigval;

#define TRIG_READY_SIG_BASE 0x100

u_int32 bufsize = (48000*2*2*2); /* Default is large enough to hold at
									least 2 seconds of data -- fastest
									rate * 2 channels * 2 16-bit
									samples * 2 seconds */
u_int32 loop_count = 0;
u_int32 pause_interval = 0;
u_int32 kill_interval = 0;
u_int32 encoding_rate = 0;
path_id devpath;
SND_DEV_CAP *devcap;
SND_BLOCK_TYPE block_type = SND_BLOCK_FINISH;
BOOLEAN async_linked = FALSE;
BOOLEAN infoonly = FALSE;
BOOLEAN force_unsigned = FALSE;

/*******************************************************************************
 * Prototypes
 *******************************************************************************/
void sighand(int);
static error_code parse_options_pass1(int argc, char **argv);
static error_code parse_options_pass2(int argc, char **argv);
static void print_usage(char* modname);
static void pick_nearest_sample_rate(SND_SMAP *smap, SND_DEV_CAP *devcap);

/* system functions missing prototypes */
int tsleep(unsigned);
int intercept (void (*icpthand)(int));
int sigmask(int level);

/*******************************************************************************
 * MAINLINE
 *******************************************************************************/
void main(int argc, char **argv)
{
  u_int32 rsize;
  error_code err;
  SND_SMAP smap[2];
  u_int8 *buf[2] = {0,0};
  int i;
  BOOLEAN eof;
  AUPath au_path;
 
  /* Make first pass on command line options */
  parse_options_pass1(argc, argv);
  
  /* set up signal handler */
  intercept (sighand);

  if (!infoonly) {
    /* allocate the memory up front */
    for (i = 0; i < 2; i++) {
      if ((err = _os_srqmem (&bufsize, (void**)&buf[i], 0)) != SUCCESS) {
	exit(errmsg(err, "Could not allocate %s %d bytes\n", 
		    i ? "second" : "first", bufsize));
      }
    }

    /* Get the name of the audio device from the CDB */
    cdb_init(); /* Make sure we can make a cdb call. This returns fast
                   if already inited */
    /* trying to get the name of the sound device */
    if ((err = cdb_get_ddr(CDB_TYPE_SOUND, 1, snd_name, NULL)) != SUCCESS) {
      /* if cdb_get_addr call failed use "/snd" as the default name */	
      snd_name = "/snd";      
    }
    cdb_term();
    
    /* Prepare the Sound device */
    if ((err = _os_open(snd_name, FAM_WRITE, &devpath)) != SUCCESS) {
      exit(errmsg(err, "Cannot open sound device %s\n", snd_name));
    }
    
    /* Read device capabilities */
    if ((err = _os_gs_snd_devcap(devpath, &devcap)) != SUCCESS) {
      exit(errmsg(err, "Error getting dev cap\n"));
    }
    
    /* Print device capabilities */
    print_snd_devcap(devcap);
    if (printdevcaps)
      exit(0);
  }
  
  /* Make second pass on command line options */
  parse_options_pass2(argc, argv);
  
  /* Print device status */
  print_snd_status(devpath);
  
  while (*++argv) {  /* for each command line argument */
    
    if (**argv == '-') /* skip the options arguments */
      continue;
    
    printf ("Open %s\n", *argv);
    
    /* When this returns the file pointer is positioned at the start
       of the data and smap->buf_size indicates the about of data in
       the file */
    if ((err = au_open(&au_path, &smap[0], *argv)) != SUCCESS) {
      switch (err) {
      case EOS_MAUI_BADID:
      case EOS_MAUI_BADVALUE:
        errmsg(err, "%s not a recognized audio format\n", *argv);
        break;
      case EOS_MAUI_NOTIMPLEMENTED:
        errmsg(err, "%s contains an unsupported audio format\n", *argv);
        break;
      default:
        errmsg(err, "Cannot open file %s\n", *argv);
        break;
      }
      continue;
    }
    
    /* are we in "info only" mode */
    if (infoonly) {
      print_fileinfo(&smap[0]);
      continue;
    }
    
    /* was a sample rate override specified */
    if (encoding_rate)
      smap[0].sample_rate = encoding_rate;
    
    /* if sample rate not supported pick the closest */
    pick_nearest_sample_rate(&smap[0], devcap);
    
    /* setup loop count if any */
    if (loop_count) {
      smap[0].loop_end = smap[0].buf_size;
      smap[0].loop_count = loop_count;
    }

    /* set trigger mask and signal */
    if (async_linked) {
      smap[0].trig_mask = SND_TRIG_READY;
      smap[0].trig_signal = TRIG_READY_SIG_BASE;
    }

    /* init the second smap from the first */
    smap[1]=smap[0];
    smap[0].buf = buf[0];
    smap[1].buf = buf[1];

    /* have soundmaps reference each other to provide uninterrupted play */
    if (async_linked) {
      smap[0].next = &smap[1];
      smap[1].next = &smap[0];
    }
    
    /* print info */
    print_smap(&smap[0], "First smap");
    
    i = 1;
    eof = FALSE;
    do {
      /* read in the next part */
      i = (i ? 0 : 1); /* flip the state of i */
      rsize = bufsize;
      if ((err = au_read(au_path, buf[i], &rsize)) != SUCCESS) {
	if (err == EOS_EOF) {
	  eof = TRUE;
	} else {
	  errmsg(err, "Error reading %d bytes from %s\n", rsize, *argv);
	  au_close(au_path, 0);
	}
	continue;
      }
      
      /* reset some of the smap values */
      smap[i].buf_size = rsize;
      smap[i].cur_offset = 0;
      smap[i].trig_status = SND_TRIG_NONE;
      smap[i].err_code = 0;
      smap[i].loop_end = smap[i].buf_size;
      smap[i].loop_count = loop_count;
      smap[i].loop_counter = 0;
      
      /* Force signed 8-bit PCM data to Unsigned? */
      if (force_unsigned) {
        if (smap[i].sample_size == 8) {
          u_char *ptr = smap[i].buf - 1;
          u_char *limit = ptr + smap[i].buf_size;
          while (++ptr < limit) {
            *ptr = *(char*)ptr + 128;
          }
        } else {
          printf ("The '-u' option is not valid for %s. Ignoring '-u' option.\n", *argv);
        }
      }
      
      /* Play the Sound */
      if (block_type < SND_NOBLOCK) {
	if ((err = _os_ss_snd_play(devpath, &smap[i], block_type)) != SUCCESS) {
	  errmsg(err, "Error playing sound\n");
	}
      }
      
      if (async_linked) {
	if (block_type == SND_NOBLOCK) {
	  /* wait until a sound map has been exhausted, so we
	  ** know when to fill it with new data. */
	  sigmask(1);
	  if (sigval != TRIG_READY_SIG_BASE)
	    tsleep(0);
	  else 
	    sigmask(-1);
	  
	  sigval = 0;
	} else {
	  /* don't sleep the first time.  We are currently playing from 
	  ** smap[0], and we need to fill smap[1]. */
	  block_type = SND_NOBLOCK;
	}
      } else if (kill_interval) {
	alarm_id alarm;
	BOOLEAN done = FALSE;
	
	sigval = 0;
	if ((err = _os_ss_sendsig(devpath, READY_SIG)) != SUCCESS) {
	  errmsg(err, "Error calling _os_ss_sendsig()\n");
	}
	
	if ((err = _os_alarm_set(&alarm, ABORT_SIG, kill_interval)) != SUCCESS) {
	  errmsg(err, "Error calling _os_alarm_set()\n");
	}
	
	/* While the play is not finished */
	while (!done) {
	  
	  sigmask(1);
	  switch (sigval) {
	  case 0:
	    tsleep(0);
	    break;
	    
	  case ABORT_SIG:
	    sigval = 0;
	    sigmask(-1);
	    if ((err = _os_ss_snd_abort(devpath)) != SUCCESS) {
	      errmsg(err, "Error calling _os_ss_snd_abort()\n");
	    }
	    break;
	    
	  case READY_SIG:
	    sigmask(-1);
	    done = TRUE;
	    break;
	    
	  default:
	    {
	      sigmask(-1);
	      fprintf (stderr, "Recived signal %d while waiting for abort\n", sigval);
	    }
	    break;
	  }
	}
      } else if (pause_interval) {

	/* if a pause interval has been specified */
	BOOLEAN pause_flag = FALSE;
	
	sigval = 0;
	if ((err = _os_ss_sendsig(devpath, READY_SIG)) != SUCCESS) {
	  errmsg(err, "Error calling _os_ss_sendsig()\n");
	}
	
	/* While the play is not finished */
	while (sigval != READY_SIG) {
	  /* alternatively pause and continue */
          pause_flag = !pause_flag;
	  if (pause_flag) {
	    if ((err = _os_ss_snd_pause(devpath)) != SUCCESS) {
	      errmsg(err, "Error calling _os_ss_snd_pause()\n");
	    }
	  } else {
	    if ((err = _os_ss_snd_cont(devpath)) != SUCCESS) {
	      errmsg(err, "Error calling _os_ss_snd_cont()\n");
	    }
	  }
	  tsleep(pause_interval);
	}
      }
      
      /*      if (verbose || smap->err_code) */
	eprint (smap->err_code);
      
    } while (!eof);
    
    /* unlink sound maps from each other when stopping play*/
    smap[0].next = smap[1].next = NULL;
    
    /* close the audio file */
    au_close(au_path, 0);
    
    sigval = 0;
    if ((err = _os_ss_sendsig(devpath, READY_SIG)) != SUCCESS) {
      errmsg(err, "Error calling _os_ss_sendsig()\n");
    }
    
    /* wait for play of this file to finish */
    while (sigval != READY_SIG)
      tsleep(2);
    
    /* verify that the sendsig generated a signal */
    if (sigval != READY_SIG)
      printf ("***** Did not detect a signal following the play *****\n");
    else
      printf ("_os_ss_sndsig() generated the correct signal\n");
    
  }
  
  /* Return allocated memory */
  for (i = 0; i < 2; i++) {
    _os_srtmem (bufsize, buf[i]);
  }
  
  /* Close the Sound Device */
  _os_close (devpath);
  
  /* bug out of here */
  exit (SUCCESS);
}

void sighand(int sig)
{
  switch (sigval = sig) {
  case 0:
	fprintf (stderr, "SIG ZERO?\n");
	break;
	
  case 2:	/* fatal signals */
  case 3:
	exit(sig);
	
  default:
	/* ignore signal */
	break;
  }
}


/* Find the nearest hw suppored sample rate */
static void pick_nearest_sample_rate(SND_SMAP *smap, SND_DEV_CAP *devcap)
{
  u_int32 *rate = devcap->sample_rates;
  u_int32 cnt = devcap->num_rates;
  u_int32 diff;
  u_int32 best_diff = 0xffffffff;
  u_int32 best_rate = 0;

  /* this search for the closest sample rate makes no assumptions
	 about the ordering of devcap->sample_rates */
  while (cnt-- > 0) {
	/* do we have a perfect match? Don't change a thing. */
	if (smap->sample_rate == *rate)
	  return;
	/* compute a the difference */
	diff = abs(smap->sample_rate - *rate);
	if (diff < best_diff) {
	  best_diff = diff;
	  best_rate = *rate;
	}
	rate++;
  }
  smap->sample_rate = best_rate;
  return;
}


static error_code parse_options_pass1(int argc, char **argv)
{
  char *modname = *argv;
  char *ptr;
  
  /* parse the command line parameters */
  while (--argc) {
	/* only interested in options right now */
    if (*(ptr = *++argv) == '-') {
      while(*++ptr) {
        switch(*ptr|0x20) { /* to lower */
		  
		case 'a':           /* Asynchronous play, block play until
                               START instead of FINISH */
		  block_type = SND_BLOCK_START;
		  if ((ptr[1] | 0x20) == 'l')
		    async_linked = TRUE;
		  break;

		case 'b':			/* user specified input buffer size */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  bufsize = atoi(ptr) * 1024;
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;
		  
		case 'c':           /* print device Capabilities and quit */
		  printdevcaps = TRUE;
		  break;

		case 'l':			/* loop count */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  loop_count = atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;
		  
		case 'g':           /* gain */
          /* this option is handled in parse_options_pass2(). So just
             parse past it for now */
          while (*++ptr && *ptr != ':'); /* skip to ':' */
          if (*ptr++ != ':') {/**/
            fprintf (stderr, "Missing ':' in -g option\n");
            print_usage(modname);
            exit(1);
          }
          if (*ptr|0x20 == 'r') *ptr++; /* skip past 'R' */
		  while (*ptr >= '0' && *ptr <= '9') ptr++; /* skip past <l> */
		  if (*ptr == ',') {          /* optional ',<r>' */
            ptr++;
			while (*ptr >= '0' && *ptr <= '9') ptr++; /* skip past <r> */
		  }
		  ptr--;
		  break;

		case 'o':           /* pick output device */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  snd_name = ptr;
		  while (*++ptr);
		  ptr--;
		  break;

		case 'i':           /* only print info about the audio files */
		  infoonly = TRUE;
		  break;

		case 'k':           /* set kill interval */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  kill_interval = atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  block_type = SND_BLOCK_START;
		  break;

		case 'p':           /* set pause interval */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  pause_interval = atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  block_type = SND_BLOCK_START;
		  break;

		case 'r':			/* Override sample rate */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  encoding_rate = atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;
		  
		case 's':           /* print device Status */
		  printstatus = TRUE;
		  break;

		case 'u':			/* force signed 8-bit PCM data to Unsigned */
		  force_unsigned = TRUE;
		  break;
		  
		case 'v':			/* turn on verbose messages */
		  verbose = TRUE;
		  break;
		  
		case 'h':
        case '?':   /* help message */
		  print_usage(modname);
          exit(0);
		  
        default:    /* illegal option */
		  fprintf (stderr, "\nunknown option: '%c'\n",*ptr);
		  print_usage(modname);
          exit(1);
		  
        }   /* end of switch */
      }   /* end of while */
    } /* end of if */
  } /* end of while */

  if (pause_interval && kill_interval) {
	fprintf (stderr, "\n*** -k option incompatible with -p option ***\n");
	print_usage(modname);
	exit(1);
  }
  
  return SUCCESS;
}

static error_code parse_options_pass2(int argc, char **argv)
{
  char *modname = *argv;
  char *ptr;
  
  /* parse the command line parameters */
  while (--argc) {
	/* only interested in options right now */
    if (*(ptr = *++argv) == '-') {
      while(*++ptr) {
        switch(*ptr|0x20) { /* to lower */
		  
          /* The -g option is in the second option pass to allow
             setting of the gain for the mix devices after the -o
             sound device is opened. */
		case 'g':           /* gain */
          { 
            SND_GAIN gain;
            SND_LINE line;
            u_int8 left;
            error_code err;

            /* Clear the gain structure */
            memset(&gain, 0, sizeof(gain));

            if (*++ptr == '=') ptr++;   /* optional '=' */
            
            /* build line mix mask */
            while (*ptr != ':') {
              if (*ptr == ',') ptr++;
              if ((line = atoi(ptr)) == 99) {
                gain.lines = SND_LINE_MASK_ALL;
              } else if (line < SND_NUM_LINES) {
                gain.lines |= (1<<line);
              } else {
                fprintf (stderr, "\nillegal -g mix value: '%d'\n",line);
                print_usage(modname);
                exit(1);
              }
              while (*ptr >= '0' && *ptr <= '9') ptr++;
            }

            if ((*++ptr|0x20) == 'r') {
              gain.cmd = SND_GAIN_CMD_RESET;
            } else {
              /* First build SND_GAIN as if mono */
              gain.cmd = SND_GAIN_CMD_MONO;
              gain.param.mono.m = left = atoi(ptr);
              while (*ptr >= '0' && *ptr <= '9') ptr++;
              
              if (*ptr == ',') {          /* optional ',<r>' for stereo */
                gain.cmd = SND_GAIN_CMD_STEREO;
                gain.param.stereo.ll = left;
                gain.param.stereo.rr = atoi(++ptr);
                while (*ptr >= '0' && *ptr <= '9') ptr++;
              }
            }
            ptr--;

            /* Now we know enough to set the gain */
            if ((err = _os_ss_snd_gain(devpath, &gain)) != SUCCESS) {
              errmsg(err, "Warning: Problem setting gain\n");
              /* continue anyways */
            }

            break;
          }

        default:    /* illegal options caught in first pass */
            break;
        }   /* end of switch */
      }   /* end of while */
    } /* end of if */
  } /* end of while */

  return SUCCESS;
}

static void print_usage(char* modname)
{
  fprintf (stderr, "Usage: $s <opts> <file list...>\n", modname);
  fprintf (stderr, "Description: Play one or more SND or WAVE format audio files\n");
  fprintf (stderr, "<opts>: -h or -?       This help\n");
  fprintf (stderr, "        -a             Async (play while reading next chunk, default: Sync)\n");
  fprintf (stderr, "        -al            Async with linked soundmaps (not compatible\n");
  fprintf (stderr, "                       with -k or -p)\n");
  fprintf (stderr, "        -b[=]<size>    size of the two read buffers in K bytes (default: %d)\n",
		   bufsize/1024);
  fprintf (stderr, "        -c             print device Capabilities and quit\n");
  fprintf (stderr, "        -g[=]<line-1>[,<line-n>]:<l>[,<r>] Set left and right gain for\n");
  fprintf (stderr, "                       <line-1...n>.  If <r> is not specified, <l> is\n");
  fprintf (stderr, "                       used as a mono specifier. 'R' or <l> resets\n");
  fprintf (stderr, "                       the lines. The option may be repeated for\n");
  fprintf (stderr, "                       different sets of <line>s. <line> codes are:\n");
  fprintf (stderr, "            0 VOLUME   Master output level\n");
  fprintf (stderr, "            1 BASE     Base level of all output lines\n");
  fprintf (stderr, "            2 TREBLE   Treble level of all output lines\n");
  fprintf (stderr, "            3 SYNTH    Synthesizer input\n");
  fprintf (stderr, "            4 PCM      Output level for the audio (CODEC, PCM) line\n");
  fprintf (stderr, "            5 SPEAKER  Output level for the PC speaker\n");
  fprintf (stderr, "            6 LINE     Input level for the line in jack\n");
  fprintf (stderr, "            7 MIC      Input level for the microphone in jack\n");
  fprintf (stderr, "            8 CD       Input level for CD in jack\n");
  fprintf (stderr, "            9 IMIX     Recording monitor level (output of rec input)\n");
  fprintf (stderr, "           10 OMIX     Loopback of output to input level\n");
  fprintf (stderr, "           11 ALTPCM   Output level of alternative codec device\n");
  fprintf (stderr, "           12 RECLEV   Global recording level setting\n");
  fprintf (stderr, "           13 IGAIN    Input gain control\n");
  fprintf (stderr, "           14 OGAIN    Output gain control\n");
  fprintf (stderr, "           15 LINE1    Generic mixer line one\n");
  fprintf (stderr, "           16 LINE2    Generic mixer line two\n");
  fprintf (stderr, "           17 LINE3    Generic mixer line three\n");
  fprintf (stderr, "           Other special values:\n");
  fprintf (stderr, "           99 ALL      Selects all devices\n");
  fprintf (stderr, "        -i             just print Info about the audio files, don't play\n");
  fprintf (stderr, "        -k[=]<ticks>   kill/abort play in <ticks> (implies -s but acts like\n");
  fprintf (stderr, "                       FINISH, not compatible with -p) (default: 0=none)\n");
  fprintf (stderr, "        -l[=]<num>     number times to loop individual soundmaps (default %d)\n",
		   loop_count);
  fprintf (stderr, "        -o[=]<dev>     specify alternative Output sound device (default: %s)\n",
		   snd_name);
  fprintf (stderr, "        -p[=]<ticks>   pause interval in ticks (implies -s but acts like\n");
  fprintf (stderr, "                       FINISH, not compatible with -k) (default: 0=none)\n");
  fprintf (stderr, "        -r[=]<r>       override the files sample Rate (samples per second)\n");
  fprintf (stderr, "        -s             print device Status info\n");
  fprintf (stderr, "        -u             force signed 8-bit PCM data to Unsigned\n");
  fprintf (stderr, "        -v             verbose - prints more info\n");
  fprintf (stderr, "\n");
} 

