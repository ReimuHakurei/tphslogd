// TP-Link HS-Series Smart Plug Client Library (libtphs)
// Common network-related functions.
//
// File path: libtphs/network/common.h
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "nullcbc.h"

void fatal_socket_error(char *Error);

size_t json_request(size_t iSize, char *iBuffer, char **oBuffer, char *deviceAddress);