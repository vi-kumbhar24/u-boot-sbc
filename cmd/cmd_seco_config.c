/*
 * (C) Copyright 2015 Seco
 *
 * Author: Davide Cardillo <davide.cardillo@seco.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *
 * This code provide a tool to properly configure the boot environment
 * for the Seco Boards.
 *
 */


/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <environment.h> 
#include <stdlib.h>
#include <asm/mach-imx/sys_proto.h>

#include <seco_env.h>

DECLARE_GLOBAL_DATA_PTR;


#define ALWAYS_SAVE_FDT_OVERLAY_CMD  0


#ifndef CONFIG_SPL_BUILD

static data_boot_dev_t *kern_dev_list;
static data_boot_dev_t *fdt_dev_list;
#ifdef CONFIG_OF_LIBFDT_OVERLAY
static data_boot_dev_t *fdt_overlay_dev_list;
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
static data_boot_dev_t *ramfs_dev_list;
static data_boot_dev_t *filesystem_dev_list;

static int k_num, fdt_num, ramfs_num, fs_num;

static int kernel_selected_device = -1;
static int fdt_selected_device = -1;
static int use_ramfs = -1;
static int ramfs_selected_device = -1;
static int filesystem_selected_device = -1;



#if SCFG_SET_VIDEOMODE
static lvds_video_spec_t lvds_video_spec_list [] = {
	{ "WVGA	   [800x480]",   "LDB-WVGA",    "RGB666",      "datamap=spwg",  "",   1 },
	{ "SVGA	   [800x600]",   "LDB-SVGA",    "RGB666",      "datamap=spwg",  "",   1 },
	{ "XGA	   [1024x768]",  "LDB-XGA",     "RGB666",      "datamap=spwg",  "",   1 },
	{ "WXGA	   [1368x768]",  "LDB-WXGA",    "RGB24",       "datamap=jeida", "",   1 },
	{ "WXGAP60 [1280x800]",  "LDB-1280P60", "RGB24,bpp=32","datamap=spwg",  "",   1 },
	{ "SXGA	   [1280x1024]", "LDB-SXGA",    "RGB24",       "datamap=jeida", "",   1 },
	{ "HD1080  [1920x1080]", "LDB-1080P60", "RGB24",       "datamap=spwg", "ldb=spl0", 2 },
};
#endif




/*  __________________________________________________________________________
 * |                                                                          |
 * |                            RAM Size Selection                            |
 * |__________________________________________________________________________|
 */
char *do_ramsize (ulong min, ulong max) {
	char *line;
	do {printf ("\n __________________________________________________");
		printf ("\n       Chose Kernel RAM size to allocate\n");
		printf (" __________________________________________________\n");
		printf ("[min size: %luM - max size %luM]\n", min, max);
		printf ("> ");
		line = getline ();
	}while ((ulong)atoi(line) < min || (ulong)atoi(line) > max);
	printf ("Will use %luM of RAM memory\n",(ulong)atoi(line));
	return line;
}



/*  __________________________________________________________________________
 * |                                                                          |
 * |                           Parameters acquiring                           |
 * |__________________________________________________________________________|
 */

int selection_dev (char *scope, data_boot_dev_t dev_list[], int num_element) {
	char ch;
	int i, selection = 0;

	do {
		printf ("\n __________________________________________________");
		printf ("\n            Chose boot Device for %s.\n", scope);
		printf (" __________________________________________________\n");
		for ( i = 0 ; i < num_element ; i++ ) {
			printf ("%d) %s\n", i+1, dev_list[i].label);
		}
		printf ("> ");
		ch = getc ();
		printf ("%c\n", ch);
	} while ((ctoi(ch) < 1) || (ctoi(ch) > num_element));
	
	selection = ctoi(ch) - 1;

	return selection;
}


void select_partition_id (char *partition_id) {
	char ch;

	do {
		printf ("Chose the partition\n");
		printf ("> ");
		ch = getc ();
		printf ("%c\n", ch);
	} while (ctoi(ch) < MIN_PARTITION_ID || ctoi(ch) > MAX_PARTITION_ID);

	sprintf (partition_id, "%c", ch);
}


void select_source_path (char *source_path, char *subject, char *default_path) {
	char *line;

	printf ("Path of the %s (enter for default %s) > ", subject, default_path);
	line = getline ();

	strcpy (source_path, strcmp (line, "") == 0 ? default_path : line);
}



void select_tftp_parameters (char *serverip_tftp , char *ipaddr_tftp) {
	char *line;
	static char str[20];
	char *pstr;
	
	printf ("\n ________________________________________________________\n");
	printf (" Select TFTP as source for kernel, please set the net\n");
	printf (" ________________________________________________________\n");

	do {
		pstr = env_get ("serverip");
		if ( pstr == NULL ) {
			strcpy (str, __stringify(CONFIG_SERVERIP));
			pstr = &str[0];
		}
		printf ("\nInsert the address ip of the tftp server (enter for current: %s)\n", 
				pstr);
		printf ("> ");
                line = getline ();
                printf ("%s\n", line);
        } while (0);
	strcpy (serverip_tftp, strcmp (line, "") == 0 ? pstr : line);

	do {
		pstr = env_get ("ipaddr");
		if ( pstr == NULL ) {
			strcpy (str, __stringify(CONFIG_IPADDR));
			pstr = &str[0];
		}
		printf ("\nInsert the address ip of this tftp client (enter for current: %s)\n",
				pstr);
		printf ("> ");
                line = getline ();
                printf ("%s\n", line);
        } while (0);
	strcpy (ipaddr_tftp, strcmp (line, "") == 0 ? pstr : line);
}


