// TP-Link HS-Series Smart Plug CLI Client (tphs-cli)
// Testing client
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

// Include libtphs header
#include "../libtphs/common.h"
#include "tphs-cli.h"

int main() {
	// TODO: Make this program take arguments and stuff.
	
	//FILE *Input;
	//FILE *Output;

	if (true) {
	//if ((Input  = fopen("Request.txt" , "r")) && (Output = fopen("Response.txt", "w"))) {
	//	int inFileSize;

	//	fseek(Input, 0L, SEEK_END);
	//	inFileSize = ftell(Input);
	//	rewind(Input);

		// Allocate a buffer for the Input file.
		//char *iBuffer = (char *) calloc(inFileSize + 1, sizeof(char));
		char *oBuffer = NULL;

		size_t iBytes;
		size_t oBytes;

		//if (iBytes = fread(iBuffer, 1, inFileSize, Input)) {
		if (true) {
			char iBufferTxt[] = "{\"system\":{\"get_sysinfo\":null},\"emeter\":{\"get_realtime\":null}}";
			iBytes = strlen(iBufferTxt);
			char *iBuffer = malloc(iBytes);
			strcpy(iBuffer,iBufferTxt);

			char *deviceAddress = malloc(sizeof("192.168.1.69") + 1);
			strcpy(deviceAddress,"192.168.1.69");

			oBytes = json_request(iBytes, iBuffer, &oBuffer, deviceAddress);

			//if (oBuffer != NULL) {
			//	fwrite(oBuffer, oBytes, 1, Output);
			//}

			device_response *Response = calloc(sizeof(device_response),1);
			decode_json(oBuffer, Response);

			printf("Power information:\n\t%F volts\n\t%F amps\n\t%F watts\n\nYou have used %F kWh through this meter to date.\n",Response->Volts, Response->Amps, Response->Watts, Response->kWh_To_Date);

			free_device_response(Response);
		} else {
			printf("Failed to read request data.\n");
		}
	} else {
		printf("Failed to open input or output file.\n");
	}

	return 0;
}
