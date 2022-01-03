/*****
*	Userspace counterpart for experimental kernel module for Yaskawa JASP-NT001
*	ISA <-> Mechatrolink interface card.
* 
*	Usage: try to run without arguments.
*	
* 
***/

#pragma once

#include <stdio.h>
#include <stdint.h>

enum my_exit_codes
{
	EXIT_CODE_OK = 0,
	EXIT_CODE_NO_ARGS,
	EXIT_CODE_INVALID_ARGS,
	EXIT_CODE_INACCESSIBLE_DEV,
	EXIT_CODE_CLOSE_FAILED,
	EXIT_CODE_IOCTL_FAILED,
	EXIT_CODE_READ_FAILED,
};