/*!
 * @file    lid_pid.c
 * @brief   Compute the PID given an ID, and sprinke some CLI and possibly a GUI on top.
 * 
 * @author  Abdullah Almosalami @memphis242
 * @date    Wed Apr 9, 2025
 * @copyright MIT License
 */

/* File Inclusions */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "lin_pid.h"

/* Local Macro Definitions */
#define GET_BIT(x, n) ((x >> n) & 0x01)

/* Datatypes */

/* Meat of the Program */
/**
 * @fn main
 *
 * @param[in] argc The number of tokens passed in
 * @param[in] argv Array of strings that contain the tokens in string form
 * @note Refer to 5.1.2.2.1 Program startup of the C11 (and probably other)
 *       standards for the expectations around the main function
 *
 * @return int value : stdlib.h macros : EXIT_SUCCESS or EXIT_FAILURE
 */
#ifdef TEST
int lin_pid_cli(int argc, char * argv[])
#else
int main(int argc, char * argv[])
#endif
{
   /* Local variables */
   uint8_t pid = 0xFFu;
   unsigned int user_input;

   /* Get user input */
   // printf("Input ID: ");
   // TODO: Use an alternative to scanf()
   // TODO: What about negative input?
   // TODO: What about non-hexadecimal digit inputs?
   if ( argc >= 2 )
   {
      sscanf(argv[1], "%X", &user_input);
   }

   /* Process input */
   if ( user_input > MAX_ID_ALLOWED )
   {
      fprintf(stderr, "ID is out of range!\nID: 0x%-5X\n", user_input);
      return EXIT_FAILURE;
   }
   pid = (uint8_t)user_input;

   // TODO: FSM state to print out welcome message and options */
   // TODO: FSM state to print a full table of all possible PIDs */

   /* Perform computation */
   pid = ComputePID(pid);

   /* Print Output */
   printf( "ID:  0x%02X\n", user_input );
   printf( "PID: 0x%02X\n", pid );

   return EXIT_SUCCESS;
}

uint8_t ComputePID(uint8_t id)
{
   uint8_t pid = id;

   if ( id > MAX_ID_ALLOWED )
   {
      // TODO: Throw exception
      return 0xFFu;
   }

   // From the LIN Protocol Specification 2.1, section 2.3.1.3 Protected identifier field
   // 
   // The ...
   //   "... protected identifier field consists of two sub-fields; the frame
   //    identifier and the parity. Bits 0 to 5 are the frame identifier and
   //    bits 6 and 7 are the parity."
   // 
   // To calculate the parity bits, the following equations are given:
   //    P0 = ID0 xor ID1 xor ID2 xor ID4
   //    P1 = neg( ID1 xor ID3 xor ID4 xor ID5 )
   // 
   // So, the eventual bits will look like, MSb first:
   //   P1 P0 ID5 ID4 ID3 ID2 ID1 ID0
   pid |= (uint8_t)( ( GET_BIT(pid, 0) ^ GET_BIT(pid, 1) ^ GET_BIT(pid, 2) ^ GET_BIT(pid, 4) ) << 6 );
   pid |= (uint8_t)(!( GET_BIT(pid, 1) ^ GET_BIT(pid, 3) ^ GET_BIT(pid, 4) ^ GET_BIT(pid, 5) ) << 7 );

   return pid;
}

