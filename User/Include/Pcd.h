/** @file
  Copyright (c) 2020, PMheart. All rights reserved.
  SPDX-License-Identifier: BSD-3-Clause
**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/OcCryptoLib.h>

extern const UINT32 _gPcd_FixedAtBuild_PcdUefiLibMaxPrintBufferSize;
extern const BOOLEAN  _gPcd_FixedAtBuild_PcdUgaConsumeSupport;
extern const UINT8 _gPcd_FixedAtBuild_PcdDebugPropertyMask;
extern const UINT8 _gPcd_FixedAtBuild_PcdDebugClearMemoryValue;
extern const UINT32 _gPcd_FixedAtBuild_PcdFixedDebugPrintErrorLevel;
extern const UINT32 _gPcd_FixedAtBuild_PcdDebugPrintErrorLevel;
extern const UINT32 _gPcd_FixedAtBuild_PcdMaximumAsciiStringLength;
extern const UINT32 _gPcd_FixedAtBuild_PcdMaximumUnicodeStringLength;
extern const UINT32 _gPcd_FixedAtBuild_PcdMaximumLinkedListLength;
extern const BOOLEAN _gPcd_FixedAtBuild_PcdVerifyNodeInList;

#define _PCD_GET_MODE_32_PcdUefiLibMaxPrintBufferSize  _gPcd_FixedAtBuild_PcdUefiLibMaxPrintBufferSize
#define _PCD_GET_MODE_BOOL_PcdUgaConsumeSupport  _gPcd_FixedAtBuild_PcdUgaConsumeSupport
#define _PCD_GET_MODE_8_PcdDebugPropertyMask  _gPcd_FixedAtBuild_PcdDebugPropertyMask
#define _PCD_GET_MODE_8_PcdDebugClearMemoryValue  _gPcd_FixedAtBuild_PcdDebugClearMemoryValue
#define _PCD_GET_MODE_32_PcdFixedDebugPrintErrorLevel  _gPcd_FixedAtBuild_PcdFixedDebugPrintErrorLevel
#define _PCD_GET_MODE_32_PcdDebugPrintErrorLevel  _gPcd_FixedAtBuild_PcdDebugPrintErrorLevel
#define _PCD_GET_MODE_32_PcdMaximumAsciiStringLength  _gPcd_FixedAtBuild_PcdMaximumAsciiStringLength
#define _PCD_GET_MODE_32_PcdMaximumUnicodeStringLength  _gPcd_FixedAtBuild_PcdMaximumUnicodeStringLength
#define _PCD_GET_MODE_32_PcdMaximumLinkedListLength  _gPcd_FixedAtBuild_PcdMaximumLinkedListLength
#define _PCD_GET_MODE_BOOL_PcdVerifyNodeInList  _gPcd_FixedAtBuild_PcdVerifyNodeInList
#define _PCD_GET_MODE_16_PcdOcCryptoAllowedRsaModuli  (512U | 256U)
#define _PCD_GET_MODE_16_PcdOcCryptoAllowedSigHashTypes  \
  (1U << OcSigHashTypeSha256) | (1U << OcSigHashTypeSha384) | (1U << OcSigHashTypeSha512)
