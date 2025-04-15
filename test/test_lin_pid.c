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
#include "unity.h"
#include "lin_pid.h"

/* Local Macro Definitions */

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
void Test_FullRangeOfValidIDs(void);

/* Meat of the Program */

int main(void)
{
   UNITY_BEGIN();
   
   RUN_TEST(Test_FullRangeOfValidIDs);

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

void Test_FullRangeOfValidIDs(void)
{
   for ( uint8_t i = 0; i < MAX_ID_ALLOWED; i++ )
   {
      TEST_ASSERT_EQUAL_UINT8( REFERENCE_PID_TABLE[i], ComputePID(i) );
   }
}

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
