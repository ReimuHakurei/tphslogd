// TP-Link HS-Series Smart Plug CLI Client (tphs-cli)
// Testing client
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

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
		char *iBuffer = (char *) calloc(inFileSize + 1, sizeof(char));
		char *oBuffer = NULL;

		size_t iBytes;
		size_t oBytes;

		if (iBytes = fread(iBuffer, 1, inFileSize, Input)) {

			oBytes = json_request(iBytes, iBuffer, &oBuffer);

			if (oBuffer != NULL) {
				fwrite(oBuffer, oBytes, 1, Output);
			}


			while(true) {
				device_response *Response = calloc(sizeof(device_response),1);

				decode_json(oBuffer, Response);

				free_device_response(Response);
			}

			//printf("%F volts, %F amps, %F watts, %F kWh used to date.\n",Response->Realtime.Volts, Response->Realtime.Amps, Response->Realtime.Watts, Response->Realtime.kWh_To_Date);
		} else {
			printf("Failed to read request data.\n");
		}
	} else {
		printf("Failed to open input or output file.\n");
	}

	return 0;
}