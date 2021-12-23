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
	NO_VIDEO = 0,
	HDMI = 1,
	LVDS_S,
	LVDS_D,
	DP,
	LVDS_HDMI,
	LVDS_DP,
	eDP = 20,		/* Not used inserted for compatibility with c43 secoconfig choice */
	HDMI_LVDS_eDP = 21,	/* Not used inserted for compatibility with c43 secoconfig choice */
	HDMI_eDP = 22, 		/* Not used inserted for compatibility with c43 secoconfig choice */
	NUM_VIDEO_MODES,
} VIDEO_MODES;

typedef enum  {
	NOADDONS = 0,
	OV5640_CSI0,
	OV5640_CSI1,
	HDMI_IN,
	NUM_ADDONS,
} ADDONS;

#define BOOT_DEV_ID_EMMC      __stringify(BOOT_ID_EMMC)"\0"
#define BOOT_DEV_ID_U_SD      __stringify(BOOT_ID_USD)"\0"
#define BOOT_DEV_ID_EXT_SD    __stringify(BOOT_ID_EXT_SD)"\0"
#define BOOT_DEV_ID_SPI       "0"
#define BOOT_DEV_ID_SATA      "0"
#define BOOT_DEV_ID_USB       "0"

#define ROOT_DEV_ID_EMMC      __stringify(ROOT_ID_EMMC)"\0"
#define ROOT_DEV_ID_U_SD      __stringify(ROOT_ID_USD)"\0"
#define ROOT_DEV_ID_EXT_SD    __stringify(ROOT_ID_EXT_SD)"\0"

int c26_get_board_configuration (void);

typedef enum {
	RAM_1GB = 0,
	RAM_2GB = 0x1,
	RAM_4GB	= 0x2,
	RAM_8GB = 0x3,
} RAM_STRAPS;

#define GET_C26_STRAPS	(c26_get_board_configuration())



#define C26_IS_1GB 	(GET_C26_STRAPS == RAM_1GB)
#define C26_IS_2GB      (GET_C26_STRAPS == RAM_2GB)
#define C26_IS_4GB      (GET_C26_STRAPS == RAM_4GB)
#define C26_IS_8GB      (GET_C26_STRAPS == RAM_8GB)
