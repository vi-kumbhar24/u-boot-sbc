/*
 *
 * SECO S.p.A. 
 * Additional Uart command 
 * Written by: Davide Cardillo, Marco Sandrelli
 *
 */
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <console.h>
#include <environment.h>
#include <stdio_dev.h>
#include <linux/ctype.h>
#include <watchdog.h>
#include <serial.h>


DECLARE_GLOBAL_DATA_PTR;

static int is_initialized = 0;
static int baudrate = 0;

/* Send a Byte */
void uart_putc (char ch) 
{
	struct udevice *additional_uart = gd->add_serial_dev;
	serial_dev_putc(ch, additional_uart);
}

/* Send a string */
static void uart_write (char *str) {
	serial_dev_puts(gd->add_serial_dev, str);
}

/* Get uart baudrate */
static int uart_get_baudrate(void)
{
	struct udevice *additional_uart = gd->add_serial_dev;
	struct serial_device_info *info;

	info = (struct serial_device_info * )malloc(sizeof(info));

	serial_getinfo(additional_uart, info);

	return info->baudrate;
}

/* Read string from stdin */
static void uart_read (void) {

	struct udevice *additional_uart = gd->add_serial_dev;
	int ret;	

        do {
                ret = serial_dev_getc(additional_uart);
		printf ("%c", ret);
		WATCHDOG_RESET();
		if (ctrlc())
			break;
	} while (1);

}

/* Read n characters from uart and return */
static int uart_read_n (int num_char) {

	struct udevice *additional_uart = gd->add_serial_dev;
        int ret, count = 0;
	do {
		ret = serial_dev_getc(additional_uart);
		printf ("%c", ret);
		if (ctrlc()) {
			break;
		}
	num_char--;
	count++;
	} while (num_char > 0);
	
	return count;
}

/* Check if character is waiting to be read in RX FIFO */
static int uart_pending (void) 
{
	struct udevice *additional_uart = gd->add_serial_dev;
	return serial_dev_tstc(additional_uart);
}

/* Flush uart RX FIFO */
static int uart_flush (char **buff) {

	int size = 0;
        char *s = NULL;
	struct udevice *additional_uart = gd->add_serial_dev;
        while (uart_pending ()) {
                if (s == NULL) {
                        s = (char *) malloc(sizeof(char));
                } else {
                        s = (char *) realloc(s, (size + 1) * sizeof(char));
                }
                s[size] = (char)serial_dev_getc(additional_uart);
                size ++;
        }
        *buff = s;
        return size;

	return 0;
}

/* Init Uart */
static int uart_init_dev (void) 
{
	if (gd->add_serial_dev != NULL ) {
		if(serial_dev_getname(gd->add_serial_dev) != NULL ) {
			printf("serial = %s\n",serial_dev_getname(gd->add_serial_dev));
			baudrate = uart_get_baudrate();
			is_initialized = 1;
		} else {
			printf("no serial associated with uart command: please add additional-uart option to dts\n");
			is_initialized = 0;
		}
	} else {
		printf("no serial associated with uart command: please add additional-uart option to dts\n");
                is_initialized = 0;
	}

	return is_initialized;
}

static int do_uart ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[] ) {
	int cmd_done = 0;
	struct udevice *additional_uart = gd->add_serial_dev;
	char *buff = NULL;
	int retval = 0;
	int i = 0;
	if (argc > 1) {
		if (strcmp(argv[1], "init") == 0 && argc == 2) {
			if (uart_init_dev ()) 
				printf ("init done\n");
			cmd_done = 1;
		}
		/* Check if serial is initialized */
		if(!is_initialized) {
			printf("uart command must be initialized with:\nuart init\n");	
			cmd_done = 1;
			return 0;
		}

		if (strcmp(argv[1], "write") == 0 && argc == 3) {
			uart_write (argv[2]);
			cmd_done = 1;
		}
		if (strcmp(argv[1], "read") == 0 && argc == 2) {
			uart_read ();
			cmd_done = 1;
		}
		if (strcmp(argv[1], "flush") == 0 && argc == 2) {
			retval = uart_flush (&buff);
			if (retval > 0) {
			printf ("Characters flushed: ");
				for (i = 0 ; i < retval ; i++) 
					printf ("%c", buff[i]);
			}
			printf ("\nTot: %d\n", retval);
			cmd_done = 1;
		}
		if (strcmp(argv[1], "readn") == 0 && argc == 3) {
			retval = uart_read_n (simple_strtoul(argv[2], NULL, 10));
			if (retval > 0)
				printf("\n"); 
			cmd_done = 1;
		}
		if (strcmp(argv[1], "set") == 0 && argc == 4) {
			if (strcmp(argv[2], "baudrate") == 0) {
				baudrate = simple_strtoul(argv[3], NULL, 10);
				if (baudrate == 115200 || 
					baudrate == 57600 || 
					baudrate == 38400 || 
					baudrate == 19200 || 
					baudrate == 9600) {
					serial_dev_setbrg (baudrate, additional_uart); 
					cmd_done = 1;
				} else {
					printf ("Invalid BaudRate!\n");
				}
			}
		cmd_done = 1;
		}
		if (strcmp(argv[1], "get") == 0 && argc == 3) {
			if (strcmp(argv[2], "baudrate") == 0) {
				printf ("BaudRate: %d\n", baudrate);
				cmd_done = 1;
			}
		}			
	}
	if (!cmd_done) {
		cmd_usage(cmdtp);
	}
	return 0;
}


U_BOOT_CMD(
	uart, 4, 1, do_uart,
	"additional uart commands",
	"                    - prints this help\n"
	"uart [init]             - inits the uart controller\n"
	"uart [get] [obj]        - gets status of the specific object. Possibily object is:\n"
	"           baudrate     - gets actual baudrate\n"
	"uart [set] [obj] <val>  - sets status of the specific object. Possibily object is:\n"
	"           baudrate     - sets baudrate with follow possible values: 115200, 9600, 38400\n"
	"uart [write] <string>   - sends a word without space\n"
	"uart [read]		 - reads indefinitely\n"
	"uart [flush]            - flushes RX buffer end prompts the content to console\n"
	"uart [readn] <num>      - reads max num characters\n"
);
