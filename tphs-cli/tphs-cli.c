// TP-Link HS-Series Smart Plug CLI Client (tphs-cli)
// Testing client
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// Include libtphs header
#include "../libtphs/common.h"
#include "tphs-cli.h"

int main() {
	FILE *Input;
	FILE *Output;

	if ((Input  = fopen("Request.txt" , "r")) && (Output = fopen("Response.txt", "w"))) {
		int inFileSize;

		fseek(Input, 0L, SEEK_END);
		inFileSize = ftell(Input);
		rewind(Input);

		// Allocate a buffer for the Input file.
		char *iBuffer = (char *) malloc((sizeof(char) * inFileSize) + 1);

		// Allocate a buffer for the Output; the output
		//     also contains a 5-byte header, and as such
		//     should be 5 bytes larger.
		char *oBuffer = (char *) malloc((sizeof(char) * inFileSize) + 6);


		size_t iBytes;
		size_t oBytes;

		if (iBytes = fread(iBuffer, 1, inFileSize, Input)) {
			oBytes = nullcbc_encode(iBuffer, oBuffer, iBytes);

			int Socket;
			struct sockaddr_in Device;

			Socket = socket(AF_INET, SOCK_STREAM, 0);
			if (Socket < 0) fatal_socket_error("Failed to open socket.");

			memset((char *) &Device, 0, sizeof(Device));
			Device.sin_family = AF_INET;

			// TODO: Make this read the IP (and optionally, port) from the args.
			Device.sin_addr.s_addr = inet_addr("192.168.1.45");
			Device.sin_port = htons(9999);

			if (connect(Socket,(struct sockaddr *) &Device, sizeof(Device)) < 0) fatal_socket_error("Failed to connect to device.");
			if (write(Socket, oBuffer, oBytes) < 0) fatal_socket_error("Failed to write to socket.");

			// Now that everything has been written, we can free (and re-use) the iBuffer/oBuffers.
			free(iBuffer);
			free(oBuffer);

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
			if (iBytes != 4) fatal_socket_error("Empty response from device. Malformed request?");

			// It's probably in the wrong byte order. Check it, then swap it!
			tcpResponseSize = check_swap_byte_order(tcpResponseSize);

			// Allocate a buffer. We need to put these four bytes back in, so we
			//     make the buffer large enough for them.
			iBuffer = (char *) malloc((sizeof(char) * tcpResponseSize) + 5);

			// Normally, we would need to malloc one extra char for a null
			//     terminating character, but in this case, there is still
			//     one more header byte yet to be received, so we can just
			//     allocate the size of tcpResponseSize.
			oBuffer = (char *) malloc((sizeof(char) * tcpResponseSize));

			// Zero our shiny new buffers...
			memset(iBuffer, 0, tcpResponseSize + 5);
			memset(oBuffer, 0, tcpResponseSize);

			strcpy(iBuffer, (char *) &tcpResponseSize);

			// Read from the socket. If we get a different size response,
			//     panic and throw an error.
			iBytes = read(Socket, (char *) &iBuffer[4], tcpResponseSize);

			if (iBytes != tcpResponseSize) {
				char errorBuffer[512];
				sprintf(errorBuffer, "Incomplete response received.\nExpected: %i bytes\nReceived: %i bytes", tcpResponseSize, iBytes);
				fatal_socket_error(errorBuffer);
			}

			oBytes = nullcbc_decode(iBuffer, oBuffer, iBytes + 4);
			free(iBuffer);

			fwrite(oBuffer,1,oBytes,Output);
		} else {
			printf("Failed to read request data.\n");
		}
	} else {
		printf("Failed to open input or output file.\n");
	}

	return 0;
}

void fatal_socket_error(char *Error) {
	// Throw an error and exit upon a socket error.

	printf("\n\nFATAL ERROR: A fatal network error occurred. Operation cannot continue.\nThe following additional information is availiable:\n%s\n\n", Error);
	exit(1);
}