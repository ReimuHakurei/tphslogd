// TP-Link HS-Series Smart Plug Client Library (libtphs)
// Common network-related functions.
//
// File path: libtphs/network/common.c
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include "../common.h"
#include "network.h"

// This function sends a JSON object (iBuffer) to the device, and
//     returns a JSON object to oBuffer.
// On success, it returns the size of oBuffer. On failure, it returns 0.
size_t json_request(size_t iSize, char *iBuffer, char **oBuffer) {
	size_t iBytes;
	size_t oBytes;

	memmove(iBuffer, iBuffer + 1, iSize - 1);
	iSize--;

	*oBuffer = (char *) calloc(iSize + 4, sizeof(char));

	oBytes = nullcbc_encode(iBuffer, *oBuffer, iSize);

	// Free iBuffer and reset it to NULL.
	free(iBuffer);
	iBuffer = NULL;

	int Socket;
	struct sockaddr_in Device;

	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket < 0) {
		fatal_socket_error("Failed to open socket.");
		return 0;
	}

	memset((char *) &Device, 0, sizeof(Device));
	Device.sin_family = AF_INET;

	// TODO: Make this read the IP (and optionally, port) from the args.
	Device.sin_addr.s_addr = inet_addr("192.168.1.45");
	Device.sin_port = htons(9999);

	if (connect(Socket,(struct sockaddr *) &Device, sizeof(Device)) < 0) {
		fatal_socket_error("Failed to connect to device.");
		return 0;
	}

	if (write(Socket, *oBuffer, oBytes) < 0) {
		fatal_socket_error("Failed to write to socket.");
		return 0;
	}

	// Now that everything has been written, we can free (and re-use) the iBuffer/oBuffers.
	free(*oBuffer);
	*oBuffer = NULL;

	// We need to loop through the socket, and read in the response
	//     to a buffer. We don't know how big the response will be yet,
	//     so we start by reading the first four bytes of the response.
	//
	// The first four bytes contain the size of the remainder of the
	//     response, in network byte order, so we may need to swap the
	//     byte order, but then we can allocate memory and read the
	//     rest.
	//
	// The decoded response will lack this 5-byte header (4-byte int,
	//     1-byte IV) so we can allocate that to be 5 bytes smaller.

	// Allocate a buffer to store those four bytes, then zero it.
	int tcpResponseSize = 0;
	iBytes = read(Socket, (char *) &tcpResponseSize, 4);

	// If the device doesn't send us anything, it's probably a malformed request.
	if (iBytes != 4) {
		fatal_socket_error("Empty response from device. Malformed request?");
		return 0;
	}

	// Allocate a buffer. We need to put these four bytes back in, so we
	//     make the buffer large enough for them.
	iBuffer = (char *) calloc(ntohl(tcpResponseSize) + 5, sizeof(char));

	// Normally, we would need to malloc one extra char for a null
	//     terminating character, but in this case, there is still
	//     one more header byte yet to be received, so we can just
	//     allocate the size of tcpResponseSize.
	*oBuffer = (char *) calloc(ntohl(tcpResponseSize), sizeof(char));

	memcpy(iBuffer, (char *) &tcpResponseSize, 4);

	// Read from the socket. If we get a different size response,
	//     panic and throw an error.
	iBytes = read(Socket, (char *) &iBuffer[4], ntohl(tcpResponseSize));

	if (iBytes != ntohl(tcpResponseSize)) {
		char errorBuffer[512];
		sprintf(errorBuffer, "Incomplete response received.\nExpected: %u bytes\nReceived: %u bytes", ntohl(tcpResponseSize), iBytes);
		fatal_socket_error(errorBuffer);
		return 0;
	}

	oBytes = nullcbc_decode(iBuffer, *oBuffer, ntohl(tcpResponseSize) + 4);

	// This device outputs malformed JSON, missing the opening brace.
	// We add it in before returning.
	*oBuffer = realloc(*oBuffer, oBytes + 1);

	if (*oBuffer != NULL) {
		memmove(*oBuffer + 1, *oBuffer, oBytes);
		*oBuffer[0] = '{';
	} else return 0;

	free(iBuffer);

	return oBytes + 1;
}

void fatal_socket_error(char *Error) {
	// Throw an error and exit upon a socket error.

	printf("\nWARNING: A network error occurred. The operation has been canceled.\nThe following additional information is availiable:\n%s\n\n", Error);
}