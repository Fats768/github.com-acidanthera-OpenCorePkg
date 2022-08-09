#include <UserFile.h>
#include <Library/OcCryptoLib.h>
#include <BigNumLib.h>

CONST UINT8  TestMessage1[] = {
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

int
ENTRY_POINT (
  int   argc,
  char  *argv[]
  )
{
  UINT8  hash[64];

  Streebog (TestMessage1, 63, hash, 256);
  for (int i = 0; i < 32; ++i) {
    DEBUG ((DEBUG_ERROR, "%X", (int)hash[i]));
  }

  DEBUG ((DEBUG_ERROR, "\n"));
  Streebog (TestMessage1, 63, hash, 512);
  for (int i = 0; i < 64; ++i) {
    DEBUG ((DEBUG_ERROR, "%X", (int)hash[i]));
  }

  DEBUG ((DEBUG_ERROR, "\n"));
  return 0;
}