void select_nfs_parameters (char *ip_local, char *ip_server, char *nfs_path, char *netmask, int *dhcp_set) {
	char *line;
	char ch;
	static char str[30];
	char *pstr;


	/*  Set DHCP configuration  */
	do { 
		printf ("\nDo you want to use dynamic ip assignment (DHCP)? (y/n)\n");
		printf ("> ");
		ch = getc ();
	} while (ch != 'y' && ch != 'n');
	if (ch == 'y') {
		*dhcp_set = 1;
		printf ("\nYou have select to use dynamic ip\n");
	} else {
		*dhcp_set = 0;
		printf ("\nYou have select to use static ip\n");
	}


	/*  Set HOST IP  */
	do {
		pstr = GET_NFS_IP_SERVER;
		if ( pstr == NULL ) {
			strcpy (str, __stringify(CONFIG_SERVERIP));
			pstr = &str[0];
		}

		printf ("Insert the address ip of the host machine (enter for current: %s)\n",
				pstr);
		printf ("> ");
		line = getline ();
		printf ("%s\n", line);
	} while (0);
	strcpy (ip_server, strcmp (line, "") == 0 ? pstr : line);


	/* Set the NFS Path  */
	do {
		pstr = GET_NFS_PATH;
		if ( pstr == NULL ) {
			strcpy (str, DEF_NFS_PATH);
			pstr = &str[0];
		}

		printf ("Insert the nfs path of the host machine (enter for current: %s)\n",
				pstr);
		printf ("> ");
		line = getline ();
		printf ("%s\n", line);
	} while (0);
	strcpy (nfs_path, strcmp (line, "") == 0 ? pstr : line);


	if (*dhcp_set == 0) {
		/* Set CLIENT IP  */
		do {
			pstr = GET_NFS_IP_CLIENT;
			if ( pstr == NULL ) {
				strcpy (str, __stringify(CONFIG_IPADDR));
				pstr = &str[0];
			}

			printf ("Insert an address ip for this board (enter for current: %s)\n",
					 pstr);
			printf ("> ");
			line = getline ();
			printf ("%s\n", line);
		} while (0);
		strcpy (ip_local, strcmp (line, "") == 0 ? pstr : line);


		/* set NETMASK of the CLIENT  */
		do {
			pstr = GET_NFS_NETMASK;
			if ( pstr == NULL ) {
				strcpy (str, __stringify(DEF_NETMASK));
				pstr = &str[0];
			}

			printf ("Insert the netmask (enter for current: %s)\n",
					pstr);
			printf ("> ");
			line = getline ();
			printf ("%s\n", line);
		} while (0);
		strcpy (netmask, strcmp (line, "") == 0 ? pstr : line);
	}

}


#ifdef CONFIG_OF_LIBFDT_OVERLAY
char *create_fdt_overlay_load_string( char *file_list ) {

	int i = 0;
	int sel_str_load = 0;
	char *list;
	char *fdt_load;
	char *fdtoverlay2ram;

	list = strtok( file_list, " " );

	if ( list != NULL ) {

		fdtoverlay2ram = malloc( sizeof(char) * 512 );
		if ( fdtoverlay2ram == NULL )
			return NULL;

		memset( fdtoverlay2ram, 0,  sizeof( fdtoverlay2ram ) );	

		if ( fdt_selected_device == -1 ) {
			fdt_load = GET_FDT_BOOT_LOAD;
			if ( fdt_load != NULL ) {
				for ( i = 0 ; i < fdt_num ; i++ ) {
					if ( strcmp( &fdt_dev_list[i].env_str[0], fdt_load ) == 0 ) {
						sel_str_load = i;
						break;
					}
				}
			} 
		} else {
			sel_str_load = fdt_selected_device;
		}
	
		while ( list != NULL ) {
			
			sprintf( fdtoverlay2ram, "%s %s 0x%08X %s; %s  0x%08X;",
										fdtoverlay2ram,
										fdt_overlay_dev_list[sel_str_load].env_str,
										ENV_FDT_OVERLAY_BASEADDR + ENV_FDT_OVERLAY_BASEADDR_OFFSET * i,
										list,
										STR(MACRO_ENV_FDT_OVERLAY_APPLY),
										ENV_FDT_OVERLAY_BASEADDR + ENV_FDT_OVERLAY_BASEADDR_OFFSET * i );

			i++;
			list = strtok( NULL, " " );

		}

	} else {

		return NULL;

	}

	return fdtoverlay2ram;
}
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
/*  __________________________________________________________________________
 * |                                                                          |
 * |              Kernel/FDT/RAMFS/FileSystem Source Selection                |
 * |__________________________________________________________________________|
 */
