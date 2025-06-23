/*!
 * @file    lin_pid.c
 * @brief   Test file for the LIN PID CLI and computation
 * 
 * @author  Abdullah Almosalami @c85hup8
 * @date    Tues Apr 15, 2025
 * @copyright MIT License
 */

/* File Inclusions */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "unity.h"
#include "lin_pid.h"

/* Local Macro Definitions */
#define MAX_ARGS_TO_CHECK  5  // e.g., lin_pid XX --hex --quiet --no-new-line
#define MAX_NUM_LEN        6  // strlen("0x3F") + 1
#define MAX_ARG_LEN        (strlen("--no-new-line"))
#define MAX_ERR_MSG_LEN    100

/* Datatypes */

#define LIN_PID_NUMERIC_FORMAT( enum, regexp, prnt_fmt, ish, isd ) \
   enum,

enum NumericFormat_E
{
   #include "lin_pid_supported_formats.h"
   NUM_OF_NUMERIC_FORMATS,
   INVALID_NUMERIC_FORMAT
};

#undef LIN_PID_NUMERIC_FORMAT

/* Local Variables */
static const uint8_t REFERENCE_PID_TABLE[MAX_ID_ALLOWED + 1] =
{
   0x80, 0xC1, 0x42, 0x03, 0xC4, 0x85, 0x06, 0x47, 
   0x08, 0x49, 0xCA, 0x8B, 0x4C, 0x0D, 0x8E, 0xCF, 
   0x50, 0x11, 0x92, 0xD3, 0x14, 0x55, 0xD6, 0x97, 
   0xD8, 0x99, 0x1A, 0x5B, 0x9C, 0xDD, 0x5E, 0x1F, 
   0x20, 0x61, 0xE2, 0xA3, 0x64, 0x25, 0xA6, 0xE7, 
   0xA8, 0xE9, 0x6A, 0x2B, 0xEC, 0xAD, 0x2E, 0x6F, 
   0xF0, 0xB1, 0x32, 0x73, 0xB4, 0xF5, 0x76, 0x37, 
   0x78, 0x39, 0xBA, 0xFB, 0x3C, 0x7D, 0xFE, 0xBF
};

/* External Data */

/* Forward Function Declarations */

/* Test Setup */
void setUp(void);
void tearDown(void);

/* ComputePID */

void test_ComputePID_FullRangeOfValidIDs(void);
void test_ComputePID_FullRangeOfInvalidIDs(void);

/* GetID */

// Acceptable formats:
// Hex:     0xZZ, ZZ, Z, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ
// Decimal: ZZd, ZZD

// TODO: Make sure all the test assertions on the result of GetID have an extra msg
void test_GetID_HexRange_0xZZ_Format(void);
void test_GetID_HexRange_ZZ_Default_Format(void);
void test_GetID_HexRange_ZZh_Format(void);
void test_GetID_HexRange_ZZH_Format(void);
void test_GetID_HexRange_ZZx_Format(void);
void test_GetID_HexRange_ZZX_Format(void);
void test_GetID_HexRange_xZZ_Format(void);
void test_GetID_HexRange_XZZ_Format(void);
void test_GetID_DecRange_ZZd_Format(void);
void test_GetID_DecRange_ZZD_Format(void);
void test_GetID_DecRange_ZZ_Format_PreemptivelyDec(void);
void test_GetID_DecRange_ZZd_Format_PreemptivelyDec(void);
void test_GetID_DecRange_ZZD_Format_PreemptivelyDec(void);

void test_GetID_HexRange_0x0Z_Format(void);
void test_GetID_HexRange_0Z_Default_Format(void);
void test_GetID_HexRange_0Zh_Format(void);
void test_GetID_HexRange_0ZH_Format(void);
void test_GetID_HexRange_0Zx_Format(void);
void test_GetID_HexRange_0ZX_Format(void);
void test_GetID_HexRange_x0Z_Format(void);
void test_GetID_HexRange_X0Z_Format(void);
void test_GetID_DecRange_0Zd_Format(void);
void test_GetID_DecRange_0ZD_Format(void);
void test_GetID_DecRange_0Z_Format_PreemptivelyDec(void);
void test_GetID_DecRange_0Zd_Format_PreemptivelyDec(void);
void test_GetID_DecRange_0ZD_Format_PreemptivelyDec(void);

void test_GetID_HexRange_0xZ_Format(void);
void test_GetID_HexRange_Z_Default_Format(void);
void test_GetID_HexRange_Zh_Format(void);
void test_GetID_HexRange_ZH_Format(void);
void test_GetID_HexRange_Zx_Format(void);
void test_GetID_HexRange_ZX_Format(void);
void test_GetID_HexRange_xZ_Format(void);
void test_GetID_HexRange_XZ_Format(void);
void test_GetID_NumRange_Zd_Format(void);
void test_GetID_NumRange_ZD_Format(void);
void test_GetID_DecRange_Z_Format_PreemptivelyDec(void);

