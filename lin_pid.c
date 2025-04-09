/*!
 * \file    lid_pid.c
 * \brief   Compute the PID given an ID, and sprinke some CLI and possibly a GUI on top.
 * 
 * \author  Abdullah Almosalami @memphis242
 * \date    Wed Apr 9, 2025
 */

/* File Inclusions */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Local Macro Definitions */
#define GET_BIT(x, n) ((x >> n) & 0x01)

/* Datatypes */

/* Meat of the Program */
/**
 * @brief Brief description of the function.
 *
 * Detailed description of what the function does, if necessary.
 *
 * @param[in] param1 Description of the first input parameter.
 * @param[in] param2 Description of the second input parameter.
 * @param[out] param3 Description of the output parameter, if applicable.
 * @return Description of the return value, if any.
 */
int main(void)
{
   /* Local variables */
   uint8_t pid = 0xFFu;
   unsigned int user_input;

   /* Get user input */
   printf("Input ID: ");
   // TODO: Use an alternative to scanf()
   // TODO: What about negative input?
   // TODO: What about non-hexadecimal digit inputs?
   scanf("%X", &user_input);

   /* Process input */
   if ( user_input > 0x3F )
   {
      fprintf(stderr, "ID is out of range!\n");
      return EXIT_FAILURE;
   }
   pid = (uint8_t)user_input;

   // TODO: FSM state to print out welcome message and options */
   // TODO: FSM state to print a full table of all possible PIDs */

   /* Perform computation */
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
   pid |= ( ( GET_BIT(pid, 0) ^ GET_BIT(pid, 1) ^ GET_BIT(pid, 2) ^ GET_BIT(pid, 4) ) << 6 );
   pid |= (!( GET_BIT(pid, 1) ^ GET_BIT(pid, 3) ^ GET_BIT(pid, 4) ^ GET_BIT(pid, 5) ) << 7 );

   printf( "ID:  0x%02X\n", user_input );
   printf( "PID: 0x%02X\n", pid );

   return EXIT_SUCCESS;
}