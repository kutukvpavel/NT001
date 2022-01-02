#include "main.h"
#include "driver.h"

#include <fcntl.h>
#include <unistd.h>

int dump(int fd);

int main(int argc, char** argv)
{
	if (argc <= 0)
	{
		printf("Usage:\n"
		"r[eset] - Reset the card;\n"
		"s[tatus] - Query status register\n"
		"d[ump] - Dump the whole register file\n");
		return EXIT_CODE_NO_ARGS;
	}
	int fd = open("/dev/nt001", O_RDWR);
	if (fd < 0) return EXIT_CODE_INACCESSIBLE_DEV;
	switch (argv[0][0])
	{
	case 'r':
		if (ioctl(fd, IOCTL_RESET) < 0) return EXIT_CODE_IOCTL_FAILED;
		printf("RESET successful.");
		break;
	case 's':
	{
		uint16_t resp = 0;
		if (ioctl(fd, IOCTL_GET_STATUS, &resp) < 0) return EXIT_CODE_IOCTL_FAILED;
		printf("STATUS = 0x%4X", resp);
		break;
	}
	case 'd':
		return dump(fd);
	default:
		return EXIT_CODE_INVALID_ARGS;
	}
	return EXIT_CODE_OK;
}

int dump(int fd)
{
	uint16_t buf[REGISTER_FILE_LEN];
	if (read(fd, buf, REGISTER_FILE_LEN) < 0) return EXIT_CODE_READ_FAILED;
	for (int i = 0; i < REGISTER_FILE_LEN; i++)
	{
		printf("%s = %4X", register_descriptions[i], buf[i]);
	}
	return EXIT_CODE_OK;
}