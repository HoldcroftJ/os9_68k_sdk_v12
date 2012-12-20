/*******************************************************************************
 *
 * DESCRIPTION:	Record an Audio file using the MFM snd device
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

#ifndef fatalsignal
#define fatalsignal(s) ((s > 0) && (s <= 31))
#endif

#ifndef FAP_GREAD /* this is a hack to make OS9 and OS9000 compatible */
#define FAP_GREAD 0
#define FAP_GWRITE 0
#endif

#define NUM_SMAPS 3
#define TRIG_READY_SIG_BASE 0x100
#define CAPUTRE_DONE_SIG 0x200

u_int32 bufsize = (48000*2*2*2); /* Default is large enough to hold at
									least 2 seconds of data -- fastest
									rate * 2 channels * 2 16-bit
									samples * 2 seconds */
char *filename;
SND_DEV_CAP *devcap;
path_id devpath;
AUFileType aufiletype = AU_FILE_TYPE_SND;
BOOLEAN wave = FALSE;
BOOLEAN replace_mode = FALSE;
u_int32 size_limit = 0;
u_int32 time_limit = 0;
u_int32 num_channels = 2;
u_int32 encoding_rate = 8000;
u_int32 encoding_depth = 8;
SND_CM encoding_format = SND_CM_PCM_ULAW;

char cdbname[CDB_MAX_DNAME];
char *snd_name = &cdbname[0];

/* Signal handler flags should always be declared as volatile */
volatile u_int32 num_smaps_ready;
volatile u_int32 last_ready_smap;
volatile BOOLEAN abort_sig = FALSE;
volatile BOOLEAN done_sig = FALSE;


/*******************************************************************************
 * Prototypes
 *******************************************************************************/
void sighand(int);
static error_code parse_options_pass1(int argc, char **argv);
static error_code parse_options_pass2(int argc, char **argv);
static void print_usage(char* modname);
static error_code capture(u_int8 *buf[]);

/* system functions missing prototypes */
int tsleep(unsigned);
int intercept (void (*icpthand)(int));
int sigmask(int level);

/*******************************************************************************
 * MAINLINE
 *******************************************************************************/
