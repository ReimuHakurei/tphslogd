// TP-Link HS-Series Smart Plug Client Library (libtphs)
// NullCBC and Header Functions
//
// (C) 2016 Alex Ingram <alex@alexingram.net>. All Rights Reserved.
// This file is licensed under the MIT License.

// Function declarations.
size_t nullcbc_encode(char *iBuffer, char *oBuffer, size_t iMaxBytes, size_t iDataSize);
size_t nullcbc_decode(char *iBuffer, char *oBuffer, size_t iMaxBytes, size_t iDataSize);
