; @file
; Copyright (C) 2021, ISP RAS. All rights reserved.
;*
;*   Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
;*   SPDX-License-Identifier: BSD-2-Clause-Patent
;*
; This program and the accompanying materials
; are licensed and made available under the terms and conditions of the BSD License
; which accompanies this distribution.  The full text of the license may be found at
; http://opensource.org/licenses/bsd-license.php
;
; THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
; WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
;
BITS 64

extern ASM_PFX(__stack_chk_fail)
extern ASM_PFX(__security_cookie)

section .text

; #######################################################################
; VOID
; __security_check_cookie (
;   IN UINTN  Value
;   )
; #######################################################################
align 8
global ASM_PFX(__security_check_cookie)
ASM_PFX(__security_check_cookie):
  mov  rax, qword [rel ASM_PFX(__security_cookie)]
  cmp  rax, rcx
  jnz  ASM_PFX(__stack_chk_fail)
  ret
