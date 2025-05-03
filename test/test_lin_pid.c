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

void test_GetID_HexRange_0xZ_Format(void);
void test_GetID_HexRange_Z_Default_Format(void);
void test_GetID_HexRange_Zh_Format(void);
void test_GetID_HexRange_ZH_Format(void);
void test_GetID_HexRange_Zx_Format(void);
void test_GetID_HexRange_ZX_Format(void);
void test_GetID_HexRange_xZ_Format(void);
void test_GetID_HexRange_XZ_Format(void);
void test_GetID_DecRange_Zd_Format(void);
void test_GetID_DecRange_ZD_Format(void);
void test_GetID_DecRange_Z_Format_PreemptivelyDec(void);

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

   RUN_TEST(test_GetID_HexRange_0xZ_Format);
   RUN_TEST(test_GetID_HexRange_Z_Default_Format);
   RUN_TEST(test_GetID_HexRange_Zh_Format);
   RUN_TEST(test_GetID_HexRange_ZH_Format);
   RUN_TEST(test_GetID_HexRange_Zx_Format);
   RUN_TEST(test_GetID_HexRange_ZX_Format);
   RUN_TEST(test_GetID_HexRange_xZ_Format);
   RUN_TEST(test_GetID_HexRange_XZ_Format);
   RUN_TEST(test_GetID_DecRange_Zd_Format);
   RUN_TEST(test_GetID_DecRange_ZD_Format);
   RUN_TEST(test_GetID_DecRange_Z_Format_PreemptivelyDec);


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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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

void test_GetID_HexRange_ZZH_Format(void)
{
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
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
   for ( uint8_t id = 0x10; id < MAX_ID_ALLOWED; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%d", id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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
   for ( uint8_t id = 0; id < 0x10; id++ )
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
   for ( uint8_t id = 0; id < 0x10; id++ )
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
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%02d", id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
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

void test_GetID_DecRange_Zd_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
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

void test_GetID_DecRange_ZD_Format(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
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

void test_GetID_DecRange_Z_Format_PreemptivelyDec(void)
{
   for ( uint8_t id = 0; id < 0x10; id++ )
   {
      char str[MAX_NUM_LEN] = {0};
      uint8_t parsed_id;
      enum LIN_PID_Result_E result;

      snprintf(str, MAX_NUM_LEN, "%d", id);
      result = GetID(str, &parsed_id, false, true);

      TEST_ASSERT_EQUAL_INT( (int)GoodResult, (int)result );
      TEST_ASSERT_EQUAL_UINT8( id, parsed_id );
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
