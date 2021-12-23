/*
 * 
 *
 * Reading C20 Board type
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
#include <seco/seco_imx8mq_c20_setup.h>

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE)

/* ____________________________________________________________________________
  |                                                                            |
  |                                  C20 REVISION                              |
  |____________________________________________________________________________|
*/


#define C20_IOMUX_REG(x)        ((x))
#define C20_PADCTRL_REG(x)      ((x))
#define C20_GDIR_REG_IN(x,n)    writel((readl(x + 0x4)) & ~(1<<n), x + 0x4 )
#define C20_PSR_REG(x,n)         (readl(x + 0x8) & (1<<n))

#define shift_0			0
#define shift_1			1
#define shift_2			2
#define shift_3			3
#define shift_4			4
#define shift_5			5
#define shift_6			6
#define shift_7			7
#define shift_8			8
#define shift_9			9
#define shift_10		10

#define GPIO3_PAD_BASE		0x30220000
#define GPIO4_PAD_BASE          0x30230000
#define GPIO_CFG_0		2
#define GPIO_CFG_1              3
#define GPIO_CFG_3              7


struct sizes {
	u32 s0;
	u32 s1;
};

DECLARE_GLOBAL_DATA_PTR;

iomux_v3_cfg_t const board_conf_pads[] = {

        /* RAM CONFIG */
	IMX8MQ_PAD_SAI1_RXD0__GPIO4_IO2 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),   /* cfg_0 */
	IMX8MQ_PAD_SAI1_RXD1__GPIO4_IO3 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_1 */

};

iomux_v3_cfg_t const board_rev_pads[] = {

        /* REV CONFIG */
        IMX8MQ_PAD_SAI1_RXD5__GPIO4_IO7 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_3 */

};

int c20_get_board_configuration ( void ) {

        ulong value = 0;

	/* 
         *
         * CFG code is composed in this way:
         * cfg_0 -> first bit   X 
         * cfg_1 -> second bit  X
         *  
         */
	
	imx_iomux_v3_setup_multiple_pads(board_conf_pads, ARRAY_SIZE(board_conf_pads));

	/* Mux as Input */
	C20_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_0);
	C20_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_1);

	/* Read Conf value */
	value = (C20_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_0) >> GPIO_CFG_0) << shift_0 | 
		(C20_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_1) >> GPIO_CFG_1) << shift_1; 

        return value;

}

int c20_get_board_rev ( void ) {

        ulong value = 0;

	/* 
         *
         * CFG code is composed in this way:
         * cfg_3 -> first bit   X 
         *  
         */
	
	imx_iomux_v3_setup_multiple_pads(board_rev_pads, ARRAY_SIZE(board_rev_pads));

	/* Mux as Input */
	C20_GDIR_REG_IN(GPIO4_PAD_BASE,GPIO_CFG_3);

	/* Read Conf value */
	value = (C20_PSR_REG(GPIO4_PAD_BASE,GPIO_CFG_3) >> GPIO_CFG_3); 

        return value;

}

/* Substitute in the fdt kernel file the right dram setup */
void c20_fdt_ram_setup(void *blob, bd_t *bdev) { 

	int offset, ret;
	struct sizes ssize;
	

	printf("Overlay dts /reserved-memory/linux,cma/: size = ");
	offset = fdt_path_offset(blob, "/reserved-memory/linux,cma/");
        if (offset < 0) {
                printf("ERROR: find node /: %s.\n", fdt_strerror(offset));
                return;
        }
        if(C20_IS_1GB) {
		ssize.s0 = cpu_to_fdt32(0x0);
		ssize.s1 = cpu_to_fdt32(0x14000000);
		printf("<0x0 0x14000000>\n");
	}
        if(C20_IS_2GB) {
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

void c20_fdt_vpu_setup(void *blob, bd_t *bdev) {


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

void c20_fdt_rev_setup(void *blob, bd_t *bdev) {

	int offset, ret;
        char status[1];

	if(C20_IS_REVB)
		printf("Overlay dts: /regulators/reg_clklvds_enable/ - enable-active-low\n");
	else
		printf("Overlay dts: /regulators/reg_clklvds_enable/ - enable-active-high\n");

	offset = fdt_path_offset(blob, "/regulators/reg_clklvds_enable/");
        if (offset < 0) {
                printf("ERROR: find node /: %s.\n", fdt_strerror(offset));
                return;
        }

	sprintf(status, "%s","" );
	if(C20_IS_REVB)
		ret = fdt_setprop(blob, offset, "enable-active-low", status, sizeof(status));
	else
		ret = fdt_setprop(blob, offset, "enable-active-high", status, sizeof(status));
        if (ret < 0)
                printf("ERROR: could not update revision property %s.\n",
                        fdt_strerror(ret));

        return;

}