int select_kernel_source (char *partition_id, char *file_name,
			char *spi_load_addr, char *spi_load_len, int *use_tftp) {

	char *str;
	int dev_selected = selection_dev ("Kernel", kern_dev_list, k_num);

	switch ( kern_dev_list[dev_selected].dev_type ) {
		case DEV_EMMC:
		case DEV_U_SD:
		case DEV_EXT_SD:
		case DEV_SATA:
		case DEV_USB:
			select_partition_id (partition_id);
			break;
		case DEV_SPI:
			//select_spi_parameters (spi_load_addr, spi_load_len);
			break;
		case DEV_TFTP:
			*use_tftp = 1;
			break;
		default:
			break;
	}

	str = GET_KERNEL_PATH;
	if ( str != NULL )
		select_source_path (file_name, "Kernel", str);
	else
		select_source_path (file_name, "Kernel", kern_dev_list[dev_selected].def_path);

	return dev_selected;
}


int select_fdt_source (char *partition_id, char *file_name,
			char *spi_load_addr, char *spi_load_len, int *use_tftp) {

	char *str;
	char *autodetect_str;
	int dev_selected = selection_dev ("FDT", fdt_dev_list, fdt_num);

	switch ( fdt_dev_list[dev_selected].dev_type ) {
		case DEV_EMMC:
		case DEV_U_SD:
		case DEV_EXT_SD:
		case DEV_SATA:
		case DEV_USB:
			select_partition_id (partition_id);
			break;
		case DEV_SPI:
			//select_spi_parameters (spi_load_addr, spi_load_len);
			break;
		case DEV_TFTP:
			*use_tftp = 1;
			break;
		default:
			break;
	}

	autodetect_str = env_get ("fdt_autodetect");
	if ( (autodetect_str != NULL) && (strcmp(autodetect_str, "yes") == 0) ) {
		printf( "\nFDT file name autodetect: %s\n", env_get ("fdt_file") );
	} else {

		str = GET_FDT_PATH;
		if ( str != NULL )
			select_source_path (file_name, "FDT", str);
		else
			select_source_path (file_name, "FDT", fdt_dev_list[dev_selected].def_path);
	}
	return dev_selected;
}

int select_ramfs_source (char *partition_id, char *file_name,
                        char *spi_load_addr, char *spi_load_len, int *use_tftp) {

        char *str;
        int dev_selected = selection_dev ("Ramfs", ramfs_dev_list, ramfs_num);

	    switch ( ramfs_dev_list[dev_selected].dev_type ) {
				case DEV_NONE:
					return dev_selected;
                case DEV_EMMC:
                case DEV_U_SD:
                case DEV_EXT_SD:
                case DEV_SATA:
                case DEV_USB:
                        select_partition_id (partition_id);
                        break;
                case DEV_SPI:
                        //select_spi_parameters (spi_load_addr, spi_load_len);
                        break;
                case DEV_TFTP:
                        *use_tftp = 1;
                        break;
                default:
                        break;
        }

        str = GET_RAMFS_PATH;
        if ( str != NULL )
                select_source_path (file_name, "Ramfs", str);
        else
                select_source_path (file_name, "Ramfs", ramfs_dev_list[dev_selected].def_path);

        return dev_selected;
}


int select_filesystem_souce (char *partition_id, char *nfs_path, 
			char *serverip_nfs , char *ipaddr_nfs, char *netmask_nfs, int *use_dhcp) {

	int dev_selected = selection_dev ("FileSystem", filesystem_dev_list, fs_num);

	switch ( filesystem_dev_list[dev_selected].dev_type ) {
		case DEV_EMMC:
		case DEV_U_SD:
		case DEV_EXT_SD:
		case DEV_SATA:
		case DEV_USB:
			select_partition_id (partition_id);
			break;
		case DEV_NFS:
			select_nfs_parameters (ipaddr_nfs, serverip_nfs, nfs_path, netmask_nfs, use_dhcp);
			break;
		default:
			break;
	}

	return dev_selected;
}


/*  __________________________________________________________________________
 * |                                                                          |
 * |                         VIDEO Settings Selection                         |
 * |__________________________________________________________________________|
 */

#if SCFG_SET_VIDEOMODE

int selection_video_mode (video_mode_t  video_mode_list[], int num_element) {
	char ch;
	int i, selection = 0;

	do {
		printf ("\n __________________________________________________");
		printf ("\n               Chose Video Setting.\n");
		printf (" __________________________________________________\n");
		for ( i = 0 ; i < num_element ; i++ ) {
			printf ("%d) %s\n", i+1, video_mode_list[i].label);
		}
		printf ("> ");
		ch = getc ();
		printf ("%c\n", ch);
	} while ((ctoi(ch) < 1) || (ctoi(ch) > num_element));

	selection = ctoi(ch) - 1;

	return selection;
}


int selection_lvds_spec (char *scope, lvds_video_spec_t  video_list[], int num_element) {
	char ch;
	int i, selection = 0;

	do {
		printf ("\n __________________________________________________");
		printf ("\n        Chose LVDS resolution for %s.\n", scope);
		printf (" __________________________________________________\n");
		for ( i = 0 ; i < num_element ; i++ ) {
			printf ("%d) %s\n", i+1, video_list[i].label);
		}
		printf ("> ");
		ch = getc ();
		printf ("%c\n", ch);
	} while ((ctoi(ch) < 1) || (ctoi(ch) > num_element));

	selection = ctoi(ch) - 1;

	return selection;
}


