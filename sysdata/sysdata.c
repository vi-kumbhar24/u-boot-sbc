
#ifndef USE_HOSTCC

#include <memalign.h>
#include <environment.h> 

#else

#include <compiler.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/fs.h>
#include <linux/stringify.h>
#include <linux/stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <u-boot/crc.h>

#endif

#include <malloc.h>
#include <errno.h>
#include <sysdata.h>



#ifndef USE_HOSTCC

/** 
 * _sysdata_driver_find( ) - search device to use
 * 
 * This gets the most suitable device to use
 * 
 * @return data structure of the device to use
*/
struct sysdata_driver *_sysdata_driver_find( void ) {
	struct sysdata_driver *drv;
	const int             n_ents = ll_entry_count( struct sysdata_driver, sysdata_driver );
	struct sysdata_driver *entry;

	drv = ll_entry_start( struct sysdata_driver, sysdata_driver );
	for ( entry = drv ; entry != drv + n_ents ; entry++ ) {
		return entry;
	}

	/* Not found */
	return NULL;
}


/** 
 * _sysdata_wr( ) - simple write operation 
 * 
 * This uses the device selected at compilation time
 * 
 * @table: main data structure to save
 * @return 0 if ok, negative value if operation fails
*/
int _sysdata_wr( sysdata_t *table ) {
	struct sysdata_driver *drv = _sysdata_driver_find( );

	if ( drv == NULL )
		return -ENODEV;

	if ( drv->save )
		return drv->save( table );

	return -ENOSYS;
}


/** 
 * _sysdata_rd( ) - simple read operation 
 * 
 * This uses the device selected at compilation time
 * 
 * @table: main data structure in which to load stored data
 * @return 0 if ok, negative value if operation fails
*/
int _sysdata_rd( sysdata_t *table ) {
	struct sysdata_driver *drv = _sysdata_driver_find( );

	if ( drv == NULL )
		return -ENODEV;

	if ( drv->load )
		return drv->load( table );

	return -ENOSYS;
}


/** 
 * get_sysdata_device_name( ) - name of device in using

 * This gets the name of the device the device have been found
 * 
 * @return the string with the device name
*/
const char *get_sysdata_device_name( ) {
	struct sysdata_driver *drv = _sysdata_driver_find( );

	if ( drv == NULL )
		return NULL;

	return  drv->get_device_name( );
}
#endif    /*  USE_HOSTCC  */


/** 
 * _get_data_crc32( ) - checksum evaluation
 * 
 * This gets the CRC value of the sensitive data
 * 
 * @table: sensitive data of the sysdata structure 
 * @return crc32 value
*/
inline int _get_data_crc32( sysdata_data_t *data ) {
	return crc32( 0, (u_char *)data, SYSTEMDATA_CONTENT_SIZE );
} 


/** 
 * _sysdata_check_with_read( ) - stored data integration check
 * 
 * This reads data from the device and check that the
 * CRC of the sensitive data match with the stored CRC
 * 
 * @return 0 for data integration, -1 for corrupted data
*/
int _sysdata_check_with_read( void ) {
	int       ret;
	int       crc;
	
#ifndef USE_HOSTCC
	ALLOC_CACHE_ALIGN_BUFFER( sysdata_t, tester, 1 );
#else
	sysdata_t *tester;
	tester = malloc( sizeof( sysdata_t ) );
#endif

	if ( tester == NULL )
		return -ENOMEM;

	ret = _sysdata_rd( tester );
	if ( ret ) {
		return ret;
	}

	crc = _get_data_crc32( &tester->data );

	if ( crc == tester->header.crc)
		return 0;
	else
		return -1;
}


/** 
 * _sysdata_read( ) read operation with data integration check
 * 
 * This uses the device selected at compilation time
 * and provides redundance data management
 * 
 * @table: main data structure in which to load stored data
 * @return 0 if ok, negative value if operation fails
*/
int _sysdata_read( sysdata_t *table ) { 
	int ret;
	int crc;

	if ( table == NULL )
		return -ENOMEM;

	ret = _sysdata_rd( table );
	if ( ret ) {
		return ret;
	}

	crc = _get_data_crc32( &table->data );

	if ( crc == table->header.crc)
		return 0;
	else
		return -1;
}


/** 
 * _sysdata_write( ) write operation with data integration check
 * 
 * This uses the device selected at compilation time
 * and provides redundance data management. 
 * the CRC is evaluated before the writing operation
 * A read follows the write operation in order to provide data integration.
 * 
 * @table: main data structure to store
 * @return 0 if ok, negative value if operation fails
*/
int _sysdata_write( sysdata_t *sysdata ) { 
	int ret;

	if ( sysdata == NULL )
		return -ENOMEM;

	sysdata->header.crc = _get_data_crc32( &sysdata->data );

	ret = _sysdata_wr( sysdata );
	if ( ret ) {
		return ret;
	}

	return _sysdata_check_with_read( );
}


