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

/* Forward Function Declarations */
void setUp(void);
void tearDown(void);

void test_FullRangeOfValidIDs(void);

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

void test_GetID_InvalidNum_TooManyDigits_ZZ_Format(void);
void test_GetID_InvalidNum_TooManyDigits_ZZh_Format(void);
void test_GetID_InvalidNum_TooManyDigits_ZZH_Format(void);
void test_GetID_InvalidNum_TooManyDigits_ZZx_Format(void);
void test_GetID_InvalidNum_TooManyDigits_ZZX_Format(void);
void test_GetID_InvalidNum_TooManyDigits_xZZ_Format(void);
void test_GetID_InvalidNum_TooManyDigits_0xZZ_Format(void);
void test_GetID_InvalidNum_TooManyDigits_ZZd_Format(void);
void test_GetID_InvalidNum_TooManyDigits_ZZD_Format(void);
void test_GetID_InvalidNum_TooManyDigits_PreemptivelyDec(void);

void test_GetID_InvalidNum_DecFormatWithHexNum_0xZZd(void);
void test_GetID_InvalidNum_DecFormatWithHexNum_xZZd(void);
void test_GetID_InvalidNum_DecFormatWithHexNum_XZZd(void);
void test_GetID_InvalidNum_DecFormatWithHexNum_PreemptivelyDec(void);

void test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZd(void);
void test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZD(void);

void test_GetID_InvalidNum_ZZhd(void);
void test_GetID_InvalidNum_ZZHd(void);

void test_GetID_InvalidNum_PreemptivelyDec_0xZZ(void);
void test_GetID_InvalidNum_PreemptivelyDec_xZZ(void);
void test_GetID_InvalidNum_PreemptivelyDec_XZZ(void);
void test_GetID_InvalidNum_PreemptivelyDec_ZZh(void);
void test_GetID_InvalidNum_PreemptivelyDec_ZZH(void);
void test_GetID_InvalidNum_PreemptivelyDec_ZZx(void);
void test_GetID_InvalidNum_PreemptivelyDec_ZZX(void);

void test_GetID_InvalidFirstChar_0xKZ(void);
void test_GetID_InvalidFirstChar_xKZ(void);
void test_GetID_InvalidFirstChar_XKZ(void);
void test_GetID_InvalidFirstChar_KZh(void);
void test_GetID_InvalidFirstChar_KZH(void);
void test_GetID_InvalidFirstChar_KZx(void);
void test_GetID_InvalidFirstChar_KZX(void);

void test_GetID_InvalidSecondChar_0xZK(void);
void test_GetID_InvalidSecondChar_xZK(void);
void test_GetID_InvalidSecondChar_XZK(void);
void test_GetID_InvalidSecondChar_ZKh(void);
void test_GetID_InvalidSecondChar_ZKH(void);
void test_GetID_InvalidSecondChar_ZKx(void);
void test_GetID_InvalidSecondChar_ZKX(void);
void test_GetID_InvalidSecondChar_0Kd(void);
void test_GetID_InvalidSecondChar_0KD(void);
void test_GetID_InvalidSecondChar_0Kx(void);
void test_GetID_InvalidSecondChar_0KX(void);

/* Extern Functions */
extern enum LIN_PID_Result_E GetID( char const * str,
                                    uint8_t * id,
                                    bool pre_emptively_hex,
                                    bool pre_emptively_dec );

/* Meat of the Program */

int main(void)
{
   UNITY_BEGIN();
   
   RUN_TEST(test_FullRangeOfValidIDs);

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

   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZ_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZh_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZH_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZx_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZX_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_xZZ_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_0xZZ_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZd_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_ZZD_Format);
   RUN_TEST(test_GetID_InvalidNum_TooManyDigits_PreemptivelyDec);

   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_0xZZd);
   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_xZZd);
   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_XZZd);
   RUN_TEST(test_GetID_InvalidNum_DecFormatWithHexNum_PreemptivelyDec);

   RUN_TEST(test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZd);
   RUN_TEST(test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZD);

   RUN_TEST(test_GetID_InvalidNum_ZZhd);
   RUN_TEST(test_GetID_InvalidNum_ZZHd);

   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_0xZZ);
   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_xZZ);
   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_XZZ);
   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZh);
   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZH);
   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZx);
   RUN_TEST(test_GetID_InvalidNum_PreemptivelyDec_ZZX);

   RUN_TEST(test_GetID_InvalidFirstChar_0xKZ);
   RUN_TEST(test_GetID_InvalidFirstChar_xKZ);
   RUN_TEST(test_GetID_InvalidFirstChar_XKZ);
   RUN_TEST(test_GetID_InvalidFirstChar_KZh);
   RUN_TEST(test_GetID_InvalidFirstChar_KZH);
   RUN_TEST(test_GetID_InvalidFirstChar_KZx);
   RUN_TEST(test_GetID_InvalidFirstChar_KZX);

   RUN_TEST(test_GetID_InvalidSecondChar_0xZK);
   RUN_TEST(test_GetID_InvalidSecondChar_xZK);
   RUN_TEST(test_GetID_InvalidSecondChar_XZK);
   RUN_TEST(test_GetID_InvalidSecondChar_ZKh);
   RUN_TEST(test_GetID_InvalidSecondChar_ZKH);
   RUN_TEST(test_GetID_InvalidSecondChar_ZKx);
   RUN_TEST(test_GetID_InvalidSecondChar_ZKX);
   RUN_TEST(test_GetID_InvalidSecondChar_0Kd);
   RUN_TEST(test_GetID_InvalidSecondChar_0KD);
   RUN_TEST(test_GetID_InvalidSecondChar_0Kx);
   RUN_TEST(test_GetID_InvalidSecondChar_0KX);


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

