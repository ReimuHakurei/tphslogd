// TP-Link HS-Series Smart Plug Client Library (libtphs)
// NullCBC and Header Functions
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include "../common.h"

// Function: nullcbc_encode
// Paramaters: char *iBuffer (input data)
//             char *oBuffer (output data)
//             size_t iMaxBytes (unused)
//             size_t iDataSize (size of input data)
size_t nullcbc_encode(char *iBuffer, char *oBuffer, size_t iMaxBytes, size_t iDataSize) {
	int bShift;

	unsigned int  fSize = iDataSize + 1;
	unsigned char fInit = 0xD0;

	bShift = fInit;

	// Check endianness of host system. The device uses big-endian.
	volatile uint32_t EndianTest=0x01234567;
	if ((*((uint8_t*)(&EndianTest))) == 0x67) {
		fSize = SWAP_UINT32(fSize);
	}

	*((int *)&oBuffer[0]) = fSize;
	oBuffer[4] = fInit;

	for (int i = 0; i < iDataSize; i++) {
		oBuffer[i + 5] = bShift ^ iBuffer[i];
		bShift = oBuffer[i + 5];
	}

	return iDataSize + 5;
}

size_t nullcbc_decode(char *iBuffer, char *oBuffer, size_t iMaxBytes, size_t iDataSize) {
	int bShift;

	// fSize is the size of the input buffer; dSize is the size it should be.
	unsigned int  fSize = iDataSize - 4;
	unsigned int  dSize = *((int *)&iBuffer[0]);
	unsigned char fInit = 0xD0;
  
	bShift = fInit;

	// Check endianness of host system. The device uses big-endian.
	volatile uint32_t EndianTest=0x01234567;
	if ((*((uint8_t*)(&EndianTest))) == 0x67) {
		dSize = SWAP_UINT32(dSize);
	}

	for (int i = 0; i < iDataSize; i++) {
		oBuffer[i] = bShift ^ iBuffer[i + 5];
		bShift = iBuffer[i + 5];
	}

	return iDataSize - 5;
}