int create_lvds_video_args (char *video_args, int list_idx, video_hw_t video,
			lvds_video_spec_t  video_list[], int num_element) {

	if ( (list_idx < 0) && (list_idx > (num_element - 1)) )
			return -1;

	sprintf (video_args, "video=%s:dev=%s,%s,%s,if=%s %s",
            video.buffer,
			video.driver,
			lvds_video_spec_list[list_idx].name,
			lvds_video_spec_list[list_idx].datamap,
			lvds_video_spec_list[list_idx].if_map,
			lvds_video_spec_list[list_idx].opt);
	return 0;
}

#endif


#ifdef CONFIG_OF_LIBFDT_OVERLAY
/*  __________________________________________________________________________
 * |                                                                          |
 * |                             FDT Overlay Selection                        |
 * |__________________________________________________________________________|
 */
void selection_fdt_overlay (overlay_list_t list[], int *selections, int num_element) {
	int i, j, num_opts;
	char ch;

	for ( i = 0 ; i < num_element ; i++ ) {

		if ( list[i].title == NULL )
			break;

		do {
			printf ("\n __________________________________________________");
			printf ("\n     Chose FDT overlay option for %s.\n", list[i].title);
			printf (" __________________________________________________\n");

			j=0;
			while ( list[i].options[j].label != NULL ) {
				printf( "%d) %s\n", j+1, list[i].options[j].label);
				j++;
			}
			num_opts = j;
			
			printf ("> ");
			ch = getc ();
			printf ("%c\n", ch);
		} while ((ctoi(ch) < 1) || (ctoi(ch) > num_opts));

		*(selections + i) = ctoi(ch) - 1;
	}
}
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */



