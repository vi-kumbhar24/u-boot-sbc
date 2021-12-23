/*
 * 
 * Copyright SECO S.p.A. 
 * Mantained by marco.sandrelli@seco.com
 *
 */

/*
 *	This typedef must be matched with:
 *		overlay_video_mode_list[]
 *		overlay_addons_list[]
 *	in env_conf.c files 
 *
*/

typedef enum {
      HDMI = 0,
      HDMI_4K,
      LVDS_S,
      LVDS_D,
      DP,
      LVDS_HDMI,
      LVDS_DP,
      NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
      NOADDONS = 0,
      OV5640_CSI0,
      OV5640_CSI1,
      NUM_ADDONS,
} ADDONS;

#define BOOT_DEV_ID_EMMC      __stringify(BOOT_ID_EMMC)"\0"
#define BOOT_DEV_ID_SPI       "0"
#define BOOT_DEV_ID_SATA      "0"
#define BOOT_DEV_ID_USB       "0"

#define ROOT_DEV_ID_EMMC      __stringify(ROOT_ID_EMMC)"\0"

int c57_get_board_configuration (void);

typedef enum {
	RAM_1GB = 0,
	RAM_2GB = 0x1,
	RAM_4GB = 0x2,
} RAM_STRAPS;

#define GET_C57_STRAPS	(c57_get_board_configuration())

#define C57_IS_1GB 	(GET_C57_STRAPS == RAM_1GB)
#define C57_IS_2GB      (GET_C57_STRAPS == RAM_2GB)
#define C57_IS_4GB      (GET_C57_STRAPS == RAM_4GB)
