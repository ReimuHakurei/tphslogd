// TP-Link HS-Series Smart Plug CLI Client (tphs-cli)
// Testing client
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

// Include libtphs header
#include "../libtphs/common.h"
#include "tphslogd.h"

#define SW_VERSION "WIP"
#define DATA_PATH "/var/tphslogd/"
#define DBASEPATH "/var/tphslogd/database.sqlite3"

#define SYSINFO_QUERY "{\"system\":{\"get_sysinfo\":null}}"
#define REALTIME_QUERY "{\"emeter\":{\"get_realtime\":null}}"

sqlite3 *dBase;

int main(int argc, char *argv[]) {
	printf("\nTP-Link HS-Series Smart Plug Power Consumption Logger %s\n(C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.\nThis software is licensed under the terms of the MIT License.\n\n", SW_VERSION);

	if (argc == 1) {
		printf("This program requires a space-delimited list of IP addresses for HS-Series Smart Plugs as an argument.\n");

		return 1;
	}

	DIR *Path = opendir(DATA_PATH);
	if (!Path) {
		printf("Data directory does not exist, creating... ");
		if (mkdir(DATA_PATH, 000777)) {
			printf("ERROR!\n Failed to create data directory.\nPlease create %s and make sure it is world writable.\n", DATA_PATH);

			return 1;
		}
		printf("OK\n");
	}

	char *zErrMsg = 0;

	printf("Opening database... ");

	if (sqlite3_open(DBASEPATH, &dBase) == SQLITE_OK) {
		printf("OK\n\n");
	} else {
		printf("ERROR!\nAn error occurred: %s\n", sqlite3_errmsg(dBase));

		return 1;
	}

	printf("Spawning threads...\n\n");

	// Create an array of threads based on the size of argc.
	pthread_t Threads[argc - 1];

	// Set SQLite3 to asynchronous mode.
	sqlite3_exec(dBase,"PRAGMA synchronous = 0;",0,0,0);

	for (int i = 1; i < argc; i++) {
		// We won't worry about memory leaks here; we are allocating a fixed
		//     amount of memory and it will remain in use until the program
		//     exits anyway.
		char *Device = malloc(sizeof(argv[i]) + 1);

		strcpy(Device,argv[i]);

		pthread_create(&Threads[i - 1], NULL, device_thread, (void *) Device);
	}

	for (int i = 1; i < argc; i++) {
		pthread_join(Threads[i - 1], NULL);
	}

	return 0;
}

void *device_thread(void *Device) {
	char *RequestString = NULL;
	char *ResponseString = NULL;
	size_t RequestBytes, ResponseBytes;
	char *zErrMsg = 0;

	device_response *SystemInfo = calloc(sizeof(device_response),1);

	while (true) {
		RequestString = malloc(strlen(SYSINFO_QUERY) + 1);
		RequestBytes  = strlen(SYSINFO_QUERY);
		strcpy(RequestString, SYSINFO_QUERY);



		// json_request frees RequestString for us so we don't need to.
		ResponseBytes = json_request(RequestBytes, RequestString, &ResponseString, Device);

		if (ResponseBytes) {
			decode_json(ResponseString, SystemInfo);

			if (SystemInfo->Device_ID != NULL) {
				char cBuffer[1024];

				sprintf(cBuffer, "Connected to device at %s, with Device ID %s.\n", Device, SystemInfo->Device_ID);
				if ((SystemInfo->Software_Version != NULL) && (SystemInfo->Hardware_Version != NULL)) {
					sprintf(cBuffer + strlen(cBuffer), "\tHardware Version %s, running %s\n", SystemInfo->Hardware_Version, SystemInfo->Software_Version);
				}

				if (SystemInfo->Device_Name != NULL) {
					sprintf(cBuffer + strlen(cBuffer), "\tThis device is:\t%s\n", SystemInfo->Device_Name);
				}

				if (SystemInfo->Alias != NULL) {
					sprintf(cBuffer + strlen(cBuffer), "\tDevice Alias:\t%s\n", SystemInfo->Alias);
				}

				if (SystemInfo->MAC_Address != NULL) {
					sprintf(cBuffer + strlen(cBuffer), "\tMAC Address:\t%s\n", SystemInfo->MAC_Address);
				}

				printf("%s\n", cBuffer);

				break;
			}
		}

		// On error, sleep for 10 seconds then retry.
		printf("\nError! Failed to retrive device information from %s!\nRetrying in 10 seconds...\n", Device);
		sleep(10);
	}


	char cTableString[1024];
	sprintf(cTableString, "CREATE TABLE IF NOT EXISTS Dev_%s (id INTEGER PRIMARY KEY AUTOINCREMENT, time DATETIME DEFAULT CURRENT_TIMESTAMP, volts REAL, amps REAL, watts REAL, kwh REAL);", SystemInfo->Device_ID);

	if (sqlite3_exec(dBase, cTableString, 0, 0, &zErrMsg) != SQLITE_OK) {
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return;
	}

	time_t lastTime = time(NULL);

	while(1) {
		if (time(NULL) != lastTime) {
			// Logger loop here.

			RequestString = malloc(strlen(REALTIME_QUERY) + 1);
			RequestBytes = strlen(REALTIME_QUERY);
			strcpy(RequestString, REALTIME_QUERY);

			ResponseString = NULL;

			ResponseBytes = json_request(RequestBytes, RequestString, &ResponseString, Device);

			device_response *Response = calloc(sizeof(device_response), 1);
			decode_json(ResponseString, Response);

			char Query[1024];

			sprintf(Query, "INSERT INTO Dev_%s (volts, amps, watts, kwh) VALUES (%f, %f, %f, %f)", SystemInfo->Device_ID, Response->Volts, Response->Amps, Response->Watts, Response->kWh_To_Date);

			if (sqlite3_exec(dBase, Query, 0, 0, zErrMsg) != SQLITE_OK) {
				printf("SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}

			free_device_response(Response);

			lastTime = time(NULL);
		}
		usleep(100000);
	}
}
