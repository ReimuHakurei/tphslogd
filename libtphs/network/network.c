// TP-Link HS-Series Smart Plug Client Library (libtphs)
// Common network-related functions.
//
// File path: libtphs/network/common.c
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include "../common.h"
#include "network.h"

int check_swap_byte_order(int Input) {
	volatile uint32_t EndianTest=0x01234567;
	if ((*((uint8_t*)(&EndianTest))) == 0x67) {
		return SWAP_UINT32(Input);
	} else {
		return Input;
	}
}