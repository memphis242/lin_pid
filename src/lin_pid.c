/*!
 * @file    lid_pid.c
 * @brief   Compute the PID given an ID, and sprinke some CLI
 *          and possibly a non-terminal GUI on top.
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
#define MAX_ARGS_TO_CHECK  5  // e.g., lin_pid XX --hex --quiet --no-new-line
#define MAX_NUM_LEN        (strlen("0x3F") + 1)
#define MAX_ARG_LEN        (strlen("--no-new-line"))
#define MAX_ERR_MSG_LEN    100

#define GET_BIT(x, n)      ((x >> n) & 0x01)

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

#define LIN_PID_EXCEPTION(enum, err_msg) "\n\033[31;1mError: " err_msg "\033[0m\n\n",

static const char * ErrorMsgs[NUM_OF_EXCEPTIONS] =
{
   #include "lin_pid_exceptions.h"
};

#undef LIN_PID_EXCEPTION

/* Private Function Prototypes */

#ifndef NDEBUG

STATIC int UInt8_Cmp( const void * a, const void * b );

#endif

STATIC enum LIN_PID_Result_E GetID( char const * str,
                                    uint8_t * id,
                                    bool pre_emptively_hex,
                                    bool pre_emptively_dec );

STATIC bool MyAtoI(char digit, uint8_t * converted_digit);

STATIC bool ArgsContain( char const * args[],
                         char const * str,
                         int argc,
                         uint8_t * idx );

static void PrintHelpMsg(void);

static void PrintReferenceTable(void);

/* Meat of the Program */