// TODO: GetID Invalid digits in
//void test_GetID_InvalidNum_TooManyDigits_ZZ_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_ZZh_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_ZZH_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_ZZx_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_ZZX_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_xZZ_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_0xZZ_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_ZZd_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_ZZD_Format(void);
//void test_GetID_InvalidNum_TooManyDigits_PreemptivelyDec(void);
//
//void test_GetID_InvalidNum_DecFormatWithHexNum_0xZZd(void);
//void test_GetID_InvalidNum_DecFormatWithHexNum_xZZd(void);
//void test_GetID_InvalidNum_DecFormatWithHexNum_XZZd(void);
//void test_GetID_InvalidNum_DecFormatWithHexNum_PreemptivelyDec(void);
//
//void test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZd(void);
//void test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZD(void);
//
//void test_GetID_InvalidNum_ZZhd(void);
//void test_GetID_InvalidNum_ZZHd(void);
//
//void test_GetID_InvalidNum_PreemptivelyDec_0xZZ(void);
//void test_GetID_InvalidNum_PreemptivelyDec_xZZ(void);
//void test_GetID_InvalidNum_PreemptivelyDec_XZZ(void);
//void test_GetID_InvalidNum_PreemptivelyDec_ZZh(void);
//void test_GetID_InvalidNum_PreemptivelyDec_ZZH(void);
//void test_GetID_InvalidNum_PreemptivelyDec_ZZx(void);
//void test_GetID_InvalidNum_PreemptivelyDec_ZZX(void);
//
//void test_GetID_InvalidFirstChar_0xKZ(void);
//void test_GetID_InvalidFirstChar_xKZ(void);
//void test_GetID_InvalidFirstChar_XKZ(void);
//void test_GetID_InvalidFirstChar_KZh(void);
//void test_GetID_InvalidFirstChar_KZH(void);
//void test_GetID_InvalidFirstChar_KZx(void);
//void test_GetID_InvalidFirstChar_KZX(void);
//
//void test_GetID_InvalidSecondChar_0xZK(void);
//void test_GetID_InvalidSecondChar_xZK(void);
//void test_GetID_InvalidSecondChar_XZK(void);
//void test_GetID_InvalidSecondChar_ZKh(void);
//void test_GetID_InvalidSecondChar_ZKH(void);
//void test_GetID_InvalidSecondChar_ZKx(void);
//void test_GetID_InvalidSecondChar_ZKX(void);
//void test_GetID_InvalidSecondChar_0Kd(void);
//void test_GetID_InvalidSecondChar_0KD(void);
//void test_GetID_InvalidSecondChar_0Kx(void);
//void test_GetID_InvalidSecondChar_0KX(void);
//
//void test_GetID_NoDigitsEntered(void);

/* MyAtoI */

void test_MyAtoI_ValidDecimalDigits(void);
void test_MyAtoI_ValidHexadecimalDigitsLowercase(void);
void test_MyAtoI_ValidHexadecimalDigitsUppercase(void);
void test_MyAtoI_InvalidCharacters(void);
void test_MyAtoI_EmptyCharacter(void);

#ifndef NDEBUG

/* UInt8_Cmp */

void test_UInt8_Cmp_A_Greater_Than_B(void);
void test_UInt8_Cmp_A_Less_Than_B(void);
void test_UInt8_Cmp_A_Equal_To_B(void);
void test_UInt8_Cmp_Min_And_Max_Values(void);
void test_UInt8_Cmp_Adjacent_Values(void);
void test_UInt8_Cmp_Zero_Comparison(void);

#endif

/* OnlyValidFlagsArePresent */

void test_OnlyValidFlagsArePresent_AllValidFlags(void);
void test_OnlyValidFlagsArePresent_BasicArgs_NumFirst(void);
void test_OnlyValidFlagsArePresent_BasicArgs_NumLast(void);
void test_OnlyValidFlagsArePresent_ClearlyInvalidFlagPresent(void);
void test_OnlyValidFlagsArePresent_OffByOneCharFlag(void);
void test_OnlyValidFlagsArePresent_DuplicateValidFlags(void);
void test_OnlyValidFlagsArePresent_ValidFlagsWithNullEntry(void);
void test_OnlyValidFlagsArePresent_ValidFlagsWithEmptyString(void);
void test_OnlyValidFlagsArePresent_ValidFlagsWithWhitespace(void);
void test_OnlyValidFlagsArePresent_NoFlagsJustNum(void);

/* ArgOccurenceCount */

void test_ArgOccurrenceCount_SingleOccurrence(void);
void test_ArgOccurrenceCount_MultipleOccurrences(void);
void test_ArgOccurrenceCount_NoOccurrence(void);
void test_ArgOccurrenceCount_NullIdxPointer(void);
void test_ArgOccurrenceCount_ArgAtEnd(void);
void test_ArgOccurrenceCount_ArgAtBeginning(void);
void test_ArgOccurrenceCount_EmptyArgs(void);
void test_ArgOccurrenceCount_NullArgEntry(void);
void test_ArgOccurrenceCount_MaxArgsLimit(void);

/* DetermineEntryFormat */

void test_DetermineEntryFormat_DecNoPrefixOrSuffix_NoLeadingZeros(void);
void test_DetermineEntryFormat_DecNoPrefixOrSuffix_LeadingZeros(void);

void test_DetermineEntryFormat_HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_HexNoPrefixOrSuffix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_HexNoPrefixOrSuffix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_ClassicHexPrefix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_ClassicHexPrefix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_ClassicHexPrefix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_ClassicHexPrefix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_LowercasexPrefix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_LowercasexPrefix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_LowercasexPrefix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_LowercasexPrefix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_UppercaseXPrefix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_UppercaseXPrefix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_UppercaseXPrefix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_UppercaseXPrefix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_LowercasehSuffix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_LowercasehSuffix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_LowercasehSuffix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_LowercasehSuffix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_UppercaseHSuffix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_UppercaseHSuffix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_UppercaseHSuffix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_UppercaseHSuffix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_LowercasexSuffix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_LowercasexSuffix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_LowercasexSuffix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_LowercasexSuffix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_UppercaseXSuffix_NoLeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_UppercaseXSuffix_NoLeadingZeros_Lowercase(void);
void test_DetermineEntryFormat_UppercaseXSuffix_LeadingZeros_Uppercase(void);
void test_DetermineEntryFormat_UppercaseXSuffix_LeadingZeros_Lowercase(void);

