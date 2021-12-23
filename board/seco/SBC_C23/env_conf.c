#include <common.h>
#include <command.h>
#include <environment.h>
#include <configs/mx6_seco_dtbo.h>


#include <seco_env.h>

DECLARE_GLOBAL_DATA_PTR;
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)



/* *********************************** IMX6SX *********************************** */

data_boot_dev_t kern_dev_imx6sx_list [] = {
	{ DEV_EMMC,     "eMMC onboard",   STR(MACRO_ENV_KERNEL_SRC_USDHCI),   SCFG_BOOT_DEV_ID_EMMC,    "zImage" },
	{ DEV_EXT_SD,   "external SD",    STR(MACRO_ENV_KERNEL_SRC_USDHCI),   SCFG_BOOT_DEV_ID_EXT_SD,  "zImage" },
	{ DEV_SPI,      "SPI onboard",    STR(MACRO_ENV_KERNEL_SRC_SPI),      SCFG_BOOT_DEV_ID_SPI,     "zImage" },
	{ DEV_TFTP,     "TFTP",           STR(MACRO_ENV_KERNEL_SRC_TFTP),     "",                       "zImage" },
	{ DEV_USB,      "USB",            STR(MACRO_ENV_KERNEL_SRC_USB),      SCFG_BOOT_DEV_ID_USB,	"zImage" },
};


data_boot_dev_t fdt_dev_imx6sx_list [] = {
	{ DEV_EMMC,     "eMMC onboard",   STR(MACRO_ENV_FDT_SRC_USDHCI),    SCFG_BOOT_DEV_ID_EMMC,    SCFG_DEFAULT_FDT_IMX6SX_FILE },
	{ DEV_EXT_SD,   "external SD",    STR(MACRO_ENV_FDT_SRC_USDHCI),    SCFG_BOOT_DEV_ID_EXT_SD,  SCFG_DEFAULT_FDT_IMX6SX_FILE },
	{ DEV_SPI,      "SPI onboard",    STR(MACRO_ENV_FDT_SRC_SPI),       SCFG_BOOT_DEV_ID_SPI,     ""                           },
	{ DEV_TFTP,     "TFTP",           STR(MACRO_ENV_FDT_SRC_TFTP),      "",                       SCFG_DEFAULT_FDT_IMX6SX_FILE },
	{ DEV_USB,      "USB",            STR(MACRO_ENV_FDT_SRC_USB),       SCFG_BOOT_DEV_ID_USB,     SCFG_DEFAULT_FDT_IMX6SX_FILE },
};


#ifdef CONFIG_OF_LIBFDT_OVERLAY
data_boot_dev_t fdt_overlay_dev_imx6sx_list [] = {
	{ DEV_EMMC,     "eMMC onboard",   STR(MACRO_ENV_FDT_OVERLAY_SRC_USDHCI),    SCFG_BOOT_DEV_ID_EMMC,    "" },
	{ DEV_EXT_SD,   "external SD",    STR(MACRO_ENV_FDT_OVERLAY_SRC_USDHCI),    SCFG_BOOT_DEV_ID_EXT_SD,  "" },
	{ DEV_SPI,      "SPI onboard",    STR(MACRO_ENV_FDT_OVERLAY_SRC_SPI),       SCFG_BOOT_DEV_ID_SPI,     "" },
	{ DEV_TFTP,     "TFTP",           STR(MACRO_ENV_FDT_OVERLAY_SRC_TFTP),      "",                       "" },
	{ DEV_USB,      "USB",            STR(MACRO_ENV_FDT_OVERLAY_SRC_USB),       SCFG_BOOT_DEV_ID_USB,     "" },
};
#endif  /* CONFIG_OF_LIBFDT_OVERLAY  */


data_boot_dev_t ramfs_dev_imx6sx_list [] = {
	{ DEV_NONE,     "Not use",        "0x0",                            "0",                      ""          },
	{ DEV_EMMC,     "eMMC onboard",   STR(MACRO_ENV_RAMFS_SRC_USDHCI),  SCFG_BOOT_DEV_ID_EMMC,    "ramfs.img" },
	{ DEV_EXT_SD,   "external SD",    STR(MACRO_ENV_RAMFS_SRC_USDHCI),  SCFG_BOOT_DEV_ID_EXT_SD,  "ramfs.img" },
	{ DEV_SPI,      "SPI onboard",    STR(MACRO_ENV_RAMFS_SRC_SPI),     SCFG_BOOT_DEV_ID_SPI,     "ramfs.img" },
	{ DEV_TFTP,     "TFTP",           STR(MACRO_ENV_RAMFS_SRC_TFTP),    "",                       "ramfs.img" },
	{ DEV_USB,      "USB",            STR(MACRO_ENV_RAMFS_SRC_USB),     SCFG_BOOT_DEV_ID_USB,     "ramfs.img" },
};


data_boot_dev_t filesystem_dev_imx6sx_list [] = {
	{ DEV_EMMC,     "eMMC onboard",    STR(MACRO_ENV_FS_SRC_USDHCI),  SCFG_ROOT_DEV_ID_EMMC,    "" },
	{ DEV_EXT_SD,   "external SD",     STR(MACRO_ENV_FS_SRC_USDHCI),  SCFG_ROOT_DEV_ID_EXT_SD,  "" },
	{ DEV_NFS,      "NFS",             STR(MACRO_ENV_FS_SRC_NFS),     "",                       "" },
	{ DEV_USB,      "USB",             STR(MACRO_ENV_FS_SRC_USB),     "",		            "" },
};

#ifdef CONFIG_TARGET_MX6SX_SECO_SBC_C23
video_mode_t video_mode_list [] = {
	{   
        .label    = "no display",
        .fb_lvds1 = { VIDEO_NOT_USED, },
		.panel_name = "none",
		.dtbo_conf_file = NULL,
        .primary  = 0,
    }, {   
        .label    = "LVDS",
        .fb_lvds1 = { VIDEO_USED, "mxcfb0", "ldb" },
		.panel_name = "LDB-WVGA",
#ifdef CONFIG_OF_LIBFDT_OVERLAY
		.dtbo_conf_file = STR(ENV_DTBO_SBC_C23_LVDS),
#else
		.dtbo_conf_file = "",
#endif
        .primary  = VIDEO_PRIMARY_LVDS,
    },
};
#endif

#ifdef CONFIG_OF_LIBFDT_OVERLAY
/* *********************************** FDT OVERLAY *********************************** */

overlay_list_t overlay_peripheral_list [] = {
};
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
