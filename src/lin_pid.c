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
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "lin_pid.h"

/* Local Macro Definitions */
// Put an upper cap on how many characters will be processed from the input stream.

#define GET_BIT(x, n) ((x >> n) & 0x01)

#ifdef TEST
   #define STATIC // Set to nothing
#else
   #define STATIC static
#endif

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

#ifndef NDEBUG
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
#endif

/* Private Function Prototypes */

#ifndef NDEBUG
STATIC int UInt8_Cmp( const void * a, const void * b );
#endif
STATIC bool GetID( char const * str, uint8_t * id );
STATIC bool MyAtoI(char digit, uint8_t * converted_digit);
STATIC bool ArgsContain( char const * args[], char const * str );
static void PrintHelpMsg(void);
static void PrintReferenceTable(void);

/* Meat of the Program */

#ifdef TEST
int lin_pid_cli(int argc, char * argv[])
#else
int main(int argc, char * argv[])
#endif
{
   /* Local variables */
   uint8_t pid = 0xFFu;
   uint8_t user_input;
   int ret_val = EXIT_SUCCESS;

   /* Get user input */
   if ( argc >= 2 )
   {
      // First, check for --help or --table or -t
      if ( NULL == argv[1] )
      {
         fprintf(stderr, "\033[31margv[1] is NULL!\033[0m");
         ret_val = EXIT_FAILURE;
      }
      else if ( strcmp("--help", argv[1]) == 0 )
      {
         PrintHelpMsg();
      }
      else if
      (
         ( strcmp("--table", argv[1]) == 0 ) ||
         ( strcmp("-t",      argv[1]) == 0 )
      )
      {
         PrintReferenceTable();
      }
      else
      {
         // TODO: Support [hex | dec]
         bool digits_read_successfully = GetID(argv[1], &user_input);
         if ( !digits_read_successfully )
         {
            fprintf(stderr, "\n\033[31;1mInvalid user input.\033[0m\n");
            ret_val = EXIT_FAILURE;
         }
         else
         {
            /* Process input */
            if ( user_input > MAX_ID_ALLOWED )
            {
               fprintf(stderr, "\n\033[31mID is out of range! \033[31;1mID: 0x%-5X\033[0m\n", user_input);
               return EXIT_FAILURE;
            }
            pid = user_input;

            /* Perform computation */
            pid = ComputePID(pid);
            // PID should be of a certain subset of possible 8-bit ints...
            assert( (INVALID_PID == pid) ||
                    (bsearch( &pid,
                                SORTED_REFERENCE_PID_TABLE,
                                sizeof(SORTED_REFERENCE_PID_TABLE)/sizeof(uint8_t),
                                sizeof(uint8_t),
                                UInt8_Cmp ) != NULL) );

            /* Print Output */
            printf( "\nID:  \033[36m0x%02X\033[0m\n", user_input );
            printf( "PID: \033[32m0x%02X\033[0m\n", pid );

         }
      }
   }
   else
   {
      PrintHelpMsg();
   }

   return ret_val;
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

#define MAX_ARGS_TO_CHECK  5
#define MAX_ARG_LEN        (strlen("--no-new-line"))
STATIC bool ArgsContain( char const * args[], char const * str )
{
   if ( (NULL == args) || (NULL == str) )
   {
      return false;
   }

   bool ret_val = false;

   // Scan through str
   for ( uint8_t i = 0; i < MAX_ARGS_TO_CHECK; i++ )
   {
      if ( args[i] == NULL )
      {
         break;
      }
      else if ( (strncmp( str, args[i], MAX_ARG_LEN ) == 0) )
      {
         ret_val = true;
         break;
      }
      else
      {
         // Keep looking...
      }
   }

   return ret_val;
}

// Acceptable formats:
// Hex:     0xZZ, ZZ, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ, ZZ, Z
// Decimal: ZZd, ZZD
#define MAX_INPUT_CHARS 5  // Enough to cover the possible formats + margin for growth
STATIC bool GetID( char const * str, uint8_t * id )
{
   enum ParserState_E
   {
      ParserInit,
      ParserOneZeroIn,
      ParserHexPrefix,
      ParserIndeterminateOneDigitIn,
      ParserIndeterminateTwoDigitsIn,
      ParserDecDigits,
      ParserHexDigits,
      ParserTwoZerosIn,
      ParserTwoDigitsAlreadyRead
   } parser_state = ParserInit;

   // Check input validity
   if ( (NULL == str) || (NULL == id) )
   {
      return false;
   }

   char first_digit = '\0';
   char second_digit = '\0';
   bool ishex = false;
   bool isdec = false;
   bool ret_val = false;
   size_t idx = 0;
   size_t loop_limit_counter = 0;

   // Skip over any leading whitespace
   // TODO: Do I need to check for this? Can I trust that the terminal will not
   //       pass in white-space leading characters?
   while ( (loop_limit_counter <= MAX_INPUT_CHARS) &&
           (str[idx] != '\0') &&
           (isblank( (int)str[idx] )) )
   {
      idx++;
      loop_limit_counter++;
   }
   if ( str[idx] == '\0' )
   {
      // TODO: Throw exception: only encountered whitespace
      return false;
   }

   // Parser State Machine Time!
   loop_limit_counter = 0;
   bool exit_loop = false;
   while ( (loop_limit_counter <= MAX_INPUT_CHARS) && /* Upper-bound on loop to prevent ∞ */
           (str[idx] != '\0') && /* Keep going until we hit a terminating null character */
           !exit_loop /* Redundant here but I like extra guard rails... */ )
   {
      char ch = str[idx];

      switch (parser_state)
      {
         case ParserInit:
            if ( '0' == ch )
            {
               first_digit = ch;
               parser_state = ParserOneZeroIn;
            }
            else if ( isxdigit(ch) )
            {
               if ( isdigit(ch) )
               {
                  // Determined to be within hex range but also within dec range
                  // so, still indeterminate...
                  parser_state = ParserIndeterminateOneDigitIn;
               }
               else
               {
                  // Must be a uniquely hex digit
                  ishex = true;
                  parser_state = ParserHexDigits;
               }
               first_digit = ch;
            }
            else if ( ('x' == ch) || ('X' == ch) )
            {
               ishex = true;
               parser_state = ParserHexPrefix;
            }
            else
            {
               // TODO: Throw exception - invalid character
               exit_loop = true;
            }
            break;

         case ParserOneZeroIn:
            if ( '0' == ch )
            {
               second_digit = ch;
               parser_state = ParserTwoZerosIn;
            }
            else if ( ('x' == ch) || ('X' == ch) )
            {
               ishex = true;
               parser_state = ParserHexPrefix;
            }
            break;

         case ParserHexPrefix:
            if ( isxdigit(ch) )
            {
               first_digit = ch;
               parser_state = ParserHexDigits;
            }
            else
            {
               // Invalid character detected
               // TODO: Throw exception - invalid character
               exit_loop = true;
            }
            break;

         case ParserIndeterminateOneDigitIn:
            if ( isxdigit(ch) )
            {
               if ( isdigit(ch) )
               {
                  parser_state = ParserIndeterminateTwoDigitsIn;
               }
               else
               {
                  ishex = true;
                  parser_state = ParserHexDigits;
               }
               second_digit = ch;
            }
            else
            {
               // Invalid character detected
               // TODO: Throw exception - invalid character
               exit_loop = true;
            }
            break;
         
         case ParserIndeterminateTwoDigitsIn:
            if ( ('x' == ch) || ('X' == ch) || ('h' == ch) || ('H' == ch) )
            {
               ishex = true;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else if ( ('d' == ch) || ('D' == ch) )
            {
               isdec = true;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               // TODO: Throw exception - invalid character sequence
               exit_loop = true;
            }
            break;

         case ParserDecDigits:
            // TODO: Add in when I add support for -dec or --decimal flag
            break;
         
         case ParserHexDigits:
            if ( isxdigit(ch) )
            {
               second_digit = ch;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               // Invalid character detected
               // TODO: Throw exception - invalid character
               exit_loop = true;
            }
            break;

         case ParserTwoZerosIn:
            if ( ('x' == ch) || ('X' == ch) || ('h' == ch) || ('H' == ch) ||
                 ('d' == ch) || ('D' == ch) )
            {
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               // TODO: Throw exception - too many 0s in a row.
               exit_loop = true;
            }
            break;

         case ParserTwoDigitsAlreadyRead:
            // TODO: Throw exception - too many digits passed in.
            exit_loop = true;
            break;

         default:
            break;
      }
      
      if ( exit_loop )
      {
         break;
      }

      idx++;
      loop_limit_counter++;
   }
   
   // Logic should not determine that input was both hex and dec at the same time
   assert( !(ishex && isdec) );
   
   // Post state machine processing
   if ( loop_limit_counter >= MAX_INPUT_CHARS )
   {
      // TODO: Throw exception because we should have stopped sooner than that!
      // This means the string was not null-terminated.
   }
   else if ( exit_loop )
   {
      // Invalid digits were detected! Don't update ID.
   }
   else
   {
      // We got valid digits! Let's get that ID.
      uint8_t most_significant_digit = 0xFF;
      uint8_t least_significant_digit = 0xFF;

      if ( (second_digit == '\0') && (first_digit != '\0') )
      {
         // We only got one digit → assume it is hex
         most_significant_digit = 0x00u;
         bool conv = MyAtoI( first_digit, &least_significant_digit );
         ishex = true;
         // We should have handled the input well enough by here that the conversion runs successfully...
         assert( conv );
         // Obtained digits should in-range at this point
         assert( (most_significant_digit == 0x00) && (least_significant_digit <= 0x0F) );
      }
      else if ( (first_digit != '\0') && (second_digit != '\0') )
      {
         bool conv1 = MyAtoI( first_digit, &most_significant_digit );
         bool conv2 = MyAtoI( second_digit, &least_significant_digit );
         // We should have handled the input well enough by here that the conversion runs successfully...
         assert( conv1 && conv2 );
         // Digits should be <= 0xF
         assert( (most_significant_digit <= 0x0F) && (least_significant_digit <= 0x0F) );
      }

      // Digits obtained should be <= 0xF
      assert( (most_significant_digit <= 0x0F) && (least_significant_digit <= 0x0F) );

      // Final step!
      if ( ishex || !isdec )
      {
         *id = (uint8_t)( (most_significant_digit * 0x10) + least_significant_digit );
      }
      else  // Must be decimal
      {
         *id = (uint8_t)( (most_significant_digit * 10) + least_significant_digit );
      }

      ret_val = true;
   }
   
   return ret_val;
}

STATIC bool MyAtoI(char digit, uint8_t * converted_digit)
{
   bool ret_val = false;

   // Check input validity
   if ( ( converted_digit != NULL ) ||
          ( (digit >= '0') && (digit <= '9') ) ||
          ( (digit >= 'A') && (digit <= 'F') ) ||
          ( (digit >= 'a') && (digit <= 'f') )
      )
   {
      if ( (digit >= '0') && (digit <= '9') )
      {
         *converted_digit = (uint8_t)(digit - '0');
         ret_val = true;
      }
      else if ( (digit >= 'A') && (digit <= 'F') )
      {
         *converted_digit = (uint8_t)( 10 + (digit - 'A') );
         ret_val = true;
      }
      else
      {
         *converted_digit = (uint8_t)( 10 + (digit - 'a') );
         ret_val = true;
      }
   }

   return ret_val;
}

static void PrintHelpMsg(void)
{
   fprintf(stdout,
      "\n\033[36;4mLIN Protected Identifier (PID) Calculator\033[0m\n"

      "\nBasic Program usage:\n\n"

      "\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[34;1m<hex or decimal number>\033[0m \033[35m[(--hex | -h) | (--dec | -d)]\033[0m \033[;3mto get the PID that corresponds to an ID.\033[0m\n"
      "\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[34;1m<hex or decimal number>\033[0m \033[35m[(--hex | -h) | (--dec | -d)]\033[0m \033[35m(--quiet | -q)\033[0m \033[0m \033[35m[--no-new-line]\033[0m \033[;3msame as above but quieter and not colored.\033[0m\n"
      "\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[35m[--help]\033[0m \033[;3mto print the help message.\033[0m\n"
      "\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[35m[(--table | -t)]\033[0m \033[;3mto print a full LIN ID vs PID table for reference.\033[0m\n"

      "\n\033[;3mNote that deviations from the above usage will result in an\033[0m \033[31;3merror message\033[0m.\n"

      "\nSupported hexadecimal number formats:\n\n"

         "\t0xZZ, ZZ, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ, \033[;1mZZ\033[0m, Z, or \033[35mZZ (-h | --hex)\033[0m\n"

      "\nSupported decimal number formats:\n\n"

         "\tZZd, ZZD, or \033[35mZZ (-d | --dec)\033[0m\n"

      "\nHere are some examples of basic usage:\n\n"

         "\t\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[34;1m0x27\033[0m\033[0m --> \033[3m0xE7 will be included in the reply as the corresponding PID\n"
         "\t\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[34;1m27\033[0m\033[0m --> \033[3mHex assumed, so 0xE7 will be included in the reply as the corresponding PID\n"
         "\t\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[34;1m27d\033[0m\033[0m --> \033[3m0x1B will be included in the reply as the corresponding PID\n"
         "\t\033[33m<path>/\033[0m\033[36;1mlin_pid.exe\033[0m \033[34;1m27\033[0m \033[35m--dec\033[0m\033[0m --> \033[3m0x1B will be included in the reply as the corresponding PID\n"

      "\n\033[;3mNote that entering two digits with\033[0m \033[;4mno prefix/suffix\033[0m, \033[;3mby default, the number is assumed to be\033[0m \033[;1mhexadecimal\033[0m \033[;3munless the\033[0m \033[35m--dec\033[0m or \033[35m-d\033[0m \033[;3mflag is specified.\033[0m\n"
      "\033[;3mNote that deviations from the above supported formats will result in an\033[0m \033[31;3merror message\033[0m.\n"

      "\nContact \033[35m@memphis242\033[0m on GitHub or raise an issue in the \033[35;4mgithub.com/memphis242/lin_pid\033[0m repository if confusion remains. Cheers!\n\n"
   );
}

static void PrintReferenceTable(void)
{
   fprintf(stdout, "\n\033[35;4mReference Table\033[0m\n\n");
   fprintf(stdout, "---------------\n");
   fprintf(stdout, "|  \033[36mID\033[0m  |  \033[32mPID\033[0m |\n");
   fprintf(stdout, "---------------\n");
   for ( size_t i = 0; i < sizeof(REFERENCE_PID_TABLE); i++ )
   {
      fprintf(stdout, "| \033[36m0x%-3X\033[0m| \033[32m0x%-3X\033[0m|\n", (unsigned int)i, REFERENCE_PID_TABLE[i]);
   }
   fprintf(stdout, "---------------\n");
   fprintf(stdout, "\n");
}


#ifndef NDEBUG

int UInt8_Cmp( const void * a, const void * b )
{
   uint8_t * c = (uint8_t *)a;
   uint8_t * d = (uint8_t *)b;

   return ( *c - *d );
}

#endif
