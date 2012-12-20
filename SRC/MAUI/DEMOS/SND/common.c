
/*******************************************************************************
 *
 * DESCRIPTION:	Common function used in both play and record
 *
 * COPYRIGHT:	Copyright 1995 Microware Systems Corporation. All Rights
 *				Reserved. Use and/or distribution subject to terms and
 *				conditions of applicable Microware license agreement(s).
 *
 */

#include "common.h"

BOOLEAN printdevcaps = FALSE;
BOOLEAN printstatus = FALSE;
BOOLEAN verbose = FALSE;

static void print_trig(SND_TRIGGER trig)
{
  if (trig == SND_TRIG_NONE)
	printf ("\tSND_STATUS_NONE\n");
  else if (trig == SND_TRIG_ANY)
	printf ("\tSND_STATUS_ANY\n");
  if (trig & SND_TRIG_START)
	printf ("\tSND_TRIG_START\n");
  if (trig & SND_TRIG_FINISH)
	printf ("\tSND_TRIG_FINISH\n");
}

/* A.K.A. cmprint(SND_CM cm) in #define of common.h */
static void print_coding_method(char* title, SND_CM cm)
{
  printf (title);

  switch (snd_get_cm_name(cm)) {
  case SND_CM_UNKNOWN:
    printf ("SND_CM_UNKNOWN"); break;
  case SND_CM_PCM_ULAW:
    printf ("SND_CM_PCM_ULAW"); break;
  case SND_CM_PCM_ALAW:
    printf ("SND_CM_PCM_ALAW"); break;
  case SND_CM_PCM_ULINEAR:
    printf ("SND_CM_PCM_ULINEAR"); break;
  case SND_CM_PCM_SLINEAR:
    printf ("SND_CM_PCM_SLINEAR"); break;
  case SND_CM_ADPCM_G721:
    printf ("SND_CM_ADPCM_G721"); break;
  case SND_CM_ADPCM_G723:
    printf ("SND_CM_ADPCM_G723"); break;
  case SND_CM_ADPCM_IMA:
    printf ("SND_CM_ADPCM_IMA"); break;
  default:
	printf ("***Illegal***"); break;
  }

  /* print the modifiers */
  if (cm & SND_CM_LSBYTE1ST)
	printf ("|SND_CM_LSBYTE1ST");

  if (cm & SND_CM_LSBIT1ST)
	printf ("|SND_CM_LSBIT1ST");

  printf (" (0x%x)\n", cm);
}

void print_fileinfo(SND_SMAP *smap)
{
  if (!smap) {
	putchar('\n');
    return;
  }

  cmprint(smap->coding_method);
  dprint (smap->num_channels);
  dprint (smap->sample_size);
  dprint (smap->sample_rate);
  printf("time estimate: %.2f seconds\n\n", (double)(8.0 * smap->buf_size) /
		 (smap->num_channels * smap->sample_rate * smap->sample_size));
  return;
}

void print_smap(SND_SMAP *smap, char *title)
{
  if (!verbose) return;
  
  if (!smap) {
	putchar('\n');
    return;
  }

  printf ("\n%s\n", title);
  
  xprint (smap->trig_status);
  print_trig (smap->trig_status);
  xprint (smap->trig_mask);
  print_trig (smap->trig_mask);
  xprint (smap->trig_signal);
  eprint (smap->err_code);
  print_fileinfo(smap);
  xprint (smap->buf);
  dprint (smap->buf_size);
  dprint (smap->cur_offset);
  dprint (smap->loop_start);
  dprint (smap->loop_end);
  dprint (smap->loop_count);
  dprint (smap->loop_counter);
  xprint (smap->next);
  
  return;
}

