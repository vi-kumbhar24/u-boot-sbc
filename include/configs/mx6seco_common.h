
#ifndef __MX6SECO_COMMON_H
#define __MX6SECO_COMMON_H

#include "mx6_common.h"
#include "imx_env.h"

#ifdef CONFIG_SPL

	#include "imx6_spl.h"

	#define CONFIG_SYS_SPI_U_BOOT_OFFS     0x12000

#endif

#ifdef CONFIG_SPL_BUILD
	#define CONFIG_SPL_DRIVERS_MISC_SUPPORT
#endif

#ifdef CONFIG_MX6S
#define SYS_NOSMP "nosmp"
#else
#define SYS_NOSMP
#endif


/* ____________________________________________________________________________
  |                                                                            |
  |                                   MEMORY                                   |
  |____________________________________________________________________________|
*/
/* Physical Memory Map */

#define PHYS_SDRAM                               MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE                    PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR                 IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE                 IRAM_SIZE

#define DEFAULT_CMA_VALUE                        396


#if defined(CONFIG_MX6SL) || defined(CONFIG_MX6SLL) || \
	defined(CONFIG_MX6SX) || \
	defined(CONFIG_MX6UL) || defined(CONFIG_MX6ULL)
#define CONFIG_FDT_LOADADDR                      0x85700000
#else
#define CONFIG_FDT_LOADADDR                      0x18000000
#endif

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)


#if defined CONFIG_SECOMX6_256MB_1x256

	#define CONFIG_DDR_MB 256
	#define CONFIG_DDR_32BIT
	#define PHYS_SDRAM_SIZE        (256u * 1024 * 1024)

#elif defined CONFIG_SECOMX6_512MB_2x256

	#define CONFIG_DDR_MB 512
	#define CONFIG_DDR_32BIT
	#define PHYS_SDRAM_SIZE        (512u * 1024 * 1024)

#elif defined CONFIG_SECOMX6_1GB_2x512

	#define CONFIG_DDR_MB 1024
	#define CONFIG_DDR_64BIT
	#define PHYS_SDRAM_SIZE        (1u * 1024 * 1024 * 1024)

#elif defined CONFIG_SECOMX6_1GB_4x256

	#define CONFIG_DDR_MB 1024
	#define CONFIG_DDR_64BIT
	#define PHYS_SDRAM_SIZE        (1u * 1024 * 1024 * 1024)

#elif defined CONFIG_SECOMX6_2GB_4x512

	#define CONFIG_DDR_MB 2048
	#define CONFIG_DDR_64BIT
	#define PHYS_SDRAM_SIZE        (2u * 1024 * 1024 * 1024)

#elif defined CONFIG_SECOMX6_4GB_8x512

	#define CONFIG_DDR_MB 3800
	#define CONFIG_DDR_64BIT
	#define PHYS_SDRAM_SIZE        (4u * 1024 * 1024 * 1024 - 256u * 1024 * 1024)

#elif defined CONFIG_SECOMX6_DDR_DYNAMIC_SIZE

	#define CONFIG_DDR_MB_DYNAMIC

#endif


/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN                    (16 * SZ_1M)


/* Miscellaneous configurable options */
#define CONFIG_SYS_MEMTEST_START                 0x10000000
#define CONFIG_SYS_MEMTEST_END                   0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH               0x10800000




/* ____________________________________________________________________________
  |                                                                            |
  |                                     UART                                   |
  |____________________________________________________________________________|
*/
#define CONFIG_MXC_UART
#define CONFIG_CONS_INDEX                        1
#define CONFIG_BAUDRATE                          115200



/* ____________________________________________________________________________
  |                                                                            |
  |                                       I2C                                  |
  |____________________________________________________________________________|
*/
#define CONFIG_SYS_I2C_SPEED		             100000



