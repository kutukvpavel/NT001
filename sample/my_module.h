/****
*	Experimental kenrel module for JASP-NT001 ISA-Mechatrolink card
*	Based on Yaskawa MELDEMO sources for DOS
*	Remaps 16 16-bit registers of the card into virtual memory to be accessed
*	Exposes IOCTLs (currently only: RESET/STATUS)
* 
*	Character device.
*	READ: reads from the register file (RAW, string manipulation left to be done in the userspace counterpart)
*	WRITE: NOT SUPPORTED
* 
****/

#pragma once

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "NT001"
#define ISA_CARD_ADDR 0xDA000
#define ISA_SEGMENT_LEN 0x1000
#define MY_MAGIC_NUMBER 0x8F
#define MY_IOCTL_START 0xD0

enum my_ioctls
{
	MY_IOCTL_RESET = MY_IOCTL_START,
	MY_IOCTL_GET_STATUS
};

#define IOCTL_RESET _IO(MY_MAGIC_NUMBER, MY_IOCTL_RESET)
#define IOCTL_GET_STATUS _IOR(MY_MAGIC_NUMBER, MY_IOCTL_GET_STATUS, uint16_t)

enum yn_cmds
{
	YN_CMD_C1MTX_R		= 0x0001 ,
	YN_CMD_C2MTX_R		= 0x0002 ,
	YN_CMD_PAR_R		= 0x0004 ,
	YN_CMD_C1MRX_R		= 0x0008 ,
	YN_CMD_C2MRX_R		= 0x0010 ,
	YN_CMD_WD_R			= 0x0020 ,
	YN_CMD_LTX_RA		= 0x0040 ,
	YN_CMD_LTX_RB		= 0x0080 ,
	YN_CMD_ENA_CER		= 0x0100 ,
	YN_CMD_P_RSTRQ		= 0x0400 ,
	YN_CMD_C1ABT_R		= 0x0800 ,
	YN_CMD_C2ABT_R		= 0x1000 ,
	YN_CMD_RST_CER		= 0x8000 
};

typedef struct _YENET_HOST_IF_REGS {
	volatile uint16_t cmd;	/* Command to CERF */
	volatile uint16_t sts;	/* Status */
	volatile uint16_t ints;	/* Interrupt Status */
	volatile uint16_t imk;	/* Interrupt Mask, same as ints register */
	volatile uint16_t ests;	/* Error Status */
	volatile uint16_t mod;	/* Mode */
	volatile uint16_t lbs;	/* Link Buffer Select */
	volatile uint16_t lbp;	/* Link Buffer Access Pointer */
	volatile uint16_t lbd;	/* Link Buffer Data Port */
	volatile uint16_t ubp;	/* Universal Buffer Access Pointer */
	volatile uint16_t ubd;	/* Universal Buffer Data Port */
	volatile uint16_t tmbc;	/* Transmit Message Buffer Control */
	volatile uint16_t tmbd;	/* Transmit Message Buffer Data Port */
	volatile uint16_t rmbc;	/* Receive Message Buffer Control */
	volatile uint16_t rmbd;	/* Receive Message Buffer Data Port */
	volatile uint16_t mctr;	/* Message Conter */
} YENET_HOST_IF_REGS;

#define ynb_get_cmd(pJL)          readw(&(pJL->cmd))
#define ynb_set_cmd(pJL, data)    writew(data, &(pJL->cmd))
#define ynb_get_sts(pJL)          readw(&(pJL->sts))
#define ynb_get_ints(pJL)         readw(&(pJL->ints))
#define ynb_clear_ints(pJL, data) writew(data, &(pJL->ints))
#define ynb_set_imk(pJL, data)    writew(data, &(pJL->imk))
#define ynb_get_ests(pJL)         readw(&(pJL->ests))
#define ynb_clear_ests(pJL, data) writew(data, &(pJL->ests))

#define ynb_get_mod(pJL)          readw(&(pJL->mod))
#define ynb_set_mod(pJL, data)    writew(data, &(pJL->mod))
#define ynb_get_lbs(pJL)          readw(&(pJL->lbs))
#define ynb_set_lbs(pJL, data)    writew(data, &(pJL->lbs))
#define ynb_get_lbp(pJL)          readw(&(pJL->lbp))
#define ynb_set_lbp(pJL, data)    writew(data, &(pJL->lbp))
#define ynb_get_lbd(pJL)          readw(&(pJL->lbd))
#define ynb_set_lbd(pJL, data)    writew(data, &(pJL->lbd))
#define ynb_get_ubp(pJL)          readw(&(pJL->ubp))
#define ynb_set_ubp(pJL, data)    writew(data, &(pJL->ubp))
#define ynb_get_ubd(pJL)          readw(&(pJL->ubd))
#define ynb_set_ubd(pJL, data)    writew(data, &(pJL->ubd))
#define ynb_get_tmbc(pJL)         readw(&(pJL->tmbc))
#define ynb_set_tmbc(pJL, data)   writew(data, &(pJL->tmbc))
#define ynb_get_tmbd(pJL)         readw(&(pJL->tmbd))
#define ynb_set_tmbd(pJL, data)   writew(data, &(pJL->tmbd))
#define ynb_get_rmbc(pJL)         readw(&(pJL->rmbc))
#define ynb_set_rmbc(pJL, data)   writew(data, &(pJL->rmbc))
#define ynb_get_rmbd(pJL)         readw(&(pJL->rmbd))
#define ynb_set_pars(pJL, data)   writew(data, &(pJL->pars))
#define ynb_get_mctr(pJL)         readw(&(pJL->mctr))
#define ynb_set_lbss(pJL, data)   writew(data, &(pJL->lbss))