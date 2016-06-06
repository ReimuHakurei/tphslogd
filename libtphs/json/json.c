// TP-Link HS-Series Smart Plug Client Library (libtphs)
// JSON encoding and decoding functions.
//
// File path: libtphs/json/json.c
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include "../common.h"

int decode_json(char * iData, device_response *Device) {
	json_t *Response;
	json_error_t Error;

	Response = json_loads (iData, 0, &Error);

	if (!Response) {
		printf("JSON Error: Error on line %d: %s\n", Error.line, Error.text);

		printf("DEBUG: %s\n", iData);

		return 1;
	}

	json_t *EnergyMeter = json_object_get(Response, "emeter");

	if (EnergyMeter) {
		json_t *Realtime = json_object_get(EnergyMeter, "get_realtime");

		if (Realtime) {
			json_t *Volts		= json_object_get(Realtime, "voltage");
			json_t *Amps		= json_object_get(Realtime, "current");
			json_t *Watts		= json_object_get(Realtime, "power");
			json_t *kWh_To_Date	= json_object_get(Realtime, "total");

			if (Volts)			Device->Volts 		= json_real_value(Volts);
			if (Amps)			Device->Amps 		= json_real_value(Amps);
			if (Watts)			Device->Watts		= json_real_value(Watts);
			if (kWh_To_Date)	Device->kWh_To_Date = json_real_value(kWh_To_Date);

			json_decref(Volts);
			json_decref(Amps);
			json_decref(Watts);
			json_decref(kWh_To_Date);
		}

		json_decref(Realtime);
	}

	json_decref(EnergyMeter);


	json_t *System = json_object_get(Response, "system");

	if (System) {
		json_t *SystemInfo = json_object_get(System, "get_sysinfo");

		if (SystemInfo) {
			json_t *Software_Version	= json_object_get(SystemInfo, "sw_ver");
			json_t *Hardware_Version	= json_object_get(SystemInfo, "hw_ver");
			json_t *Type				= json_object_get(SystemInfo, "type");
			json_t *Model				= json_object_get(SystemInfo, "model");
			json_t *MAC_Address			= json_object_get(SystemInfo, "mac");
			json_t *Device_ID 			= json_object_get(SystemInfo, "deviceId");
			json_t *Hardware_ID 		= json_object_get(SystemInfo, "hwId");
			json_t *Firmware_ID 		= json_object_get(SystemInfo, "fwId");
			json_t *OEM_ID 				= json_object_get(SystemInfo, "oemId");

			json_t *Alias 				= json_object_get(SystemInfo, "alias");
			json_t *Device_Name 		= json_object_get(SystemInfo, "dev_name");

			json_t *Latitude			= json_object_get(SystemInfo, "latitude");
			json_t *Longitude			= json_object_get(SystemInfo, "longitude");

			json_t *RSSI				= json_object_get(SystemInfo, "rssi");
			json_t *Relay_State			= json_object_get(SystemInfo, "relay_state");
			json_t *LED_Off				= json_object_get(SystemInfo, "led_off");

			if (Software_Version) {
				Device->Software_Version = malloc(json_string_length(Software_Version) + 1);
				if (Device->Software_Version) strcpy(Device->Software_Version,json_string_value(Software_Version));
			}

			if (Hardware_Version) {
				Device->Hardware_Version = malloc(json_string_length(Hardware_Version) + 1);
				if (Device->Hardware_Version) strcpy(Device->Hardware_Version,json_string_value(Hardware_Version));
			}

			if (Type) {
				Device->Type = malloc(json_string_length(Type) + 1);
				if (Device->Type) strcpy(Device->Type,json_string_value(Type));
			}

			if (Model) {
				Device->Model = malloc(json_string_length(Model) + 1);
				if (Device->Model) strcpy(Device->Model,json_string_value(Model));
			}

			if (MAC_Address) {
				Device->MAC_Address = malloc(json_string_length(MAC_Address) + 1);
				if (Device->MAC_Address) strcpy(Device->MAC_Address,json_string_value(MAC_Address));
			}

			if (Device_ID) {
				Device->Device_ID = malloc(json_string_length(Device_ID) + 1);
				if (Device->Device_ID) strcpy(Device->Device_ID,json_string_value(Device_ID));
			}

			if (Hardware_ID) {
				Device->Hardware_ID = malloc(json_string_length(Hardware_ID) + 1);
				if (Device->Hardware_ID) strcpy(Device->Hardware_ID,json_string_value(Hardware_ID));
			}

			if (Firmware_ID) {
				Device->Firmware_ID = malloc(json_string_length(Firmware_ID) + 1);
				if (Device->Firmware_ID) strcpy(Device->Firmware_ID,json_string_value(Firmware_ID));
			}

			if (OEM_ID) {
				Device->OEM_ID = malloc(json_string_length(OEM_ID) + 1);
				if (Device->OEM_ID) strcpy(Device->OEM_ID,json_string_value(OEM_ID));
			}

			if (Alias) {
				Device->Alias = malloc(json_string_length(Alias) + 1);
				if (Device->Alias) strcpy(Device->Alias,json_string_value(Alias));
			}

			if (Device_Name) {
				Device->Device_Name = malloc(json_string_length(Device_Name) + 1);
				if (Device->Device_Name) strcpy(Device->Device_Name,json_string_value(Device_Name));
			}

			if (Latitude)		Device->Latitude		= json_real_value(Latitude);
			if (Longitude)		Device->Longitude 		= json_real_value(Latitude);

			if (RSSI)			Device->RSSI 			= json_integer_value(RSSI);
			if (Relay_State)	Device->Relay_State 	= json_integer_value(Relay_State);
			if (LED_Off)		Device->LED_Off 		= json_integer_value(LED_Off);

			json_decref(Software_Version);
			json_decref(Hardware_Version);
			json_decref(Type);
			json_decref(Model);
			json_decref(MAC_Address);
			json_decref(Device_ID);
			json_decref(Hardware_ID);
			json_decref(Firmware_ID);
			json_decref(OEM_ID);
			json_decref(Alias);
			json_decref(Device_Name);
			json_decref(Latitude);
			json_decref(Longitude);
			json_decref(RSSI);
			json_decref(Relay_State);
			json_decref(LED_Off);
		}

		json_decref(SystemInfo);
	}

	json_decref(System);
	json_decref(Response);

	return 0;
}

void free_device_response(device_response *Device) {
	free(Device->Software_Version);
	free(Device->Hardware_Version);
	free(Device->Type);
	free(Device->Model);
	free(Device->MAC_Address);
	free(Device->Device_ID);
	free(Device->Hardware_ID);
	free(Device->Firmware_ID);
	free(Device->OEM_ID);
	free(Device->Alias);
	free(Device->Device_Name);

	free(Device);
}