void main(int argc, char **argv)
{
  error_code err;
  u_int8 *buf[NUM_SMAPS];
  int i;
  
  parse_options_pass1(argc, argv);
  
  /* set up signal handler */
  intercept (sighand);
  
  /* allocate the memory up front */
  for (i = 0; i < NUM_SMAPS; i++) {
	if ((err = _os_srqmem (&bufsize, (void**)&buf[i], 0)) != SUCCESS) {
	  exit(errmsg(err, "Could not allocate %d bytes\n",  bufsize));
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
  if ((err = _os_open(snd_name, FAM_READ, &devpath)) != SUCCESS) {
	exit(errmsg(err, "Cannot open %s\n", snd_name));
  }
  
  /* Read device capabilities */
  if ((err = _os_gs_snd_devcap(devpath, &devcap)) != SUCCESS) {
	exit(errmsg(err, "Error getting dev cap\n"));
  }

  print_snd_devcap(devcap);
  if (printdevcaps)
	exit(0);
  
  parse_options_pass2(argc, argv);
  
  err = capture(buf);

  /* Return allocated memory */
  for (i = 0; i < NUM_SMAPS; i++) {
	_os_srtmem (bufsize, buf[i]);
  }
  
  /* Close the Sound Device */
  _os_close (devpath);
  
  /* bug out of here */
  exit (err);
}

/*******************************************************************************
 * 
 *******************************************************************************/
static error_code capture(u_int8 *buf[]) 
{
  SND_SMAP smap[NUM_SMAPS];
  AUPath au_path;
  u_int32  filesize;
  error_code err;
  int i;

  /* init the first smap */
  memset (&smap[0], 0, sizeof smap[0]);
  smap[0].trig_mask = SND_TRIG_READY;
  smap[0].trig_signal = TRIG_READY_SIG_BASE+0;   /* signal to send when buffer ready */

  /* Wave files want to be encoded LSB */
  if ((encoding_format == SND_CM_PCM_SLINEAR) && 
	  (aufiletype == AU_FILE_TYPE_WAVE)) {
	encoding_format |= SND_CM_LSBYTE1ST;
  }
  smap[0].coding_method = encoding_format;

  if (encoding_depth) {
	smap[0].sample_size = encoding_depth;
  } else {
	switch (encoding_format) { /* Coding method */
	case SND_CM_PCM_ULAW:              /* ULAW encoded PCM */
	case SND_CM_PCM_ALAW:              /* ALAW encoded PCM */
	  smap[0].sample_size = 8;
	  break;
	case SND_CM_PCM_ULINEAR:            /* Linear Unsigned encoded PCM */
	  if (aufiletype == AU_FILE_TYPE_WAVE) { /* Wave files want to be encoded LSB */
		smap[0].coding_method |= SND_CM_LSBYTE1ST;
	  }
	  smap[0].sample_size = 8;
	  break;
	case SND_CM_PCM_SLINEAR:            /* Linear Signed encoded PCM */
	  if (aufiletype == AU_FILE_TYPE_WAVE) { /* Wave files want to be encoded LSB */
		smap[0].coding_method |= SND_CM_LSBYTE1ST;
	  }
	  smap[0].sample_size = 16;
	  break;
	case SND_CM_ADPCM_IMA:             /* IMA ADPCM */
	  smap[0].sample_size = 4;
	  break;
	case SND_CM_ADPCM_G721:            /* CCITT G.721 ADPCM */
	case SND_CM_ADPCM_G723:            /* CCITT G.723 ADPCM */
	default:
	  smap[0].sample_size = 0;
	}
  }
  smap[0].num_channels = num_channels;	/* Number of channels */
  smap[0].sample_rate = encoding_rate;  /* Number of samples per sec */
  smap[0].buf = buf[0];                 /* Sound data buffer */
  smap[0].buf_size = bufsize;           /* Size of sound data buffer */

  /* init the rest of the smaps */
  for  (i = 1; i < NUM_SMAPS; i++) {
	smap[i] = smap[0];                  /* Copy entire structure from first */
	smap[i].trig_signal = TRIG_READY_SIG_BASE+i;  /* Patch signal value */
	smap[i].buf = buf[i];               /* Sound data buffer */
	smap[i-1].next = &smap[i];          /* Link prevous smap to the current one */
  }
  smap[NUM_SMAPS-1].next = &smap[0];    /* Point the last smap to the first one */
	
	
  /* calculate how much data we plan to capture */
  if (time_limit) {
	filesize = smap[0].sample_rate * smap[0].num_channels *
	  ((smap[0].sample_size+7) / 8) * time_limit;
  } else if (size_limit) {
	filesize = size_limit;
  } else {
	filesize = 0;
  }

  if (verbose) printf ("Creating %s\n", filename);

  { u_int32 mode = (replace_mode) ? FAM_WRITE : (FAM_WRITE|FAM_NOCREATE);
	u_int32 perm = FAP_READ|FAP_WRITE|FAP_GREAD|FAP_GWRITE;

	if ((err = au_create(&au_path, &smap[0], aufiletype, filename, mode,
							   perm, filesize)) != SUCCESS)
	  return errmsg(err, "Error creating audio file %s\n", filename);
  }

  {
	u_int32 cur_smap = 0;
	u_int32 bytes_in = 0;
	BOOLEAN done = FALSE;
	
	last_ready_smap = NUM_SMAPS; /* set to illegal value for now */
	num_smaps_ready = 0;
	
	/* check if we end this first smap early */
	if (filesize && (filesize < smap[0].buf_size)) {
	  smap[0].buf_size = filesize;
	}

	/* Start capture */
	if ((err = _os_ss_snd_record(devpath, &smap[0], SND_BLOCK_START)) != SUCCESS) {
	  au_close(au_path, 0);     /* close the file as is */
	  return errmsg(err, "Error starting audio capture\n");
	}
	
	printf ("Capture started\n"); fflush(stdout);

	/* Send a signal when the device has stopped capturing */
	if ((err = _os_ss_sendsig(devpath, CAPUTRE_DONE_SIG)) != SUCCESS) {
	  _os_ss_snd_abort(devpath);		/* abort the capture */
	  au_close(au_path, 0);     /* close the file as is */
	  return errmsg(err, "Error calling _os_ss_sendsig()\n");
	}
	
	while (!done) {
	  
	  if (abort_sig) {
		_os_ss_snd_abort(devpath);		/* abort the capture */
		printf ("Abort issued\n"); fflush(stdout);
		abort_sig = FALSE;
	  }
	  
	  sigmask(1);
	  
	  if (!num_smaps_ready) {
		tsleep(0);
	  } else {
		/* sync check. If we are really caught up then cur_smap should
		   equal the last signal */
		if ((num_smaps_ready == 1) && (last_ready_smap != cur_smap)) {
		  _os_ss_snd_abort(devpath);		/* abort the capture */
		  au_close(au_path, 0);   	/* close the file as is */
		  return errmsg(1, "smap syncronization error\n");
		}
		sigmask(-1);
		
		{
		  u_int32 count = smap[cur_smap].cur_offset;
		  
		  if (((err = au_write_data(au_path, smap[cur_smap].buf,
                                    &count)) != SUCCESS) ||
			  (count != smap[cur_smap].cur_offset)) {
			_os_ss_snd_abort(devpath);		/* abort the capture */
			au_close(au_path, 0);     /* close the file as is */
			return errmsg(err, "Error writing data\n");
		  }
		  
		  /* Did we overrun the sound maps? */
		  if (num_smaps_ready >= NUM_SMAPS) {
			_os_ss_snd_abort(devpath);		/* abort the capture */
			au_close(au_path, 0);     /* close the file as is */
			return errmsg(1, "Data overrun\n");
		  } 
		  
		  bytes_in += count;
		}
		
		--num_smaps_ready;

		/* are we done */
		if (filesize && (bytes_in >= filesize)) {
		  _os_ss_snd_abort(devpath);		/* abort the capture */
		  done = TRUE;
		} else if (done_sig && !num_smaps_ready) {
		  done = TRUE;
		} else {
		  /* reset some of the smap values */
		  smap[cur_smap].trig_status = SND_TRIG_NONE;
		  smap[cur_smap].err_code = 0;
		  smap[cur_smap].cur_offset = 0;
		  
		  /* increment the current smap index */
		  if (++cur_smap == NUM_SMAPS)
			cur_smap = 0;

		  /* check if we end this new smap early */
		  if (filesize && (filesize < (smap[cur_smap].buf_size + bytes_in))) {
			smap[cur_smap].buf_size = filesize - bytes_in;
		  }
		}
	  }
	}
  }

  printf ("Capture stopped\n"); fflush(stdout);
	
  /* close the audio file */
  if ((err = au_close(au_path, filesize)) != SUCCESS)
	return errmsg(err, "Error closiing audio file\n");
	
  return SUCCESS;
}



void sighand(int sig)
{
  if ((sig & 0xff00) == TRIG_READY_SIG_BASE) {
	num_smaps_ready++;
	last_ready_smap = sig & 0xff;
  } else if (sig == SIGQUIT) {
	abort_sig = TRUE;
  } else if (sig == CAPUTRE_DONE_SIG) {
	done_sig = TRUE;
  } else if (fatalsignal(sig)) {
	exit(sig);
  } else {
	/* ignore the signal */
  }

  return;
}


static error_code parse_options_pass1(int argc, char **argv)
{
  char *modname = *argv;
  char *ptr;
  u_int8 cnt = 0;
  
  /* parse the command line parameters */
  while (--argc) {
	/* only interested in options right now */
    if (*(ptr = *++argv) == '-') {
      while(*++ptr) {
        switch(*ptr|0x20) { /* to lower */
		  
		case 'b':			/* user specified input buffer size */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  bufsize = atoi(ptr) * 1024;
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;

		case 'e':			/* encoding selection */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  encoding_format = atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  if (*ptr == ',') {          /* optional ',<rate>' */
			encoding_rate = atoi(++ptr);
			while (*ptr >= '0' && *ptr <= '9') ptr++;
			if (*ptr == ',') {          /* optional ',<depth>' */
			  encoding_depth = atoi(++ptr);
			  while (*ptr >= '0' && *ptr <= '9') ptr++;
			}
		  }
		  ptr--;
		  break;

		case 'f':			/* pick file type */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  aufiletype = (AUFileType)atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;

		case 'g':           /* gain */
          /* this option is handled in parse_options_pass2(). So just
             parse past it for now */
          while (*++ptr && *ptr != ':'); /* skip to ':' */
          if (*ptr++ != ':') {
            fprintf (stderr, "Missing ':' in -g option\n");
            print_usage(modname);
            exit(1);
          }
		  while (*ptr >= '0' && *ptr <= '9') ptr++; /* skip past <l> */
		  if (*ptr == ',') {          /* optional ',<r>' */
            ptr++;
			while (*ptr >= '0' && *ptr <= '9') ptr++; /* skip past <r> */
		  }
		  ptr--;
		  break;

		case 'i':           /* pick input device */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  snd_name = ptr;
		  while (*++ptr);
		  ptr--;
		  break;

		case 'm':			/* mono instead of stereo */
		  num_channels = 1;
		  break;

		case 'r':			/* format selection */
		  replace_mode = TRUE;
		  break;

		case 's':			/* size limit in K-bytes */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  size_limit = atoi(ptr) * 1024;
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;

		case 't':			/* time limit in seconds */
		  if (*++ptr == '=') ptr++;   /* optional '=' */
		  time_limit = atoi(ptr);
		  while (*ptr >= '0' && *ptr <= '9') ptr++;
		  ptr--;
		  break;

		case 'c':           /* print device Capabilities and quit */
		  printdevcaps = TRUE;
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
	} else {
	  switch (cnt++) {
	  case 0:
		filename = ptr;
		break;
	  default:
		fprintf (stderr, "\nToo many filenames: '%s'\n",ptr);
		print_usage(modname);
		exit(1);
	  }
    } /* end of if */
  } /* end of while */

  if (!printdevcaps && cnt != 1) {
	fprintf (stderr, "\nNo output filename specified\n");
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

            /* First build SND_GAIN as if mono */
            gain.cmd = SND_GAIN_CMD_MONO;
            gain.param.mono.m = left = atoi(++ptr);
            while (*ptr >= '0' && *ptr <= '9') ptr++;

            if (*ptr == ',') {          /* optional ',<r>' for stereo */
              gain.cmd = SND_GAIN_CMD_STEREO;
              gain.param.stereo.ll = left;
              gain.param.stereo.rr = atoi(++ptr);
              while (*ptr >= '0' && *ptr <= '9') ptr++;
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

#define enum_printf(expr) fprintf (stderr, "                         %d = " #expr "\n", expr)

static void print_usage(char* modname)
{
  fprintf (stderr, "Usage: $s <opts> <filename>\n", modname);
  fprintf (stderr, "Description: Record a SND or WAVE format audio file to <filename>\n");
  fprintf (stderr, "<opts>: -h or -?       This help\n");
  fprintf (stderr, "        -b[=]<size>    size of the two read buffers in K bytes (default: %d)\n",
		   bufsize/1024);
  fprintf (stderr, "        -c             print device Capabilities and quit\n");
  fprintf (stderr, " -e[=]<f>[,<r>[,<d>]] encoding Format, Rate, Depth (default %d,%d,%d)\n",
		   encoding_format, encoding_rate, encoding_depth);
  fprintf (stderr, "                       where <f> is:\n");
  enum_printf(SND_CM_PCM_ULAW);
  enum_printf(SND_CM_PCM_ALAW);
  enum_printf(SND_CM_PCM_SLINEAR);
  enum_printf(SND_CM_PCM_ULINEAR);
  enum_printf(SND_CM_ADPCM_G721);
  enum_printf(SND_CM_ADPCM_G723);
  enum_printf(SND_CM_ADPCM_IMA);
  fprintf (stderr, "                       <r> is the sample rate (samples per second)\n");
  fprintf (stderr, "                       and <d> is the bit depth (eg. 8,16,32)\n");
  fprintf (stderr, "        -f[=]<type>    format file as <type> (default %d)\n", aufiletype);
  fprintf (stderr, "                       where <type> is:\n");
  enum_printf(AU_FILE_TYPE_SND);
  enum_printf(AU_FILE_TYPE_WAVE);
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
  fprintf (stderr, "        -i[=]<dev>     specify alternative Input sound device (default: %s)\n",
		   snd_name);
  fprintf (stderr, "        -m             mono instead of stereo\n");
  fprintf (stderr, "        -r             replace existing file\n");
  fprintf (stderr, "        -s[=]<kbytes>  size limit of capture in K bytes (default: no limit)\n");
  fprintf (stderr, "        -t[=]<seconds> time limit of capture in seconds (default: no limit)\n");
  fprintf (stderr, "                       note: -t has precedence over -s\n");
  fprintf (stderr, "        -v             verbose - prints more info\n");
  fprintf (stderr, "\n");
} 

