#include "driver.h"

const char* register_descriptions[REGISTER_FILE_LEN] =
{
	" Command to CERF ",
	" Status ",
	" Interrupt Status ",
	" Interrupt Mask, same as ints register ",
	" Error Status ",
	" Mode ",
	" Link Buffer Select ",
	" Link Buffer Access Pointer ",
	" Link Buffer Data Port ",
	" Universal Buffer Access Pointer ",
	" Universal Buffer Data Port ",
	" Transmit Message Buffer Control ",
	" Transmit Message Buffer Data Port ",
	" Receive Message Buffer Control ",
	" Receive Message Buffer Data Port ",
	" Message Conter "
};