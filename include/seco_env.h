

typedef enum {
	DEV_NONE,
	DEV_EMMC,
	DEV_U_SD,
	DEV_EXT_SD,
	DEV_NAND,
	DEV_SPI,
	DEV_SATA,
	DEV_USB,
	DEV_TFTP,
	DEV_NFS,
} device_t;


typedef struct data_boot_dev {
	device_t         dev_type;
	char             *label;
	char             *env_str;
	char             *device_id;
	char             *def_path;
} data_boot_dev_t;


#ifdef CONFIG_OF_LIBFDT_OVERLAY
typedef struct overlay_struct_mode {
    char *label;
    char *dtb_overlay;
} overlay_struct_mode_t;


typedef struct overlay_list {
	char *title;
	overlay_struct_mode_t options[10];
} overlay_list_t;
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */


#if (defined(CONFIG_MX6Q)) || (defined(CONFIG_MX6DL)) || (defined(CONFIG_MX6QDL))
extern data_boot_dev_t kern_dev_imx6q_list [SCFG_KERNEL_BOOT_DEV_IMX6Q_NUM];
extern data_boot_dev_t fdt_dev_imx6q_list [SCFG_FDT_BOOT_DEV_IMX6Q_NUM];
#ifdef CONFIG_OF_LIBFDT_OVERLAY
extern data_boot_dev_t fdt_overlay_dev_imx6q_list [SCFG_FDT_BOOT_DEV_IMX6Q_NUM];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
extern data_boot_dev_t ramfs_dev_imx6q_list [SCFG_RAMFS_BOOT_DEV_IMX6Q_NUM];
extern data_boot_dev_t filesystem_dev_imx6q_list [SCFG_ROOTFS_BOOT_DEV_IMX6Q_NUM];

extern data_boot_dev_t kern_dev_imx6dl_list [SCFG_KERNEL_BOOT_DEV_IMX6DL_NUM];
extern data_boot_dev_t fdt_dev_imx6dl_list [SCFG_FDT_BOOT_DEV_IMX6DL_NUM];
#ifdef CONFIG_OF_LIBFDT_OVERLAY
extern data_boot_dev_t fdt_overlay_dev_imx6dl_list [SCFG_FDT_BOOT_DEV_IMX6Q_NUM];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
extern data_boot_dev_t ramfs_dev_imx6dl_list [SCFG_RAMFS_BOOT_DEV_IMX6DL_NUM];
extern data_boot_dev_t filesystem_dev_imx6dl_list [SCFG_ROOTFS_BOOT_DEV_IMX6DL_NUM];
#endif

#if (defined(CONFIG_MX6SX))
extern data_boot_dev_t kern_dev_imx6sx_list [SCFG_KERNEL_BOOT_DEV_IMX6SX_NUM];
extern data_boot_dev_t fdt_dev_imx6sx_list [SCFG_FDT_BOOT_DEV_IMX6SX_NUM];
extern data_boot_dev_t ramfs_dev_imx6sx_list [SCFG_RAMFS_BOOT_DEV_IMX6SX_NUM];
extern data_boot_dev_t filesystem_dev_imx6sx_list [SCFG_ROOTFS_BOOT_DEV_IMX6SX_NUM];
#ifdef CONFIG_OF_LIBFDT_OVERLAY
extern data_boot_dev_t fdt_overlay_dev_imx6sx_list [SCFG_FDT_BOOT_DEV_IMX6SX_NUM];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
#endif

#ifdef CONFIG_OF_LIBFDT_OVERLAY
extern overlay_list_t overlay_peripheral_list [SCFG_FDT_OVERLAY_PERIPHERAL_NUM];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */


#define MIN_PARTITION_ID 1
#define MAX_PARTITION_ID 9
#define MAX_DEVICE 4


extern int atoi (char *string);
extern int ctoi (char ch);
extern char *getline (void);


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   KERNEL                                 |
 * |__________________________________________________________________________|
*/
#define GET_KERNEL_PATH                   env_get ("kernel_file")

#define SAVE_KERNEL_DEVICE_ID(x)          env_set ("kernel_device_id", (x))
#define SAVE_KERNEL_PARTITION(x)          env_set ("kernel_partition", (x))
#define SAVE_KERNEL_PATH(x)               env_set ("kernel_file", (x))
#define SAVE_KERNEL_SPI_ADDR(x)           env_set ("kernel_spi_addr", (x))
#define SAVE_KERNEL_SPI_LEN(x)            env_set ("kernel_spi_len", (x))
#define GET_ENV_KERNEL_SRC(srv)           ( ENV_KERNEL_SRC_ ## (src) )
#define SAVE_KERNEL_BOOT_LOAD(x)          env_set ("kernel_load2ram", (x))


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                    FDT                                   |
 * |__________________________________________________________________________|
*/
#define GET_FDT_PATH                   env_get ("fdt_file")

