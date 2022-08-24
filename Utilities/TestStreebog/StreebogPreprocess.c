/** StreebogPreprocess.c
  Copyright (c) 2013, Alexey Degtyarev <alexey@renatasystems.org>. All rights reserved.<BR>
  Copyright (c) 2022 Maxim Kuznetsov. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause
**/

#include <UserFile.h>
#include <Library/OcCryptoLib.h>
#include <BigNumLib.h>

CONST UINT8  M1[] = {
  0x30, 0x31,
  0x32, 0x33,
  0x34, 0x35,
  0x36, 0x37,
  0x38, 0x39,
  0x30, 0x31,
  0x32, 0x33,
  0x34, 0x35,
  0x36, 0x37,
  0x38, 0x39,
  0x30, 0x31,
  0x32, 0x33,
  0x34, 0x35,
  0x36, 0x37,
  0x38, 0x39,
  0x30, 0x31,
  0x32, 0x33,
  0x34, 0x35,
  0x36, 0x37,
  0x38, 0x39,
  0x30, 0x31,
  0x32, 0x33,
  0x34, 0x35,
  0x36, 0x37,
  0x38, 0x39,
  0x30, 0x31,
  0x32, 0x33,
  0x34, 0x35,
  0x36, 0x37,
  0x38, 0x39,
  0x30, 0x31,
  0x32
};

CONST CHAR8  M1STR[] = {
  "012345678901234567890123456789012345678901234567890123456789012"
};

CONST CHAR8  M2[] = {
  "Се ветри, Стрибожи внуци, веютъ с моря стрелами на храбрыя плъкы Игоревы"
};

CONST CHAR8  M2TEMP[] = {
  0xfb, 0xe2,
  0xe5, 0xf0,
  0xee, 0xe3,
  0xc8, 0x20,
  0xfb, 0xea,
  0xfa, 0xeb,
  0xef, 0x20,
  0xff, 0xfb,
  0xf0, 0xe1,
  0xe0, 0xf0,
  0xf5, 0x20,
  0xe0, 0xed,
  0x20, 0xe8,
  0xec, 0xe0,
  0xeb, 0xe5,
  0xf0, 0xf2,
  0xf1, 0x20,
  0xff, 0xf0,
  0xee, 0xec,
  0x20, 0xf1,
  0x20, 0xfa,
  0xf2, 0xfe,
  0xe5, 0xe2,
  0x20, 0x2c,
  0xe8, 0xf6,
  0xf3, 0xed,
  0xe2, 0x20,
  0xe8, 0xe6,
  0xee, 0xe1,
  0xe8, 0xf0,
  0xf2, 0xd1,
  0x20, 0x2c,
  0xe8, 0xf0,
  0xf2, 0xe5,
  0xe2, 0x20,
  0xe5, 0xd1
};

CONST UINT8  H1_256[] = {
  0x9d, 0x15,
  0x1e, 0xef,
  0xd8, 0x59,
  0x0b, 0x89,
  0xda, 0xa6,
  0xba, 0x6c,
  0xb7, 0x4a,
  0xf9, 0x27,
  0x5d, 0xd0,
  0x51, 0x02,
  0x6b, 0xb1,
  0x49, 0xa4,
  0x52, 0xfd,
  0x84, 0xe5,
  0xe5, 0x7b,
  0x55, 0x00
};

CONST UINT8  H1_512[] = {
  0x1b, 0x54,
  0xd0, 0x1a,
  0x4a, 0xf5,
  0xb9, 0xd5,
  0xcc, 0x3d,
  0x86, 0xd6,
  0x8d, 0x28,
  0x54, 0x62,
  0xb1, 0x9a,
  0xbc, 0x24,
  0x75, 0x22,
  0x2f, 0x35,
  0xc0, 0x85,
  0x12, 0x2b,
  0xe4, 0xba,
  0x1f, 0xfa,
  0x00, 0xad,
  0x30, 0xf8,
  0x76, 0x7b,
  0x3a, 0x82,
  0x38, 0x4c,
  0x65, 0x74,
  0xf0, 0x24,
  0xc3, 0x11,
  0xe2, 0xa4,
  0x81, 0x33,
  0x2b, 0x08,
  0xef, 0x7f,
  0x41, 0x79,
  0x78, 0x91,
  0xc1, 0x64,
  0x6f, 0x48
};

