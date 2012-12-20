/*******************************************************************************
 * FILENAME : maui_snd.h
 *
 * DESCRIPTION :
 *
 *  This file contains definitions for the MAUI Sound API
 *
 * WARNING:
 *
 *  When the MAUI sound API is finished, this interface will be hidden from
 *  applications. Directly using this interface in you applications is only
 *  recommended until the official API is available.
 *
 * COPYRIGHT:
 *
 *  This source code is the proprietary confidential property of Microware
 *  Systems Corporation, and is provided to licensee solely for documentation
 *  and educational purposes. Reproduction, publication, or distribution in
 *  any form to any party other than the licensee is strictly prohibited.
 *
 * EDITION  DATE      WHO  DETAIL
 *       1  07/02/96  SRM  First version
 *       2  10/17/00  SRM  Fixed 'Sound Coding Methods' comment to match 
 *                         the macros for byte/bit endianess.
 */

#ifndef _MAUI_SND_H
#define _MAUI_SND_H

#include <types.h>
#include <signal.h>
#include <MAUI/maui_com.h>

/*******************************************************************************
 * INPUT/OUTPUT MIXER CONTROL LINES
 *******************************************************************************/
typedef int SND_LINE;
#define SND_LINE_VOLUME  (1<<0)  /* This is the master output level
                                    (headphone/line out volume. */
#define SND_LINE_BASS    (1<<1)  /* This line controls the bass
                                    level of all of the output
                                    lines. */
#define SND_LINE_TREBLE  (1<<2)  /* This line controls the treble
                                    level of all of the output
                                    lines. */
#define SND_LINE_SYNTH   (1<<3)  /* This line controls the
                                    synthesizer input (FM, wave table)
                                    of the soundcard. */
#define SND_LINE_PCM     (1<<4)  /* Output level for the audio (CODEC,
                                    PCM, ADC) line. */
#define SND_LINE_SPEAKER (1<<5)  /* Output level for the PC speaker
                                    signals. Typically Mono. */
#define SND_LINE_LINE    (1<<6)  /* Input level for the line in
                                    jack. */
#define SND_LINE_MIC     (1<<7)  /* Volume for the signal coming from
                                    the microphone in jack. */
#define SND_LINE_CD      (1<<8)  /* Volume level for signal connected
                                    to the CD audio input. */
#define SND_LINE_IMIX    (1<<9)  /* The recording monitor. For
                                    instance, on PAS16 and some other
                                    cards. Controls the output
                                    (headphone jack) volume of the
                                    selected recording sources while
                                    recording. This line has effect
                                    just when recording. */
#define SND_LINE_OMIX    (1<<10) /* Loopback of output to input. */
#define SND_LINE_ALTPCM  (1<<11) /* Volume of the alternative codec
                                    line (the SB emulation of the
                                    PAS16 board). */
#define SND_LINE_RECLEV  (1<<12) /* Global recording level setting. In
                                    the SB16 card this controls the
                                    input gain which has 4 possible
                                    levels. */
#define SND_LINE_IGAIN   (1<<13) /* Input gain control */
#define SND_LINE_OGAIN   (1<<14) /* Output gain control */
#define SND_LINE_LINE1   (1<<15) /* LINE1-3 are generic mixer lines */
#define SND_LINE_LINE2   (1<<16) /* which are used in cases when */
#define SND_LINE_LINE3   (1<<17) /* precise meaning of a physical mixer
                                    line is not known. Actual
                                    meaning of these signals are
                                    "vendor defined". Usually these
                                    lines are connected to synth,
                                    line in, and CD inputs of the card,
                                    but order of the assignment is not
                                    known to the driver. */
/* limits and mask defines for SND_LINE */
#define SND_NUM_LINES     18     /* Current number of defined mixer
                                    lines */
#define SND_MAX_LINES     32     /* Maximum number of definable mixer
                                    lines */
#define SND_LINE_MASK_ALL ((1<<SND_NUM_LINES)-1)/* Mask to select all
                                                   mixer lines */

/* Gain Commands */
typedef int SND_GAIN_CMD;
#define SND_GAIN_CMD_NONE    0x00000000 /* Unknown mix command */
#define SND_GAIN_CMD_UP      0x00000001 /* Up N levels */
#define SND_GAIN_CMD_DOWN    0x00000002 /* Down N levels */
#define SND_GAIN_CMD_RESET   0x00000004 /* Reset gain level */
#define SND_GAIN_CMD_MUTE    0x00000080 /* Control Mute */
#define SND_GAIN_CMD_MONO    0x00000100 /* Set Mono gain */
#define SND_GAIN_CMD_STEREO  0x00000200 /* Set Stereo gain */
#define SND_GAIN_CMD_XSTEREO 0x00000400 /* Set Cross Stereo gain */

/* Gain Level Constants */
#define SND_LEVEL_MIN      0  /* Lowest, quietest gain level */
#define SND_LEVEL_MAX    127  /* Highest, loudest gain level */
#define SND_LEVEL_MUTE  0x80  /* Mute mask */

/* Up Command Parameters */
typedef struct _SND_GAIN_UP {
  u_int8 levels;
} SND_GAIN_UP;

