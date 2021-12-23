
#ifndef __SECO_WDT_H
#define __SECO_WDT_H

#include <common.h>
#include <asm/io.h>
#include <watchdog.h>
#include <asm/arch/imx-regs.h>


#ifdef CONFIG_APX_WATCHDOG

#define WDT_IOMUX_REG(x)        ((x))
#define WDT_PADCTRL_REG(x)      ((x))
#define WDT_GDIR_REG(x)         ((x) + 0x4) 
#define WDT_DR_REG(x)           ((x))


 
#define WDT_PAD_MUX_GPIO(x)     writel(readl(WDT_IOMUX_REG((x))) | 0x5, WDT_IOMUX_REG((x)))

#define WDT_PAD_CTRL_GPIO(x)    writel(0x06028, WDT_PADCTRL_REG((x)))

#define WDT_DIR_OUT_GPIO(x,n)   writel(readl(WDT_GDIR_REG((x))) | ( 1 << (n)), WDT_GDIR_REG((x)))

#define WDT_SET_H_GPIO(x,n)     writel(readl(WDT_DR_REG((x))) | (1 << (n)) , WDT_DR_REG((x)))

#define WDT_SET_L_GPIO(x,n)     writel(readl(WDT_DR_REG((x))) & ~(1 << (n)) , WDT_DR_REG((x)))



#endif /* __SECO_WDT_H */
#endif

