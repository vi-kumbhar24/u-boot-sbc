#include <common.h>
#include <command.h>
#include <environment.h> 
#include <watchdog.h>

#include <seco_env.h>

DECLARE_GLOBAL_DATA_PTR;


int atoi (char *string)
{
	int length;
	int retval = 0;
	int i;
	int sign = 1;

	length = strlen(string);
	for (i = 0; i < length; i++) {
		if (0 == i && string[0] == '-') {
			sign = -1;
			continue;
		}
		if (string[i] > '9' || string[i] < '0') {
			break;
		}
		retval *= 10;
		retval += string[i] - '0';
	}
	retval *= sign;
	return retval;
}

int ctoi (char ch) {
	int retval = 0;
	if (ch <= '9' && ch >= '0') {
		retval = ch - '0';
	}
	return retval;
}

char *getline (void) {
	static char buffer[100];
	char c;
	size_t i;

	i = 0;
	while (1) {
		buffer[i] = '\0';
		while (!tstc()){
			WATCHDOG_RESET();
			continue;
		}

		c = getc();
		/* Convert to uppercase */
		//if (c >= 'a' && c <= 'z')
		//	c -= ('a' - 'A');

		switch (c) {
			case '\r':	/* Enter/Return key */
			case '\n':
				puts("\n");
				return buffer;

			case 0x03:	/* ^C - break */
				return NULL;

			case 0x08:	/* ^H  - backspace */
			case 0x7F:	/* DEL - backspace */
				if (i) {
					puts("\b \b");
					i--;
				}
				break;

			default:
				/* Ignore control characters */
				if (c < 0x20)
					break;
				/* Queue up all other characters */
				buffer[i++] = c;
				printf("%c", c);
				break;
		}
	}
}


