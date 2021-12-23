#include <common.h>

#include <command.h>
#include <sysdata.h>
#include <fdtdec.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <memalign.h>
#include <part.h>
#include <search.h>
#include <errno.h>

#include <mmc.h>

#define CONFIG_SYSDATA_MMC_DEV    1
#define CONFIG_SYSDATA_OFFSET     (890 * 1024)




static unsigned char env_mmc_orig_hwpart;

static int mmc_set_sysdata_part( struct mmc *mmc ) {
	uint part = mmc_get_env_part( mmc );
	int dev = mmc_get_env_dev( );
	int ret = 0;

	env_mmc_orig_hwpart = mmc_get_blk_desc( mmc )->hwpart;
	ret = blk_select_hwpart_devnum( IF_TYPE_MMC, dev, part );

	return ret;
}


static void mmc_clean_setting( struct mmc *mmc ) {
#ifdef CONFIG_SYS_MMC_ENV_PART
	int dev = mmc_get_env_dev( );

	blk_select_hwpart_devnum( IF_TYPE_MMC, dev, env_mmc_orig_hwpart );
#endif
}


__weak int mmc_get_sysdata_addr( struct mmc *mmc, int copy, u32 *sysdata_addr ) {
	s64 offset = CONFIG_SYSDATA_OFFSET;// mmc_offset(copy);

	if ( offset < 0 )
		offset += mmc->capacity;

	*sysdata_addr = offset;

	return 0;
}


static inline ulong write_sysdata( struct mmc *mmc, unsigned long size,
			    unsigned long offset, const void *buffer )
{
	ulong           blk_start, blk_cnt, n;
	struct blk_desc *desc = mmc_get_blk_desc( mmc );

	blk_start	= ALIGN( offset, mmc->write_bl_len ) / mmc->write_bl_len;
	blk_cnt		= ALIGN( size, mmc->write_bl_len ) / mmc->write_bl_len;

	n = blk_dwrite( desc, blk_start, blk_cnt, (u_char *)buffer );

	return ( n == blk_cnt ) ? 0 : -1;
}


static inline ulong read_sysdata( struct mmc *mmc, unsigned long size,
			   unsigned long offset, void *buffer )
{
	ulong           blk_start, blk_cnt, n;
	struct blk_desc *desc = mmc_get_blk_desc(mmc);

	blk_start	= ALIGN( offset, mmc->read_bl_len ) / mmc->read_bl_len;
	blk_cnt		= ALIGN( size, mmc->read_bl_len ) / mmc->read_bl_len;

	n = blk_dread( desc, blk_start, blk_cnt, (u_char *)buffer );

	return (n == blk_cnt) ? 0 : -1;
}


static int init_device( struct mmc **mmc ) {
    int dev = mmc_get_env_dev( );

    *mmc = find_mmc_device( dev );

     if ( !(*mmc) )
        return -ENODEV;

#if CONFIG_IS_ENABLED(BLK)
	struct udevice *udev;

	if ( blk_get_from_parent( (*mmc)->dev, &udev ) )
	 	return -EINVAL;
#else
	if ( mmc_init( *mmc ) )
		return -ENOMEM;
#endif

	mmc_set_sysdata_part( *mmc );
    return 0;
}


static int sysdata_mmc_load( sysdata_t *rd_data ) {
	int        dev = mmc_get_env_dev( );
    struct mmc *mmc = find_mmc_device(dev);
    int        ret = 0;   
    u32        offset = 0;

    ret = init_device( &mmc );
    if ( ret ) {
        // error
        return ret;
    }

	ALLOC_CACHE_ALIGN_BUFFER( u_char, buf, 
			mmc->write_bl_len > sizeof(sysdata_t) ? mmc->write_bl_len : sizeof(sysdata_t) );

	if ( mmc_get_sysdata_addr( mmc, 0, &offset ) ) {
        // error
        return -EINVAL;
    }

	if ( read_sysdata( mmc, SYSTEMDATA_SIZE, offset, (u_char *)buf ) ) {
        // error
        return -ENOMEM;
    }

	mmc_clean_setting( mmc );

	memcpy( rd_data, buf, sizeof(sysdata_t) );

    return ret;
}


static int sysdata_mmc_save( sysdata_t *new_data ) {
	int        dev = mmc_get_env_dev( );
    struct mmc *mmc = find_mmc_device(dev);
    int        ret = 0;   
    u32        offset = 0;

	

    ret = init_device( &mmc );
    if ( ret ) {
        // error
        return ret;
    }

	ALLOC_CACHE_ALIGN_BUFFER( u_char, buf, 
			mmc->write_bl_len > sizeof(sysdata_t) ? mmc->write_bl_len : sizeof(sysdata_t) );

    if ( mmc_get_sysdata_addr( mmc, 0, &offset ) ) {
        // error
        return -EINVAL;
    }

	/* the write block size may be bigger then the data size we are writing, so
       a read of the whole block is need.
	 */
	if ( read_sysdata( mmc, SYSTEMDATA_SIZE, offset, (u_char *)buf ) ) {
        // error
        return -ENOMEM;
    }

	memcpy( buf, new_data, sizeof(sysdata_t) );

    if ( write_sysdata( mmc, SYSTEMDATA_SIZE, offset, (u_char *)buf ) ) {
        // error
        return -ENOMEM;
    }

    mmc_clean_setting( mmc );
    return ret;
}


static const char *get_device_name( void ) {
	int dev = mmc_get_env_dev( );
	char *name;

	name = malloc( sizeof(char) * SYSDATA_DEV_NAME_LEN );
	if ( !name )
		return NULL;

	sprintf( name, "MMC%d", dev );
	return (const char *)name;
}


U_BOOT_SYSDATA_LOCATION(mmc) = {
	//.location	= 0,
	.name            = "MMC",
	.load		     = sysdata_mmc_load,
	.save		     = sysdata_mmc_save,
	.get_device_name = get_device_name,
};