// TP-Link HS-Series Smart Plug Client Library (libtphs)
// JSON encoding and decoding functions.
//
// File path: libtphs/json/json.h
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

// We completely ignore the history info the device outputs, because it doesn't
//     seem to be very trustworthy; it seems to suffer from buffer overflows /
//     underflows, and outputs / logs random values occasionally. On the other
//     hand, the realtime data seems to be pretty accurate.

typedef struct {
	double Amps;
	double Volts;
	double Watts;
	double kWh_To_Date;

	char *Software_Version;
	char *Hardware_Version;
	char *Type;
	char *Model;
	char *MAC_Address;
	char *Device_ID;
	char *Hardware_ID;
	char *Firmware_ID;
	char *OEM_ID;

	char *Alias;
	char *Device_Name;

	// WTF? Yes, this thing has a GPS in it. And yes, it seems to log this shit!
	double Latitude;
	double Longitude;

	int RSSI;

	bool Relay_State;
	bool LED_Off;

} device_response;

int decode_json(char * iData, device_response *Device);
void free_device_response(device_response *Device);