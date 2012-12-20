/******************************************************************************
 *
 * DESCRIPTION: List device capability information
 *
 * CREATED ON:  09/18/97
 *
 * COPYRIGHT:   Copyright 1997 Microware Systems Corporation. All Rights
 *              Reserved. Use and/or distribution subject to terms and
 *              conditions of applicable Microware license agreement(s).
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <MAUI/maui_gfx.h>
#include <MAUI/maui_cdb.h>
#include <MAUI/mauidemo.h>

#define BOOL_STRING(exp) ((exp) ? "TRUE" : "FALSE")

void list_res(u_int32 idx, GFX_DEV_RES *devres) {
  printf("Resolution %d:\n", idx);
  printf("    Size = %dx%d\n", devres->disp_width,
         devres->disp_height);
  printf("    Refresh rate = %d\n", devres->refresh_rate);
  switch (devres->intl_mode) {
  case GFX_INTL_OFF:
    printf("    Interlace mode = GFX_INTL_OFF\n");
    break;
  case GFX_INTL_ON:
    printf("    Interlace mode = GFX_INTL_ON\n");
    break;
  case GFX_INTL_REPEAT:
    printf("    Interlace mode = GFX_INTL_REPEAT\n");
    break;
  default:
    printf("    Interlace mode = Unknown\n");
    break;
  }
  printf("    Aspect = %dx%d\n", devres->aspect_x, devres->aspect_y);
  printf("\n");
}

void list_cm(u_int32 idx, GFX_DEV_CM *devcm)
{
  GFX_COLOR_TYPE *color_type;   /* Color type */
  u_int32 j;                    /* Loop counter */

  printf("Coding method %d:\n    Name = ", idx);
  switch (gfx_get_cm_name(devcm->coding_method)) {
  case GFX_CM_1BIT:
    printf("GFX_CM_1BIT\n");
    break;
  case GFX_CM_2BIT:
    printf("GFX_CM_2BIT\n");
    break;
  case GFX_CM_3BIT:
    printf("GFX_CM_3BIT\n");
    break;
  case GFX_CM_4BIT:
    printf("GFX_CM_4BIT\n");
    break;
  case GFX_CM_5BIT:
    printf("GFX_CM_5BIT\n");
    break;
  case GFX_CM_6BIT:
    printf("GFX_CM_6BIT\n");
    break;
  case GFX_CM_7BIT:
    printf("GFX_CM_7BIT\n");
    break;
  case GFX_CM_8BIT:
    printf("GFX_CM_8BIT\n");
    break;
  case GFX_CM_RGB555:
    printf("GFX_CM_RGB555\n");
    break;
  case GFX_CM_RGB888:
    printf("GFX_CM_RGB888\n");
    break;
  case GFX_CM_CDI_RL3:
    printf("GFX_CM_CDI_RL3\n");
    break;
  case GFX_CM_CDI_RL7:
    printf("GFX_CM_CDI_RL7\n");
    break;
  case GFX_CM_1A7_8BIT:
    printf("GFX_CM_1A7_8BIT\n");
    break;
  case GFX_CM_A1_RGB555:
    printf("GFX_CM_A1_RGB555\n");
    break;
  case GFX_CM_YCBCR422:
    printf("GFX_CM_YCBCR422\n");
    break;
  case GFX_CM_YCRCB422:
    printf("GFX_CM_YCRCB422\n");
    break;
  case GFX_CM_YCBCR420:
    printf("GFX_CM_YCBCR420\n");
    break;
  case GFX_CM_YCRCB420:
    printf("GFX_CM_YCRCB420\n");
    break;
  case GFX_CM_RGB565:
    printf("GFX_CM_RGB565\n");
    break;
  case GFX_CM_RGB655:
    printf("GFX_CM_RGB655\n");
    break;
  case GFX_CM_RGB556:
    printf("GFX_CM_RGB556\n");
    break;
  default:
    printf("Unknown\n");
    break;
  }
  printf("    ByteOrder = ");
  if (gfx_get_cm_byte_order(devcm->coding_method) == 0) {
    printf("Big\n");
  } else {
    printf("Little\n");
  }
  printf("    BitOrder = ");
  if (gfx_get_cm_bit_order(devcm->coding_method) == 0) {
    printf("Big\n");
  } else {
    printf("Little\n");
  }
  printf("    Depth = ");
  switch (gfx_get_cm_depth(devcm->coding_method)) {
  case 0:
    printf("See name\n");
    break;
  case 1:
    printf("1\n");
    break;
  case 2:
    printf("2\n");
    break;
  case 3:
    printf("4\n");
    break;
  case 4:
    printf("8\n");
    break;
  case 5:
    printf("16\n");
    break;
  case 6:
    printf("32\n");
    break;
  case 7:
    printf("Reserved\n");
    break;
  default:
    printf("Unknown\n");
    break;
  }
  printf("    Align = %d\n", 1 << (gfx_get_cm_align(devcm->coding_method) + 2));
  printf("    X Multiplier = %d\n", devcm->dm2dp_xmul);
  printf("    Y Multiplier = %d\n", devcm->dm2dp_ymul);
  printf("    CLUT based = %s\n", BOOL_STRING(devcm->clut_based));
  printf("    Num Color Types = %d\n", devcm->num_color_types);
  printf("    Color Types = (");
  color_type = &devcm->color_types[0];
  for (j = devcm->num_color_types; j-- > 0; color_type++) {
    switch (*color_type) {
    case GFX_COLOR_RGB:
      printf("GFX_COLOR_RGB");
      break;
    case GFX_COLOR_YUV:
      printf("GFX_COLOR_YUV");
      break;
    case GFX_COLOR_A1_RGB:
      printf("GFX_COLOR_A1_RGB");
      break;
    case GFX_COLOR_YCBCR:
      printf("GFX_COLOR_YCBCR");
      break;
    default:
      printf("Unknown");
      break;
    }
    if (j) {
      printf(", ");
    }
  }
  printf(")\n");
}

