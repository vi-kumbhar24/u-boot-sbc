/*
 * seco_imx8_env.h
 * Copyright SECO S.p.A. 
 * Mantained by marco.sandrelli@seco.com
 *
 */

#include <common.h>
#if defined CONFIG_TARGET_SECO_IMX8QM_C26
#include <seco/seco_imx8qm_c26_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8QM_C43
#include <seco/seco_imx8qm_c43_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8QXP_C57
#include <seco/seco_imx8qxp_c57_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8QXP_D16
#include <seco/seco_imx8qxp_d16_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8MM_C61
#include <seco/seco_imx8mm_c61_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8MM_C72 
#include <seco/seco_imx8mm_c72_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8MN_C72
#include <seco/seco_imx8mn_c72_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8MQ_C12
#include <seco/seco_imx8mq_c12_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8MQ_C20
#include <seco/seco_imx8mq_c20_setup.h>
#endif
#if defined CONFIG_TARGET_SECO_IMX8MQ_C25
#include <seco/seco_imx8mq_c25_setup.h>
#endif

/*
 *  FDT 
 */

#define ENV_FDT_SRC_MMC     \
        "mmc dev ${fdt_device_id}; fatload mmc ${fdt_device_id}:${fdt_partition} ${fdt_loadaddr} ${fdt_file}"

#define ENV_FDT_SRC_USD     \
        "mmc dev ${fdt_device_id}; fatload mmc ${fdt_device_id}:${fdt_partition} ${fdt_loadaddr} ${fdt_file}"

#define ENV_FDT_SRC_EXTSD   \
        "mmc dev ${fdt_device_id}; fatload mmc ${fdt_device_id}:${fdt_partition} ${fdt_loadaddr} ${fdt_file}"

#define ENV_FDT_SRC_SATA    \
        "sata init; fatload sata 0:${fdt_partition} ${fdt_loadaddr} ${fdt_file}"

#define ENV_FDT_SRC_USB     \
        "usb start; fatload usb 0:${fdt_partition} ${fdt_loadaddr} ${fdt_file}"

#define ENV_FDT_SRC_SPI     \
        "sf probe 0; sf ${fdt_loadaddr} ${fdt_spi_addr} ${fdt_spi_len}"

#define ENV_FDT_SRC_TFTP    \
        "tftp ${fdt_loadaddr} ${fdt_file}"

/*
 *  KERNEL
 */

#define ENV_KERNEL_SRC_MMC     \
        "mmc dev ${kernel_device_id}; fatload mmc ${kernel_device_id}:${kernel_partition} ${kernel_loadaddr} ${kernel_file}"

#define ENV_KERNEL_SRC_USD     \
        "mmc dev ${kernel_device_id}; fatload mmc ${kernel_device_id}:${kernel_partition} ${kernel_loadaddr} ${kernel_file}"

#define ENV_KERNEL_SRC_EXTSD   \
        "mmc dev ${kernel_device_id}; fatload mmc ${kernel_device_id}:${kernel_partition} ${kernel_loadaddr} ${kernel_file}"

#define ENV_KERNEL_SRC_SATA    \
        "sata init; fatload sata 0:${kernel_partition} ${kernel_loadaddr} ${kernel_file}"

#define ENV_KERNEL_SRC_USB     \
        "usb start; fatload usb 0:${kernel_partition} ${kernel_loadaddr} ${kernel_file}"

#define ENV_KERNEL_SRC_SPI     \
        "sf probe 0; sf ${kernel_loadaddr} ${kernel_spi_addr} ${kernel_spi_len}"

#define ENV_KERNEL_SRC_TFTP    \
        "tftp ${kernel_loadaddr} ${kernel_file}"

/*
 * HDP RX TX
 */

#define ENV_HDPRX_SRC_MMC     \
        "mmc dev ${hdprx_device_id}; fatload mmc ${hdprx_device_id}:${hdprx_partition} ${hdprx_loadaddr} ${hdprx_file}"
#define ENV_HDPTX_SRC_MMC     \
        "mmc dev ${hdptx_device_id}; fatload mmc ${hdptx_device_id}:${hdptx_partition} ${hdptx_loadaddr} ${hdptx_file}"

#define ENV_HDPRX_SRC_USD     \
        "mmc dev ${hdprx_device_id}; fatload mmc ${hdprx_device_id}:${hdprx_partition} ${hdprx_loadaddr} ${hdprx_file}"
#define ENV_HDPTX_SRC_USD     \
        "mmc dev ${hdptx_device_id}; fatload mmc ${hdptx_device_id}:${hdptx_partition} ${hdptx_loadaddr} ${hdptx_file}"

#define ENV_HDPRX_SRC_EXTSD   \
        "mmc dev ${hdprx_device_id}; fatload mmc ${hdprx_device_id}:${hdprx_partition} ${hdprx_loadaddr} ${hdprx_file}"
#define ENV_HDPTX_SRC_EXTSD   \
        "mmc dev ${hdptx_device_id}; fatload mmc ${hdptx_device_id}:${hdptx_partition} ${hdptx_loadaddr} ${hdptx_file}"

#define ENV_HDPRX_SRC_SATA    \
        "sata init; fatload sata 0:${hdprx_partition} ${hdprx_loadaddr} ${hdprx_file}"
#define ENV_HDPTX_SRC_SATA    \
        "sata init; fatload sata 0:${hdptx_partition} ${hdptx_loadaddr} ${hdptx_file}"

#define ENV_HDPRX_SRC_USB     \
        "usb start; fatload usb 0:${hdprx_partition} ${hdprx_loadaddr} ${hdprx_file}"
