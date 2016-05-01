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

// Header files for the rest of the library.
#include "network/network.h"
#include "../depends/jansson-2.7/src/jansson.h"