/** @file
  Copyright (C) 2018, vit9696. All rights reserved.
  Copyright (C) 2020, PMheart. All rights reserved.

  All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
**/

#include "ocvalidate.h"
#include "OcValidateLib.h"

UINT32
CheckMisc (
  IN  OC_GLOBAL_CONFIG  *Config
  )
{
  UINT32          ErrorCount;
  OC_MISC_CONFIG  *UserMisc;
  OC_UEFI_CONFIG  *UserUefi;
  CONST CHAR8     *HibernateMode;

  DEBUG ((DEBUG_VERBOSE, "config loaded into Misc checker!\n"));

  ErrorCount    = 0;
  UserMisc      = &Config->Misc;
  UserUefi      = &Config->Uefi;
  HibernateMode = OC_BLOB_GET (&UserMisc->Boot.HibernateMode);

  //
  // TODO: Check value of ConsoleAttributes.
  //

  if (AsciiStrCmp (HibernateMode, "None") != 0
    && AsciiStrCmp (HibernateMode, "Auto") != 0
    && AsciiStrCmp (HibernateMode, "RTC") != 0
    && AsciiStrCmp (HibernateMode, "NVRAM") != 0) {
    DEBUG ((DEBUG_WARN, "Misc->Boot->HibernateMode is borked (Can only be None, Auto, RTC, or NVRAM)!\n"));
    ++ErrorCount;
  }

  return ReportError (__func__, ErrorCount);
}