void test_DetermineEntryFormat_LowercasedSuffix_NoLeadingZeros(void);
void test_DetermineEntryFormat_LowercasedSuffix_LeadingZeros(void);

void test_DetermineEntryFormat_UppercaseDSuffix_NoLeadingZeros(void);
void test_DetermineEntryFormat_UppercaseDSuffix_LeadingZeros(void);

/* Extern Functions */
extern enum LIN_PID_Result_E GetID( const char * str,
                                    uint8_t * id,
                                    bool * ishex,
                                    bool * isdec );

extern bool MyAtoI(char digit, uint8_t * converted_digit);

#ifndef NDEBUG
extern int UInt8_Cmp( const void * a, const void * b );
#endif

extern bool OnlyValidFlagsArePresent( char const * args[], int argc );

extern size_t ArgOccurrenceCount( char const * args[],
                                  char const * str,
                                  int argc,
                                  uint8_t * idx_of_first_occurrence );

extern enum NumericFormat_E DetermineEntryFormat( const char * str,
                                                  bool ishex,
                                                  bool isdec );

extern void CompileAllRegexPatterns(void);
extern void FreeAllRegexPatterns(void);

/* Meat of the Program */

int main(void)
{
   UNITY_BEGIN();
   
   /* ComputePID */

   RUN_TEST(test_ComputePID_FullRangeOfValidIDs);
   RUN_TEST(test_ComputePID_FullRangeOfInvalidIDs);

   /* GetID */
   
   RUN_TEST(test_GetID_HexRange_0xZZ_Format);
   RUN_TEST(test_GetID_HexRange_ZZ_Default_Format);
   RUN_TEST(test_GetID_HexRange_ZZh_Format);
   RUN_TEST(test_GetID_HexRange_ZZH_Format);
   RUN_TEST(test_GetID_HexRange_ZZx_Format);
   RUN_TEST(test_GetID_HexRange_ZZX_Format);
   RUN_TEST(test_GetID_HexRange_xZZ_Format);
   RUN_TEST(test_GetID_HexRange_XZZ_Format);
   RUN_TEST(test_GetID_DecRange_ZZd_Format);
   RUN_TEST(test_GetID_DecRange_ZZD_Format);
   RUN_TEST(test_GetID_DecRange_ZZ_Format_PreemptivelyDec);
   RUN_TEST(test_GetID_DecRange_ZZd_Format_PreemptivelyDec);
   RUN_TEST(test_GetID_DecRange_ZZD_Format_PreemptivelyDec);

   RUN_TEST(test_GetID_HexRange_0x0Z_Format);
   RUN_TEST(test_GetID_HexRange_0Z_Default_Format);
   RUN_TEST(test_GetID_HexRange_0Zh_Format);
   RUN_TEST(test_GetID_HexRange_0ZH_Format);
   RUN_TEST(test_GetID_HexRange_0Zx_Format);
   RUN_TEST(test_GetID_HexRange_0ZX_Format);
   RUN_TEST(test_GetID_HexRange_x0Z_Format);
   RUN_TEST(test_GetID_HexRange_X0Z_Format);
   RUN_TEST(test_GetID_DecRange_0Zd_Format);
   RUN_TEST(test_GetID_DecRange_0ZD_Format);
   RUN_TEST(test_GetID_DecRange_0Z_Format_PreemptivelyDec);
   RUN_TEST(test_GetID_DecRange_0Zd_Format_PreemptivelyDec);
   RUN_TEST(test_GetID_DecRange_0ZD_Format_PreemptivelyDec);

   RUN_TEST(test_GetID_HexRange_0xZ_Format);
   RUN_TEST(test_GetID_HexRange_Z_Default_Format);
   RUN_TEST(test_GetID_HexRange_Zh_Format);
   RUN_TEST(test_GetID_HexRange_ZH_Format);
   RUN_TEST(test_GetID_HexRange_Zx_Format);
   RUN_TEST(test_GetID_HexRange_ZX_Format);
   RUN_TEST(test_GetID_HexRange_xZ_Format);
   RUN_TEST(test_GetID_HexRange_XZ_Format);
   RUN_TEST(test_GetID_NumRange_Zd_Format);
   RUN_TEST(test_GetID_NumRange_ZD_Format);
   RUN_TEST(test_GetID_DecRange_Z_Format_PreemptivelyDec);

   // TODO: GetID Invalid test cases
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZ_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZh_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZH_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZx_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZX_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_xZZ_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_0xZZ_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZd_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZD_Format);
//   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_PreemptivelyDec);
//
//   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_0xZZd);
//   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_xZZd);
//   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_XZZd);
//   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_PreemptivelyDec);
//
//   RUN_TEST(test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZd);
//   RUN_TEST(test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZD);
//
//   RUN_TEST(test_GetID_InvalidNum_ZZhd);
//   RUN_TEST(test_GetID_InvalidNum_ZZHd);
//
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_0xZZ);
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_xZZ);
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_XZZ);
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZh);
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZH);
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZx);
//   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZX);
//
//   RUN_TEST(test_GetID_InvalidFirstChar_0xKZ);
//   RUN_TEST(test_GetID_InvalidFirstChar_xKZ);
//   RUN_TEST(test_GetID_InvalidFirstChar_XKZ);
//   RUN_TEST(test_GetID_InvalidFirstChar_KZh);
//   RUN_TEST(test_GetID_InvalidFirstChar_KZH);
//   RUN_TEST(test_GetID_InvalidFirstChar_KZx);
//   RUN_TEST(test_GetID_InvalidFirstChar_KZX);
//
//   RUN_TEST(test_GetID_InvalidSecondChar_0xZK);
//   RUN_TEST(test_GetID_InvalidSecondChar_xZK);
//   RUN_TEST(test_GetID_InvalidSecondChar_XZK);
//   RUN_TEST(test_GetID_InvalidSecondChar_ZKh);
//   RUN_TEST(test_GetID_InvalidSecondChar_ZKH);
//   RUN_TEST(test_GetID_InvalidSecondChar_ZKx);
//   RUN_TEST(test_GetID_InvalidSecondChar_ZKX);
//   RUN_TEST(test_GetID_InvalidSecondChar_0Kd);
//   RUN_TEST(test_GetID_InvalidSecondChar_0KD);
//   RUN_TEST(test_GetID_InvalidSecondChar_0Kx);
//   RUN_TEST(test_GetID_InvalidSecondChar_0KX);
//
//   RUN_TEST(test_GetID_NoDigitsEntered);

   /* MyAtoI */

   RUN_TEST(test_MyAtoI_ValidDecimalDigits);
   RUN_TEST(test_MyAtoI_ValidHexadecimalDigitsLowercase);
   RUN_TEST(test_MyAtoI_ValidHexadecimalDigitsUppercase);
   RUN_TEST(test_MyAtoI_InvalidCharacters);
   RUN_TEST(test_MyAtoI_EmptyCharacter);