void test_FullRangeOfValidIDs(void)
{
   for ( uint8_t i = 0; i < MAX_ID_ALLOWED; i++ )
   {
      TEST_ASSERT_EQUAL_UINT8( REFERENCE_PID_TABLE[i], ComputePID(i) );
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

      snprintf(str, MAX_NUM_LEN, "0x%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%Xh", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%XH", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%Xx", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%XX", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "x%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "X%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%dd", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%dD", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%d", id);
      result = GetID(str, &parsed_id, false, true);

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

      snprintf(str, MAX_NUM_LEN, "%dd", id);
      result = GetID(str, &parsed_id, false, true);

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

      snprintf(str, MAX_NUM_LEN, "%dD", id);
      result = GetID(str, &parsed_id, false, true);

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

      snprintf(str, MAX_NUM_LEN, "0x%02X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02Xh", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02XH", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02Xx", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02XX", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "x%02X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "X%02X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02dd", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02dD", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%02d", id);
      result = GetID(str, &parsed_id, false, true);

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

      snprintf(str, MAX_NUM_LEN, "%02dd", id);
      result = GetID(str, &parsed_id, false, true);

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

      snprintf(str, MAX_NUM_LEN, "%02dD", id);
      result = GetID(str, &parsed_id, false, true);

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

      snprintf(str, MAX_NUM_LEN, "0x%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%Xh", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%XH", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%Xx", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%XX", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "x%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "X%X", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%dd", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%dD", id);
      result = GetID(str, &parsed_id, false, false);

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

      snprintf(str, MAX_NUM_LEN, "%d", id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
   }
}


/******************************************************************************/

void test_GetID_InvalidNum_TooManyDigits_ZZ_Format(void)
{
   // Decimal numbers
   for ( uint16_t id = 0; id < 9999; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%03d", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   // Hexadecimal numbers
   for ( uint16_t id = 0x0000; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      if ( (id & 0xF) == 0xD )   continue;   // Skip over the ZZD numbers

      snprintf(str, MAX_NUM_LEN + 2, "%03X", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_ZZh_Format(void)
{
   // Hexadecimal numbers
   for ( uint16_t id = 0x100; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%Xh", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_ZZH_Format(void)
{
   // Hexadecimal numbers
   for ( uint16_t id = 0x100; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%XH", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_ZZx_Format(void)
{
   // Hexadecimal numbers
   for ( uint16_t id = 0x100; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%Xx", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_ZZX_Format(void)
{
   // Hexadecimal numbers
   for ( uint16_t id = 0x100; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%XX", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_xZZ_Format(void)
{
   // Hexadecimal numbers
   for ( uint16_t id = 0x100; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "x%X", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_0xZZ_Format(void)
{
   // Hexadecimal numbers
   for ( uint16_t id = 0x100; id < 0xFFFF; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "0x%X", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_ZZd_Format(void)
{
   // Decimal numbers
   for ( uint16_t id = 100; id < 9999; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%dd", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_ZZD_Format(void)
{
   // Decimal numbers
   for ( uint16_t id = 100; id < 9999; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%dD", id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_TooManyDigits_PreemptivelyDec(void)
{
   // Decimal numbers
   for ( uint16_t id = 100; id < 9999; id++ )
   {
      char str[MAX_NUM_LEN + 2] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN + 2, "%d", id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

/******************************************************************************/

void test_GetID_InvalidNum_DecFormatWithHexNum_0xZZd(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x%02Xd", (uint8_t)id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_DecFormatWithHexNum_xZZd(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x%02Xd", (uint8_t)id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_DecFormatWithHexNum_XZZd(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X%02Xd", (uint8_t)id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_DecFormatWithHexNum_PreemptivelyDec(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      // Only check uniquely hex number entries.
      if ( (id & 0xF) < 0xA ) continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02X", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

/******************************************************************************/

void test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZd(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      // Skip over uniquely hex number entries.
      if ( (id & 0xF) >= 0xA ) continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02Xd", (uint8_t)id);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_HexFormatWithDecNum_PreemptivelyHex_ZZD(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      // Skip over uniquely hex number entries.
      if ( (id & 0xF) >= 0xA ) continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02XD", (uint8_t)id);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

/******************************************************************************/

void test_GetID_InvalidNum_ZZhd(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02Xhd", (uint8_t)id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_ZZHd(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02XHd", (uint8_t)id);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

/******************************************************************************/

void test_GetID_InvalidNum_PreemptivelyDec_0xZZ(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x%02X", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_PreemptivelyDec_xZZ(void)
{
   for (uint16_t id = 0; id <= 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x%02X", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_PreemptivelyDec_XZZ(void)
{
   for (uint16_t id = 0; id < 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X%02X", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_PreemptivelyDec_ZZh(void)
{
   for (uint16_t id = 0; id < 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02Xh", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_PreemptivelyDec_ZZH(void)
{
   for (uint16_t id = 0; id < 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02XH", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_PreemptivelyDec_ZZx(void)
{
   for (uint16_t id = 0; id < 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02Xx", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidNum_PreemptivelyDec_ZZX(void)
{
   for (uint16_t id = 0; id < 0xFF; id++)
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02XX", (uint8_t)id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

/******************************************************************************/

void test_GetID_InvalidFirstChar_0xKZ(void)
{
   // Start at 1 because "0x" is valid input - it is '0' with the supported hex
   // 'x' suffix.
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x%c", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x%c", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x%c", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidFirstChar_xKZ(void)
{
   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x%c", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x%c", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x%c", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidFirstChar_XKZ(void)
{
   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X%c", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X%c", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X%c", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidFirstChar_KZh(void)
{
   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%ch", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%ch", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%ch", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidFirstChar_KZH(void)
{
   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cH", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cH", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cH", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidFirstChar_KZx(void)
{
   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cx", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cx", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cx", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidFirstChar_KZX(void)
{
   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cX", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cX", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 0; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%cX", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

/******************************************************************************/

#define VALID_DIGIT "1"
void test_GetID_InvalidSecondChar_0xZK(void)
{
   // Start at 1 because "0x" is valid input - it is '0' with the supported hex
   // 'x' suffix.
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0x" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_xZK(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "x" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_XZK(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "X" VALID_DIGIT "%c", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_ZKh(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%ch", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%ch", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%ch", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_ZKH(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cH", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cH", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cH", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_ZKx(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cx", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cx", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cx", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_ZKX(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cX", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cX", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "%cX", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_0Kd(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cd", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cd", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cd", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_0KD(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cD", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cD", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cD", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_0Kx(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cx", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cx", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, VALID_DIGIT "0%cx", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}

void test_GetID_InvalidSecondChar_0KX(void)
{
   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0%cX", (char)first_char);
      result = GetID(str, &parsed_id, false, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0%cX", (char)first_char);
      result = GetID(str, &parsed_id, true, false);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }

   for ( uint16_t first_char = 1; first_char < 0xFF; first_char++ )
   {
      if ( isxdigit((char)first_char) )   continue;

      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "0%cX", (char)first_char);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_NOT_EQUAL_INT_MESSAGE( (int)GoodResult, (int)result, str );
   }
}


/******************************************************************************/

/* CLI */
// TODO
// Example from GH CP
// #include "unity.h"
// #include <stdio.h>
// #include <string.h>
// 
// void component_to_test() {
//     printf("Unity test output\n");
// }
// 
// void test_component_output() {
//     char buffer[100];
//     FILE *temp_file = freopen("temp_output.txt", "w+", stdout);
// 
//     component_to_test();
// 
//     fflush(temp_file);
//     rewind(temp_file);
//     fgets(buffer, sizeof(buffer), temp_file);
// 
//     TEST_ASSERT_EQUAL_STRING("Unity test output\n", buffer);
// 
//     fclose(temp_file);
//     remove("temp_output.txt");
// }
// 
// int main() {
//     UNITY_BEGIN();
//     RUN_TEST(test_component_output);
//     return UNITY_END();
// }
