
#ifndef _SYSTEMDATA_H_
#define _SYSTEMDATA_H_


/* Note: use USE_HOSTCC to determinate if we are compiling
 * for u-boot or for external tool (in this case these is present) 
 */


#include <linux/kconfig.h>
#ifndef USE_HOSTCC
#include <common.h>
#else
#include <sys/types.h>
#include <stdint.h>
#include <ctype.h>
#endif


#define SECO_SWITCHPART_BASEADDR   (890 * 1024)
#define VERSION_MAJOR              1
#define VERSION_MINOR              0

#define SYSDATA_DEV_NAME_LEN       20
#define SYSDATA_VERSION_LEN        7


#define SYSDATA_MAX_PART_GROUP     2

#define SYSDATA_MAX_FAIL_BOOT      3



typedef enum switch_group_alg {
	SWITCH_PART_GROUP_ROUND_WAY,
	SWITCH_PART_GROUP_LAST_USED,
} switch_group_alg_t;


typedef struct systemdata_header  {
	uint32_t	      crc;		/* CRC32 over data bytes	*/
	uint32_t          version_major;
    uint32_t          version_minor;
} sysdata_header_t;


typedef struct switching_table {

    uint32_t          boot_partition_counter[SYSDATA_MAX_PART_GROUP];
    uint32_t          current_boot_partition;
	uint32_t          previous_boot_partition;
    uint32_t          boot_fail_counter;
} switching_table_t;


typedef struct systemdata_data {
	switching_table_t   switch_table;
} sysdata_data_t;


typedef struct systemdata {
	sysdata_header_t    header;
	sysdata_data_t      data;	
} sysdata_t;


#define SYSTEMDATA_SIZE           (unsigned long)(sizeof(sysdata_t))
#define SYSTEMDATA_CONTENT_SIZE   (unsigned long)(sizeof(sysdata_data_t))



#ifndef USE_HOSTCC
struct sysdata_driver {
	const char        *name;
	//enum env_location location;

	int (*load)( sysdata_t *rd_data );
	int (*save)( sysdata_t *new_data );
	const char *(*get_device_name)( void );
};


extern int sysdata_init( void );
extern int sysdata_integration_check( void );

extern const char *get_sysdata_device_name( void );
extern const char *get_sysdata_version( void );
/* switchpart functions */
extern int get_switchpart_data( switching_table_t *switch_data );

extern int switch_part_group( switch_group_alg_t alg );

extern int sysdata_switchpart_check( void );
extern int sysdata_switchpart_boot_inc( void );
extern int sysdata_switchpart_boot_ok( void );

#define U_BOOT_SYSDATA_LOCATION(__name)					\
	ll_entry_declare(struct sysdata_driver, __name, sysdata_driver)

	void sysdata_switchpart_current_env( void );
#else

/* the I/O basic functions will be implemeted by the external
 * tool
 */
extern int _sysdata_rd( sysdata_t *table );
extern int _sysdata_wr( sysdata_t *table );

#endif    /* USE_HOSTCC */

#endif     /*  _SYSTEMDATA_H_  */