static int do_seco_config (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]) {

	/* generic  */
	int use_tftp = 0;
	char serverip_tftp[25];
	char ipaddr_tftp[25];

	int set_memory = 0;
	int set_kernel = 0;
	int set_ramfs = 0;
	int set_fdt = 0;
	int set_filesystem = 0;
#ifdef CONFIG_OF_LIBFDT_OVERLAY
	int set_fdt_overlay_peripheral = 0;
	int set_fdt_overlay_load = 0;
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
	char *autodetect_str;
#if SCFG_SET_VIDEOMODE
	int set_video = 0;
#endif

	/*  memory RAM  */
	ulong min_size = 512;
	ulong max_size;
	char memory_buff[50];
	char *line;

	/*  for kernel  */
	char kernel_part_id[2];
	char kernel_filename[100];
	char kernel_spi_load_address[20];
	char kernel_spi_load_length[20];

	/*  for fdt  */
	char fdt_part_id[2];
	char fdt_filename[100];
	char fdt_spi_load_address[20];
	char fdt_spi_load_length[20];

	/*  for ramfs  */
    char ramfs_part_id[2];
    char ramfs_filename[100];
    char ramfs_spi_load_address[20];
    char ramfs_spi_load_length[20];

	/*  for filesystem  */
	char filesystem_part_id[2];
	char filesystem_server_nfs[25];
	char filesystem_ipaddr_nfs[25];
	char filesystem_netmask_nfs[25];
	int  filesystem_use_dhcp;
	char filesystem_path_nfs[100];
	
#if SCFG_SET_VIDEOMODE
	/*  for video  */
	int video_mode_selection;
	int video_lvds1_selection;
	int video_lvds2_selection;
	char video_hdmi_video[80];
	char video_lvds1_video[80];
	char video_lvds2_video[80];
	char video_mode[300];

#ifdef CONFIG_OF_LIBFDT_OVERLAY
	/*  for Video FDT overlay  */
	char video_overlay_list[256];
	char *fdtoverlay_video_load;
	char *video_list_overlay;
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */

#endif  /* SCFG_SET_VIDEOMODE */

#ifdef CONFIG_OF_LIBFDT_OVERLAY
	/*  for Peripheral FDT overlay  */
	int i;
	int *peripheral_overlay_selections;
	char peripheral_overlay_list[256];
	char *fdtoverlay_peripheral_load;
	char *peripheral_list_overlay;
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */


#if (defined(CONFIG_MX6Q)) || (defined(CONFIG_MX6DL)) || (defined(CONFIG_MX6QDL))
	if ( is_mx6dl() || is_mx6solo() ) {

		k_num     = ARRAY_SIZE( kern_dev_imx6dl_list );
		fdt_num   = ARRAY_SIZE( fdt_dev_imx6dl_list );
		ramfs_num = ARRAY_SIZE( ramfs_dev_imx6dl_list );
		fs_num    = ARRAY_SIZE( filesystem_dev_imx6dl_list );

		kern_dev_list        = &kern_dev_imx6dl_list[0];
		fdt_dev_list         = &fdt_dev_imx6dl_list[0];
#ifdef CONFIG_OF_LIBFDT_OVERLAY
		fdt_overlay_dev_list = &fdt_overlay_dev_imx6dl_list[0];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
		ramfs_dev_list       = &ramfs_dev_imx6dl_list[0];
		filesystem_dev_list  = &filesystem_dev_imx6dl_list[0];

	} else if ( is_mx6dq() ) {

		k_num     = ARRAY_SIZE( kern_dev_imx6q_list );
		fdt_num   = ARRAY_SIZE( fdt_dev_imx6q_list );
		ramfs_num = ARRAY_SIZE( ramfs_dev_imx6q_list );
		fs_num    = ARRAY_SIZE( filesystem_dev_imx6q_list );

		kern_dev_list        = &kern_dev_imx6q_list[0];
		fdt_dev_list         = &fdt_dev_imx6q_list[0];
#ifdef CONFIG_OF_LIBFDT_OVERLAY
		fdt_overlay_dev_list = &fdt_overlay_dev_imx6q_list[0];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
		ramfs_dev_list       = &ramfs_dev_imx6q_list[0];
		filesystem_dev_list  = &filesystem_dev_imx6q_list[0];

	} else
		return 1;
#endif

#if (defined(CONFIG_MX6SX))
	if ( is_mx6sx() ) {

		k_num     = ARRAY_SIZE( kern_dev_imx6sx_list );
		fdt_num   = ARRAY_SIZE( fdt_dev_imx6sx_list );
		ramfs_num = ARRAY_SIZE( ramfs_dev_imx6sx_list );
		fs_num    = ARRAY_SIZE( filesystem_dev_imx6sx_list );

		kern_dev_list        = &kern_dev_imx6sx_list[0];
		fdt_dev_list         = &fdt_dev_imx6sx_list[0];
#ifdef CONFIG_OF_LIBFDT_OVERLAY
		fdt_overlay_dev_list = &fdt_overlay_dev_imx6sx_list[0];
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
		ramfs_dev_list       = &ramfs_dev_imx6sx_list[0];
		filesystem_dev_list  = &filesystem_dev_imx6sx_list[0];

	} else
		return 1;
#endif

	if (argc > 2)
		return cmd_usage (cmdtp);


	if (argc == 2 && strcmp(argv[1], "help") == 0)
		return cmd_usage (cmdtp);


	if (argc == 2) {

		if (strcmp(argv[1], "default") == 0) {
		//	set_default_env ("## Resetting to default environment");
		}

		if (strcmp(argv[1], "memory") == 0) {
			set_memory = 1;
		}

		if (strcmp(argv[1], "ksrc") == 0) {
			set_kernel = 1;
		}

		if (strcmp(argv[1], "ramfssrc") == 0) {
                        set_ramfs = 1;
                }

		if (strcmp(argv[1], "fdtsrc") == 0) {
			set_fdt = 1;
		}

		if (strcmp(argv[1], "fssrc") == 0) {
			set_filesystem = 1;
		}

#ifdef CONFIG_OF_LIBFDT_OVERLAY
		if (strcmp(argv[1], "peripheral") == 0) {
			set_fdt_overlay_peripheral = 1;
		}

		if (strcmp(argv[1], "fdtoverlay") == 0) {
			set_fdt_overlay_load = 1;
		}
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */

#if SCFG_SET_VIDEOMODE
		if (strcmp(argv[1], "video") == 0) {
			set_video = 1;
		}
#endif

	}


	if (argc == 1) {
		set_memory = 1;
		set_kernel = 1;
		set_ramfs = 1;
		set_fdt = 1;
		set_filesystem = 1;
#if SCFG_SET_VIDEOMODE
		set_video = 1;
#endif
#ifdef CONFIG_OF_LIBFDT_OVERLAY
		set_fdt_overlay_peripheral = 1;
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
	}


/*  __________________________________________________________________________
 * |______________________________ MEMORY RAM ________________________________|
 */
	if ( set_memory ) {
		autodetect_str = env_get ("mem_autodetect");
		if ( (autodetect_str != NULL) && (strcmp(autodetect_str, "yes") == 0) ) {
			printf( "\nKernel memory size autodetect: %s\n", env_get ("memory") );
		} else {
			ulong size = gd->ram_size;
			max_size = size / (1 << 20); // get size in MB
			if (max_size > 2100)
				max_size = 3800;
			if ( min_size > max_size )
				min_size = max_size;
			line = do_ramsize (min_size, max_size);
			if ( (ulong)atoi(line) < DEFAULT_CMA_VALUE ) {
				sprintf (memory_buff, "mem=%sM cma=96M", line);
			} else {
				sprintf (memory_buff, "mem=%sM", line);
			}
			env_set ("memory", memory_buff);
		}
	}



/*  __________________________________________________________________________
 * |________________________________ KERNEL __________________________________|
 */
	if ( set_kernel) {
		kernel_selected_device = select_kernel_source (kernel_part_id,
				kernel_filename, kernel_spi_load_address,
				kernel_spi_load_length, &use_tftp);

		SAVE_KERNEL_DEVICE_ID(kern_dev_list[kernel_selected_device].device_id);

		if ( kern_dev_list[kernel_selected_device].dev_type != DEV_TFTP ) {
			SAVE_KERNEL_PATH(kernel_filename);
		}

		switch (kern_dev_list[kernel_selected_device].dev_type) {
			case DEV_EMMC:
			case DEV_U_SD:
			case DEV_EXT_SD:
			case DEV_SATA:
			case DEV_USB:
				SAVE_KERNEL_PARTITION(kernel_part_id);
				break;
			case DEV_SPI:
				//SAVE_KERNEL_SPI_ADDR(kernel_spi_load_address);
				//SAVE_KERNEL_SPI_LEN(kernel_spi_load_length);
				break;
			case DEV_TFTP:
				break;
			default:
				break;
		}

		SAVE_KERNEL_BOOT_LOAD(kern_dev_list[kernel_selected_device].env_str);
	}

/*  __________________________________________________________________________
 * |__________________________________ FDT ___________________________________|
*/
	if ( set_fdt ) {
		fdt_selected_device = select_fdt_source (fdt_part_id,
				fdt_filename, fdt_spi_load_address,
				fdt_spi_load_length, &use_tftp);

		SAVE_FDT_DEVICE_ID(fdt_dev_list[fdt_selected_device].device_id);

		if ( fdt_dev_list[fdt_selected_device].dev_type != DEV_TFTP ) {
			autodetect_str = env_get ("fdt_autodetect");
			if ( (autodetect_str == NULL) || (strcmp(autodetect_str, "no") == 0) ) {
				SAVE_FDT_PATH(fdt_filename);
			}
		}

		switch (fdt_dev_list[fdt_selected_device].dev_type) {
			case DEV_EMMC:
			case DEV_U_SD:
			case DEV_EXT_SD:
			case DEV_SATA:
			case DEV_USB:
				SAVE_FDT_PARTITION(fdt_part_id);
				break;
			case DEV_SPI:
		//		SAVE_FDT_SPI_ADDR(fdt_spi_load_address);
			//	SAVE_FDT_SPI_LEN(fdt_spi_load_length);
				break;
			case DEV_TFTP:
				break;
			default:
				break;
		}

		SAVE_FDT_BOOT_LOAD(fdt_dev_list[fdt_selected_device].env_str);

#ifdef CONFIG_OF_LIBFDT_OVERLAY
#if ALWAYS_SAVE_FDT_OVERLAY_CMD
		if ( set_fdt_overlay_peripheral == 0 ) {
			peripheral_list_overlay = GET_FDT_OVERLAY_PER_LIST;
			if ( peripheral_list_overlay != NULL ) {
				fdtoverlay_peripheral_load = create_fdt_overlay_load_string( peripheral_list_overlay );
				SAVE_FDT_OVERLAY_LOAD( fdtoverlay_peripheral_load );
			}
		}
#endif
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
	}


	/*  __________________________________________________________________________
	 * |________________________________ RAMFS ___________________________________|
	 */
	if ( set_ramfs) {
		ramfs_selected_device = select_ramfs_source (ramfs_part_id,
				ramfs_filename, ramfs_spi_load_address,
				ramfs_spi_load_length, &use_tftp);

		SAVE_RAMFS_DEVICE_ID(ramfs_dev_list[ramfs_selected_device].device_id);
		//  SAVE_RAMFS_LOADADDR(ramfs_dev_list[ramfs_selected_device].load_address);

		if ( ramfs_dev_list[ramfs_selected_device].dev_type != DEV_TFTP ) {
			SAVE_RAMFS_PATH(ramfs_filename);
		}

		use_ramfs = 0;
		switch (ramfs_dev_list[ramfs_selected_device].dev_type) {
			case DEV_EMMC:
			case DEV_U_SD:
			case DEV_EXT_SD:
			case DEV_SATA:
			case DEV_USB:
				use_ramfs = 1;
				SAVE_RAMFS_PARTITION(kernel_part_id);
				break;
			case DEV_SPI:
				use_ramfs = 1;
				SAVE_RAMFS_SPI_ADDR(ramfs_spi_load_address);
				SAVE_RAMFS_SPI_LEN(ramfs_spi_load_length);
				break;
			case DEV_TFTP:
				use_ramfs = 1;
				break;
			default:
				break;
		}

		SAVE_RAMFS_BOOT_LOAD(ramfs_dev_list[ramfs_selected_device].env_str);
		if ( use_ramfs )
			env_set ("ramfs_use", "1");
		else
			env_set ("ramfs_use", "0");
	}


	/*  __________________________________________________________________________
 * |_______________________________ NET OF TFTP ______________________________|
 */
	if ( set_kernel || set_fdt || set_ramfs) {
		if ( use_tftp ) {
			select_tftp_parameters (serverip_tftp , ipaddr_tftp);
			env_set ("serverip", serverip_tftp);
			env_set ("ipaddr", ipaddr_tftp);
		}
	}

/*  __________________________________________________________________________
 * |______________________________ FILE SYSTEM _______________________________|
 */
	if ( set_filesystem ) {
		filesystem_selected_device = select_filesystem_souce (filesystem_part_id,
				filesystem_path_nfs, filesystem_server_nfs, filesystem_ipaddr_nfs,
				filesystem_netmask_nfs, &filesystem_use_dhcp);

		switch (filesystem_dev_list[filesystem_selected_device].dev_type) {
			case DEV_EMMC:
			case DEV_U_SD:
			case DEV_EXT_SD:
				SAVE_FILESYSTEM_DEVICE_ID(filesystem_dev_list[filesystem_selected_device].device_id);
				SAVE_FILESYSTEM_PARTITION(filesystem_part_id);
				break;
			case DEV_SATA:
			case DEV_USB:
				SAVE_FILESYSTEM_PARTITION(filesystem_part_id);
				break;
			case DEV_SPI:
				SAVE_FDT_SPI_ADDR(fdt_spi_load_address);
				SAVE_FDT_SPI_LEN(fdt_spi_load_length);
				break;
			case DEV_NFS:
				SAVE_NFS_USE("1");
				SAVE_NFS_IP_CLIENT(filesystem_ipaddr_nfs);
				SAVE_NFS_IP_SERVER(filesystem_server_nfs);
				SAVE_NFS_NETMASK(filesystem_netmask_nfs);
				SAVE_NFS_PATH(filesystem_path_nfs);
				if ( filesystem_use_dhcp == 1 ) {
					SAVE_NFS_USE_DHCP("1");
				} else {
					SAVE_NFS_USE_DHCP("0");
				}
				break;
			case DEV_TFTP:
				break;
			default:
				break;
		}

		if ( filesystem_dev_list[filesystem_selected_device].dev_type != DEV_NFS )
			SAVE_NFS_USE("0");

//		sprintf (filesystem_boot_string, "env_set root_dev \'%s\'",
//				filesystem_dev_list[filesystem_selected_device].env_str);

		SAVE_FILESYSTEM_ROOT(filesystem_dev_list[filesystem_selected_device].env_str);
	}

#if SCFG_SET_VIDEOMODE
	/*  __________________________________________________________________________
	 * |_____________________________ VIDEO SETTING ______________________________|
	 */
	if ( set_video ) {
		video_mode_selection = selection_video_mode (video_mode_list,
				ARRAY_SIZE(video_mode_list));
		
		/* video setting for u-boot slash screen */
		if ( video_mode_list[video_mode_selection].panel_name != NULL ) {
			SET_VIDEO_PANEL( video_mode_list[video_mode_selection].panel_name );
		} 

#ifdef CONFIG_OF_LIBFDT_OVERLAY
		/* video setting for a fdt overlay file */
		memset( video_overlay_list, 0,  sizeof( video_overlay_list ) );
		if ( video_mode_list[video_mode_selection].dtbo_conf_file != NULL ) {
			if ( strcmp( video_mode_list[video_mode_selection].dtbo_conf_file, "" ) != 0 ) {
				
				sprintf( video_overlay_list, "%s", video_mode_list[video_mode_selection].dtbo_conf_file );
				SAVE_FDT_OVERLAY_VIDEO_LIST( video_overlay_list );

#if ALWAYS_SAVE_FDT_OVERLAY_CMD
				fdtoverlay_video_load = create_fdt_overlay_load_string( video_overlay_list );
				SAVE_FDT_OVERLAY_VIDEO_CMD( fdtoverlay_video_load );
#endif
			}
		} else {
			SAVE_FDT_OVERLAY_VIDEO_LIST( "" );
			SAVE_FDT_OVERLAY_VIDEO_CMD( "" );
		}
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */


        if ( video_mode_list[video_mode_selection].fb_hdmi.buffer != NULL &&
			video_mode_list[video_mode_selection].fb_hdmi.driver != NULL && 
		    video_mode_list[video_mode_selection].fb_hdmi.used == VIDEO_USED ) 
        {
            sprintf (video_hdmi_video, "video=%s:dev=%s,1920x1080M@60,if=RGB24",
		 		    	video_mode_list[video_mode_selection].fb_hdmi.buffer,
					    video_mode_list[video_mode_selection].fb_hdmi.driver);

			/*  Set U-boot panel display if HDMI is primary display */
			if ( strcmp( video_mode_list[video_mode_selection].fb_hdmi.buffer, "mxcfb0") == 0 )
				env_set ("panel", "HDMI");

		} else
		    sprintf (video_hdmi_video, " ");


        if ( video_mode_list[video_mode_selection].fb_lvds1.buffer != NULL &&
			video_mode_list[video_mode_selection].fb_lvds1.driver != NULL && 
		    video_mode_list[video_mode_selection].fb_lvds1.used == VIDEO_USED ) 
        {
            video_lvds1_selection = selection_lvds_spec ("LVDS1",
					lvds_video_spec_list, ARRAY_SIZE(lvds_video_spec_list));

			create_lvds_video_args (video_lvds1_video, video_lvds1_selection,
					video_mode_list[video_mode_selection].fb_lvds1,
					lvds_video_spec_list, ARRAY_SIZE(lvds_video_spec_list));

			/*  Set U-boot panel display if HDMI is primary display */
			if ( strcmp( video_mode_list[video_mode_selection].fb_lvds1.buffer, "mxcfb0") == 0 )
				env_set ("panel", lvds_video_spec_list[video_lvds1_selection].name);

		} else
		    sprintf (video_lvds1_video, " ");


        if ( video_mode_list[video_mode_selection].fb_lvds2.buffer != NULL &&
			video_mode_list[video_mode_selection].fb_lvds2.driver != NULL && 
		    video_mode_list[video_mode_selection].fb_lvds2.used == VIDEO_USED ) 
        {
            video_lvds2_selection = selection_lvds_spec ("LVDS2",
					lvds_video_spec_list, ARRAY_SIZE(lvds_video_spec_list));

			create_lvds_video_args (video_lvds2_video, video_lvds2_selection,
					video_mode_list[video_mode_selection].fb_lvds2,
					lvds_video_spec_list, ARRAY_SIZE(lvds_video_spec_list));

			/*  Set U-boot panel display if HDMI is primary display */
			if ( strcmp( video_mode_list[video_mode_selection].fb_lvds2.buffer, "mxcfb0") == 0 )
				env_set ("panel", lvds_video_spec_list[video_lvds2_selection].name);

		} else
		    sprintf (video_lvds2_video, " ");

		sprintf (video_mode, "%s %s %s", video_hdmi_video, video_lvds1_video,
				video_lvds2_video);

		if ( strcmp (video_hdmi_video, " ") ) {
			env_set ("video_hdmi", video_hdmi_video);
		}

		if ( strcmp (video_lvds1_video, " ") ) {
			env_set ("video_lvds1", video_lvds1_video);
		}

		if ( strcmp (video_lvds2_video, " ") ) {
			env_set ("video_lvds2", video_lvds2_video);
		}

		env_set ("videomode", video_mode);
	}
#endif

#ifdef CONFIG_OF_LIBFDT_OVERLAY
	/*  __________________________________________________________________________
	 * |_____________________FDT OVERLAY PERIPHERAL SETTING ______________________|
	 */
	if ( set_fdt_overlay_peripheral ) {
		peripheral_overlay_selections = malloc( sizeof(int) *  ARRAY_SIZE(overlay_peripheral_list) );
		memset( peripheral_overlay_selections, 0,  ARRAY_SIZE(overlay_peripheral_list) );
		memset( peripheral_overlay_list, 0,  sizeof( peripheral_overlay_list ) );

		selection_fdt_overlay( overlay_peripheral_list, peripheral_overlay_selections, ARRAY_SIZE(overlay_peripheral_list) );
		
		for ( i = 0 ; i < ARRAY_SIZE(overlay_peripheral_list) ; i++ ) {

			if ( overlay_peripheral_list[i].options[peripheral_overlay_selections[i]].dtb_overlay == NULL )
				break;

			if ( i == 0 )
				sprintf( peripheral_overlay_list, "%s", overlay_peripheral_list[i].options[peripheral_overlay_selections[i]].dtb_overlay );
			else
				sprintf( peripheral_overlay_list, "%s %s",
											peripheral_overlay_list,
											overlay_peripheral_list[i].options[peripheral_overlay_selections[i]].dtb_overlay );

			SAVE_FDT_OVERLAY_PER_LIST( peripheral_overlay_list );			

		}

#if ALWAYS_SAVE_FDT_OVERLAY_CMD
		fdtoverlay_peripheral_load = create_fdt_overlay_load_string( peripheral_overlay_list );
		SAVE_FDT_OVERLAY_PER_CMD( fdtoverlay_peripheral_load );
#endif
	}

	/*  __________________________________________________________________________
	 * |_______________________FDT OVERLAY DYNAMIC LOAD CMD ______________________|
	 */
	if ( set_fdt_overlay_load ) {
		peripheral_list_overlay = GET_FDT_OVERLAY_PER_LIST;
		if ( peripheral_list_overlay != NULL ) {
			fdtoverlay_peripheral_load = create_fdt_overlay_load_string( peripheral_list_overlay );
			SAVE_FDT_OVERLAY_LOAD( fdtoverlay_peripheral_load );
		} 		

#if SCFG_SET_VIDEOMODE
		video_list_overlay = GET_FDT_OVERLAY_VIDEO_LIST;
		if ( video_list_overlay != NULL ) {
			fdtoverlay_video_load = create_fdt_overlay_load_string( video_list_overlay );
				SAVE_FDT_OVERLAY_VIDEO_CMD( fdtoverlay_video_load );
		}
#endif
	}
	

/*  __________________________________________________________________________
 * |__________________________________________________________________________|
*/
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */

	if ( ( argc == 1 ) || 
		( ( argc == 2 ) && ( ( set_memory == 1 ) || ( set_kernel == 1 ) || ( set_ramfs == 1) ||
		  ( set_fdt == 1 ) || ( set_filesystem == 1 )
#if SCFG_SET_VIDEOMODE
		 || ( set_video == 1 )
#endif
#ifdef CONFIG_OF_LIBFDT_OVERLAY
		|| ( set_fdt_overlay_peripheral == 1 )
#endif  /*  CONFIG_OF_LIBFDT_OVERLAY */
	   	) ) )
	{
	
		printf ("\n\n");
		env_save ();
		printf ("\n\n");

	}

	return 0;
}



/***************************************************/

U_BOOT_CMD(
	seco_config, 3, 1, do_seco_config,
	"Interactive setup for seco configuration.",
	"                          - for whole environment\n"
	"seco_config [default]     - resetting to default environment\n"
	"seco_config [memory]      - for kernel RAM size.\n"
	"seco_config [ksrc]        - for Kernel source device.\n"
	"seco_config [fdtsrc]      - for FDT source device.\n"
#ifdef CONFIG_OF_LIBFDT_OVERLAY
	"seco_config [peripheral]  - for board's peripheral setting.\n"
	"seco_config [fdtoverlay]  - create commands to load fdt overlay file\n"
#endif  /* CONFIG_OF_LIBFDT_OVERLAY */
	"seco_config [ramfssrc]    - for RAMFS source device.\n"
	"seco_config [fssrc]       - for FileSystem source device.\n"
#if SCFG_SET_VIDEOMODE
	"seco_config [video]       - for video usage mode\n"
#endif
);

#endif   /* CONFIG_SPL_BUILD */
