#
# Copyright 2017 Freescale Semiconductor
#
# SPDX-License-Identifier:      GPL-2.0+
#

obj-y += seco_imx8mq_c25.o
obj-$(CONFIG_CMD_SECO_CONFIG_IMX8) += env_conf.o

ifdef CONFIG_SPL_BUILD
obj-y += spl.o
obj-y += ddr/helper.o
ifdef CONFIG_TARGET_SECO_IMX8MQ_C25
obj-y += ddr/ddr4/ddr_init.o ddr/ddr4/ddrphy_train.o
endif
endif