#ifndef NDEBUG

   /* UInt8_Cmp */

   RUN_TEST(test_UInt8_Cmp_A_Greater_Than_B);
   RUN_TEST(test_UInt8_Cmp_A_Less_Than_B);
   RUN_TEST(test_UInt8_Cmp_A_Equal_To_B);
   RUN_TEST(test_UInt8_Cmp_Min_And_Max_Values);
   RUN_TEST(test_UInt8_Cmp_Adjacent_Values);
   RUN_TEST(test_UInt8_Cmp_Zero_Comparison);

#endif

   RUN_TEST(test_OnlyValidFlagsArePresent_AllValidFlags);
   RUN_TEST(test_OnlyValidFlagsArePresent_BasicArgs_NumFirst);
   RUN_TEST(test_OnlyValidFlagsArePresent_BasicArgs_NumLast);
   RUN_TEST(test_OnlyValidFlagsArePresent_ClearlyInvalidFlagPresent);
   RUN_TEST(test_OnlyValidFlagsArePresent_OffByOneCharFlag);
   RUN_TEST(test_OnlyValidFlagsArePresent_DuplicateValidFlags);
   RUN_TEST(test_OnlyValidFlagsArePresent_ValidFlagsWithNullEntry);
   RUN_TEST(test_OnlyValidFlagsArePresent_ValidFlagsWithEmptyString);
   RUN_TEST(test_OnlyValidFlagsArePresent_ValidFlagsWithWhitespace);
   RUN_TEST(test_OnlyValidFlagsArePresent_NoFlagsJustNum);

   RUN_TEST(test_ArgOccurrenceCount_SingleOccurrence);
   RUN_TEST(test_ArgOccurrenceCount_MultipleOccurrences);
   RUN_TEST(test_ArgOccurrenceCount_NoOccurrence);
   RUN_TEST(test_ArgOccurrenceCount_NullIdxPointer);
   RUN_TEST(test_ArgOccurrenceCount_ArgAtEnd);
   RUN_TEST(test_ArgOccurrenceCount_ArgAtBeginning);
   RUN_TEST(test_ArgOccurrenceCount_EmptyArgs);
   RUN_TEST(test_ArgOccurrenceCount_NullArgEntry);
   RUN_TEST(test_ArgOccurrenceCount_MaxArgsLimit);

   RUN_TEST(test_DetermineEntryFormat_DecNoPrefixOrSuffix_NoLeadingZeros);
   RUN_TEST(test_DetermineEntryFormat_DecNoPrefixOrSuffix_LeadingZeros);
   RUN_TEST(test_DetermineEntryFormat_HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_HexNoPrefixOrSuffix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_HexNoPrefixOrSuffix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_ClassicHexPrefix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_ClassicHexPrefix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_ClassicHexPrefix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_ClassicHexPrefix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexPrefix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexPrefix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexPrefix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexPrefix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXPrefix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXPrefix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXPrefix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXPrefix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasehSuffix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasehSuffix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasehSuffix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasehSuffix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseHSuffix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseHSuffix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseHSuffix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseHSuffix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexSuffix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexSuffix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexSuffix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasexSuffix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXSuffix_NoLeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXSuffix_NoLeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXSuffix_LeadingZeros_Uppercase);
   RUN_TEST(test_DetermineEntryFormat_UppercaseXSuffix_LeadingZeros_Lowercase);
   RUN_TEST(test_DetermineEntryFormat_LowercasedSuffix_NoLeadingZeros);
   RUN_TEST(test_DetermineEntryFormat_LowercasedSuffix_LeadingZeros);
   RUN_TEST(test_DetermineEntryFormat_UppercaseDSuffix_NoLeadingZeros);
   RUN_TEST(test_DetermineEntryFormat_UppercaseDSuffix_LeadingZeros);

   return UNITY_END();
}

void setUp(void)
{
   // Do nothing
}
void tearDown(void)
{
   // Do nothing
}

/* Computation */

/******************************************************************************/

void test_ComputePID_FullRangeOfValidIDs(void)
{
   for ( uint8_t i = 0; i < MAX_ID_ALLOWED; i++ )
   {
      TEST_ASSERT_EQUAL_UINT8( REFERENCE_PID_TABLE[i], ComputePID(i) );
   }
}