#ifdef TEST
int lin_pid_cli(int argc, char * argv[])
#else
int main(int argc, char * argv[])
#endif
{
   /* Early return opportunities */
   if ( argc > MAX_ARGS_TO_CHECK )
   {
      fprintf(stderr, "%.*s", MAX_ERR_MSG_LEN, ErrorMsgs[TooManyInputArgs]);
      return EXIT_FAILURE;
   }

   else if ( (1 == argc) || ( strcmp("--help", argv[1]) == 0 ) )
   {
      PrintHelpMsg();
      return EXIT_SUCCESS;
   }

   else if
   (
      (2 == argc) &&
      (
         ( strcmp("--table", argv[1]) == 0 ) ||
         ( strcmp("-t",      argv[1]) == 0 )
      )
   )
   {
      PrintReferenceTable();
      return EXIT_SUCCESS;
   }

   else
   {
      bool ishex = false;
      bool isdec = false;
      uint8_t pid = 0xFFu;
      uint8_t user_input;
      enum LIN_PID_Result_E result_status;
      uint8_t idx_of_arg = 0;
      const char * id_arg = argv[1];

      if ( ArgsContain( (const char **)argv, "--hex", argc, &idx_of_arg ) ||
           ArgsContain( (const char **)argv, "-h"   , argc, &idx_of_arg ) )
      {
         ishex = true;
         if ( 1 == idx_of_arg ) id_arg = argv[2];
      }

      if ( ArgsContain( (const char **)argv, "--dec", argc, &idx_of_arg ) ||
           ArgsContain( (const char **)argv, "-d"   , argc, &idx_of_arg ) )
      {
         isdec = true;
         if ( 1 == idx_of_arg ) id_arg = argv[2];
      }

      if ( ishex && isdec )
      {
         fprintf(stderr, "%.*s", MAX_ERR_MSG_LEN, ErrorMsgs[HexAndDecFlagsSimultaneouslyUsed]);
         return EXIT_FAILURE;
      }

      /* Now try to get the ID the user provided... */
      result_status = GetID(id_arg, &user_input, ishex, isdec);
      assert( ((int)result_status >= 0) && ((int)result_status < NUM_OF_EXCEPTIONS) );
      if ( GoodResult != result_status )
      {
         fprintf(stderr, "%.*s", MAX_ERR_MSG_LEN, ErrorMsgs[result_status]);
         return EXIT_FAILURE;
      }

      /* Process input */
      if ( user_input > MAX_ID_ALLOWED )
      {
         fprintf(stderr, "%.*s", MAX_ERR_MSG_LEN, ErrorMsgs[ID_OOR]);
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
      if ( ArgsContain((const char **)argv, "--quiet", argc, &idx_of_arg) ||
           ArgsContain((const char **)argv, "-q", argc, &idx_of_arg) )
      {
         if ( !ArgsContain( (const char **)argv, "--no-new-line", argc, &idx_of_arg ) )
         {
            printf("%02X\n", pid);
         }
         else
         {
            printf("%02X", pid);
         }
      }
      else
      {
         printf( "\nID:  \033[36m" "0x%02X" "\033[0m\n", user_input );
         printf( "PID: \033[32m"   "0x%02X" "\033[0m\n", pid );
         printf("\n");
      }

   }

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

STATIC bool ArgsContain( char const * args[],
                         char const * str,
                         int argc,
                         uint8_t * idx )
{
   // Assert instead of return false because this function is internal and we
   // have control on what it is called with.
   assert( (args != NULL) && (str != NULL) && (argc >= 2) && (idx != NULL) );

   bool ret_val = false;
   const uint8_t MAX_ARGS = (argc < MAX_ARGS_TO_CHECK) ?
                              (uint8_t)argc :
                              MAX_ARGS_TO_CHECK;

   // Scan through str
   for ( uint8_t i = 1; i < MAX_ARGS; i++ )
   {
      if ( args[i] == NULL )
      {
         break;
      }
      else if ( (strncmp( str, args[i], MAX_ARG_LEN ) == 0) )
      {
         ret_val = true;
         *idx = i;
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
// Hex:     0xZZ, Z, ZZ, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ
// Decimal: ZZd, ZZD
STATIC enum LIN_PID_Result_E GetID( char const * str,
                                    uint8_t * id,
                                    bool pre_emptively_hex,
                                    bool pre_emptively_dec )
{
   assert( (str != NULL) &&
           (id  != NULL) &&
           (!pre_emptively_hex || !pre_emptively_dec) );

   enum LIN_PID_Result_E result = GoodResult;
   size_t idx = 0;
   size_t loop_limit_counter = 0;

   // Skip over any leading whitespace
   // TODO: Do I need to check for this? Can I trust that the terminal will not
   //       pass in white-space leading characters?
   while (
      /* x2 as max allowance of leading whitespace */
      (loop_limit_counter <= (MAX_NUM_LEN * 2)) &&
      (str[idx] != '\0') &&
      (isblank( (int)str[idx] ))
   )
   {
      idx++;
      loop_limit_counter++;
   }
   if ( str[idx] == '\0' )
   {
      return WhiteSpaceOnlyIDArg;
   }

   // Parser State Machine Time!
   enum ParserState_E
   {
      ParserInit,
      ParserOneZeroIn,
      ParserHexPrefix,
      ParserIndeterminateOneDigitIn,
      ParserIndeterminateTwoDigitsIn,
      ParserDecDigits,
      ParserTwoDecDigits,
      ParserHexDigits,
      ParserTwoZerosIn,
      ParserTwoDigitsAlreadyRead,
      ParserPreemptivelyHex,
      ParserPreemptivelyDec,
      ParserPreemptivelyDecOneZeroIn,
      ParserPreemptivelyDecTwoZerosIn
   } parser_state;
   loop_limit_counter = 0;
   bool exit_loop = false;
   char first_digit = '\0';
   char second_digit = '\0';
   bool ishex = pre_emptively_hex;
   bool isdec = pre_emptively_dec;

   if ( pre_emptively_hex )
   {
      parser_state = ParserPreemptivelyHex;
   }
   else if ( pre_emptively_dec )
   {
      parser_state = ParserPreemptivelyDec;
   }
   else
   {
      parser_state = ParserInit;
   }

   while (
      (loop_limit_counter <= MAX_NUM_LEN) &&
      (str[idx] != '\0') && /* Continue until terminating null character */
      !exit_loop /* Redundant here but I like extra guard rails */
   )
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
               result = InvalidCharacterEncountered_FirstChar;
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
               result = InvalidDigitEncountered_FirstDigit;
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
               result = InvalidDigitEncountered_SecondDigit;
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
               result = TooManyDigitsEntered;
               exit_loop = true;
            }
            break;

         case ParserDecDigits:
            if ( ('x' == ch) || ('X' == ch) || (isxdigit(ch) && !isdigit(ch)) )
            {
               result = HexDigitEncounteredUnderDecSetting_SecondDigit;
               exit_loop = true;
            }
            else if ( isdigit(ch) )
            {
               second_digit = ch;
               parser_state = ParserTwoDecDigits;
            }
            else
            {
               result = InvalidDigitEncountered_SecondDigit;
               exit_loop = true;
            }
            break;

         case ParserTwoDecDigits:
            // Two decimal digits will have already been read in...
            if ( ('d' == ch) || ('D' == ch) )
            {
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = InvalidDecimalSuffixEncountered;
               exit_loop = true;
            }
            break;
         
         case ParserHexDigits:
            if ( isxdigit(ch) )
            {
               second_digit = ch;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = InvalidDigitEncountered_SecondDigit;
               exit_loop = true;
            }
            break;

         case ParserTwoZerosIn:
            if ( ( ishex &&
                     ( ('x' == ch) || ('X' == ch) || ('h' == ch) || ('H' == ch) ) ) ||
                 ( isdec &&
                     ( ('d' == ch) || ('D' == ch) ) )
               )
            {
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = TooManyDigitsEntered;
               exit_loop = true;
            }
            break;

         case ParserTwoDigitsAlreadyRead:
            result = TooManyDigitsEntered;
            exit_loop = true;
            break;

         case ParserPreemptivelyHex:
            if ( '0' == ch )
            {
               parser_state = ParserOneZeroIn;
            }
            else if ( ('x' == ch) || ('X' == ch) )
            {
               parser_state = ParserHexPrefix;
            }
            else if ( isxdigit(ch) )
            {
               first_digit = ch;
               parser_state = ParserHexDigits;
            }
            else
            {
               result = InvalidCharacterEncountered_FirstChar;
               exit_loop = true;
            }
            break;

         case ParserPreemptivelyDec:
            if ( ('x' == ch) || ('X' == ch) || (isxdigit(ch) && !isdigit(ch)) )
            {
               result = HexDigitEncounteredUnderDecSetting_FirstDigit;
               exit_loop = true;
            }
            else if ( '0' == ch )
            {
               parser_state = ParserPreemptivelyDecOneZeroIn;
            }
            else if ( isdigit(ch) )
            {
               first_digit = ch;
               parser_state = ParserDecDigits;
            }
            else
            {
               result = InvalidCharacterEncountered_FirstChar;
               exit_loop = true;
            }
            break;

         case ParserPreemptivelyDecOneZeroIn:
            if ( ('x' == ch) || ('X' == ch) || (isxdigit(ch) && !isdigit(ch)) )
            {
               result = HexDigitEncounteredUnderDecSetting_SecondDigit;
               exit_loop = true;
            }
            else if ( '0' == ch )
            {
               parser_state = ParserPreemptivelyDecTwoZerosIn;
            }
            else if ( isdigit(ch) )
            {
               first_digit = ch;
               parser_state = ParserDecDigits;
            }
            else
            {
               result = InvalidCharacterEncountered_SecondChar;
               exit_loop = true;
            }
            break;

         case ParserPreemptivelyDecTwoZerosIn:
            if ( ('d' == ch) || ('D' == ch) )
            {
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = InvalidDecimalSuffixEncountered;
               exit_loop = true;
            }
            break;

         default:
            break;
      }
      
      // Do we need to exit the loop pre-emptively due to an erroneous situation?
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
   if ( loop_limit_counter >= MAX_NUM_LEN )
   {
      result = TooManyDigitsEntered;
   }
   else if ( exit_loop )
   {
      // Something erroneous was detected. Don't update ID!
   }
   else
   {
      // We got valid digits! Let's get that ID.
      uint8_t most_significant_digit = 0xFF;
      uint8_t least_significant_digit = 0xFF;
      // By this point, I assume we have at least one digit placed in first_digit.
      // If nothing is in first_digit, nothing should also be in the second_digit.
      assert( (first_digit != '\0') || (second_digit) == '\0' );

      if ( (first_digit != '\0') && (second_digit == '\0') )
      {
         most_significant_digit = 0x00u;
         bool conv = MyAtoI( first_digit, &least_significant_digit );
         assert( conv );
         assert( (most_significant_digit == 0x00) && (least_significant_digit <= 0x0F) );
      }
      else if ( (first_digit != '\0') && (second_digit != '\0') )
      {
         bool conv1 = MyAtoI( first_digit, &most_significant_digit );
         bool conv2 = MyAtoI( second_digit, &least_significant_digit );
         assert( conv1 && conv2 );
         assert( (most_significant_digit <= 0x0F) && (least_significant_digit <= 0x0F) );
      }
      else
      {
         most_significant_digit = 0;
         least_significant_digit = 0;
      }

      // Digits obtained should be <= 0xF
      assert( (most_significant_digit <= 0x0F) && (least_significant_digit <= 0x0F) );

      // Final step!
      if ( (0 == most_significant_digit) && (0 == least_significant_digit) )
      {
         *id = 0x00u;
      }
      else if ( ishex || !isdec )
      {
         *id = (uint8_t)( (most_significant_digit * 0x10) + least_significant_digit );
      }
      else  // Must be decimal
      {
         *id = (uint8_t)( (most_significant_digit * 10) + least_significant_digit );
      }
   }
   
   return result;
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

      "\033[0m\033[36;1mlin_pid\033[0m \033[35m[FORMAT]\033[0m \033[34;1m<hex or dec num>\033[0m \033[;3mto get the PID that corresponds to an ID.\033[0m\n"
      "\033[0m\033[36;1mlin_pid\033[0m \033[35m[FORMAT]\033[0m \033[34;1m<hex or dec num>\033[0m \033[35m(--quiet | -q)\033[0m \033[0m \033[35m[--no-new-line]\033[0m \033[;3msame as above but quieter and not colored.\033[0m\n"
      "\033[0m\033[36;1mlin_pid\033[0m \033[35m[--help]\033[0m \033[;3mto print the help message.\033[0m\n"
      "\033[0m\033[36;1mlin_pid\033[0m \033[35m(--table | -t)\033[0m \033[;3mto print a full LIN ID vs PID table for reference.\033[0m\n"

      "\n\033[;3mNote that deviations from the above usage will result in an\033[0m \033[31;3merror message\033[0m.\n"

      "\n\033[35mFORMAT\033[0m is either:"
         "\n\t\033[35m--hex\033[0m or \033[35m-h\033[0m for \033[;3mhexadecimal (base-16)\033[0m entries"
         "\n\t\033[35m--dec\033[0m or \033[35m-d\033[0m for \033[;3mdecimal (base-10)\033[0m entries"

      "\n\n\033[;3mNote that the\033[0m \033[35mFORMAT\033[0m \033[;3mflag can actually be placed either \033[;1mbefore or after\033[0m \033[;3mthe number entry, but\033[0m \033[;1mnot both.\033[0m"

      "\n\nSupported hexadecimal number formats:\n\n"

         "\t0xZZ, ZZ, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ, \033[;1mZZ\033[0m, Z, \033[35m(-h | --hex) ZZ\033[0m, or \033[35mZZ (-h | --hex)\033[0m\n"

      "\nSupported decimal number formats:\n\n"

         "\tZZd, ZZD, \033[35m(-d | --dec) ZZ\033[0m, or \033[35mZZ (-d | --dec)\033[0m\n"

      "\nHere are some \033[32mexamples\033[0m of basic usage:\n\n"

         "\t\033[0m\033[36;1mlin_pid\033[0m \033[34;1m0x27\033[0m\033[0m --> \033[3m0xE7 will be included in the reply as the corresponding PID\n"
         "\t\033[0m\033[36;1mlin_pid\033[0m \033[34;1m27\033[0m\033[0m --> \033[3mHex assumed, so 0xE7 will be included in the reply as the corresponding PID\n"
         "\t\033[0m\033[36;1mlin_pid\033[0m \033[34;1m27d\033[0m\033[0m --> \033[3m0x1B will be included in the reply as the corresponding PID\n"
         "\t\033[0m\033[36;1mlin_pid\033[0m \033[34;1m27\033[0m \033[35m--dec\033[0m\033[0m --> \033[3m0x1B will be included in the reply as the corresponding PID\n"
         "\t\033[0m\033[36;1mlin_pid\033[0m \033[35m--dec\033[0m\033[0m \033[34;1m27\033[0m --> \033[3msame as above\n"

      "\n\033[;3mNote that two digits entries\033[0m \033[;4mwithout a prefix/suffix\033[0m, \033[;3mby default, are assumed to be\033[0m \033[;1mhexadecimal\033[0m \033[;3munless the\033[0m \033[35m--dec\033[0m or \033[35m-d\033[0m \033[;3mflag is specified.\033[0m\n"

      "\nContact \033[35m@memphis242\033[0m on GitHub or raise an issue in the \033[35;4mgithub.com/memphis242/lin_pid\033[0m repository if confusion remains or issues are encountered. Cheers!\n\n"
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

STATIC int UInt8_Cmp( const void * a, const void * b )
{
   uint8_t * c = (uint8_t *)a;
   uint8_t * d = (uint8_t *)b;

   return ( *c - *d );
}

#endif