/* ____________________________________________________________________________
  |                                                                            |
  |                                      PMIC                                  |
  |____________________________________________________________________________|
*/
#if 0
#ifndef CONFIG_DM_PMIC
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	         0x08
#define CONFIG_POWER_PFUZE3000
#define CONFIG_POWER_PFUZE3000_I2C_ADDR	         0x08
#endif
#define PFUZE3000_SW1AB_SETP(x)                  ((x - 7000) / 250)
#define PFUZE3000_SW3_SETP(x)                    ((x - 9000) / 500)
#define PFUZE3000_VLDO_SETP(x)                   ((x - 8000) / 500)
#endif 


/* ____________________________________________________________________________
  |                                                                            |
  |                              DISPLAY (FRAMEBUFFER)                         |
  |____________________________________________________________________________|
*/
#ifdef CONFIG_VIDEO
	#define CONFIG_VIDEO_IPUV3
	#define CONFIG_VIDEO_BMP_RLE8
	#define CONFIG_SPLASH_SCREEN
	#define CONFIG_SPLASH_SCREEN_ALIGN
	#define CONFIG_BMP_16BPP
	#define CONFIG_VIDEO_LOGO
	#define CONFIG_VIDEO_BMP_LOGO
	#define CONFIG_IMX_HDMI
	#define CONFIG_IMX_VIDEO_SKIP
#endif



/* ____________________________________________________________________________
  |                                                                            |
  |                             ENV MEM SIZE/OFFSET                            |
  |____________________________________________________________________________|
*/
/* #define CONFIG_ENV_IS_IN_MMC */
/* #define CONFIG_SYS_NO_FLASH  */

#define CONFIG_ENV_SIZE                       SZ_8K

#if defined(CONFIG_ENV_IS_IN_MMC)

	#define CONFIG_ENV_OFFSET                (896 * 1024)

#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)

	#define CONFIG_ENV_OFFSET                (896 * 1024)
	#define CONFIG_ENV_SECT_SIZE             (64 * 1024)
	#define CONFIG_ENV_SPI_BUS               CONFIG_SF_DEFAULT_BUS
	#define CONFIG_ENV_SPI_CS                CONFIG_SF_DEFAULT_CS
	#define CONFIG_ENV_SPI_MODE              CONFIG_SF_DEFAULT_MODE
	#define CONFIG_ENV_SPI_MAX_HZ            CONFIG_SF_DEFAULT_SPEED

#elif defined(CONFIG_ENV_IS_IN_FLASH)

	#undef CONFIG_ENV_SIZE
	#define CONFIG_ENV_SIZE                  CONFIG_SYS_FLASH_SECT_SIZE
	#define CONFIG_ENV_SECT_SIZE             CONFIG_SYS_FLASH_SECT_SIZE
	#define CONFIG_ENV_OFFSET                (7 * CONFIG_SYS_FLASH_SECT_SIZE)

#elif defined(CONFIG_ENV_IS_IN_NAND)

	#undef CONFIG_ENV_SIZE
	#define CONFIG_ENV_OFFSET                (60 << 20)
	#define CONFIG_ENV_SECT_SIZE             (128 << 10)
	#define CONFIG_ENV_SIZE                  CONFIG_ENV_SECT_SIZE

#elif defined(CONFIG_ENV_IS_IN_SATA)

	#define CONFIG_ENV_OFFSET                (896 * 1024)
	#define CONFIG_SATA_ENV_DEV              0
	#define CONFIG_SYS_DCACHE_OFF /* remove when sata driver support cache */

#endif


/* Falcon Mode */
#define CONFIG_SPL_FS_LOAD_ARGS_NAME	"args"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME	"uImage"
#define CONFIG_SYS_SPL_ARGS_ADDR       0x18000000

/* Falcon Mode - MMC support: args@1MB kernel@2MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR  0x800   /* 1MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS (0x20000 / 512)
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR        0x1000  /* 2MB */


#include "mx6seco_env.h"


#endif    /*  __MX6SECO_COMMON_H  */
