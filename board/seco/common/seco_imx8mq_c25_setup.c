/*
 * 
 *
 * Reading C25 Board type
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
#include <seco/seco_imx8mq_c25_setup.h>

#define PULLUP_PAD_CTRL  (PAD_CTL_PUE)

/* ____________________________________________________________________________
  |                                                                            |
  |                                  C25 REVISION                              |
  |____________________________________________________________________________|
*/


#define C25_IOMUX_REG(x)        ((x))
#define C25_PADCTRL_REG(x)      ((x))
#define C25_GDIR_REG_IN(x,n)    writel((readl(x + 0x4)) & ~(1<<n), x + 0x4 )
#define C25_PSR_REG(x,n)         (readl(x + 0x8) & (1<<n))

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
#define GPIO_CFG_0		16
#define GPIO_CFG_1              18
#define GPIO_CFG_2              17
#define GPIO_CFG_3		15
#define GPIO_CFG_4		14
#define GPIO_CFG_5               4
#define GPIO_CFG_6               3
#define GPIO_CFG_7		 2
#define GPIO_CFG_8              10
#define GPIO_CFG_9              11
#define GPIO_CFG_10             12


struct sizes {
	u32 s0;
	u32 s1;
};

DECLARE_GLOBAL_DATA_PTR;

iomux_v3_cfg_t const board_conf_pads[] = {

        /* RAM CONFIG */
        IMX8MQ_PAD_NAND_READY_B__GPIO3_IO16 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),   /* cfg_0 */
        IMX8MQ_PAD_NAND_WP_B__GPIO3_IO18 | MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_1 */
        /* Undefined */
        IMX8MQ_PAD_NAND_WE_B__GPIO3_IO17	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_2 */
	IMX8MQ_PAD_NAND_RE_B__GPIO3_IO15	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_3 */	
	IMX8MQ_PAD_NAND_DQS__GPIO3_IO14		| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_4 */
	IMX8MQ_PAD_NAND_CE3_B__GPIO3_IO4	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_5 */
	IMX8MQ_PAD_NAND_CE2_B__GPIO3_IO3  	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_6 */
	IMX8MQ_PAD_NAND_CE1_B__GPIO3_IO2	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_7 */
	IMX8MQ_PAD_NAND_DATA04__GPIO3_IO10	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_8 */
	IMX8MQ_PAD_NAND_DATA05__GPIO3_IO11	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_9 */
	IMX8MQ_PAD_NAND_DATA06__GPIO3_IO12	| MUX_PAD_CTRL(PULLUP_PAD_CTRL),      /* cfg_10 */

};

int c25_get_board_configuration ( void ) {

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
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_0);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_1);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_2);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_3);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_4);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_5);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_6);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_7);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_8);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_9);
	C25_GDIR_REG_IN(GPIO3_PAD_BASE,GPIO_CFG_10);

	/* Read Conf value */
	value = (C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_0) >> GPIO_CFG_0) << shift_0 | 
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_1) >> GPIO_CFG_1) << shift_1 | 
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_2) >> GPIO_CFG_2) << shift_2 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_3) >> GPIO_CFG_3) << shift_3 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_4) >> GPIO_CFG_4) << shift_4 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_5) >> GPIO_CFG_5) << shift_5 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_6) >> GPIO_CFG_6) << shift_6 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_7) >> GPIO_CFG_7) << shift_7 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_8) >> GPIO_CFG_8) << shift_8 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_9) >> GPIO_CFG_9) << shift_9 |
		(C25_PSR_REG(GPIO3_PAD_BASE,GPIO_CFG_10) >> GPIO_CFG_10) << shift_10;

        return value;

}

/* Substitute in the fdt kernel file the right dram setup */
void c25_fdt_ram_setup(void *blob, bd_t *bdev) { 

	int offset, ret;
	struct sizes ssize;
	

	printf("Overlay dts /reserved-memory/linux,cma/: size = ");
	offset = fdt_path_offset(blob, "/reserved-memory/linux,cma/");
        if (offset < 0) {
                printf("ERROR: find node /: %s.\n", fdt_strerror(offset));
                return;
        }
        if(C25_IS_1GB) {
		ssize.s0 = cpu_to_fdt32(0x0);
		ssize.s1 = cpu_to_fdt32(0x14000000);
		printf("<0x0 0x14000000>\n");
	}
        if(C25_IS_2GB) {
		ssize.s0 = cpu_to_fdt32(0x0);
                ssize.s1 = cpu_to_fdt32(0x20000000);
		printf("<0x0 0x20000000>\n");
	}
        if(C25_IS_4GB) {
		ssize.s0 = cpu_to_fdt32(0x0);
                ssize.s1 = cpu_to_fdt32(0x3c000000);
		printf("<0x0 0x3c000000>\n");
	}

        ret = fdt_setprop(blob, offset, "size", &ssize, sizeof(ssize));
        if (ret < 0)
                printf("ERROR: could not update revision property %s.\n",
                        fdt_strerror(ret));

	return;
}

void c25_fdt_vpu_setup(void *blob, bd_t *bdev) {


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
