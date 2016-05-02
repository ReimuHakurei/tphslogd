// TP-Link HS-Series Smart Plug CLI Client (tphs-cli)
// Testing client
//
// tphs-cli/tphs-cli.h
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include <sys/stat.h>
#include <dirent.h>

#include "../sqlite3/sqlite3.h"

#include <pthread.h>
#include <time.h>

// Function stubs.
void fatal_socket_error();
void *device_thread(void *Device);

