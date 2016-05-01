// TP-Link HS-Series Smart Plug Client Library (libtphs)
// NullCBC and Header Functions
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include "../common.h"
#include "network.h"

// Function: nullcbc_encode
// Paramaters:	char *iBuffer (input data)
//				char *oBuffer (output data)
//					Note: Must be five (5) chars longer!
//				size_t iDataSize (size of input data)
size_t nullcbc_encode(char *iBuffer, char *oBuffer, size_t iDataSize) {
	int bShift;

	unsigned int  fSize = iDataSize + 1;
	unsigned char fInit = 0xD0;

	bShift = fInit;

	fSize = htonl(fSize);

	*((int *)&oBuffer[0]) = fSize;
	oBuffer[4] = fInit;

	for (int i = 0; i < iDataSize; i++) {
		oBuffer[i + 5] = bShift ^ iBuffer[i];
		bShift = oBuffer[i + 5];
	}

	return iDataSize + 5;
}

// Function: nullcbc_encode
// Paramaters:	char *iBuffer (input data)
//				char *oBuffer (output data)
//				size_t iDataSize (size of input data)
size_t nullcbc_decode(char *iBuffer, char *oBuffer, size_t iDataSize) {
	int bShift;

	// fSize is the size of the input buffer; dSize is the size it should be.
	unsigned int  fSize = iDataSize - 5;
	unsigned int  dSize = *((int *)&iBuffer[0]);
	unsigned char fInit = 0xD0;

	dSize = ntohl(dSize);

	if (fSize == (dSize - 1)) {
		bShift = fInit;

		for (int i = 0; i < fSize; i++) {
			oBuffer[i] = bShift ^ iBuffer[i + 5];
			bShift = iBuffer[i + 5];
		}

		return fSize;
	}

	return 0;
}