/* Down Command Parameters */
typedef struct _SND_GAIN_DOWN {
  u_int8 levels;
} SND_GAIN_DOWN;

/* Mute Command Parameters */
typedef struct _SND_GAIN_MUTE {
  BOOLEAN state;
} SND_GAIN_MUTE;

/* Mono Command Parameters */
typedef struct _SND_GAIN_MONO {
  u_int8 m;                     /* Gain for mono signal*/
} SND_GAIN_MONO;

/* Stereo Command Parameters */
typedef struct _SND_GAIN_STEREO {
  u_int8 ll;                    /* Left input to left output */
  u_int8 rr;                    /* Right input to right output*/
} SND_GAIN_STEREO;

/* Stereo Command with Cross-Over Parameters */
typedef struct _SND_GAIN_XSTEREO {
  u_int8 ll;                    /* Left input to left output */
  u_int8 rr;                    /* Right input to right output*/
  u_int8 rl;                    /* Right input to left output */
  u_int8 lr;                    /* Left input to right output */
} SND_GAIN_XSTEREO;

/* Gain Control Structure */
typedef struct _SND_GAIN {
  SND_LINE lines;            /* Mixer lines mask */
  SND_GAIN_CMD cmd;             /* Mix Command */
  union {                       /* cmd == */
    SND_GAIN_UP up;                /*   SND_GAIN_CMD_UP */
    SND_GAIN_DOWN down;            /*   SND_GAIN_CMD_DOWN */
    /* SND_GAIN_CMD_RESET has no params */
    SND_GAIN_MUTE mute;            /*   SND_GAIN_CMD_MUTE */
    SND_GAIN_MONO mono;            /*   SND_GAIN_CMD_MONO */
    SND_GAIN_STEREO stereo;        /*   SND_GAIN_CMD_STEREO */
    SND_GAIN_XSTEREO xstereo;      /*   SND_GAIN_CMD_XSTEREO */
  } param;
} SND_GAIN;

/*******************************************************************************
 * BLOCKING TYPES
 *******************************************************************************/

typedef enum {
  SND_BLOCK_START,              /* Block until sound operation starts */
  SND_BLOCK_FINISH,             /* Block until sound operation ends */
  SND_NOBLOCK                   /* Never block */
} SND_BLOCK_TYPE;

/*******************************************************************************
 * Sound Coding Methods
 *
 * The bits of the sound Coding Methods are reserved as follows:
 * Range of Bits    Description
 * 0 - 9            Coding Method Name
 * 10 - 17          Reserved by Microware
 * 18               Bit endianess (0 = Big, 1 = Little)
 * 19               Byte endianess (0 = Big, 1 = Little)
 * 10 - 24          Reserved by Microware
 * 25 - 32          Defined by OEMs
 *
 * The coding method "name" is divided into the following numeric ranges.
 * Range            Description
 * 0 - 255          Standard coding methods
 * 256 - 767        Reserved by Microware
 * 768 - 1023       Defined by OEMs (Should be prefixed with SND_CM_OEM_)
 * 
 *******************************************************************************/

typedef u_int32 SND_CM;

/* Coding Method Names */
#define snd_set_cm_name(name) (name & 0x3ff)
#define snd_get_cm_name(cm)   (cm & 0x3ff)
#define SND_CM_UNKNOWN     0        /* Unknown or not yet determined */
#define SND_CM_PCM_ULAW    1        /* ULAW encoded PCM */
#define SND_CM_PCM_ALAW    2        /* ALAW encoded PCM */
#define SND_CM_PCM_SLINEAR 3        /* Signed Linear encoded PCM */
#define SND_CM_PCM_ULINEAR 4        /* Unsigned Linear encoded PCM */
#define SND_CM_ADPCM_G721  5        /* CCITT G.721 ADPCM */
#define SND_CM_ADPCM_G723  6        /* CCITT G.723 ADPCM */
#define SND_CM_ADPCM_IMA   7        /* IMA ADPCM */

/* Byte ordering */
#define snd_set_cm_byte_order(order) ((order & 0x1) << 19)
#define snd_get_cm_byte_order(cm)    ((cm >> 19) & 0x1)
#define SND_CM_MSBYTE1ST snd_set_cm_byte_order(MSBFIRST)  /* Most Significant Byte First */
#define SND_CM_LSBYTE1ST snd_set_cm_byte_order(LSBFIRST)  /* Least Significant Byte First */

/* Bit ordering */
#define snd_set_cm_bit_order(order) ((order & 0x1) << 18)
#define snd_get_cm_bit_order(cm)    ((cm >> 18) & 0x1)
#define SND_CM_MSBIT1ST snd_set_cm_bit_order(MSBFIRST)  /* Most Significant Bit First */
#define SND_CM_LSBIT1ST snd_set_cm_bit_order(LSBFIRST)  /* Least Significant Bit First */

/* Coding Method Description */
typedef struct _SND_DEV_CM {
  SND_CM coding_method;         /* Coding method */
  u_int32 sample_size;          /* Number of bits per sample */
  u_int16 boundary_size;        /* Boundary limitations */
} SND_DEV_CM;