void test_ComputePID_FullRangeOfInvalidIDs(void)
{
   for ( uint16_t i = (MAX_ID_ALLOWED + 1); i <= UINT8_MAX; i++ )
   {
      TEST_ASSERT_EQUAL_UINT8( INVALID_PID, ComputePID( (uint8_t)i) );
   }
}

/******************************************************************************/

void test_GetID_HexRange_0xZZ_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "0x%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZZ_Default_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZZh_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%Xh", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZZH_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%XH", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZZx_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%Xx", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZZX_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%XX", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_xZZ_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "x%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_XZZ_Format(void)
{
   for ( uint8_t id = 0x10; id < UINT8_MAX; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "X%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_ZZd_Format(void)
{
   for ( uint8_t id = 10; id < 99; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%dd", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_ZZD_Format(void)
{
   for ( uint8_t id = 10; id < 99; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%dD", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_ZZ_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 10; id < 99; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%d", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_ZZd_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 10; id < 99; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%dd", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_ZZD_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 10; id < 99; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%dD", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

/******************************************************************************/

void test_GetID_HexRange_0x0Z_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "0x%02X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_0Z_Default_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_0Zh_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02Xh", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_0ZH_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02XH", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_0Zx_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02Xx", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_0ZX_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02XX", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_x0Z_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "x%02X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_X0Z_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "X%02X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_0Zd_Format(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02dd", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_0ZD_Format(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%02dD", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_0Z_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%02d", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_0Zd_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%02dd", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_DecRange_0ZD_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%02dD", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

/******************************************************************************/

void test_GetID_HexRange_0xZ_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "0x%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_Z_Default_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_Zh_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%Xh", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZH_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%XH", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_Zx_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%Xx", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_ZX_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%XX", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_xZ_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "x%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_HexRange_XZ_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "X%X", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

void test_GetID_NumRange_Zd_Format(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%dd", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      // By default, these single digit + 'd' entries are assumed to actually
      // be hexadecimal numbers /w the least signficant digit being 'd' (i.e., 13).
      TEST_ASSERT_EQUAL_UINT8( (id * 0x10) + 0xD, parsed_id );
   }
}

void test_GetID_NumRange_ZD_Format(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = false;

      snprintf(str, MAX_NUM_LEN, "%dD", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      // By default, these single digit + 'D' entries are assumed to actually
      // be hexadecimal numbers /w the least signficant digit being 'D' (i.e., 13).
      TEST_ASSERT_EQUAL_UINT8( (id * 0x10) + 0xD, parsed_id );
   }
}

void test_GetID_DecRange_Z_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 0; id < 10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;
      bool pre_emptively_hex = false;
      bool pre_emptively_dec = true;

      snprintf(str, MAX_NUM_LEN, "%d", id);
      result = GetID(str, &parsed_id, &pre_emptively_hex, &pre_emptively_dec);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}

/******************************************************************************/

void test_MyAtoI_ValidDecimalDigits(void)
{
   uint8_t converted_digit;
   for (char digit = '0'; digit <= '9'; digit++)
   {
      TEST_ASSERT_TRUE(MyAtoI(digit, &converted_digit));
      TEST_ASSERT_EQUAL_UINT8(digit - '0', converted_digit);
   }
}

void test_MyAtoI_ValidHexadecimalDigitsLowercase(void)
{
   uint8_t converted_digit;
   for (char digit = 'a'; digit <= 'f'; digit++)
   {
      TEST_ASSERT_TRUE(MyAtoI(digit, &converted_digit));
      TEST_ASSERT_EQUAL_UINT8(10 + (digit - 'a'), converted_digit);
   }
}

void test_MyAtoI_ValidHexadecimalDigitsUppercase(void)
{
   uint8_t converted_digit;
   for (char digit = 'A'; digit <= 'F'; digit++)
   {
      TEST_ASSERT_TRUE(MyAtoI(digit, &converted_digit));
      TEST_ASSERT_EQUAL_UINT8(10 + (digit - 'A'), converted_digit);
   }
}

void test_MyAtoI_InvalidCharacters(void)
{
   uint8_t converted_digit;
   char invalid_chars[] = {'g', 'G', 'z', 'Z', '@', '!', '-', ' '};
   for (size_t i = 0; i < sizeof(invalid_chars); i++)
   {
      char fail_msg[2] = {0};
      snprintf( fail_msg, 1, "%c", invalid_chars[i] );
      TEST_ASSERT_FALSE_MESSAGE( MyAtoI(invalid_chars[i], &converted_digit), fail_msg );
   }
}

void test_MyAtoI_EmptyCharacter(void)
{
   uint8_t converted_digit;
   TEST_ASSERT_FALSE( MyAtoI('\0', &converted_digit) );
}

/******************************************************************************/

#ifndef NDEBUG

void test_UInt8_Cmp_A_Greater_Than_B(void)
{
   uint8_t a = 10;
   uint8_t b = 5;
   TEST_ASSERT_GREATER_THAN(0, UInt8_Cmp(&a, &b));
}

void test_UInt8_Cmp_A_Less_Than_B(void)
{
   uint8_t a = 5;
   uint8_t b = 10;
   TEST_ASSERT_LESS_THAN(0, UInt8_Cmp(&a, &b));
}

void test_UInt8_Cmp_A_Equal_To_B(void)
{
   uint8_t a = 7;
   uint8_t b = 7;
   TEST_ASSERT_EQUAL(0, UInt8_Cmp(&a, &b));
}

void test_UInt8_Cmp_Min_And_Max_Values(void)
{
   uint8_t a = 0;
   uint8_t b = UINT8_MAX;
   TEST_ASSERT_LESS_THAN(0, UInt8_Cmp(&a, &b));

   a = UINT8_MAX;
   b = 0;
   TEST_ASSERT_GREATER_THAN(0, UInt8_Cmp(&a, &b));
}

void test_UInt8_Cmp_Adjacent_Values(void)
{
   uint8_t a = 100;
   uint8_t b = 101;
   TEST_ASSERT_LESS_THAN(0, UInt8_Cmp(&a, &b));

   a = 101;
   b = 100;
   TEST_ASSERT_GREATER_THAN(0, UInt8_Cmp(&a, &b));
}

void test_UInt8_Cmp_Zero_Comparison(void)
{
   uint8_t a = 0;
   uint8_t b = 0;
   TEST_ASSERT_EQUAL(0, UInt8_Cmp(&a, &b));
}

#endif

/******************************************************************************/

void test_OnlyValidFlagsArePresent_AllValidFlags(void)
{
   const char * args1[] = {"program", "0x10", "--hex"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args1, sizeof(args1) / sizeof(args1[0])));
   const char * args2[] = {"program", "0x10", "-h"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args2, sizeof(args2) / sizeof(args2[0])));
   const char * args3[] = {"program", "0x10", "--hex", "--quiet"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args3, sizeof(args3) / sizeof(args3[0])));
   const char * args4[] = {"program", "0x10", "-h", "--quiet"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args4, sizeof(args4) / sizeof(args4[0])));
   const char * args5[] = {"program", "0x10", "--hex", "-q"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args5, sizeof(args5) / sizeof(args5[0])));
   const char * args6[] = {"program", "0x10", "-h", "-q"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args6, sizeof(args6) / sizeof(args6[0])));
   const char * args7[] = {"program", "0x10", "--hex", "--quiet", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args7, sizeof(args7) / sizeof(args7[0])));
   const char * args8[] = {"program", "0x10", "-h", "--quiet", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args8, sizeof(args8) / sizeof(args8[0])));
   const char * args9[] = {"program", "0x10", "--hex", "-q", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args9, sizeof(args9) / sizeof(args9[0])));
   const char * args10[] = {"program", "0x10", "-h", "-q", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args10, sizeof(args10) / sizeof(args10[0])));

   const char * args11[] = {"program", "10", "--dec"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args11, sizeof(args11) / sizeof(args11[0])));
   const char * args12[] = {"program", "10", "-d"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args12, sizeof(args12) / sizeof(args12[0])));
   const char * args13[] = {"program", "10", "--dec", "--quiet"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args13, sizeof(args13) / sizeof(args13[0])));
   const char * args14[] = {"program", "10", "-d", "--quiet"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args14, sizeof(args14) / sizeof(args14[0])));
   const char * args15[] = {"program", "10", "--dec", "-q"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args15, sizeof(args15) / sizeof(args15[0])));
   const char * args16[] = {"program", "10", "-d", "-q"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args16, sizeof(args16) / sizeof(args16[0])));
   const char * args17[] = {"program", "10", "--dec", "--quiet", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args17, sizeof(args17) / sizeof(args17[0])));
   const char * args18[] = {"program", "10", "-d", "--quiet", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args18, sizeof(args18) / sizeof(args18[0])));
   const char * args19[] = {"program", "10", "--dec", "-q", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args19, sizeof(args19) / sizeof(args19[0])));
   const char * args20[] = {"program", "10", "-d", "-q", "--no-new-line"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args20, sizeof(args20) / sizeof(args20[0])));

   const char * args21[] = {"program", "--table"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args21, sizeof(args21) / sizeof(args21[0])));
   const char * args22[] = {"program", "-t"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args22, sizeof(args22) / sizeof(args22[0])));
   const char * args23[] = {"program", "--help"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args23, sizeof(args23) / sizeof(args23[0])));
}

