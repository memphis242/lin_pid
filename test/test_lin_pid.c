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
    // TODO
    TEST_ASSERT_EQUAL_UINT8( 0x80u, ComputePID(0x00) );
}

/* CLI */
// TODO
