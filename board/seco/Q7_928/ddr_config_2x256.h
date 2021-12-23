#ifndef _SECO_Q7_928_DDR_CONFIG_H_
#define _SECO_Q7_928_DDR_CONFIG_H_

#include "../common/ddr_conf.h"


#if defined( CONFIG_SECOMX6_512MB_2x256 )

static int mx6solo_2x256_dcd_table[] = {
	/* Write Leveling */
	0x021b080c, 0x001F001F,  // MX6_MMDC_P0_MPWLDECTRL0
	0x021b0810, 0x001F001F,  // MX6_MMDC_P0_MPWLDECTRL1
	/* DQS gating, read delay, write delay calibration values */
	0x021b083c, 0x417E017F,  // MX6_MMDC_P0_MPDGCTRL0
	0x021b0840, 0x01780201,  // MX6_MMDC_P0_MPDGCTRL1
	/* Read calibration */
	0x021b0848, 0x474B4949,  // MX6_MMDC_P0_MPRDDLCTL
	/* write calibrttion */
	0x021b0850, 0x403E3E35,  // MX6_MMDC_P0_MPWRDLCTL
	/* Complete calibration by forced measurement */
	0x021b08b8, 0x00000800,  // MX6_MMDC_P0_MPMUR0
	/* ========== MMDC init ========== */
	/* in DDR3, 64-bit mode, only MMDC0 is init */
	0x021b0004, 0x0002002D,  // MX6_MMDC_P0_MDPDC
	0x021b0008, 0x00333040,  // MX6_MMDC_P0_MDOTC
	0x021b000c, 0x3F4353F3,  // MX6_MMDC_P0_MDCFG0
	0x021b0010, 0xB66E8B63,  // MX6_MMDC_P0_MDCFG1
	0x021b0014, 0x01FF00DB,  // MX6_MMDC_P0_MDCFG2
	0x021b0018, 0x00001740,  // MX6_MMDC_P0_MDMISC
	0x021b001c, 0x00008000,  // MX6_MMDC_P0_MDSCR
	0x021b002c, 0x000026D2,  // MX6_MMDC_P0_MDRWD
	0x021b0030, 0x00431023,  // MX6_MMDC_P0_MDOR
	/* CS0_END = 1280MB (1024 + 256)  in step da 256Mb -> [(768*8/256) - 1] */
	0x021b0040, 0x00000017,  // MX6_MMDC_P0_MDASP
	/* SDE_1=0; ROW=3; BL=1; DSIZ=1 -> 32 bit */
	0x021b0000, 0x83190000,  // MX6_MMDC_P0_MDCTL
	/* Initialize DDR3 on CS_0 */
	0x021b001c, 0x02008032,  // MX6_MMDC_P0_MDSCR
	0x021b001c, 0x00008033,  // MX6_MMDC_P0_MDSCR
	0x021b001c, 0x00048031,  // MX6_MMDC_P0_MDSCR
	/* write 0x0940 to MR0 bank_0 (Burst Type=1 (Interlived)) */
	0x021b001c, 0x05208030,  // MX6_MMDC_P0_MDSCR
	/* ZQ - Calibration */
	0x021b0800, 0xa1390003,  // MX6_MMDC_P0_MPZQHWCTRL
	0x021b001c, 0x04008040,  // MX6_MMDC_P0_MDSCR
	0x021b0020, 0x00007800,  // MX6_MMDC_P0_MDREF
	0x021b0818, 0x00022227,  // MX6_MMDC_P0_MPODTCTRL
	0x021b0004, 0x0002556D,  // MX6_MMDC_P0_MDPDC
	0x021b0404, 0x00011006,  // MX6_MMDC_P0_MAPSR
	0x021b001c, 0x00000000,  // MX6_MMDC_P0_MDSCR
};

static int mx6dl_2x256_dcd_table[] = {
	/* Write Leveling */
	0x021b080c, 0x001F001F,  // MX6_MMDC_P0_MPWLDECTRL0
	0x021b0810, 0x001F001F,  // MX6_MMDC_P0_MPWLDECTRL1
	/* DQS gating, read delay, write delay calibration values */
	0x021b083c, 0x417E017F,  // MX6_MMDC_P0_MPDGCTRL0
	0x021b0840, 0x01780201,  // MX6_MMDC_P0_MPDGCTRL1
	/* Read calibration */
	0x021b0848, 0x474B4949,  // MX6_MMDC_P0_MPRDDLCTL
	/* write calibrttion */
	0x021b0850, 0x403E3E35,  // MX6_MMDC_P0_MPWRDLCTL
	/* Complete calibration by forced measurement */
	0x021b08b8, 0x00000800,  // MX6_MMDC_P0_MPMUR0
	/* ========== MMDC init ========== */
	/* in DDR3, 64-bit mode, only MMDC0 is init */
	0x021b0004, 0x0002002D,  // MX6_MMDC_P0_MDPDC
	0x021b0008, 0x00333040,  // MX6_MMDC_P0_MDOTC
	0x021b000c, 0x3F4353F3,  // MX6_MMDC_P0_MDCFG0
	0x021b0010, 0xB66E8B63,  // MX6_MMDC_P0_MDCFG1
	0x021b0014, 0x01FF00DB,  // MX6_MMDC_P0_MDCFG2
	0x021b0018, 0x00001740,  // MX6_MMDC_P0_MDMISC
	0x021b001c, 0x00008000,  // MX6_MMDC_P0_MDSCR
	0x021b002c, 0x000026D2,  // MX6_MMDC_P0_MDRWD
	0x021b0030, 0x00431023,  // MX6_MMDC_P0_MDOR
	/* CS0_END = 1280MB (1024 + 256)  in step da 256Mb -> [(768*8/256) - 1] */
	0x021b0040, 0x00000017,  // MX6_MMDC_P0_MDASP
	/* SDE_1=0; ROW=3; BL=1; DSIZ=1 -> 32 bit */
	0x021b0000, 0x83190000,  // MX6_MMDC_P0_MDCTL
	/* Initialize DDR3 on CS_0 */
	0x021b001c, 0x02008032,  // MX6_MMDC_P0_MDSCR
	0x021b001c, 0x00008033,  // MX6_MMDC_P0_MDSCR
	0x021b001c, 0x00048031,  // MX6_MMDC_P0_MDSCR
	/* write 0x0940 to MR0 bank_0 (Burst Type=1 (Interlived)) */
	0x021b001c, 0x05208030,  // MX6_MMDC_P0_MDSCR
	/* ZQ - Calibration */
	0x021b0800, 0xa1390003,  // MX6_MMDC_P0_MPZQHWCTRL
	0x021b001c, 0x04008040,  // MX6_MMDC_P0_MDSCR
	0x021b0020, 0x00007800,  // MX6_MMDC_P0_MDREF
	0x021b0818, 0x00022227,  // MX6_MMDC_P0_MPODTCTRL
	0x021b0004, 0x0002556D,  // MX6_MMDC_P0_MDPDC
	0x021b0404, 0x00011006,  // MX6_MMDC_P0_MAPSR
	0x021b001c, 0x00000000,  // MX6_MMDC_P0_MDSCR
};

#endif    /*  CONFIG_SECOMX6_512MB_2x256  */

#endif    /*  _SECO_Q7_928_DDR_CONFIG_H_  */
