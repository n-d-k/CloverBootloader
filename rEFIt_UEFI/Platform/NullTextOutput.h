/** @file **/

#ifndef NULL_TEXT_OUTPUT_H
#define NULL_TEXT_OUTPUT_H

/**
  Allocate new System Table with disabled text output.

  @param[in] SystemTable     Base System Table.

  @retval non NULL  The System Table table was allocated successfully.
**/
EFI_SYSTEM_TABLE *
AllocateNullTextOutSystemTable (
  EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // NULL_TEXT_OUTPUT_H