#define SAVE_FDT_DEVICE_ID(x)          env_set ("fdt_device_id", (x))
#define SAVE_FDT_PARTITION(x)          env_set ("fdt_partition", (x))
#define SAVE_FDT_PATH(x)               env_set ("fdt_file", (x))
#define SAVE_FDT_SPI_ADDR(x)           env_set ("fdt_spi_addr", (x))
#define SAVE_FDT_SPI_LEN(x)            env_set ("fdt_spi_len", (x))
#define GET_ENV_FDT_SRC(srv)           ( ENV_FDT_SRC_ ## (src) )
#define SAVE_FDT_BOOT_LOAD(x)          env_set ("fdt_load2ram", (x))

#define GET_FDT_BOOT_LOAD              env_get ("fdt_load2ram")

#ifdef CONFIG_OF_LIBFDT_OVERLAY
#define SAVE_FDT_OVERLAY_VIDEO_LIST(x)  env_set ("fdt_overlay_video_list", (x))
#define GET_FDT_OVERLAY_VIDEO_LIST      env_get ("fdt_overlay_video_list")

#define SAVE_FDT_OVERLAY_VIDEO_CMD(x)   env_set ("fdt_overlay_video_cmd", (x))
#define GET_FDT_OVERLAY_VIDEo_CMD       env_get ("fdt_overlay_video_cmd")

#define SAVE_FDT_OVERLAY_PER_LIST(x)    env_set ("fdt_overlay_per_list", (x))
#define GET_FDT_OVERLAY_PER_LIST        env_get ("fdt_overlay_per_list")

#define SAVE_FDT_OVERLAY_PER_CMD(x)     env_set ("fdt_overlay_per_cmd", (x))
#define GET_FDT_OVERLAY_PER_CMD         env_get ("fdt_overlay_per_cmd")

#define SAVE_FDT_OVERLAY_LOAD(x)        env_set ("fdt_overlay_per_cmd", (x))
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   RAMFS                                  |
 * |__________________________________________________________________________|
*/
#define GET_RAMFS_PATH                    env_get ("ramfs_file")

#define SAVE_RAMFS_DEVICE_ID(x)           env_set ("ramfs_device_id", (x))
#define SAVE_RAMFS_PARTITION(x)           env_set ("ramfs_partition", (x))
#define SAVE_RAMFS_PATH(x)                env_set ("ramfs_file", (x))
#define SAVE_RAMFS_SPI_ADDR(x)            env_set ("ramfs_spi_addr", (x))
#define SAVE_RAMFS_SPI_LEN(x)             env_set ("ramfs_spi_len", (x))
#define GET_ENV_RAMFS_SRC(srv)            ( ENV_RAMFS_SRC_ ## (src) )
#define SAVE_RAMFS_BOOT_LOAD(x)           env_set ("ramfs_load2ram", (x))


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                 FILE SYSTEM                              |
 * |__________________________________________________________________________|
*/
#define SAVE_FILESYSTEM_DEVICE_ID(x)       env_set ("root_device_id", (x))
#define SAVE_FILESYSTEM_PARTITION(x)       env_set ("root_partition", (x))
#define SAVE_FILESYSTEM_ROOT(x)            env_set ("root_mount", (x))
#define GET_ENV_FS_SRC(srv)                ( ENV_FS_SRC_ ## src )



/*  __________________________________________________________________________
 * |                                                                          |
 * |                               NFS PARAMETERS                             |
 * |__________________________________________________________________________|
*/

#define GET_NFS_IP_CLIENT        env_get ("nfs_ip_client")
#define GET_NFS_IP_SERVER        env_get ("nfs_ip_server")
#define GET_NFS_NETMASK          env_get ("nfs_netmask")
#define GET_NFS_PATH             env_get ("nfs_path")

#define SAVE_NFS_USE(x)          env_set ("run_from_nfs", (x))
#define SAVE_NFS_IP_CLIENT(x)    env_set ("nfs_ip_client", (x))
#define SAVE_NFS_IP_SERVER(x)    env_set ("nfs_ip_server", (x))
#define SAVE_NFS_NETMASK(x)      env_set ("nfs_netmask", (x))
#define SAVE_NFS_PATH(x)         env_set ("nfs_path", (x))
#define SAVE_NFS_USE_DHCP(x)     env_set ("nfs_use_dhcp", (x))




/*  __________________________________________________________________________
 * |                                                                          |
 * |                               VIDEO PARAMETERS                           |
 * |__________________________________________________________________________|
*/

#define GET_VIDEO_PANEL          env_get( "panel")
#define SET_VIDEO_PANEL(x)       env_set( "panel", (x))

#define VIDEO_PRIMARY_HDMI   1
#define VIDEO_PRIMARY_LVDS   2
#define VIDEO_PRIMARY_RGB    3

#define VIDEO_NOT_USED       -1
#define VIDEO_USED           1



typedef struct video_hw {
	int   used;
	char  *buffer;
	char  *driver;
} video_hw_t;


typedef struct video_mode {
	char        *label;
	video_hw_t  fb_hdmi;
	video_hw_t  fb_lvds1;
	video_hw_t  fb_lvds2;
	char        *panel_name;
	char        *dtbo_conf_file;
	int         primary;
} video_mode_t;


typedef struct lvds_video_spec {
	char *label;
	char *name;
	char *if_map;
	char *datamap;
	char *opt;
	int  num_channel;
} lvds_video_spec_t;


extern video_mode_t video_mode_list [SCFG_VIDEO_MODE_NUM];

/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */

