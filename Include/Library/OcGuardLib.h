/** @file

OcGuardLib

Copyright (c) 2018, vit9696

All rights reserved.

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef OC_GUARD_LIB_H
#define OC_GUARD_LIB_H

///
/// Maximum values for common UEFI Data Types
///
#define MAX_INT8    ((INT8)0x7F)
#define MAX_UINT8   ((UINT8)0xFF)
#define MAX_INT16   ((INT16)0x7FFF)
#define MAX_UINT16  ((UINT16)0xFFFF)
#define MAX_INT32   ((INT32)0x7FFFFFFF)
#define MAX_UINT32  ((UINT32)0xFFFFFFFF)
#define MAX_INT64   ((INT64)0x7FFFFFFFFFFFFFFFULL)
#define MAX_UINT64  ((UINT64)0xFFFFFFFFFFFFFFFFULL)

///
/// Minimum values for the signed UEFI Data Types
///
#define MIN_INT8   (((INT8)  -127) - 1)
#define MIN_INT16  (((INT16) -32767) - 1)
#define MIN_INT32  (((INT32) -2147483647) - 1)
#define MIN_INT64  (((INT64) -9223372036854775807LL) - 1)


//
// The macros below provide compile-time assertions.
// This is important, as UDK only has VERIFY_SIZE_OF, which is limited and broken.
// Since it is implemented as an extern, it neither lets one to verify array size and array
// element size (due to variable redeclaration) at the same time,  nor allows macro use
// within a .c file (due to unused variable warnings).
// The reason for split declarations exists due to MSVC legacy.
//
#if defined(__GUNC__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201100L)
//
// Any supported GCC-compatible implements _Static_assert.
// So do any C11-compliant compilers.
//
#define OC_GLOBAL_STATIC_ASSERT(Expr, Message) _Static_assert (Expr, Message)
#define OC_INLINE_STATIC_ASSERT(Expr, Message) _Static_assert (Expr, Message)
#elif defined(_MSC_VER) && _MSC_VER >= 1600
//
// Starting from VS 2010 MSVC supports static_assert in both C and C++ modes.
//
#define OC_GLOBAL_STATIC_ASSERT(Expr, Message) static_assert (Expr, Message)
#define OC_INLINE_STATIC_ASSERT(Expr, Message) static_assert (Expr, Message)
#else
//
// For MSVC we implement static assertions via switch, as they do not have compile-time
// offsetof implementation, yet it pointer arithmetics works fine for them in switch.
// The struct declaration is here to avoid a warning for extra ;.
// The concatenation indirection does not permit for multiple assertions on one line
// or within a macro.
//
#define OC_STATIC_ASSERT_CONCAT2(Left, Right) Left ## Right
#define OC_STATIC_ASSERT_CONCAT(Left, Right) OC_STATIC_ASSERT_CONCAT2 (Left, Right)
#define OC_GLOBAL_STATIC_ASSERT(Expr, Message) \
  VOID OC_STATIC_ASSERT_CONCAT(OC_STATIC_ASSERT__, __LINE__) (VOID) { \
  switch (0) { case 0: case (Expr):; } } \
  struct OC_STATIC_ASSERT_CONCAT(OC_STATIC_ASSERT_T__, __LINE__) { UINT32 Dummy; }
#define OC_INLINE_STATIC_ASSERT(Expr, Message) \
  do { switch (0) { case 0: case (Expr):; } } while(0)
#endif

//
// The macros below provide pointer alignment checking interfaces.
// TypedPtr - pointer of a dedicated type, which alignment is to be checked.
// Align    - valid alignment for the target platform (power of two so far).
// Type     - valid complete typename.
// Ptr      - raw pointer value, must fit into UINTN, meant to be uintptr_t equivalent.
//

#if defined(OC_FORCE_ALIGN_SUPPORT) || defined(MDE_CPU_AARCH64) || defined(MDE_CPU_X64) || defined(MDE_CPU_ARM) || defined(MDE_CPU_IA32)

#define OC_ALIGNED(TypedPtr) (0ULL == (((UINTN) (TypedPtr)) & \
  (sizeof (*(TypedPtr)) > sizeof (UINTN) ? (sizeof (UINTN) - 1U) : (sizeof (*(TypedPtr)) - 1U))))

#define OC_POT_ALIGNED(Align, Ptr) (0ULL == (((UINTN) (Ptr)) & (Align-1)))

#define OC_TYPE_ALIGNED(Type, Ptr) (0ULL == (((UINTN) (Ptr)) & \
  (sizeof (Type) > sizeof(UINTN) ? (sizeof (UINTN) - 1U) : (sizeof (Type) - 1U))))

#else

#error "Unknown target platform. Alignment macros are not applicable."

#endif

//
// The interfaces below provide base safe arithmetics, reporting
// signed integer overflow and unsigned integer wraparound similarly to
// os/overflow.h in macOS SDK.
//
// Each interface may be implemented not only as an actual function, but
// a macro as well. Macro implementations are allowed to evaluate the
// expressions no more than once, and are supposed to provide faster
// compiler builtins if available.
//
// Each interface returns FALSE when the the stored result is equal to
// the infinite precision result, otherwise TRUE. The operands should
// be read left to right with the last argument representing a non-NULL
// pointer to the resulting value of the same type.
//
// More information could be found in Clang Extensions documentation:
// http://releases.llvm.org/7.0.0/tools/clang/docs/LanguageExtensions.html#checked-arithmetic-builtins
//

//
// 32-bit integer addition, subtraction, multiplication, triple addition (A+B+C),
// triple multiplication (A*B*C), addition with multiplication ((A+B)*C),
// and multiplication with addition (A*B+C) support.
//

BOOLEAN
OcOverflowAddU32 (
  UINT32  A,
  UINT32  B,
  UINT32  *Result
  );

BOOLEAN
OcOverflowSubU32 (
  UINT32  A,
  UINT32  B,
  UINT32  *Result
  );

BOOLEAN
OcOverflowMulU32 (
  UINT32  A,
  UINT32  B,
  UINT32  *Result
  );

BOOLEAN
OcOverflowTriAddU32 (
  UINT32  A,
  UINT32  B,
  UINT32  C,
  UINT32  *Result
  );

BOOLEAN
OcOverflowTriMulU32 (
  UINT32  A,
  UINT32  B,
  UINT32  C,
  UINT32  *Result
  );

BOOLEAN
OcOverflowAddMulU32 (
  UINT32  A,
  UINT32  B,
  UINT32  C,
  UINT32  *Result
  );

BOOLEAN
OcOverflowMulAddU32 (
  UINT32  A,
  UINT32  B,
  UINT32  C,
  UINT32  *Result
  );

BOOLEAN
OcOverflowAddS32 (
  INT32  A,
  INT32  B,
  INT32  *Result
  );

BOOLEAN
OcOverflowSubS32 (
  INT32  A,
  INT32  B,
  INT32  *Result
  );

BOOLEAN
OcOverflowMulS32 (
  INT32  A,
  INT32  B,
  INT32  *Result
  );

BOOLEAN
OcOverflowTriAddS32 (
  INT32  A,
  INT32  B,
  INT32  C,
  INT32  *Result
  );

BOOLEAN
OcOverflowTriMulS32 (
  INT32  A,
  INT32  B,
  INT32  C,
  INT32  *Result
  );

BOOLEAN
OcOverflowAddMulS32 (
  INT32  A,
  INT32  B,
  INT32  C,
  INT32  *Result
  );

BOOLEAN
OcOverflowMulAddS32 (
  INT32  A,
  INT32  B,
  INT32  C,
  INT32  *Result
  );

//
// 64-bit integer addition, subtraction, multiplication, triple addition (A+B+C),
// triple multiplication (A*B*C), addition with multiplication ((A+B)*C),
// and multiplication with addition (A*B+C) support.
//

BOOLEAN
OcOverflowAddU64 (
  UINT64  A,
  UINT64  B,
  UINT64  *Result
  );

BOOLEAN
OcOverflowSubU64 (
  UINT64  A,
  UINT64  B,
  UINT64  *Result
  );

BOOLEAN
OcOverflowMulU64 (
  UINT64  A,
  UINT64  B,
  UINT64  *Result
  );

BOOLEAN
OcOverflowTriAddU64 (
  UINT64  A,
  UINT64  B,
  UINT64  C,
  UINT64  *Result
  );

BOOLEAN
OcOverflowTriMulU64 (
  UINT64  A,
  UINT64  B,
  UINT64  C,
  UINT64  *Result
  );

BOOLEAN
OcOverflowAddMulU64 (
  UINT64  A,
  UINT64  B,
  UINT64  C,
  UINT64  *Result
  );

BOOLEAN
OcOverflowMulAddU64 (
  UINT64  A,
  UINT64  B,
  UINT64  C,
  UINT64  *Result
  );


BOOLEAN
OcOverflowAddS64 (
  INT64  A,
  INT64  B,
  INT64  *Result
  );

BOOLEAN
OcOverflowSubS64 (
  INT64  A,
  INT64  B,
  INT64  *Result
  );

BOOLEAN
OcOverflowMulS64 (
  INT64  A,
  INT64  B,
  INT64  *Result
  );

BOOLEAN
OcOverflowTriAddS64 (
  INT64  A,
  INT64  B,
  INT64  C,
  INT64  *Result
  );

BOOLEAN
OcOverflowTriMulS64 (
  INT64  A,
  INT64  B,
  INT64  C,
  INT64  *Result
  );

BOOLEAN
OcOverflowAddMulS64 (
  INT64  A,
  INT64  B,
  INT64  C,
  INT64  *Result
  );

BOOLEAN
OcOverflowMulAddS64 (
  INT64  A,
  INT64  B,
  INT64  C,
  INT64  *Result
  );

//
// Native integer addition, subtraction, multiplication, triple addition (A+B+C),
// triple multiplication (A*B*C), addition with multiplication ((A+B)*C),
// and multiplication with addition (A*B+C) support.
//

BOOLEAN
OcOverflowAddUN (
  UINTN  A,
  UINTN  B,
  UINTN  *Result
  );

BOOLEAN
OcOverflowSubUN (
  UINTN  A,
  UINTN  B,
  UINTN  *Result
  );

BOOLEAN
OcOverflowMulUN (
  UINTN  A,
  UINTN  B,
  UINTN  *Result
  );

BOOLEAN
OcOverflowTriAddUN (
  UINTN  A,
  UINTN  B,
  UINTN  C,
  UINTN  *Result
  );

BOOLEAN
OcOverflowTriMulUN (
  UINTN  A,
  UINTN  B,
  UINTN  C,
  UINTN  *Result
  );

BOOLEAN
OcOverflowAddMulUN (
  UINTN  A,
  UINTN  B,
  UINTN  C,
  UINTN  *Result
  );

BOOLEAN
OcOverflowMulAddUN (
  UINTN  A,
  UINTN  B,
  UINTN  C,
  UINTN  *Result
  );

BOOLEAN
OcOverflowAddSN (
  INTN  A,
  INTN  B,
  INTN  *Result
  );

BOOLEAN
OcOverflowSubSN (
  INTN  A,
  INTN  B,
  INTN  *Result
  );

BOOLEAN
OcOverflowMulSN (
  INTN  A,
  INTN  B,
  INTN  *Result
  );

BOOLEAN
OcOverflowTriAddSN (
  INTN  A,
  INTN  B,
  INTN  C,
  INTN  *Result
  );

BOOLEAN
OcOverflowTriMulSN (
  INTN  A,
  INTN  B,
  INTN  C,
  INTN  *Result
  );

BOOLEAN
OcOverflowAddMulSN (
  INTN  A,
  INTN  B,
  INTN  C,
  INTN  *Result
  );

BOOLEAN
OcOverflowMulAddSN (
  INTN  A,
  INTN  B,
  INTN  C,
  INTN  *Result
  );

//
// Macro implemenations of the above declarations
//

#ifdef __has_builtin

//
// Type-generic checkers are available
//
#if __has_builtin(__builtin_add_overflow) && __has_builtin(__builtin_sub_overflow) && __has_builtin(__builtin_mul_overflow)

#define OcOverflowAddU32(A, B, Res) __builtin_add_overflow((UINT32)(A), (UINT32)(B), (UINT32 *)(Res))
#define OcOverflowSubU32(A, B, Res) __builtin_sub_overflow((UINT32)(A), (UINT32)(B), (UINT32 *)(Res))
#define OcOverflowMulU32(A, B, Res) __builtin_mul_overflow((UINT32)(A), (UINT32)(B), (UINT32 *)(Res))
#define OcOverflowAddS32(A, B, Res) __builtin_add_overflow((INT32)(A),  (INT32)(B),  (INT32 *)(Res))
#define OcOverflowSubS32(A, B, Res) __builtin_sub_overflow((INT32)(A),  (INT32)(B),  (INT32 *)(Res))
#define OcOverflowMulS32(A, B, Res) __builtin_mul_overflow((INT32)(A),  (INT32)(B),  (INT32 *)(Res))

#define OcOverflowAddU64(A, B, Res) __builtin_add_overflow((UINT64)(A), (UINT64)(B), (UINT64 *)(Res))
#define OcOverflowSubU64(A, B, Res) __builtin_sub_overflow((UINT64)(A), (UINT64)(B), (UINT64 *)(Res))
#define OcOverflowMulU64(A, B, Res) __builtin_mul_overflow((UINT64)(A), (UINT64)(B), (UINT64 *)(Res))
#define OcOverflowAddS64(A, B, Res) __builtin_add_overflow((INT64)(A),  (INT64)(B),  (INT64 *)(Res))
#define OcOverflowSubS64(A, B, Res) __builtin_sub_overflow((INT64)(A),  (INT64)(B),  (INT64 *)(Res))
#define OcOverflowMulS64(A, B, Res) __builtin_mul_overflow((INT64)(A),  (INT64)(B),  (INT64 *)(Res))

#define OcOverflowAddUN(A, B, Res)  __builtin_add_overflow((UINTN)(A), (UINTN)(B), (UINTN *)(Res))
#define OcOverflowSubUN(A, B, Res)  __builtin_sub_overflow((UINTN)(A), (UINTN)(B), (UINTN *)(Res))
#define OcOverflowMulUN(A, B, Res)  __builtin_mul_overflow((UINTN)(A), (UINTN)(B), (UINTN *)(Res))
#define OcOverflowAddSN(A, B, Res)  __builtin_add_overflow((INTN)(A),  (INTN)(B),  (INTN *)(Res))
#define OcOverflowSubSN(A, B, Res)  __builtin_sub_overflow((INTN)(A),  (INTN)(B),  (INTN *)(Res))
#define OcOverflowMulSN(A, B, Res)  __builtin_mul_overflow((INTN)(A),  (INTN)(B),  (INTN *)(Res))

#elif defined(__GNUC__) || defined(__clang__)

//
// Builtin type checkers are available, but we have to match their sizes.
// For this reason we force the list of supported architectures here based on ProcessorBind.h,
// and with the assumption that CHAR_BIT is 8.
//

//
// Implement 32-bit intrinsics with int and unsigned int on architectures that support it.
//
#if defined(MDE_CPU_AARCH64) || defined(MDE_CPU_ARM) || defined(MDE_CPU_X64) || defined(MDE_CPU_IA32)

VERIFY_SIZE_OF (int, 4);
VERIFY_SIZE_OF (unsigned, 4);

#define OcOverflowAddU32(A, B, Res) __builtin_uadd_overflow((UINT32)(A), (UINT32)(B), (UINT32 *)(Res))
#define OcOverflowSubU32(A, B, Res) __builtin_usub_overflow((UINT32)(A), (UINT32)(B), (UINT32 *)(Res))
#define OcOverflowMulU32(A, B, Res) __builtin_umul_overflow((UINT32)(A), (UINT32)(B), (UINT32 *)(Res))
#define OcOverflowAddS32(A, B, Res) __builtin_sadd_overflow((INT32)(A),  (INT32)(B),  (INT32 *)(Res))
#define OcOverflowSubS32(A, B, Res) __builtin_ssub_overflow((INT32)(A),  (INT32)(B),  (INT32 *)(Res))
#define OcOverflowMulS32(A, B, Res) __builtin_smul_overflow((INT32)(A),  (INT32)(B),  (INT32 *)(Res))

#endif // 32-bit integer support

//
// Implement 64-bit intrinsics with long long and unsigned long long on architectures that support it.
// Note: ProcessorBind.h may use long on X64, but it is as large as long long.
//
#if defined(MDE_CPU_AARCH64) || defined(MDE_CPU_ARM) || defined(MDE_CPU_X64) || defined(MDE_CPU_IA32)

typedef long long oc_guard_long_long;
typedef unsigned long long oc_guard_unsigned_long_long;

VERIFY_SIZE_OF (oc_guard_long_long, 8);
VERIFY_SIZE_OF (oc_guard_unsigned_long_long, 8);

#define OcOverflowAddU64(A, B, Res) __builtin_uaddll_overflow((UINT64)(A), (UINT64)(B), (UINT64 *)(Res))
#define OcOverflowSubU64(A, B, Res) __builtin_usubll_overflow((UINT64)(A), (UINT64)(B), (UINT64 *)(Res))
#define OcOverflowMulU64(A, B, Res) __builtin_umulll_overflow((UINT64)(A), (UINT64)(B), (UINT64 *)(Res))
#define OcOverflowAddS64(A, B, Res) __builtin_saddll_overflow((INT64)(A),  (INT64)(B),  (INT64 *)(Res))
#define OcOverflowSubS64(A, B, Res) __builtin_ssubll_overflow((INT64)(A),  (INT64)(B),  (INT64 *)(Res))
#define OcOverflowMulS64(A, B, Res) __builtin_smulll_overflow((INT64)(A),  (INT64)(B),  (INT64 *)(Res))

#endif // 64-bit integer support

//
// Implement native intrinsics with 32-bit or 64-bit intrinsics depending on the support.
//
#if defined(MDE_CPU_AARCH64) || defined(MDE_CPU_X64)

VERIFY_SIZE_OF (INTN, 8);
VERIFY_SIZE_OF (UINTN, 8);

#define OcOverflowAddUN(A, B, Res) OcOverflowAddU64((A), (B), (Res))
#define OcOverflowSubUN(A, B, Res) OcOverflowSubU64((A), (B), (Res))
#define OcOverflowMulUN(A, B, Res) OcOverflowMulU64((A), (B), (Res))
#define OcOverflowAddSN(A, B, Res) OcOverflowAddS64((A), (B), (Res))
#define OcOverflowSubSN(A, B, Res) OcOverflowSubS64((A), (B), (Res))
#define OcOverflowMulSN(A, B, Res) OcOverflowMulS64((A), (B), (Res))

#elif defined(MDE_CPU_ARM) || defined(MDE_CPU_IA32)

VERIFY_SIZE_OF (INTN, 4);
VERIFY_SIZE_OF (UINTN, 4);

#define OcOverflowAddUN(A, B, Res) OcOverflowAddU32((A), (B), (Res))
#define OcOverflowSubUN(A, B, Res) OcOverflowSubU32((A), (B), (Res))
#define OcOverflowMulUN(A, B, Res) OcOverflowMulU32((A), (B), (Res))
#define OcOverflowAddSN(A, B, Res) OcOverflowAddS32((A), (B), (Res))
#define OcOverflowSubSN(A, B, Res) OcOverflowSubS32((A), (B), (Res))
#define OcOverflowMulSN(A, B, Res) OcOverflowMulS32((A), (B), (Res))

#endif // native integer support

#endif // type integer support

#endif // __has_builtin

#if defined(__GNUC__) || defined(__clang__)

//
// Macro implementations for compilers supporting Statement Expressions:
// https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
//

#define OcOverflowTriAddU32(A, B, C, Res) ({             \
  UINT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowAddU32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddU32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriMulU32(A, B, C, Res) ({             \
  UINT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowMulU32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulU32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowAddMulU32(A, B, C, Res) ({             \
  UINT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowAddU32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulU32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowMulAddU32(A, B, C, Res) ({             \
  UINT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowMulU32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddU32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriAddS32(A, B, C, Res) ({             \
  INT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowAddS32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddS32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriMulS32(A, B, C, Res) ({             \
  INT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowMulS32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulS32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowAddMulS32(A, B, C, Res) ({             \
  INT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowAddS32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulS32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowMulAddS32(A, B, C, Res) ({             \
  INT32 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowMulS32((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddS32(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })

#define OcOverflowTriAddU64(A, B, C, Res) ({             \
  UINT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowAddU64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddU64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriMulU64(A, B, C, Res) ({             \
  UINT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowMulU64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulU64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowAddMulU64(A, B, C, Res) ({             \
  UINT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowAddU64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulU64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowMulAddU64(A, B, C, Res) ({             \
  UINT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowMulU64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddU64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriAddS64(A, B, C, Res) ({             \
  INT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowAddS64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddS64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriMulS64(A, B, C, Res) ({             \
  INT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowMulS64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulS64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowAddMulS64(A, B, C, Res) ({             \
  INT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowAddS64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulS64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowMulAddS64(A, B, C, Res) ({             \
  INT64 OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowMulS64((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddS64(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })

#define OcOverflowTriAddUN(A, B, C, Res) ({             \
  UINTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowAddUN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddUN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriMulUN(A, B, C, Res) ({             \
  UINTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowMulUN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulUN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowAddMulUN(A, B, C, Res) ({             \
  UINTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowAddUN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulUN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowMulAddUN(A, B, C, Res) ({             \
  UINTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;         \
  OcFirst__  = OcOverflowMulUN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddUN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriAddSN(A, B, C, Res) ({             \
  INTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowAddSN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddSN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowTriMulSN(A, B, C, Res) ({             \
  INTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowMulSN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulSN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowAddMulSN(A, B, C, Res) ({             \
  INTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowAddSN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowMulSN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })
#define OcOverflowMulAddSN(A, B, C, Res) ({             \
  INTN OcTmp__; BOOLEAN OcFirst__, OcSecond__;          \
  OcFirst__  = OcOverflowMulSN((A), (B), &OcTmp__);     \
  OcSecond__ = OcOverflowAddSN(OcTmp__, (C), (Res));    \
  OcFirst__ | OcSecond__; })

#endif // __GNUC__

#endif // OC_GUARD_LIB_H
