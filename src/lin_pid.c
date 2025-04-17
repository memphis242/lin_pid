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
#include <assert.h>
#include "lin_pid.h"

/* Local Macro Definitions */
#define GET_BIT(x, n) ((x >> n) & 0x01)

/* Datatypes */

/* Local Data */

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

static const uint8_t SORTED_REFERENCE_PID_TABLE[MAX_ID_ALLOWED + 1] =
{
   0x03, 0x06, 0x08, 0x0D, 0x11, 0x14, 0x1A, 0x1F,
   0x20, 0x25, 0x2B, 0x2E, 0x32, 0x37, 0x39, 0x3C,
   0x42, 0x47, 0x49, 0x4C, 0x50, 0x55, 0x5B, 0x5E,
   0x61, 0x64, 0x6A, 0x6F, 0x73, 0x76, 0x78, 0x7D,
   0x80, 0x85, 0x8B, 0x8E, 0x92, 0x97, 0x99, 0x9C,
   0xA3, 0xA6, 0xA8, 0xAD, 0xB1, 0xB4, 0xBA, 0xBF,
   0xC1, 0xC4, 0xCA, 0xCF, 0xD3, 0xD6, 0xD8, 0xDD,
   0xE2, 0xE7, 0xE9, 0xEC, 0xF0, 0xF5, 0xFB, 0xFE
};

/* Private Function Prototypes */

int uint8_cmp( const void * a, const void * b );

/* Meat of the Program */

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
   else
   {
      // TODO: Instead of an error, print a help
      fprintf(stdout, "No ID passed in.");
      return EXIT_SUCCESS;
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
   // PID should be of a certain subset of possible 8-bit ints...
   assert( (INVALID_PID == pid) ||
           (bsearch( &pid,
                     SORTED_REFERENCE_PID_TABLE,
                     sizeof(SORTED_REFERENCE_PID_TABLE)/sizeof(uint8_t),
                     sizeof(uint8_t),
                     uint8_cmp ) != NULL) );

   /* Print Output */
   printf( "ID:  0x%02X\n", user_input );
   printf( "PID: 0x%02X\n", pid );

   return EXIT_SUCCESS;
}

/* Public Function Implementations */

uint8_t ComputePID(uint8_t id)
{
   uint8_t pid = id;

   if ( id > MAX_ID_ALLOWED )
   {
      // TODO: Throw exception
      return INVALID_PID;
   }

   // Do the calculation instead of a look-up just for the practice. There's hardly
   // an extra cost to this.
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

   // The calculation should produce the same result as if we just did a table look-up
   assert( pid == REFERENCE_PID_TABLE[id] );

   return pid;
}

int uint8_cmp( const void * a, const void * b )
{
   uint8_t * c = (uint8_t *)a;
   uint8_t * d = (uint8_t *)b;

   return (int)( *c - *d );
}