/** 
 * _sysdata_header_init( ) - sysdata header initialization

 * This initializes the metadata of the sysdata structure
 * (the CRC field is invalid and not evaluated)
 * 
 * @return 0 if the pointer is not NULL, negative value else
*/
int _sysdata_header_init( sysdata_header_t *header ) {
	if ( header == NULL )
		return -EINVAL;

	header->version_major = VERSION_MAJOR;
	header->version_minor = VERSION_MINOR;
	header->crc           = 0; // not valid crc

	return 0;
}


/** 
 * _sysdata_switching_table_init( ) - swiching data initialization

 * This initializes the data relative to the switching partition structure
 * 
 * @return 0 if the pointer is not NULL, negative value else
*/
int _sysdata_switching_table_init( switching_table_t *table ) {
	int i = 0;
    if ( table == NULL ) {
        return -EINVAL;
	}

	for ( i = 0 ; i < SYSDATA_MAX_PART_GROUP ; i++ ) {
    	table->boot_partition_counter[i] = 0;
	}
	
    table->current_boot_partition = 1;
	table->previous_boot_partition = 1;	
    table->boot_fail_counter = 0;

	return 0;
}


/** 
 * _sysdata_data_init( ) - all data initialization

 * This initializes all sensitive data of the sysdata structure
 * (calls sequentially all sub data initialization functions )
 * 
 * @return 0 if the pointer is not NULL, negative value else
*/
int _sysdata_data_init( sysdata_data_t *data ) {
	int ret = 0;

	if ( data == NULL )
		return -EINVAL;

	ret = _sysdata_switching_table_init( &data->switch_table );
	if ( ret ) return ret;

	return 0;
}


/** 
 * _sysdata_table_init( ) - sysdata structure initialization (global function)

 * This initializes the whole sysdata structure (both meta-data and sensitive data)
 * (calls sequentially all sub data initialization functions )
 * 
 * @return 0 if the pointer is not NULL, negative value else
*/
int _sysdata_table_init( sysdata_t *table ) {
	int ret = 0;

	if ( table == NULL )
		return -EINVAL;

	ret = _sysdata_header_init( &table->header );
	if ( ret ) return ret;

	ret = _sysdata_data_init( &table->data );
	if ( ret ) return ret;

	return 0;
}


/** 
 * sysdata_table_init( ) - sysdata structure initialization (global function)

 * This initializes the whole sysdata structure (both meta-data and sensitive data)
 * (calls sequentially all sub data initialization functions )
 * 
 * @return 0 if the pointer is not NULL, negative value else
*/
int sysdata_init ( ) {
	int       ret;
	
#ifndef USE_HOSTCC
	ALLOC_CACHE_ALIGN_BUFFER( sysdata_t, sysdata, 1 );
#else
	sysdata_t *sysdata;
	sysdata = malloc( sizeof( sysdata_t ) );
#endif
	
	if ( sysdata == NULL )
		return -ENOMEM;

	ret = _sysdata_table_init( sysdata );
	if ( ret ) {
		return ret;
	}

	return _sysdata_write( sysdata );
}


/** 
 * sysdata_integration_check( ) - check integration of whole sysdata table
 *
 * This check the state of system data. If needed, this initializes the table.
 * 
 * @return 0 if the data are ok, 1 if was needed to initialize the data
 * (may be is the first boot), -1 if the initialization procedure returns an error
*/
int sysdata_integration_check( void ) {
	int ret;

	ret = _sysdata_check_with_read( );
	if ( ret ) {
		// data not valid.. initialize it
		ret = sysdata_init( );
		if ( ret ){
			// impossible to initialize it
			return -1;
		} else {
			return 1;
		}
	}

	return 0;
}


/** 
 * get_sysdata_version( ) - version of the sysdata structure
 *
 * This gets string including the version of the sysdata structure
 * in the form of <major version>.<minor version>
 * 
 * @return the string with sysdata version
*/
const char *get_sysdata_version( void ) {
	sysdata_t *table;
	int       ret;
	char      *version;

	version = malloc( sizeof( char ) * SYSDATA_VERSION_LEN );
	if ( ! version )
		return NULL;

	table = malloc( sizeof( sysdata_t ) );
	if ( !table ) {
		free( version );
		return NULL;
	}
	 
	ret = _sysdata_read( table );
	if ( ret ) {
		free( version );
		free( table );
		return NULL;
	}

	sprintf( version, "%03d.%03d", table->header.version_major, 
				table->header.version_minor );

	free( table );

	return (const char *)version;
}


/** 
 * get_switchpart_data( ) - all data about switching partition system
 *
 * This gets the data structure containing the actual switching  
 * partition system's state
 * 
 * @return the current switching_table_t data
*/
 int get_switchpart_data( switching_table_t *switch_data ) {
	sysdata_t *table;
	int       ret;
	 
	if ( switch_data == NULL )
		return -EINVAL;

	table = malloc( sizeof( sysdata_t ) );
	if ( !table ) {
		return -ENOMEM;
	}

	ret = _sysdata_read( table );
	if ( ret ) {
		free( table );
		return -ENOSYS;
	}

	memcpy( switch_data, &table->data.switch_table, sizeof( switching_table_t ) );

	return 0;
 }


