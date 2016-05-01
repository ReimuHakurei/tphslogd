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

int main() {
	FILE *Input;
	FILE *Output;

	if ((Input  = fopen("Request.txt" , "r")) && (Output = fopen("Response.txt", "w"))) {
		char iBuffer[BLOCK_SIZE];
		char oBuffer[BLOCK_SIZE];

		size_t iBytes;
		size_t oBytes;

		if (iBytes = fread(iBuffer, 1, BLOCK_SIZE, Input)) {
			oBytes = nullcbc_encode(iBuffer, oBuffer, BLOCK_SIZE, iBytes);
			
			int Socket;
			struct sockaddr_in Device;
			
			Socket = socket(AF_INET, SOCK_STREAM, 0);
			if (Socket < 0) {
				printf("Failed to open socket. Exiting...\n");
				return -1;
			}
			
			memset((char *) &Device, 0, sizeof(Device));
			Device.sin_family = AF_INET;
			Device.sin_addr.s_addr = inet_addr("192.168.1.45");
			Device.sin_port = htons(9999);
			
			if (connect(Socket,(struct sockaddr *) &Device, sizeof(Device)) < 0) {
				printf("Failed to connect to device.\n");
				return -2;
			}
			
			if (write(Socket,oBuffer,oBytes) < 0) {
				printf("Failed to write to socket.\n");
				return -3;
			}
			
			memset(iBuffer,0,BLOCK_SIZE);
			
			iBytes = read(Socket,iBuffer,BLOCK_SIZE);
			if (iBytes < 0) {
				printf("Error reading from socket.\n");
				return -4;
			}
			
			oBytes = nullcbc_decode(iBuffer, oBuffer, BLOCK_SIZE, iBytes);
			
			fwrite(oBuffer,1,oBytes,Output);		
		} else {
			printf("Failed to read data.\n");
		}
	} else {
		printf("Failed to open input or output file.\n");
	}

	return 0;
}