#define ENV_HDPTX_SRC_USB     \
        "usb start; fatload usb 0:${hdptx_partition} ${hdptx_loadaddr} ${hdptx_file}"

#define ENV_HDPRX_SRC_SPI     \
        "sf probe 0; sf ${hdprx_loadaddr} ${hdprx_spi_addr} ${hdprx_spi_len}"
#define ENV_HDPTX_SRC_SPI     \
        "sf probe 0; sf ${hdptx_loadaddr} ${hdptx_spi_addr} ${hdptx_spi_len}"

#define ENV_HDPRX_SRC_TFTP    \
        "tftp ${hdprx_loadaddr} ${hdprx_file}"
#define ENV_HDPTX_SRC_TFTP    \
        "tftp ${hdptx_loadaddr} ${hdptx_file}"

#define LOAD_ADDR_KERNEL_LOCAL_DEV    __stringify(KERNEL_LOADADDR)"\0"
#define LOAD_ADDR_KERNEL_REMOTE_DEV   __stringify(KERNEL_LOADADDR)"\0"

#define LOAD_ADDR_HDPRX_LOCAL_DEV    __stringify(HDPRX_LOADADDR)"\0"
#define LOAD_ADDR_HDPRX_REMOTE_DEV   __stringify(HDPRX_LOADADDR)"\0"
#define LOAD_ADDR_HDPTX_LOCAL_DEV    __stringify(HDPTX_LOADADDR)"\0"
#define LOAD_ADDR_HDPTX_REMOTE_DEV   __stringify(HDPTX_LOADADDR)"\0"

#define LOAD_ADDR_FDT_LOCAL_DEV       __stringify(FDT_LOADADDR)"\0"
#define LOAD_ADDR_FDT_REMOTE_DEV      __stringify(FDT_LOADADDR)"\0"

#define LOAD_ADDR_OVERLAY_LOCAL_DEV       __stringify(OVERLAY_LOADADDR_BASE)"\0"
#define LOAD_ADDR_OVERLAY_REMOTE_DEV      __stringify(OVERLAY_LOADADDR_BASE)"\0"

#define SPI_LOAD_ADDRESS              __stringify(CONFIG_SPI_KERNEL_LOADADDR)"\0"
#define SPI_LOAD_LEN                  __stringify(CONFIG_SPI_KERNEL_LEN)"\0"

/*
 *  Overlay
 */

#define ENV_OVERLAY_SRC_MMC     \
        "mmc dev ${overlay_device_id}; fatload mmc ${overlay_device_id}:${overlay_partition}"

#define ENV_OVERLAY_SRC_USD     \
        "mmc dev ${overlay_device_id}; fatload mmc ${overlay_device_id}:${overlay_partition}"

#define ENV_OVERLAY_SRC_EXTSD   \
        "mmc dev ${overlay_device_id}; fatload mmc ${overlay_device_id}:${overlay_partition}"

#define ENV_OVERLAY_SRC_SATA    \
        "sata init; fatload sata 0:${overlay_partition}"

#define ENV_OVERLAY_SRC_USB     \
        "usb start; fatload usb 0:${overlay_partition}"

#define ENV_OVERLAY_SRC_SPI     \
        "sf probe 0; sf ${overlay_loadaddr} ${overlay_spi_addr} ${overlay_spi_len}"

#define ENV_OVERLAY_SRC_TFTP    \
        "tftp"

typedef struct overlay_struct_mode {
        char *label;
        char *dtb_overlay;
} overlay_struct_mode_t;


extern overlay_struct_mode_t overlay_video_mode_list[];
extern overlay_struct_mode_t overlay_addons_list [];

extern size_t overlay_video_size;
extern size_t overlay_addons_size;

/*
 * Boot devices
 */

#define ENV_FS_COMMON  "rootwait rw"

#define ENV_FS_SRC_MMC      \
        "root=\'/dev/mmcblk${root_device_id}p${root_partition} "ENV_FS_COMMON"\'"

#define ENV_FS_SRC_USD      \
        "root=\'/dev/mmcblk${root_device_id}p${root_partition} "ENV_FS_COMMON"\'"

#define ENV_FS_SRC_EXTSD    \
        "root=\'/dev/mmcblk${root_device_id}p${root_partition} "ENV_FS_COMMON"\'"

#define ENV_FS_SRC_SATA     \
        "root=\'/dev/sda${root_partition} "ENV_FS_COMMON"\'"

#define ENV_FS_SRC_USB      \
        "root=\'/dev/sda${root_partition} "ENV_FS_COMMON"\'"

#define ENV_FS_SRC_NFS      \
        "root=\'/dev/nfs nfsroot=${nfs_ip_server}:${nfs_path},nfsvers=3 nolock,wsize=4096,rsize=4096 ip=${ip} "ENV_FS_COMMON"\'"

typedef enum {
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
        char             *load_address;
        char             *def_path;
} data_boot_dev_t;

extern data_boot_dev_t kern_dev_list [];
extern data_boot_dev_t hdprx_dev_list [];
extern data_boot_dev_t hdptx_dev_list [];
extern data_boot_dev_t fdt_dev_list [];
extern data_boot_dev_t overlay_dev_list [];
extern data_boot_dev_t filesystem_dev_list [];

extern size_t kern_dev_size;
extern size_t hdprx_dev_size;
extern size_t hdptx_dev_size;
extern size_t fdt_dev_size;
extern size_t overlay_dev_size;
extern size_t filesystem_dev_size; 
