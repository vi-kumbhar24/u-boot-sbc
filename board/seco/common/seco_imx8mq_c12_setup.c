/*
 * 
 *
 * Reading C12 Board type
 *
 * marco.sandrelli@seco.com
 *
 *
 *
 */

#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/imx8mq_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <seco/seco_imx8mq_c12_setup.h>

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE )

/* ____________________________________________________________________________
  |                                                                            |
  |                                  C12 REVISION                              |
  |____________________________________________________________________________|
*/


#define C12_IOMUX_REG(x)        ((x))
#define C12_PADCTRL_REG(x)      ((x))
#define C12_GDIR_REG_IN(x,n)    writel((readl(x + 0x4)) & ~(1<<n), x + 0x4 )
#define C12_PSR_REG(x,n)	(readl(x + 0x8) & (1<<n))

#define shift_0			0
#define shift_1			1
#define shift_2			2
#define shift_3			3

#define GPIO3_PAD_BASE		0x30220000
#define GPIO_CFG_0		16
#define GPIO_CFG_1              18
#define GPIO_CFG_2              17
/* Wilink Strap */
#define GPIO_CFG_3		13

#define GPIO1_PAD_BASE          0x30200000
#define GPIO4_PAD_BASE          0x30230000
#define GPIO_CFG_4              15
#define GPIO_CFG_5              22
#define GPIO_CFG_6              21
#define GPIO_CFG_7              0

/* Quad - Quad Lite Strap */
#define GPIO_CFG_8              5
/* SPI - CAN RTC Strap */
#define GPIO_CFG_9              29

struct sizes {
	u32 s0;
	u32 s1;
};

DECLARE_GLOBAL_DATA_PTR;

iomux_v3_cfg_t const board_conf_pads[] = {

        /* RAM CONFIG */
        IMX8MQ_PAD_NAND_READY_B__GPIO3_IO16 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),   /* cfg_0 */
        IMX8MQ_PAD_NAND_WP_B__GPIO3_IO18 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_1 */
        /* HDMI */
        IMX8MQ_PAD_NAND_WE_B__GPIO3_IO17 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_2 */
	/* Wilink Strap */
	IMX8MQ_PAD_NAND_DATA07__GPIO3_IO13 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_3 */	

};

iomux_v3_cfg_t const hw_rev_pads[] = {

        /* HW REVISION CONFIG */
        IMX8MQ_PAD_GPIO1_IO15__GPIO1_IO15 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),	/* cfg_4 */
        IMX8MQ_PAD_SAI2_RXC__GPIO4_IO22 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),	/* cfg_5 */
        IMX8MQ_PAD_SAI2_RXFS__GPIO4_IO21 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),	/* cfg_6 */

};

iomux_v3_cfg_t const q_ql_pads[] = {

        /* Quad - Quad Lite strap */
        IMX8MQ_PAD_NAND_CLE__GPIO3_IO5 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),       /* cfg_8 */

};

iomux_v3_cfg_t const spi_canrtc_pads[] = {

        /* Quad - CAN RTC strap */
        IMX8MQ_PAD_SAI3_RXC__GPIO4_IO29 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),       /* cfg_9 */

};

iomux_v3_cfg_t const pcie_pads[] = {

        /* Quad - PCIe strap */
        IMX8MQ_PAD_GPIO1_IO00__GPIO1_IO0 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),       /* cfg_7 */

};

int c12_get_board_configuration ( void ) {

        ulong value = 0;

	/* 
         *
         * CFG code is composed in this way:
         * cfg_0 -> first bit   X 
         * cfg_1 -> second bit  X
         * cfg_2 -> third bit   X
         *  
         */
	
	imx_iomux_v3_setup_multiple_pads(board_conf_pads, ARRAY_SIZE(board_conf_pads));

	/* Mux as Input */
	C12_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_0);
	C12_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_1);
	C12_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_2);
	C12_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_3);

	/* Read Conf value */
	value = (C12_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_0) >> GPIO_CFG_0) << shift_0 | 
		(C12_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_1) >> GPIO_CFG_1) << shift_1 | 
		(C12_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_2) >> GPIO_CFG_2) << shift_2 |
		(C12_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_3) >> GPIO_CFG_3) << shift_3;

        return value;

}

int c12_get_hw_revision ( void ) {

        ulong value = 0;

        /* 
         *
         * CFG code is composed in this way:
         * cfg_4 -> first bit   X 
         * cfg_5 -> second bit  X
         * cfg_6 -> third bit   X
         *  
         */

        imx_iomux_v3_setup_multiple_pads(hw_rev_pads, ARRAY_SIZE(hw_rev_pads));

        /* Mux as Input */
        C12_GDIR_REG_IN(GPIO1_PAD_BASE,GPIO_CFG_4);
        C12_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_5);
        C12_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_6);

        /* Read Conf value */
        value = (C12_PSR_REG(GPIO1_PAD_BASE,GPIO_CFG_4) >> GPIO_CFG_4) << shift_0 |
                (C12_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_5) >> GPIO_CFG_5) << shift_1 |
                (C12_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_6) >> GPIO_CFG_6) << shift_2;
	

        return value;

}

