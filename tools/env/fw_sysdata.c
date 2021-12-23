#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>
#include <version.h>


#include <sysdata.h>
#include "fw_sysdata.h"


/*  _____________________________________________________________________________
 * |                                                                             |
 * |                               ARGUMENT MANAGEMENT                           |
 * |_____________________________________________________________________________|
 */
typedef enum cmd_target {
    CMD_NONE = 0,
    CMD_SWITCHPART,
} cmd_target_t;


typedef enum cmd_switchpart {
    CMD_SWITCHPART_NONE = 0,
    CMD_SWITCHPART_INIT,
    CMD_SWITCHPART_BOOTOK,
    CMD_SWITCHPART_SWITCH,
    CMD_SWITCHPART_PRINT,
} cmd_switchpart_t;


static cmd_target_t _cmd = CMD_NONE;
static cmd_switchpart_t _cmd_swichpart = CMD_SWITCHPART_NONE;


static struct option common_options[] = {
	{"target", required_argument, NULL, 't'},
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'},
	{NULL, 0, NULL, 0}
};


static struct option switchpart_options[] = {
	{"initialize", no_argument, NULL, 'i'},
	{"bootok", no_argument, NULL, 'b'},
	{"switch", no_argument, NULL, 's'},
    {"print", no_argument, NULL, 'p'},
	{NULL, 0, NULL, 0}
};


static void print_usage( void ) {
}


static void parse_common_args( int args, char *argv[] ) {
    int c;

    while ( ( c = getopt_long( args, argv, ":t:h:v", common_options, NULL ) ) != EOF ) {
		switch ( c ) {
            case 't':
                if ( strcmp( optarg, "switchpart") == 0 ) {
                    _cmd = CMD_SWITCHPART;
                } else {
                    fprintf( stderr, "Error: invalid command type!!!\n" );
                    print_usage( );
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                print_usage( );
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                fprintf( stderr, "Compiled with 000 \n" );
                exit(EXIT_SUCCESS);
                break;
            default:
                /* ignore unknown options */
                break;
		}
	}

    /* Reset getopt for the next pass. */
	opterr = 1;
	optind = 1;
}


static void parse_switchpart_args( int args, char *argv[] ) {
    int c;

    while ( ( c = getopt_long( args, argv, ":ibsp", switchpart_options, NULL ) ) != EOF ) {

        if ( _cmd_swichpart != CMD_SWITCHPART_NONE )
            break;
       
		switch ( c ) {
            case 'i':
                _cmd_swichpart = CMD_SWITCHPART_INIT;
                break;
            case 'b':
                _cmd_swichpart = CMD_SWITCHPART_BOOTOK;
                break;
            case 's':
                _cmd_swichpart = CMD_SWITCHPART_SWITCH;
                break;
            case 'p':
                _cmd_swichpart = CMD_SWITCHPART_PRINT;
            default:
                /* ignore unknown options */
                break;
		}
	}

    /* Reset getopt for the next pass. */
	opterr = 1;
	optind = 1;
}

/*  _____________________________________________________________________________
 * |_____________________________________________________________________________|
 */


int main( int argc, char *argv[] ) {
    int ret, i;

     parse_common_args( argc, argv );
    
    if ( _cmd == CMD_NONE ) {

        fprintf( stderr, "Error: command type not defined!!!\n" );
        exit(EXIT_FAILURE);

    } else if ( _cmd == CMD_SWITCHPART ) {

        parse_switchpart_args( argc, argv );
        switch ( _cmd_swichpart ) {
            case CMD_SWITCHPART_INIT:
                fprintf( stdout, "Switchpart: data initialization... " );
                ret = sysdata_init( );
                if ( ret == 0 ) {
                    fprintf( stdout, "OK\n" );
                } else {
                    fprintf( stdout, "error (%d)\n", ret );
                }
                break;
            case CMD_SWITCHPART_BOOTOK:
                fprintf( stdout, "Switchpart: validate boot... " );
                ret = sysdata_switchpart_boot_ok( );
                 if ( ret == 0 ) {
                    fprintf( stdout, "OK\n" );
                } else {
                    fprintf( stdout, "error (%d)\n", ret );
                }
                break;
            case CMD_SWITCHPART_SWITCH:
                fprintf( stdout, "Switchpart: switch partition... " );
                ret = switch_part_group( SWITCH_PART_GROUP_ROUND_WAY );
                if ( ret > 0 ) {
                    fprintf( stdout, "OK (group %d)\n", ret );
                } else {
                    fprintf( stdout, "error (%d)\n", ret );
                }
                break;
            case CMD_SWITCHPART_PRINT:
                fprintf( stdout, "Switchpart: current state:\n" );
                
                switching_table_t switch_data;
                ret = get_switchpart_data( &switch_data );
                 if ( ret )
                    printf( "Error (%d) on retrieving data\n", ret );
                else {
                    for ( i = 1 ; i <= SYSDATA_MAX_PART_GROUP ; i++ ) {
                        printf( "   Boot on partition group %d: %05d\n", i, switch_data.boot_partition_counter[i] );
                    }
                    printf( "   Current boot partition group: %01d\n", switch_data.current_boot_partition );
                    printf( "   Previous boot partition group: %01d\n", switch_data.previous_boot_partition );
                    printf( "   Consegutive fail boot: %01d\n", switch_data.boot_fail_counter );
                }

                break;
            default:
                fprintf( stderr, "Switchpart: invalid command!\n" );
                break;
        }

    }
    return 0;
}
