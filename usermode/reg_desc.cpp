#include "driver.h"

const char* register_descriptions[REGISTER_FILE_LEN] =
{
	"Command to CERF",
	"Status",
	"Interrupt Status",
	"Interrupt Mask, same as ints register",
	"Error Status",
	"Mode",
	"Link Buffer Select",
	"Link Buffer Access Pointer",
	"Link Buffer Data Port",
	"Universal Buffer Access Pointer",
	"Universal Buffer Data Port",
	"Transmit Message Buffer Control",
	"Transmit Message Buffer Data Port",
	"Receive Message Buffer Control",
	"Receive Message Buffer Data Port",
	"Message Conter"
};

uint16_t status_bits[KNOWN_STATUS_BITS] =
{
	0x0040, //YN_STS_LTX_UA 
	0x0080, //YN_STS_LTX_UB
	0x2000, //YN_STS_RDY 
	0x4000, //YN_STS_ERRL 
	0x8000 //YN_STS_ERR 
};

const char* status_descriptions[KNOWN_STATUS_BITS] =
{
	"LTX_UA",
	"LTX_UB",
	"RDY",
	"ERRL",
	"ERR"
};