int c12_get_q_ql ( void ) {

        ulong value = 0;

        /* 
         *
         * CFG code is composed in this way:
         * cfg_8 -> first bit   X 
         *  
         */

        imx_iomux_v3_setup_multiple_pads(q_ql_pads, ARRAY_SIZE(q_ql_pads));

        /* Mux as Input */
        C12_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_8);

        /* Read Conf value */
        value = (C12_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_8) >> GPIO_CFG_8) << shift_0;
	

        return value;

}

int c12_get_spi_canrtc ( void ) {

        ulong value = 0;

        /* 
         *
         * CFG code is composed in this way:
         * cfg_8 -> first bit   X 
         *  
         */

        imx_iomux_v3_setup_multiple_pads(spi_canrtc_pads, ARRAY_SIZE(spi_canrtc_pads));

        /* Mux as Input */
        C12_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_9);

        /* Read Conf value */
        value = (C12_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_9) >> GPIO_CFG_9) << shift_0;


        return value;

}

int c12_get_pcie ( void ) {

        ulong value = 0;

        /* 
         *
         * CFG code is composed in this way:
         * cfg_7 -> first bit   X 
         *  
         */

	imx_iomux_v3_setup_multiple_pads(pcie_pads, ARRAY_SIZE(pcie_pads));

        /* Mux as Input */
        C12_GDIR_REG_IN(GPIO1_PAD_BASE,GPIO_CFG_7);

        /* Read Conf value */
        value = (C12_PSR_REG(GPIO1_PAD_BASE,GPIO_CFG_7) );
	
        return value;

}

/* Substitute in the fdt kernel file the right dram setup */
void c12_fdt_ram_setup(void *blob, bd_t *bdev) { 

	int offset, ret;
	struct sizes ssize;
	

	printf("Overlay dts /reserved-memory/linux,cma/: size = ");
	offset = fdt_path_offset(blob, "/reserved-memory/linux,cma/");
        if (offset < 0) {
                printf("ERROR: find node /: %s.\n", fdt_strerror(offset));
                return;
        }
        if(C12_IS_1GB) {
		ssize.s0 = cpu_to_fdt32(0x0);
		ssize.s1 = cpu_to_fdt32(0x14000000);
		printf("<0x0 0x14000000>\n");
	}
	if(C12_IS_4GB || C12_IS_2GB) {
                ssize.s0 = cpu_to_fdt32(0x0);
                ssize.s1 = cpu_to_fdt32(0x20000000);
		printf("<0x0 0x20000000>\n");
        }

	ret = fdt_setprop(blob, offset, "size", &ssize, sizeof(ssize));
	if (ret < 0)
		printf("ERROR: could not update revision property %s.\n",
                        fdt_strerror(ret));

	return;
}

void c12_fdt_vpu_setup(void *blob, bd_t *bdev) {


	int offset, ret;
	char status[10];	

	printf("Overlay dts: cpu is Quadlite, disabling vpu support\n");
	offset = fdt_path_offset(blob, "/vpu@38300000/");
	if (offset < 0) {
                printf("ERROR: find node /: %s.\n", fdt_strerror(offset));
                return;
        }

	sprintf(status, "%s","disabled" );
	ret = fdt_setprop(blob, offset, "status", status, sizeof(status));
        if (ret < 0)
                printf("ERROR: could not update revision property %s.\n",
                        fdt_strerror(ret));

	return;
}

void c12_fdt_pcie_setup(void *blob, bd_t *bdev) {


        int offset, ret;
	char status[10];

        printf("Overlay dts: disabling pcie\n");
        offset = fdt_path_offset(blob, "/hsio/pcie@33800000");
        if (offset < 0) {
                printf("ERROR: find node /hsio/pcie@33800000\n");
                return;
        }

	sprintf(status, "%s","disabled" );
        ret = fdt_setprop(blob, offset, "status", status, sizeof(status));
        if (ret < 0)
                printf("ERROR: could not update revision property %s.\n",
                        fdt_strerror(ret));
	
	offset = fdt_path_offset(blob, "/hsio/pcie@33c00000");
        if (offset < 0) {
                printf("ERROR: find node /hsio/pcie@33c00000\n");
                return;
        }

	sprintf(status, "%s","disabled" );
        ret = fdt_setprop(blob, offset, "status", status, sizeof(status));
        if (ret < 0)
                printf("ERROR: could not update revision property %s.\n",
                        fdt_strerror(ret));	

        return;
}
