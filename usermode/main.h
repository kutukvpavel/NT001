#pragma once

#include <stdio.h>
#include <stdint.h>

enum my_exit_codes
{
	EXIT_CODE_NO_ARGS = -255,
	EXIT_CODE_INVALID_ARGS,
	EXIT_CODE_INACCESSIBLE_DEV,
	EXIT_CODE_IOCTL_FAILED,
	EXIT_CODE_OK = 0
};