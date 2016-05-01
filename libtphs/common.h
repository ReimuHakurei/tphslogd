// TP-Link HS-Series Smart Plug Client Library (libtphs)
// Common Header and Configuration
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

// The port that the device runs on.
#define DEVICE_PORT 9999

// Most machines are Little Endian, but the device needs Big Endian values.
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

// Header files for the rest of the library.
#include "network/network.h"
