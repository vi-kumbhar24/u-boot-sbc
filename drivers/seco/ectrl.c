#include <common.h>
#include <errno.h>
#include <dm.h>
#include <fdtdec.h>
#include <i2c.h>
#include <malloc.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <dt-bindings/seco/ectrl.h>
#include <dt-bindings/seco/dm_ectrl.h>



static inline int ectrl_read_data( struct udevice *dev, u8 addr, u16 *data ) {
    int ret;
    u8 val[2];
    
    ret = dm_i2c_read( dev, addr, &val[0], 2 );
    if ( ret ) {
		dev_err( dev, "%s error\n", __func__ );
		return ret;
	}

	*data = val[0] | ( (u16)val[1] << 8 );

	return 0;
}


static inline int ectrl_write( struct udevice *dev, u8 addr, u16 data ) {
	int ret;
    
    ret = dm_i2c_write( dev, addr, (uint8_t *)&data, 2 );
    if ( ret ) {
		dev_err( dev, "%s error\n", __func__ );
		return ret;
	}
	return 0;
}


static int get_board_id( struct udevice *dev, u8 *id, char *major_v, u8 *minor_v ) {
     int ret;
    u16 data;

    ret = ectrl_read_data( dev, FW_ID_REV_REG, &data );
    if (ret) {
		dev_err(dev, "%s error\n", __func__);
		return ret;
	}

    *id = ( data & 0xFF00 ) >> 8;
    *major_v = ( ( data & 0xF0 ) >> 4 ) + 'a';
    *minor_v = ( data & 0xF );

    return 0;
}


static const struct dm_ectrl_ops ectrl_ops = {
    .get_board_id = get_board_id,
};



static int ectrl_bind( struct udevice *dev ) {
    return 0;
}


static int ectrl_probe( struct udevice *dev ) {
    return 0;
}


static const struct udevice_id ectrl_ids[] = {
	{ .compatible = "seco,ectrl", .data = 0, },
	{ }
};

U_BOOT_DRIVER( ectrl ) = {
	.name		              = "ectrl",
	.id		                  = UCLASS_I2C_GENERIC,
	.ops		              = &ectrl_ops,
	.probe		              = ectrl_probe,
    .bind = ectrl_bind,
	//.platdata_auto_alloc_size = sizeof(struct pca953x_info),
	.of_match   	          = ectrl_ids,
};