/*******************************************************************************
 * SOUND CONTROL BLOCK
 *******************************************************************************/

/* Trigger Types */
typedef int SND_TRIGGER;
#define SND_TRIG_NONE     0x00000000 /* Mask for no triggers */
#define SND_TRIG_START    0x00000001 /* Audible start */
#define SND_TRIG_FINISH   0x00000002 /* Audible finish */
#define SND_TRIG_BUSY     0x00000004 /* Buffer in use by hw */
#define SND_TRIG_READY    0x00000008 /* Buffer no longer in use by hw */
#define SND_TRIG_ANY      0x0000000f /* Mask for all triggers */

typedef struct _SND_SMAP {
  volatile SND_TRIGGER trig_status;  /* Current Soundmap status */
  		   SND_TRIGGER trig_mask;    /* Signal trigger mask */
           signal_code trig_signal;  /* Signal to send on triggers */
  volatile error_code err_code;      /* Error code for termination */
           SND_CM coding_method;     /* Coding method */
           u_int16 num_channels;     /* Number of channels */
           u_int32 sample_size;      /* Number of bits per sample */
           u_int32 sample_rate;      /* Number of samples per sec */
           u_char *buf;              /* Sound data buffer */
           u_int32 buf_size;         /* Size of sound data buffer */
  volatile u_int32 cur_offset;       /* Current offset into "buf" */
           u_int32 loop_start;       /* Offset to start of loop */
           u_int32 loop_end;         /* Offset to end of loop */
           u_int32 loop_count;       /* Num of times to play loop */
  volatile u_int32 loop_counter;     /* Num times loop has played */
           struct _SND_SMAP *next;   /* Link to next SND_SMAP */
} SND_SMAP;

/*******************************************************************************
 * SOUND STATUS
 *
 * The I/O status is a set of unique bit-field values that may tested for, by
 * using AND (&) and OR (|) operators.
 *******************************************************************************/

typedef int SND_STATUS;
#define SND_STATUS_IDLE          0x00000000 /* Non-active state */
#define SND_STATUS_PLAY          0x00000001 /* Play operation is active */
#define SND_STATUS_PLAY_PAUSED   0x00000004 /* Play or record is paused */
#define SND_STATUS_RECORD        0x00000002 /* Record operation is active */
#define SND_STATUS_RECORD_PAUSED 0x00000004 /* Play or record is paused */
#define SND_STATUS_BUSY          0x00000008 /* Device is busy */

typedef struct _SND_DEV_STATUS {
  volatile SND_STATUS status;        /* Current status */
  volatile process_id play_pid;      /* Process ID of current play operation */
  volatile process_id record_pid;    /* Process ID of current record operation */
  volatile u_int16 num_gain;         /* Number of SND_GAIN structures */
           SND_GAIN *gain;           /* Pointer to an array num_gain SND_GAIN
                                        structures */
} SND_DEV_STATUS;


/*******************************************************************************
 * DEVICE CAPABILITIES STRUCTURE
 *******************************************************************************/

/* Mixer Device Capabilities */
typedef struct _SND_GAIN_CAP {
  SND_LINE lines;               /* Mask of gain/mixer lines */
  BOOLEAN sup_mute;             /* TRUE if mute is supported */
  SND_GAIN_CMD default_type;    /* Default gain type, e.g. mono, stereo */
  u_int16 default_level;        /* Default gain level, setting applied by
                                   SND_GAIN_CMD_RESET */
  u_int16 zero_level;           /* Gain setting where dB is zero, greater
                                   levels apply gain, and lower levels
                                   apply negative gain (attenuation) */
  u_int16 num_steps;            /* Number of actual gain steps supported
                                   by the hardware */
  int16 step_size;              /* Average size of each hardware step in
                                   100ths of a dB */
  int32  mindb;                 /* dB at SND_GAIN_MIN in 100ths of a dB */
  int32  maxdb;                 /* dB at SND_GAIN_MAX in 100ths of a dB */
} SND_GAIN_CAP;


/* Device Capabilities */
typedef struct _SND_DEV_CAP {
  char *hw_type;                /* Hardware type */
  char *hw_subtype;             /* Hardware subtype */
  SND_TRIGGER sup_triggers;     /* Supported triggers */
  SND_LINE play_lines;          /* Play gain/mix lines */
  SND_LINE record_lines;        /* Record ain/mix lines */
  SND_GAIN_CMD sup_gain_cmds;   /* Gain command mask */
  u_int16 num_gain_caps;        /* Num gain caps */
  SND_GAIN_CAP *gain_caps;      /* Ptr to gain cap array */
  u_int16 num_rates;            /* Num sample rates */
  u_int32 *sample_rates;        /* Ptr to sample rate array */
  u_int16 num_chan_info;        /* Num channel infos */
  u_int16 *channel_info;        /* Ptr to channel info array */
  u_int16 num_cm;               /* Num coding methods */
  SND_DEV_CM *cm_info;          /* Ptr to coding method array */
} SND_DEV_CAP;

#endif /* _MAUI_SND_H */