CONST UINT8  H2_256[] = {
  0x9d, 0xd2,
  0xfe, 0x4e,
  0x90, 0x40,
  0x9e, 0x5d,
  0xa8, 0x7f,
  0x53, 0x97,
  0x6d, 0x74,
  0x05, 0xb0,
  0xc0, 0xca,
  0xc6, 0x28,
  0xfc, 0x66,
  0x9a, 0x74,
  0x1d, 0x50,
  0x06, 0x3c,
  0x55, 0x7e,
  0x8f, 0x50
};

CONST UINT8  H2_512[] = {
  0x1e, 0x88,
  0xe6, 0x22,
  0x26, 0xbf,
  0xca, 0x6f,
  0x99, 0x94,
  0xf1, 0xf2,
  0xd5, 0x15,
  0x69, 0xe0,
  0xda, 0xf8,
  0x47, 0x5a,
  0x3b, 0x0f,
  0xe6, 0x1a,
  0x53, 0x00,
  0xee, 0xe4,
  0x6d, 0x96,
  0x13, 0x76,
  0x03, 0x5f,
  0xe8, 0x35,
  0x49, 0xad,
  0xa2, 0xb8,
  0x62, 0x0f,
  0xcd, 0x7c,
  0x49, 0x6c,
  0xe5, 0xb3,
  0x3f, 0x0c,
  0xb9, 0xdd,
  0xdc, 0x2b,
  0x64, 0x60,
  0x14, 0x3b,
  0x03, 0xda,
  0xba, 0xc9,
  0xfb, 0x28
};

// fbe2e5f0eee3c820fbeafaebef20fffbf0e1e0f0f520e0ed20e8ece0ebe5f0f2f120fff0eeec20f120faf2fee5e2202ce8f6f3ede220e8e6eee1e8f0f2d1202ce8f0f2e5e220e5d1
// D0A1D0B520D0B2D0B5D182D180D0B82C20D0A1D182D180D0B8D0B1D0BED0B6D0B820D0B2D0BDD183D186D0B82C20D0B2D0B5D18ED182D18A20D18120D0BCD0BED180D18F20D181D1
// D0A1D0B520D0B2D0B5D182D180D0B82C20D0A1D182D180D0B8D0B1D0BED0B6D0B820D0B2D0BDD183D186D0B82C20D0B2D0B5D18ED182D18A20D18120D0BCD0BED180D18F20D181D1

int
ENTRY_POINT (
  int   argc,
  char  *argv[]
  )
{
  UINT8  hash[64];
  UINT8  TM2[72];
  UINT8  Flag = 1;

  Streebog256 (M1, 63, hash);

  for (int i = 0; i < 32; ++i) {
    if (hash[i] != H1_256[i]) {
      Flag = 0;
      break;
    }
  }

  if (Flag) {
    DEBUG ((DEBUG_ERROR, "M1 256 OK\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "M1 256 ERROR\n"));
  }

  Flag = 1;
  Streebog512 (M1, 63, hash);

  for (int i = 0; i < 64; ++i) {
    if (hash[i] != H1_512[i]) {
      Flag = 0;
      break;
    }
  }

  if (Flag) {
    DEBUG ((DEBUG_ERROR, "M1 512 OK\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "M1 512 ERROR\n"));
  }

  for (int i = 0; i < 72; ++i) {
    TM2[i] = (unsigned char)M2[i];
  }

  Streebog256 (TM2, 72, hash);

  for (int i = 0; i < 32; ++i) {
    if (hash[i] != H2_256[i]) {
      Flag = 0;
      break;
    }
  }

  if (Flag) {
    DEBUG ((DEBUG_ERROR, "M2 256 OK\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "M2 256 ERROR\n"));
  }

  Streebog512 (TM2, 72, hash);

  for (int i = 0; i < 64; ++i) {
    if (hash[i] != H2_512[i]) {
      Flag = 0;
      break;
    }
  }

  if (Flag) {
    DEBUG ((DEBUG_ERROR, "M2 512 OK\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "M2 512 ERROR\n"));
  }

  return 0;
}
