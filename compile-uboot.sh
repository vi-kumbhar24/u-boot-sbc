#!/bin/bash


export ARCH=arm64
source /opt/fsl-imx-xwayland/4.19-warrior/environment-setup-aarch64-poky-linux
unset LD_LIBRARY_PATH
#make imx8qxp_mek_defconfig  seco-imx8qm-c26
#make seco_imx8qm_c26_defconfig
make seco_imx8qxp_c57_defconfig
#make seco_imx8qm_c26_defconfig

rm -v ./u-boot.bin

#make clean
make -j20

rm -v ../imx-mkimage/iMX8QX/u-boot.bin
rm -v ../imx-mkimage/iMX8QX/flash.bin
rm -v ../flash-bin/flash.bin

cp -v u-boot.bin ../imx-mkimage/iMX8QX/
cd ../imx-mkimage/

make SOC=iMX8QX flash
cp -v iMX8QX/flash.bin ../flash-bin
cp -v iMX8QX/flash.bin /tftp_folder/
cd ../flash-bin
sudo uuu script.lst