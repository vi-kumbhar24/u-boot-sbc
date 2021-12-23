
#include <common.h>
#include <cli.h>
#include <command.h>
#include <console.h>
#include <environment.h>
#include <search.h>
#include <errno.h>
#include <malloc.h>
#include <mapmem.h>
#include <watchdog.h>
#include <linux/stddef.h>
#include <asm/byteorder.h>
#include <asm/io.h>

#include "sysdata.h"




static int do_sysdata( cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[] )
{
    int ret = 0;

    if ( argc == 2 ) {

        if ( strcmp( argv[1], "version" ) == 0 ) {

            const char *version = get_sysdata_version( );
            if ( version )
                printf( "SysData version: %s\n", version );
            else
                printf( "Error on retrieving data\n" );

        } else  if ( strcmp( argv[1], "dev" ) == 0 ) {

            const char *device = get_sysdata_device_name( );
            if ( device )
                printf( "device: %s\n", device );
            else
                printf( "Error on retrieving data\n" );

        } else if ( strcmp( argv[1], "initialize" ) == 0 ) {    
            ret = sysdata_init( );
            if ( ret )
                printf( "Error (%d), table not initialized\n", ret );
            else
                printf( "Table initialized\n" );
        }

    } else if ( argc >= 3 ) {

        if ( strcmp( argv[1], "switchpart" ) == 0 ) {
            int               i;
            switching_table_t switch_data;

            if ( strcmp( argv[2], "status" ) == 0 ) {
                ret = get_switchpart_data( &switch_data );
                if ( ret )
                    printf( "Error (%d) on retrieving data\n", ret );
                else {
                    for ( i = 1 ; i <= SYSDATA_MAX_PART_GROUP ; i++ ) {
                        printf( "Boot on partition group %d: %05d\n", i, switch_data.boot_partition_counter[i-1] );
                    }
                    printf( "Current boot partition group: %01d\n", switch_data.current_boot_partition );
                    printf( "Previous boot partition group: %01d\n", switch_data.previous_boot_partition );
                    printf( "Consegutive fail boot: %01d\n", switch_data.boot_fail_counter );
                }
            }

            if ( strcmp( argv[2], "switch" ) == 0 ) {
                if ( argc >= 4 ) {
                    if ( strcmp( argv[3], "round" ) == 0 ) {
                        ret = switch_part_group( SWITCH_PART_GROUP_ROUND_WAY );
                    } else if ( strcmp( argv[3], "last" ) == 0 ) {
                        ret = switch_part_group( SWITCH_PART_GROUP_LAST_USED );
                    } else 
                        ret = -EINVAL;
                } else 
                    ret = -EINVAL;

                if ( ret > 0 ) {
                    sysdata_switchpart_current_env( );
                    printf( "Switch to group: %d\n", ret );
                }
                else
                    printf( "Error (%d) on switching or invalid parameters.\n", ret );
            }

            if ( strcmp( argv[2], "grouppart" ) == 0 ) {
                ret = get_switchpart_data( &switch_data );
                if ( ret )
                    printf( "0" );
                else {
                    printf( "%01d", switch_data.current_boot_partition );
                }
            }
        }

    } 
        
    return 4;
}


U_BOOT_CMD_COMPLETE(
	sysdata, CONFIG_SYS_MAXARGS, 1,	do_sysdata,
	"SECO system data manager",
    "",
	var_complete
);
