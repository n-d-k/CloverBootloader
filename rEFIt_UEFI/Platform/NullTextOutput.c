/** @file **/


#include <Uefi.h>
#include <NullTextOutput.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

STATIC
EFI_STATUS
EFIAPI
NullTextReset (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN BOOLEAN                         ExtendedVerification
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextOutputString (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16                          *String
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextTestString (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN CHAR16                          *String
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextQueryMode (
  IN  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN  UINTN                           ModeNumber,
  OUT UINTN                           *Columns,
  OUT UINTN                           *Rows
  )
{
  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
NullTextSetMode (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                           ModeNumber
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextSetAttribute (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                           Attribute
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextClearScreen (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextSetCursorPosition (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN UINTN                           Column,
  IN UINTN                           Row
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NullTextEnableCursor (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
  IN BOOLEAN                         Visible
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_SIMPLE_TEXT_OUTPUT_MODE
mNullTextOutputMode;

STATIC
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
mNullTextOutputProtocol = {
  NullTextReset,
  NullTextOutputString,
  NullTextTestString,
  NullTextQueryMode,
  NullTextSetMode,
  NullTextSetAttribute,
  NullTextClearScreen,
  NullTextSetCursorPosition,
  NullTextEnableCursor,
  &mNullTextOutputMode
};

EFI_SYSTEM_TABLE *
AllocateNullTextOutSystemTable (
  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_SYSTEM_TABLE  *NewSystemTable;
  EFI_STATUS        Status;

  //
  // Patch verbose
  //
  NewSystemTable = (EFI_SYSTEM_TABLE *) AllocateZeroPool (SystemTable->Hdr.HeaderSize);

  if (NewSystemTable == NULL) {
      return NULL;
  }

  CopyMem ((VOID *) NewSystemTable, SystemTable, SystemTable->Hdr.HeaderSize);
  NewSystemTable->ConOut = &mNullTextOutputProtocol;
  NewSystemTable->Hdr.CRC32 = 0;

  Status = gBS->CalculateCrc32 (
    NewSystemTable,
    NewSystemTable->Hdr.HeaderSize,
    &NewSystemTable->Hdr.CRC32
    );

  if (EFI_ERROR (Status)) {
    FreePool (NewSystemTable);
    return NULL;
  }

  return NewSystemTable;
}