void test_OnlyValidFlagsArePresent_BasicArgs_NumFirst(void)
{
   const char * args[] = {"program", "10", "-d"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

void test_OnlyValidFlagsArePresent_BasicArgs_NumLast(void)
{
   const char * args[] = {"program", "-d", "10"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

void test_OnlyValidFlagsArePresent_ClearlyInvalidFlagPresent(void)
{
   const char * args1[] = {"program", "--hex", "-h", "--invalid-flag"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args1, sizeof(args1) / sizeof(args1[0])));

   const char * args2[] = {"program", "--hex", "--invalid-flag", "-q"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args2, sizeof(args2) / sizeof(args2[0])));
}

void test_OnlyValidFlagsArePresent_OffByOneCharFlag(void)
{
   const char * args1[] = {"program", "--hes"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args1, sizeof(args1) / sizeof(args1[0])));

   const char * args2[] = {"program", "--dex"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args2, sizeof(args2) / sizeof(args2[0])));

   const char * args3[] = {"program", "--quit"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args3, sizeof(args3) / sizeof(args3[0])));

   const char * args4[] = {"program", "--quite"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args4, sizeof(args4) / sizeof(args4[0])));

   const char * args5[] = {"program", "--hex", "--no-ne-line"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args5, sizeof(args5) / sizeof(args5[0])));

   const char * args6[] = {"program", "--dex", "--no-ne-line"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args6, sizeof(args6) / sizeof(args6[0])));

   const char * args7[] = {"program", "--hel"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args7, sizeof(args7) / sizeof(args7[0])));

   const char * args8[] = {"program", "-g"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args8, sizeof(args8) / sizeof(args8[0])));
}