/** 
 * switch_part_group_round_way( ) - switch partition group in round way
 *
 * This switches partition group to the next one. If the current group is
 * the last one (tail), the selected group will be ne first one (head)
 * 
 * @return the current partition group
*/
int switch_part_group_round_way( switching_table_t *switch_data ) {
	switch_data->previous_boot_partition = switch_data->current_boot_partition;
	if ( switch_data->current_boot_partition == SYSDATA_MAX_PART_GROUP )
		switch_data->current_boot_partition = 1;
	else
		switch_data->current_boot_partition++;

	return switch_data->current_boot_partition;
 }


/** 
 * switch_part_group_last_used( ) - switch partition group to the past one
 *
 * This switches partition group to the past one. Use the previous_boot_partition
 * information
 * 
 * @return the current partition group
*/
int switch_part_group_last_used( switching_table_t *switch_data ) {
	uint32_t tmp;
	
	if ( switch_data->previous_boot_partition == switch_data->current_boot_partition )
		return -1;

	tmp = switch_data->previous_boot_partition;
	switch_data->previous_boot_partition = switch_data->current_boot_partition;
	switch_data->current_boot_partition = tmp;

	return switch_data->current_boot_partition;
 }


/** 
 * switch_part_group( ) - switch partition group with selected algorithm
 *
 * This switches partition group with selected algorithm
 * 
 * @return the current partition group
*/
 int switch_part_group( switch_group_alg_t alg ){
	int               ret = 0;
	int               ret1 = 0;
	sysdata_t         table;

	ret = _sysdata_read( &table );
	if ( ret < 0 )
		return ret;

	switch ( alg ) {
		case SWITCH_PART_GROUP_ROUND_WAY:
			ret = switch_part_group_round_way( &table.data.switch_table );
			break;
		case SWITCH_PART_GROUP_LAST_USED:
			ret = switch_part_group_round_way( &table.data.switch_table );
			break;
		default:
			ret = -EINVAL;
			break;
	}
	table.data.switch_table.boot_fail_counter = 0;

	if ( ret > 0 ) {
		// save changes
		ret1 = _sysdata_write( &table );
		if ( ret1 < 0 )
			return ret1;
	} 

	return ret;
 }


/** 
 * sysdata_switchpart_check( ) - check if the max number of fail boot was reached
 *
 * This switches partition group with round way algorithm if the maximum number
 * of fail boot was reached
 * 
 * @return 0 if ok, number of current partition group in case of switching
*/
int sysdata_switchpart_check( void ) {
	int               ret = 0;
	sysdata_t         table;

	ret = _sysdata_read( &table );
	if ( ret < 0 )
		return ret;

	if ( table.data.switch_table.boot_fail_counter == SYSDATA_MAX_FAIL_BOOT ) {
		// Need to switch partition
		ret = switch_part_group( SWITCH_PART_GROUP_ROUND_WAY );
		return ret;
	}

	 return 0;
 }


#ifndef USE_HOSTCC
/** 
 * sysdata_switchpart_current_env( ) - create a environment variable for current partition group
 *
 * This creates a environment variable for current partition group
 * 
 * @return void
*/
void sysdata_switchpart_current_env( void ) {
	int               ret;
	char              part[5];
	switching_table_t switch_data;

	ret = get_switchpart_data( &switch_data );
    if ( ! ret ) {
		sprintf( part, "%d", switch_data.current_boot_partition );
	} else { 
		sprintf( part, "0" );
	}
	env_set ("group_part", part );
}
#endif    /*  USE_HOSTCC  */


/** 
 * sysdata_switchpart_boot_inc( ) - increment the number of fail boot
 *
 * This increments the number of fail boot
 * 
 * @return 0 if ok, error number otherwise
*/
int sysdata_switchpart_boot_inc( void ) {
	int               ret = 0;
	sysdata_t         table;

	ret = _sysdata_read( &table );
	if ( ret < 0 )
		return ret;

	table.data.switch_table.boot_partition_counter[
		table.data.switch_table.current_boot_partition - 1]++;
	table.data.switch_table.boot_fail_counter++;
	ret = _sysdata_write( &table );
	if ( ret < 0 )
		return ret;
			
	return 0;
}


/** 
 * sysdata_switchpart_boot_ok( ) - initialize the fail boot counter
 *
 * This inizializes the fail boot counter in order to sign a valid boot
 * 
 * @return 0 if ok, error number otherwise
*/
int sysdata_switchpart_boot_ok( void ) {
	int               ret = 0;
	sysdata_t         table;

	ret = _sysdata_read( &table );
	if ( ret < 0 )
		return ret;

	table.data.switch_table.boot_fail_counter = 0;
	ret = _sysdata_write( &table );
	if ( ret < 0 )
		return ret;
			
	return 0;
}
