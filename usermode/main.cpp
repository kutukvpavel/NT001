#include "main.h"
#include "driver.h"

#include <fcntl.h>
#include <unistd.h>

int dump(int fd);
int main_helper(int argc, char** argv);
char* printb(uint16_t data, char* buf);
void print_status_info(uint16_t resp);

int main(int argc, char** argv)
{
	int ret = main_helper(argc, argv);
	printf("Exit code: %i.\n", ret);
	return ret;
}

int main_helper(int argc, char** argv)
{
	printf("Usermode part started: found %i args.\n", argc - 1);
	if (argc < 2)
	{
		printf("JASP-NT001 Test. Usage:\n"
			"	r[eset] - Reset the card;\n"
			"	s[tatus] - Query status register\n"
			"	d[ump] - Dump the whole register file\n"
			"	o[pen] - Just hold the deivce open");
		return EXIT_CODE_NO_ARGS;
	}
	int fd = open(MY_DEVICE_PATH, O_RDONLY);
	printf("Got device descriptor = %i\n", fd);
	if (fd < 0) return EXIT_CODE_INACCESSIBLE_DEV;
	switch (argv[1][0])
	{
	case 'o':
		printf("Press Enter to close the device...");
		getchar();
		break;
	case 'r':
		if (ioctl(fd, IOCTL_RESET) < 0) return EXIT_CODE_IOCTL_FAILED;
		printf("RESET successful.\n");
		break;
	case 's':
	{
		uint16_t resp = 0;
		if (ioctl(fd, IOCTL_GET_STATUS, &resp) < 0) return EXIT_CODE_IOCTL_FAILED;
		print_status_info(resp);
		break;
	}
	case 'd':
		return dump(fd);
	default:
		return EXIT_CODE_INVALID_ARGS;
	}
	if (close(fd) < 0) return EXIT_CODE_CLOSE_FAILED;
	return EXIT_CODE_OK;
}

int dump(int fd)
{
	uint16_t buf[REGISTER_FILE_LEN];
	if (read(fd, buf, REGISTER_FILE_LEN) < 0) return EXIT_CODE_READ_FAILED;
	for (int i = 0; i < REGISTER_FILE_LEN; i++)
	{
		printf("%s = 0x%4X\n", register_descriptions[i], buf[i]);
	}
	return EXIT_CODE_OK;
}

void print_status_info(uint16_t resp)
{
	char buf[17];
	uint16_t unknown = resp;
	printf("STATUS = 0x%4X = %s\n", resp, printb(resp, buf));
	printf("Active KNOWN bits: ");
	for (int i = 0; i < KNOWN_STATUS_BITS; i++)
	{
		if (resp & status_bits[i])
		{
			printf("%s, ", status_descriptions[i]);
			unknown &= ~status_bits[i];
		}
	}
	fflush(stdout);
	printf("\b\b  \b\b.\n");
	printf("Active UNKNOWN bits: %s\n", printb(unknown, buf));
}

char* printb(uint16_t data, char* buf)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		buf[i] = (data & (1 << (15 - i))) ? '1' : '0';
	}
	buf[i] = '\0';
	return buf;
}