void test_OnlyValidFlagsArePresent_DuplicateValidFlags(void)
{
   const char * args[] = {"program", "--hex", "--hex", "-q", "-q"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

void test_OnlyValidFlagsArePresent_ValidFlagsWithNullEntry(void)
{
   const char * args[] = {"program", "--hex", NULL, "-q"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

void test_OnlyValidFlagsArePresent_ValidFlagsWithEmptyString(void)
{
   const char * args[] = {"program", "--hex", "", "-q"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

void test_OnlyValidFlagsArePresent_ValidFlagsWithWhitespace(void)
{
   const char * args[] = {"program", "--hex", "  ", "-q"};
   TEST_ASSERT_FALSE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

void test_OnlyValidFlagsArePresent_NoFlagsJustNum(void)
{
   const char * args[] = {"program", "0x10"};
   TEST_ASSERT_TRUE(OnlyValidFlagsArePresent(args, sizeof(args) / sizeof(args[0])));
}

/******************************************************************************/

void test_ArgOccurrenceCount_SingleOccurrence(void)
{
   const char * args[] = {"program", "--hex", "-q", "--dec"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--hex", 4, &idx);
   TEST_ASSERT_EQUAL_UINT32(1, count);
   TEST_ASSERT_EQUAL_UINT8(1, idx);
}

void test_ArgOccurrenceCount_MultipleOccurrences(void)
{
   const char * args[] = {"program", "--hex", "-q", "--hex", "--hex"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--hex", 5, &idx);
   TEST_ASSERT_EQUAL_UINT32(3, count);
   TEST_ASSERT_EQUAL_UINT8(1, idx);
}

void test_ArgOccurrenceCount_NoOccurrence(void)
{
   const char * args[] = {"program", "--hex", "-q", "--dec"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--table", 4, &idx);
   TEST_ASSERT_EQUAL_UINT32(0, count);
   // idx is not updated, but should remain unchanged
   TEST_ASSERT_EQUAL_UINT8(0xFF, idx);
}

void test_ArgOccurrenceCount_NullIdxPointer(void)
{
   const char * args[] = {"program", "--hex", "-q", "--hex"};
   // Make sure that passing in a NULL pointer for the idx doesn't crash anything
   // and still results in the correct count being returned.
   size_t count = ArgOccurrenceCount(args, "--hex", 4, NULL);
   TEST_ASSERT_EQUAL_UINT32(2, count);
}

void test_ArgOccurrenceCount_ArgAtEnd(void)
{
   const char * args[] = {"program", "-q", "--dec", "--table"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--table", 4, &idx);
   TEST_ASSERT_EQUAL_UINT32(1, count);
   TEST_ASSERT_EQUAL_UINT8(3, idx);
}

void test_ArgOccurrenceCount_ArgAtBeginning(void)
{
   const char * args[] = {"program", "--hex", "-q", "--dec"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--hex", 4, &idx);
   TEST_ASSERT_EQUAL_UINT32(1, count);
   TEST_ASSERT_EQUAL_UINT8(1, idx);
}

void test_ArgOccurrenceCount_EmptyArgs(void)
{
   const char * args[] = {"program"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--hex", 1, &idx);
   TEST_ASSERT_EQUAL_UINT32(0, count);
   TEST_ASSERT_EQUAL_UINT8(0xFF, idx);
}

void test_ArgOccurrenceCount_NullArgEntry(void)
{
   const char * args[] = {"program", "--hex", NULL, "--hex"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--hex", 4, &idx);
   // Should skip past NULL entry
   TEST_ASSERT_EQUAL_UINT32(2, count);
   TEST_ASSERT_EQUAL_UINT8(1, idx);
}

void test_ArgOccurrenceCount_MaxArgsLimit(void)
{
   // MAX_ARGS_TO_CHECK is 5, so only first 5 args are checked
   const char * args[] = {"program", "--hex", "--hex", "--hex", "--hex", "--hex", "--hex"};
   uint8_t idx = 0xFF;
   size_t count = ArgOccurrenceCount(args, "--hex", 7, &idx);
   TEST_ASSERT_EQUAL_UINT32(4, count); // Only args[1]..args[4] are checked
   TEST_ASSERT_EQUAL_UINT8(1, idx);
}

/******************************************************************************/

void test_DetermineEntryFormat_DecNoPrefixOrSuffix_NoLeadingZeros(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("20", false, true) );
   TEST_ASSERT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("63", false, true) );
   TEST_ASSERT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("20", false, false) );
   TEST_ASSERT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("63", false, false) );

   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("333", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("09",  false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("09",  true,  false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("09",  false, true) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1A", true, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("F",  false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("a",  false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("0xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("0x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("0D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_NoLeadingZeros, DetermineEntryFormat("63D", false, false) );
}

void test_DetermineEntryFormat_DecNoPrefixOrSuffix_LeadingZeros(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0",  false, false));
   TEST_ASSERT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("00", false, false));
   TEST_ASSERT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("09", false, false));
   TEST_ASSERT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0",  false, true));
   TEST_ASSERT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("00", false, true));
   TEST_ASSERT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("09", false, true));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT(DecNoPrefixOrSuffix_LeadingZeros,  DetermineEntryFormat("9", false, false));  // Will match no leading zeros first
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("F", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("a", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("0D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( DecNoPrefixOrSuffix_LeadingZeros, DetermineEntryFormat("63D", false, false) );
}

void test_DetermineEntryFormat_HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("A",  false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("DF", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1F", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("A",  true,  false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("DF", true,  false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1F", true,  false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("9", false, false));  // Will match no leading zeros first
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63D", false, false) );
}

void test_DetermineEntryFormat_HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("a",  false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("df", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1f", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("a",  true,  false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("df", true,  false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1f", true,  false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT(HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("9", false, false));  // Will match no leading zeros first
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63D", false, false) );
}

void test_DetermineEntryFormat_HexNoPrefixOrSuffix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0A", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0A", true,  false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", true,  false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Uppercase,  DetermineEntryFormat("9", false, false));  // Will match no leading zeros first
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_HexNoPrefixOrSuffix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0a", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0a", true,  false));
   TEST_ASSERT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", true,  false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT(HexNoPrefixOrSuffix_LeadingZeros_Lowercase,  DetermineEntryFormat("9", false, false));  // Will match no leading zeros first
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("63D", false, false) );
}

void test_DetermineEntryFormat_ClassicHexPrefix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x10", false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x2",  false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x10", true, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", true, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  true, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x2",  true, false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x0f", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x0F", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x0",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_ClassicHexPrefix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", true,  false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  true,  false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x10", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_ClassicHexPrefix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x01", false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x0A", false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x0",  false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x00", false, false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x01", true,  false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x0A", true,  false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x0",  true,  false));
   TEST_ASSERT_EQUAL_INT(ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x00", true,  false));
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x10", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x2",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_ClassicHexPrefix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x0a", false, false) );
   TEST_ASSERT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x0f", false, false) );
   TEST_ASSERT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x0a", true,  false) );
   TEST_ASSERT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x0f", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x01", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x0",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x00", false, false));

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x10", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x2",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("xF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("XF", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( ClassicHexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexPrefix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x9B", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xA", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xF2", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x9B", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xA", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xF2", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XF", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexPrefix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x9b", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xf2", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x9b", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xf2", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1B", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XF", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0B", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexPrefix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x0B", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x0A", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x01", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x0B", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x0A", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x01", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("xA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x1B", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x0a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x0b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("XF", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexPrefix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0b", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0a", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0b", true,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0a", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("xA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x1B", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x0B", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("XF", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X10", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXPrefix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X9B", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XF2", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X9B", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XA", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XF2", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Xa", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXPrefix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X9b", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xa", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xf2", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X9b", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xa", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Xf2", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXPrefix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X09", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X09", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X0A", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXPrefix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X0a", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X0a", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10h", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10H", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXPrefix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasehSuffix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Ah", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Ah", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasehSuffix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("bh", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("bh", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasehSuffix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ah", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasehSuffix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ah", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasehSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseHSuffix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("AH", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("AH", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseHSuffix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aH", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aH", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseHSuffix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AH", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseHSuffix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aH", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseHSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexSuffix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("A1x", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1x", true, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("A1x", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1bx", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Ax", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Fx", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexSuffix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("a1x", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", true, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("a1x", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   //1EST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexSuffix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01x", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ax", true, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01x", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ax", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasexSuffix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ax", true, false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Fx", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1x", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ax", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasexSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXSuffix_NoLeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1X", false, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("A1X", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1X", true, false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("A1X", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fx", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXSuffix_NoLeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("a1X", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("a1X", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("01X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_NoLeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXSuffix_LeadingZeros_Uppercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0X", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0X", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AX", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AX", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("aX", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("01X", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Uppercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_UppercaseXSuffix_LeadingZeros_Lowercase(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aX", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aX", true,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fX", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fX", true,  false) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("01X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0AX", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0aX", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("0d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseXSuffix_LeadingZeros_Lowercase, DetermineEntryFormat("63d", false, false) );
}

void test_DetermineEntryFormat_LowercasedSuffix_NoLeadingZeros(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("10d", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("63d", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("10d", false,  true) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("63d", false,  true) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("01X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("0fX", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("20d", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("00d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("01d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_NoLeadingZeros, DetermineEntryFormat("063d", false, false) );
}

void test_DetermineEntryFormat_LowercasedSuffix_LeadingZeros(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("00d", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01d", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("063d", false,  false) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("00d", false,  true) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01d", false,  true) );
   TEST_ASSERT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("063d", false,  true) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("63d", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0d", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01d", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("063d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("63D", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("0D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("01D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( LowercasedSuffix_LeadingZeros, DetermineEntryFormat("063D", false, false) );
}

void test_DetermineEntryFormat_UppercaseDSuffix_NoLeadingZeros(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("10D", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("63D", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("10D", false,  true) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("63D", false,  true) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("01X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("00d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("01d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("063d", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("20D", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("63D", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("00D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("01D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_NoLeadingZeros, DetermineEntryFormat("063D", false, false) );
}

void test_DetermineEntryFormat_UppercaseDSuffix_LeadingZeros(void)
{
   // Positive Detection
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("00D", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01D", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("063D", false,  false) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("00D", false,  true) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01D", false,  true) );
   TEST_ASSERT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("063D", false,  true) );
   
   // Negative Detection
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("9", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("25", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("333", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros,  DetermineEntryFormat("09", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("025", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0F", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01a", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0f", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0xFF", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0xA",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0x1A",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0x1E",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0xff", false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0xa",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0x1a",  false, false));
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0x1e",  false, false));

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("xa", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("x1b", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("XA", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("X1", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("X0A", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("X01", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0Ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0Fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0ah", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0fh", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0aH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0fH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0AH", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0FH", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1x", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("10x", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1bx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0Ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0Fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0ax", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0fx", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("10X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("1AX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("FX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("aX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01X", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0AX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0aX", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("0fX", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("20d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("63d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("00d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01d", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("063d", false, false) );

   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("20D", false, false) );
   TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("63D", false, false) );

   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("00D", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("01D", false, false) );
   //TEST_ASSERT_NOT_EQUAL_INT( UppercaseDSuffix_LeadingZeros, DetermineEntryFormat("063D", false, false) );
}