#define pdm(expr)      \
{                                 \
  if (lines & expr) {              \
    printf ("%s" #expr, prefix);  \
    prefix = newprefix;           \
  }                               \
}
static void print_snd_line(SND_LINE lines)
{
  char *prefix = "";
  char *newprefix = " | ";
  
  putchar('\t'); 

  if (!lines) {
    printf ("NULL mix line");
  } else {
    pdm(SND_LINE_VOLUME);
    pdm(SND_LINE_BASS);
    pdm(SND_LINE_TREBLE);
    pdm(SND_LINE_SYNTH);
    pdm(SND_LINE_PCM);
    pdm(SND_LINE_SPEAKER);
    pdm(SND_LINE_LINE);
    pdm(SND_LINE_MIC);
    pdm(SND_LINE_CD);
    pdm(SND_LINE_IMIX);
    pdm(SND_LINE_ALTPCM);
    pdm(SND_LINE_RECLEV);
    pdm(SND_LINE_IGAIN);
    pdm(SND_LINE_OGAIN);
    pdm(SND_LINE_LINE1);
    pdm(SND_LINE_LINE2);
    pdm(SND_LINE_LINE3);
    
    if (prefix != newprefix)
      printf ("Unkown mix line value %d", lines);
  }  
  putchar('\n'); 

  return;
}
#undef pdm

#define pdm(expr)      \
{                                 \
  if (cmd & expr) {               \
    printf ("%s" #expr, prefix);  \
    prefix = newprefix;           \
  }                               \
}

static void print_snd_cmd_mask(SND_GAIN_CMD cmd)
{
  char *prefix = "";
  char *newprefix = " | ";
  
  putchar('\t'); 

  if (cmd == SND_GAIN_CMD_NONE) {
    printf ("SND_GAIN_CMD_NONE");
  } else {
    pdm(SND_GAIN_CMD_MUTE);
    pdm(SND_GAIN_CMD_MONO);
    pdm(SND_GAIN_CMD_STEREO);
    pdm(SND_GAIN_CMD_XSTEREO);
    pdm(SND_GAIN_CMD_UP);
    pdm(SND_GAIN_CMD_DOWN);
    pdm(SND_GAIN_CMD_RESET);

    if (prefix != newprefix)
      printf ("Unkown gain comand value 0x%x", cmd);
  }  
  putchar('\n'); 

  return;
}
#undef pdm

static void print_snd_cmd(SND_GAIN_CMD cmd, SND_GAIN *gain)
{
  switch (cmd) {
  case SND_GAIN_CMD_MUTE:
    printf ("\t\tSND_GAIN_CMD_MUTE\n");
    if (gain) {
      putchar('\t');putchar('\t'); xprint (gain->param.mute.state);
    }
    break;
  case SND_GAIN_CMD_MONO:
    printf ("\t\tSND_GAIN_CMD_MONO\n");
    if (gain) {
      putchar('\t');putchar('\t'); xprint (gain->param.mono.m);
    }
    break;
  case SND_GAIN_CMD_STEREO:
    printf ("\t\tSND_GAIN_CMD_STEREO\n");
    if (gain) {
      putchar('\t');putchar('\t'); xprint (gain->param.stereo.ll);
      putchar('\t');putchar('\t'); xprint (gain->param.stereo.rr);
    }
    break;
  case SND_GAIN_CMD_XSTEREO:
    printf ("\t\tSND_GAIN_CMD_XSTEREO\n");
    if (gain) {
      putchar('\t');putchar('\t'); xprint (gain->param.xstereo.ll);
      putchar('\t');putchar('\t'); xprint (gain->param.xstereo.rr);
      putchar('\t');putchar('\t'); xprint (gain->param.xstereo.rl);
      putchar('\t');putchar('\t'); xprint (gain->param.xstereo.lr);
    }
    break;
  case SND_GAIN_CMD_NONE:
    printf ("***Unexpected SND_GAIN_CMD_NONE found in gain status\n***");
    break;
  case SND_GAIN_CMD_UP:
    printf ("***Unexpected SND_GAIN_CMD_UP found in gain status\n***");
    break;
  case SND_GAIN_CMD_DOWN:
    printf ("***Unexpected SND_GAIN_CMD_DOWN found in gain status\n***");
    break;
  case SND_GAIN_CMD_RESET:
    printf ("***Unexpected SND_GAIN_CMD_RESET found in gain status\n***");
    break;
  default:
    printf ("***Illegal Gain Control***\n");
    break;
  }
}

void print_snd_status(path_id path)
{
  SND_DEV_STATUS *status;
  SND_GAIN *gain;
  u_int16 cnt;
  error_code err;
  
  if (!verbose && !printstatus) return;
  
  printf ("\n*********************\n****Device Status****\n*********************\n");
  
  if ((err = _os_gs_snd_status(path, &status)) != SUCCESS) {
	errmsg(err, "Error getting dev status\n");
	exit (err);
  }
  
  xprint (status->status);
  dprint (status->play_pid);
  dprint (status->record_pid);
  
  /* print gain status information for each device */
  for (cnt = status->num_gain, gain = status->gain;
       cnt--;
       gain++) {
    
    print_snd_line(gain->lines);
    
    print_snd_cmd(gain->cmd, gain);
  }
  return;
}

static void print_snd_gaincap(SND_GAIN_CAP *gain_cap)
{
  if (!gain_cap) {
	putchar('\n');
    return;
  }

  print_snd_line(gain_cap->lines);
  putchar('\t');putchar('\t'); bprint(gain_cap->sup_mute);
  print_snd_cmd(gain_cap->default_type, NULL);
  putchar('\t');putchar('\t'); dprint(gain_cap->default_level);
  putchar('\t');putchar('\t'); dprint(gain_cap->zero_level);
  putchar('\t');putchar('\t'); dprint(gain_cap->num_steps);
  putchar('\t');putchar('\t'); dprint(gain_cap->step_size);
  putchar('\t');putchar('\t'); dprint(gain_cap->mindb);
  putchar('\t');putchar('\t'); dprint(gain_cap->maxdb);
}

void print_snd_devcap(SND_DEV_CAP *devcap)
{
  
  if (!verbose && !printdevcaps) return;
  
  if (!devcap) {
	putchar('\n');
    return;
  }

  printf ("\n***************************\n****Device Capabilities****\n***************************\n");
  
  strprint (devcap->hw_type);
  strprint (devcap->hw_subtype);
  
  xprint (devcap->sup_triggers);
  print_trig (devcap->sup_triggers);

  xprint(devcap->play_lines);
  print_snd_line(devcap->play_lines);
  
  xprint(devcap->record_lines);
  print_snd_line(devcap->record_lines);

  xprint(devcap->sup_gain_cmds);
  print_snd_cmd_mask(devcap->sup_gain_cmds);

  { /* Loop and print gain_caps */
	u_int16 num_gain_caps = devcap->num_gain_caps;
	SND_GAIN_CAP *gcap = devcap->gain_caps;
	
	dprint (devcap->num_gain_caps);
	while (num_gain_caps-- > 0) {
      print_snd_gaincap (gcap++);
	}
  }

  { /* Loop and print rate information */
	u_int16 num_rates = devcap->num_rates;
	u_int32 *rates = devcap->sample_rates;
	
	dprint (devcap->num_rates);
	while (num_rates-- > 0) {
	  printf ("\t%d\n", *rates++);
	}
  }

  { /* Loop and print channel information */
	u_int16 num_ci = devcap->num_chan_info;
	u_int16 *ci = devcap->channel_info;
	
	dprint (devcap->num_chan_info);
	while (num_ci-- > 0) {
	  switch (*ci) {
	  case 1:
		printf ("\tMono\n"); break;
	  case 2:
		printf ("\tStereo\n"); break;
	  default:
		printf ("\t%d is Unknown%s\n", ci); break;
	  }
	  ci++;
	}
  }

  { /* Loop and print coding method information */
	u_int8 num_cm;
	SND_DEV_CM *cm;
	
	dprint (devcap->num_cm);
    printf ("devcap->cm_info:\n", devcap->cm_info);

	num_cm = devcap->num_cm;
	cm = devcap->cm_info;
	while (num_cm > 0) {
	  
	  putchar('\t'); cmprint(cm->coding_method);
	  putchar('\t'); dprint (cm->boundary_size);
	  putchar('\t'); dprint (cm->sample_size);
	  
	  cm++; num_cm--;
	}
  }
  
  return;
}


int errmsg(error_code err, char *msg, ...)
{
  va_list args;

  va_start(args, msg);
  vfprintf(stderr, msg, args);
  fprintf(stderr, "%s\n", strerror(err));
  va_end(args);

  return err;
}
