#pragma once

#include <sys/ioctl.h>

#define MY_MAGIC_NUMBER 0x8F
#define MY_IOCTL_START 0xD0

enum my_ioctls
{
	MY_IOCTL_RESET = MY_IOCTL_START,
	MY_IOCTL_GET_STATUS
};

#define IOCTL_RESET _IO(MY_MAGIC_NUMBER, MY_IOCTL_RESET)
#define IOCTL_GET_STATUS _IOR(MY_MAGIC_NUMBER, MY_IOCTL_GET_STATUS, uint16_t)

#define REGISTER_FILE_LEN 16 //words

extern const char* register_descriptions[REGISTER_FILE_LEN];