void main(int argc, char *argv[])
{
  GFX_DEV_ID gfxdev;            /* Graphics device ID */
  const GFX_DEV_CAP *devcap;    /* Device capabilities */
  const GFX_DEV_CAPEXTEN *devcapexten; /* Extended Device capabilities */
  GFX_DEV_RES *devres;          /* Default device resolution */
  GFX_DEV_CM *devcm;            /* Default device coding method */
  u_int32 i;                    /* Loop counters */
  u_int32 devnum;               /* Ordinal device number in CDB */
  char devname[CDB_MAX_DNAME];  /* Name of device from CDB */
  error_code ec;                /* Error code temporary */
  
  /* Get devcap info */
  cdb_init();
  gfx_set_error_action(MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT, MAUI_ERR_FATAL);
  gfx_init();
  
  devnum = 1; 

  /* don't print CDB error messages */
  cdb_set_error_action(MAUI_ERR_NONE, MAUI_ERR_DEFAULT, MAUI_ERR_DEFAULT);

  /* for each graphic device on the system */
  while (cdb_get_ddr(CDB_TYPE_GRAPHIC, devnum, devname, NULL) == SUCCESS) {
    
    printf("\nDevice number = %d\n", devnum++);
    printf("Device name = %s\n", devname);
    
    /* Open the graphics device */
    if ((ec = gfx_open_dev(&gfxdev, devname)) != SUCCESS) {
      demo_error(ec, "*** Unable to open %s ***\n", devname);
      continue;
    }
    
    /* Get device capabilities */
    if ((ec = gfx_get_dev_cap(&devcap, gfxdev)) != SUCCESS) {
      demo_error(ec, "*** Unable to get device capabilities of %s ***\n", devname);
      gfx_close_dev(gfxdev);
      continue;
    }
    
    /* List information from main structure */
    printf("Hardware type = %s\n", devcap->hw_type);
    printf("Hardware subtype = %s\n", devcap->hw_subtype);
    printf("Supports viewport mixing = %s\n", BOOL_STRING(devcap->sup_vpmix));
    printf("Supports external video = %s\n", BOOL_STRING(devcap->sup_extvid));
    printf("Supports backdrop color = %s\n", BOOL_STRING(devcap->sup_bkcol));
    printf("Supports viewport transparency = %s\n",
           BOOL_STRING(devcap->sup_vptrans));
    printf("Supports viewport intensity = %s\n",
           BOOL_STRING(devcap->sup_vpinten));
    printf("Supports retrace synchronization = %s\n",
           BOOL_STRING(devcap->sup_sync));
    printf("Supports video decoding = %s\n", BOOL_STRING(devcap->sup_decode));
    printf("DAC depth = %d\n", devcap->dac_depth);
    printf("\n");
    
    /* List resolution information */
    devres = &devcap->res_info[0];
    i = 0;
    while (i < devcap->num_res)
      list_res(i++, devres++);
    
    /* List coding method information */
    devcm = &devcap->cm_info[0];
    i = 0;
    while (i < devcap->num_cm)
      list_cm(i++, devcm++);

    /* List extended device capabilities if available */
    ec = gfx_get_dev_capexten(&devcapexten, gfxdev);
    if (ec != SUCCESS) {
      printf("Extended Device Capabilties unavailable due to: ");
      switch (ec) {
      case EOS_ITRAP:
        printf("Not supported by shared library\n");
        break;
      case EOS_UNKSVC:
        printf("Not supported by driver common code\n");
        break;
      case EOS_MAUI_NODVSUPPORT:
        printf("Not supported by driver specific code\n");
        break;
      default:
        printf("\n");
        demo_error(ec, "*** trying to get the extended device capabilities of %s ***\n", devname);
        break;
      }
    } else {
      const GFX_DEV_MODES *mi = &devcapexten->mode_info[0];
      char *s;

      printf("Extension size = %d\n", devcapexten->version);
      printf("Modes supported = %d\n", devcapexten->num_modes);
      for (i = devcapexten->num_modes; i-->0; mi++) {
        printf("\tres_idx: %d\tcm_idx %d\t%s\n", mi->res_idx, mi->cm_idx, 
               (mi->desc ? mi->desc : "NO DESCRIPTION"));
      }
      
      switch (devcapexten->vp_complexity) {
      case GFX_VPC_UNKNOWN:		s = "GFX_VPC_UNKNOWN";		break;
      case GFX_VPC_OTHER:		s = "GFX_VPC_OTHER";		break;
      case GFX_VPC_ONE_EXACT:	s = "GFX_VPC_ONE_EXACT";	break;
      case GFX_VPC_ONE_ANY:		s = "GFX_VPC_ONE_ANY";		break;
      case GFX_VPC_ANY_FULL:	s = "GFX_VPC_ANY_FULL";		break;
      case GFX_VPC_ANY_NOSHARE:	s = "GFX_VPC_ANY_NOSHARE"; 	break;
      case GFX_VPC_ANY_COMPLEX:		s = "GFX_VPC_COMPLEX"; 		break;
      default:
        if (devcapexten->vp_complexity >= GFX_VPC_DEVSPECIFIC)
          s = "GFX_VPC_DEVSPECIFIC";
        else
          s = "unknown value";
        break;
      }
      printf("Viewport complexity = %d (%s)\n", devcapexten->vp_complexity, s);

      switch (devcapexten->vpdm_complexity) {
      case GFX_VPDMC_UNKNOWN:	s = "GFX_VPDMC_UNKNOWN"; 	break;
      case GFX_VPDMC_OTHER:		s = "GFX_VPDMC_OTHER"; 		break;
      case GFX_VPDMC_EXACT:		s = "GFX_VPDMC_EXACT"; 		break;
      case GFX_VPDMC_TALLER:	s = "GFX_VPDMC_TALLER"; 	break;
      case GFX_VPDMC_WIDER:		s = "GFX_VPDMC_WIDER"; 		break;
      case GFX_VPDMC_LARGER:	s = "GFX_VPDMC_LARGER"; 	break;

      default:
        if (devcapexten->vpdm_complexity >= GFX_VPDMC_DEVSPECIFIC)
          s = "GFX_VPDMC_DEVSPECIFIC";
        else
          s = "unknown value";
        break;
      }
      printf("Viewport Drawmap complexity = %d (%s)\n", devcapexten->vpdm_complexity, s);
    }
    printf("\n");
    
    gfx_close_dev(gfxdev);
  }
  
  /* Shut down */
  gfx_term();
  cdb_term();
  exit(SUCCESS);
}
