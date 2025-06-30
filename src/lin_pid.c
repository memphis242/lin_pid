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

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#ifndef _POSIX_VERSION
#error "No options available for checking stdin in a non-blocking manner"
#endif
#endif

#include "re.h"
#include "lin_pid.h"

/* Local Macro Definitions */
#define MAX_ARGS_TO_CHECK              5  // e.g., lin_pid XX --hex --quiet --no-new-line
#define MAX_NUM_LEN                    (strlen("0x3F") + 1)
#define MAX_ARG_LEN                    (strlen("--no-new-line"))
#define MAX_ERR_MSG_LEN                250
#define NO_SPECIAL_COMP_FLAGS          0

#define GET_BIT(x, n)      ((x >> n) & 0x01)

#ifdef TEST
   #define STATIC // Set to nothing
#else
   #define STATIC static
#endif

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

struct NumericFormatStrings_S
{
   const char * regex_pattern;
   const char * print_format;
   const bool ishex;
   const bool isdec;
};


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

STATIC const char * AcceptableFlags[] =
{
   "--hex",
   "-h",
   "--dec",
   "-d",
   "--quiet",
   "-q",
   "--table",
   "-t",
   "--help",
   "--no-new-line"
};

#define LIN_PID_NUMERIC_FORMAT( enum, regexp, prnt_fmt, ish, isd ) \
   {                                                               \
      .regex_pattern = regexp,                                     \
      .print_format  = prnt_fmt,                                   \
      .ishex = ish,                                                \
      .isdec = isd                                                 \
   },

const struct NumericFormatStrings_S NumericFormats[] =
{
   #include "lin_pid_supported_formats.h"
};

#undef LIN_PID_NUMERIC_FORMAT

/* Private Function Prototypes */

STATIC bool OnlyValidFlagsArePresent( char const * args[], int argc );

STATIC bool InputIsPiped(void);

STATIC char * ReadNextStdInToken(void);

STATIC size_t ArgOccurrenceCount( char const * args[],
                                  char const * str,
                                  int argc,
                                  uint8_t * idx_of_first_occurrence );

STATIC enum LIN_PID_Result_E GetID( const char * str,
                                    uint8_t * id,
                                    bool * ishex,
                                    bool * isdec );

STATIC bool MyAtoI(char digit, uint8_t * converted_digit);

#ifndef NDEBUG
STATIC int UInt8_Cmp( const void * a, const void * b );
#endif

STATIC enum NumericFormat_E DetermineEntryFormat( const char * str,
                                                  bool ishex,
                                                  bool isdec );

static void PrintHelpMsg(void);

static void PrintReferenceTable(void);

static void PrintErrMsg(enum LIN_PID_Result_E err);


/* Meat of the Program */

#ifdef TEST
int lin_pid_cli( int argc, char * argv[] )
#else
int main( int argc, char * argv[] )
#endif
{
   /* Early return opportunities */
   if ( argc > MAX_ARGS_TO_CHECK )
   {
      PrintErrMsg(TooManyInputArgs);
      return EXIT_FAILURE;
   }

   else if ( !OnlyValidFlagsArePresent( (const char **)argv, argc) )
   {
      PrintErrMsg(InvalidFlagDetected);
      return EXIT_FAILURE;
   }

   else if ( InputIsPiped() )
   {
      printf("Input pipe detected.");

      // TODO: Input is piped, so process data that way
      // Split input by spaces or ',' by default
      // Vectorize... → Use ccol!
      // Need vector of strings...
      struct Vector_S * num_list = VectorInit(
                                       sizeof(char *), // element size
                                       8, // initial capacity
                                       65536, // maximum capacity
                                       0 // initial length
                                    );
      // Oh boy... This is gonna be painful to manually manage. I now miss
      // C++'s RAII...
      do
      {
         char * next_str = ReadNextStdInToken();
         VectorPush(next_str);
      } while( next_str != NULL );

      return EXIT_SUCCESS;
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
      const char * id_arg = argv[1];   // By default, first arg is numeric entry

      // Check format arguments
      uint8_t idx_of_arg_h   = 0;
      uint8_t idx_of_arg_hex = 0;
      uint8_t idx_of_arg_d   = 0;
      uint8_t idx_of_arg_dec = 0;
      size_t arg_count_h   = ArgOccurrenceCount( (const char **)argv, "-h",    argc, &idx_of_arg_h );
      size_t arg_count_hex = ArgOccurrenceCount( (const char **)argv, "--hex", argc, &idx_of_arg_hex );
      size_t arg_count_d   = ArgOccurrenceCount( (const char **)argv, "-d",    argc, &idx_of_arg_d );
      size_t arg_count_dec = ArgOccurrenceCount( (const char **)argv, "--dec", argc, &idx_of_arg_dec );

      if ( ( (arg_count_h > 0) && (arg_count_hex > 0) ) ||
             (arg_count_h > 1) || (arg_count_hex > 1) ||
             (arg_count_d > 1) || (arg_count_dec > 1) ||
           ( (arg_count_d > 0) && (arg_count_dec > 0) )
         )
      {
         PrintErrMsg(DuplicateFormatFlagsUsed);
         return EXIT_FAILURE;
      }

      if ( (arg_count_h > 0) || (arg_count_hex > 0) )
      {
         ishex = true;
         if ( ( (1 == idx_of_arg_h) || (1 == idx_of_arg_hex) ) && argv[2][0] != '-' )
         {
            id_arg = argv[2];
         }
         else if ( ( (2 == idx_of_arg_h) || (2 == idx_of_arg_hex) ) && argv[1][0] != '-' )
         {
            id_arg = argv[1];
         }
         else
         {
            PrintErrMsg(InvalidPositionOfNumber);
            return EXIT_FAILURE;
         }
      }

      if ( (arg_count_d > 0) || (arg_count_dec > 0) )
      {
         isdec = true;
         if ( ( (1 == idx_of_arg_d) || (1 == idx_of_arg_dec) ) && argv[2][0] != '-' )
         {
            id_arg = argv[2];
         }
         else if ( ( (2 == idx_of_arg_d) || (2 == idx_of_arg_dec) ) && argv[1][0] != '-' )
         {
            id_arg = argv[1];
         }
         else
         {
            PrintErrMsg(InvalidPositionOfNumber);
            return EXIT_FAILURE;
         }
      }

      if ( ishex && isdec )
      {
         PrintErrMsg(HexAndDecFlagsSimultaneouslyUsed);
         return EXIT_FAILURE;
      }

      assert(id_arg != NULL);

      /* Now try to get the ID the user provided... */
      result_status = GetID(id_arg, &user_input, &ishex, &isdec);
      assert( ((int)result_status >= 0) && (result_status < NUM_OF_EXCEPTIONS) );
      if ( GoodResult != result_status )
      {
         PrintErrMsg(result_status);
         return EXIT_FAILURE;
      }

      /* Process input */
      if ( user_input > MAX_ID_ALLOWED )
      {
         PrintErrMsg(ID_OOR);
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

      /* Determine format to print output in */
      enum NumericFormat_E num_format = DetermineEntryFormat(id_arg, ishex, isdec);
      assert( (int)num_format < NUM_OF_NUMERIC_FORMATS );
      //enum NumericFormat_E num_format = ClassicHexPrefix_LeadingZeros_Uppercase;
      const char * print_format = NumericFormats[ (unsigned int)num_format ].print_format;

      /* Print Output */

#ifdef __GNUC__
/* -Wformat-nonliteral suppression:
* Specifically, the warning is:
*
* src/lin_pid.c:307:13: warning: format not a string literal, argument types not checked [-Wformat-nonliteral]
*   307 |             printf(print_format, pid);
*       |             ^~~~~~
* 
*
* As long as nobody puts anything dumb or malicious in lin_pid_supported_formats.h for
* the format string, we should be fine with this otherwise unsafe situation.
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
      if ( (ArgOccurrenceCount((const char **)argv, "--quiet", argc, NULL) > 0) ||
           (ArgOccurrenceCount((const char **)argv, "-q", argc, NULL) > 0) )
      {
         if ( (ArgOccurrenceCount((const char **)argv, "--no-new-line", argc, NULL) == 0) )
         {
            printf(print_format, pid);
            printf("\n");
         }
         else
         {
            printf(print_format, pid);
         }
      }
      else if ( (ArgOccurrenceCount((const char **)argv, "--no-new-line", argc, NULL) > 0) )
      {
         PrintErrMsg(CantUseNoNewLineWithoutQuiet);
         return EXIT_FAILURE;
      }
      else
      {
         printf( "\n%-5s\033[36m", "ID: " );
         printf( print_format, user_input );
         printf( "\033[0m\n" );
         printf( "%-5s\033[32m", "PID:" );
         printf( print_format, pid );
         printf( "\033[0m\n" );
         printf("\n");
      }
   
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

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

STATIC bool OnlyValidFlagsArePresent( char const * args[], int argc )
{
   assert(args != NULL);
   assert(argc >= 1);
   assert( (size_t)argc <= ( (sizeof(AcceptableFlags) / sizeof(AcceptableFlags[0])) + 1) );

   bool only_valid = true;
   for ( uint8_t i = 1; i < argc; i++ )
   {
      if ( NULL == args[i] )
      {
         only_valid = false;
         break;
      }
      else if (
           ( (args[i][0] >= '0') && (args[i][0] <= '9') ) ||  /* General Digits */
           ( (tolower(args[i][0]) == 'x') && (tolower(args[i][0]) == 'h') ) || /* Hex prefixes*/
           ( (tolower(args[i][0]) >= 'a')  && (tolower(args[i][0]) <= 'f') ) /* Unique Hex Digits */
         )
      {
         continue;   // This is the number argument. Skip it.
      }
      else if ( args[i][0] != '-' )
      {
         // Not a number argument but also doesn't start with a dash == Invalid
         only_valid = false;
         break;
      }
      else
      {
         only_valid = false;
         for ( size_t j = 0; j < (sizeof(AcceptableFlags) / sizeof(char *)); j++ )
         {
            if ( (strncmp( AcceptableFlags[j], args[i], MAX_ARG_LEN ) == 0) )
            {
               only_valid = true;
               break;
            }
         }

         if ( !only_valid )
         {
            break;
         }
      }
   }

   return only_valid;
}

STATIC bool InputIsPiped(void)
{
#ifdef _WIN32
   // Windows way
   size_t num_of_bytes_avail = 0;

   HANDLE h_stdin = GetStdHandle(STD_INPUT_HANDLE);
   DWORD bytes_avail = 0;

   assert( h_stdin != INVALID_HANDLE_VALUE );
   bool pipe_check_went_thru =
      PeekNamedPipe(
         h_stdin,       // HANDLE hNamedPipe [in]
         NULL,          // LPVOID lpBuffer [out, optional]
         0,             // DWORD nBufferSize [in]
         NULL,          // LPDWORD lpBytesRead [out, optional]
         &bytes_avail,  // LPDWORD lpTotalBytesAvail [out, optional]
         NULL           // LPDWORD lpBytesLeftThisMessage [out, optional]
      );

   num_of_bytes_avail = pipe_check_went_thru ? (size_t)bytes_avail : 0;
   
   return (num_of_bytes_avail > 0);
#else
   // POSIX way
   fd_set read_fds;  // Set of file descriptors
   struct timeval tv;
   bool data_avail = false;
   
   // Configure the set of file descriptors to be read to just be stdin
   FD_ZERO(&read_fds);
   FD_SET(0, &read_fds);   // 0 is the file-descriptor for stdin
   // No waiting
   tv.tv_sec = 0;
   tv.tv_usec = 0;

   data_avail = select(
                  1,          // int nfds (number of file descriptors in set)
                  &read_fds,  // fd_set * readfds
                  NULL,       // fd_set * writefds
                  NULL,       // fd_set * exceptfds
                  &tv         // struct timeval * timeout
               );

   return data_avail;
#endif
}

STATIC char * ReadNextStdInToken(void)
{
   size_t str_cap = 8;
   char * str = malloc(str_cap);  // NOTE: Remember to destroy after use!
   if ( NULL == str ) return NULL;
   char * ret_val = str;

   size_t str_len = 0;
   int c;
   while( ( (c = fgetc(stdin)) != EOF ) &&
          (c != ',') &&
          (c != ' ') &&
          (str_len < (str_cap - 2)) )
   {
      *(str++) = (char)c;
      str_len++;

      if ( str_len >= (str_cap - 2) )
      {
         char * tmp = realloc(ret_val, str_cap * 2);
         if ( NULL == tmp )
         {
            free(ret_val);
            return NULL;
         }
         ret_val = tmp;
         str = tmp + str_len;
         str_cap *= 2;
      }
   }
   *str = '\0';

   return ret_val;
}

STATIC size_t ArgOccurrenceCount( char const * args[],
                                  char const * str,
                                  int argc,
                                  uint8_t * idx_of_first_occurrence )
{
   // Assert instead of return false because this function is internal and we
   // have control on what it is called with.
   assert( (args != NULL) && (str != NULL) );

   size_t count = 0;
   bool found_occurrence = false;
   const uint8_t MAX_ARGS = (argc < MAX_ARGS_TO_CHECK) ?
                              (uint8_t)argc :
                              MAX_ARGS_TO_CHECK;

   // Scan through arguments
   for ( uint8_t i = 1; i < MAX_ARGS; i++ )
   {
      if ( args[i] == NULL )
      {
         continue;
      }
      else if ( !found_occurrence && (strncmp( str, args[i], MAX_ARG_LEN ) == 0) )
      {
         count++;
         found_occurrence = true;
         if ( idx_of_first_occurrence != NULL ) *idx_of_first_occurrence = i;
      }
      else if ( strncmp( str, args[i], MAX_ARG_LEN ) == 0 )
      {
         count++;
      }
      else
      {
         // Keep looking...
      }
   }

   assert( (found_occurrence && (count > 0)) || (!found_occurrence && (count == 0)) );

   return count;
}

// Acceptable formats:
// Hex:     0xZZ, Z, ZZ, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ
// Decimal: ZZd, ZZD
STATIC enum LIN_PID_Result_E GetID( const char * str,
                                    uint8_t * id,
                                    bool * ishex,
                                    bool * isdec )
{
   assert( (str != NULL) &&
           (id  != NULL) &&
           (!(*ishex) || !(*isdec)) );

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
      ParserOneDecDigit,
      ParserTwoDecDigits,
      ParserHexDigits,
      ParserTwoHexDigits,
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
   bool assume_hex = *ishex;
   bool assume_dec = *isdec;
   bool hex_prefix_already_encountered = false;

   if ( *ishex )
   {
      parser_state = ParserPreemptivelyHex;
   }
   else if ( *isdec )
   {
      parser_state = ParserPreemptivelyDec;
   }
   else
   {
      parser_state = ParserInit;
   }

   char ch = '\0';
   while (
      (loop_limit_counter <= MAX_NUM_LEN) &&
      (str[idx] != '\0') && /* Continue until terminating null character */
      !exit_loop /* Redundant here but I like extra guard rails */
   )
   {
      ch = str[idx];
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
                  assume_hex = true;
                  parser_state = ParserHexDigits;
               }
               first_digit = ch;
            }
            else if ( ('x' == ch) || ('X' == ch) )
            {
               assume_hex = true;
               hex_prefix_already_encountered = true;
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
               assume_hex = true;
               hex_prefix_already_encountered = true;
               parser_state = ParserHexPrefix;
            }
            else if ( isxdigit(ch) )
            {
               if ( !isdigit(ch) )
               {
                  // Must be a uniquely hex digit
                  // NOTE: The 'd' suffix would also lead to this line,
                  //       and the user may have intended a decimal number.
                  //       I have chosen that the hex variant will take hold unless
                  //       --dec | -d was specified.
                  assume_hex = true;
                  parser_state = ParserTwoHexDigits;
               }
               else
               {
                  parser_state = ParserIndeterminateTwoDigitsIn;
               }
               first_digit = ch;
            }
            else if ( ('h' == ch) || ('H' == ch) )
            {
               assume_hex = true;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = InvalidDigitEncountered_SecondDigit;
               exit_loop = true;
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
                  assume_hex = true;
                  parser_state = ParserTwoHexDigits;
               }
               second_digit = ch;
            }
            else if ( ('x' == ch) || ('X' == ch) ||
                      ('h' == ch) || ('H' == ch) )
            {
               assume_hex = true;
               parser_state = ParserTwoDigitsAlreadyRead;
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
               assume_hex = true;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else if ( ('d' == ch) || ('D' == ch) )
            {
               assume_dec = true;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = TooManyDigitsEntered;
               exit_loop = true;
            }
            break;

         case ParserOneDecDigit:
            if ( isdigit(ch) )
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
               parser_state = ParserTwoHexDigits;
            }
            else if ( ('x' == ch) || ('X' == ch) ||
                      ('h' == ch) || ('H' == ch) )
            {
               if ( hex_prefix_already_encountered )
               {
                  result = HexPrefixAndSuffixEncountered;
                  exit_loop = true;
               }
               else
               {
                  assume_hex = true;
                  parser_state = ParserTwoDigitsAlreadyRead;
               }
            }
            else
            {
               result = InvalidDigitEncountered_SecondDigit;
               exit_loop = true;
            }
            break;

         case ParserTwoHexDigits:
            // Two hex digits will have already been read in...
            if ( ('x' == ch) || ('X' == ch) ||
                 ('h' == ch) || ('H' == ch) )
            {
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else
            {
               result = InvalidDecimalSuffixEncountered;
               exit_loop = true;
            }
            break;

         case ParserTwoZerosIn:
            if ( !assume_dec &&
                     ( ('x' == ch) || ('X' == ch) || ('h' == ch) || ('H' == ch) ) )
            {
               assume_hex = true;
               parser_state = ParserTwoDigitsAlreadyRead;
            }
            else if ( !assume_hex &&
                     ( ('d' == ch) || ('D' == ch) ) )
            {
               assume_dec = true;
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
            if ( ('x' == ch) || ('X' == ch) )
            {
               hex_prefix_already_encountered = true;
               parser_state = ParserHexPrefix;
            }
            else if ( '0' == ch )
            {
               parser_state = ParserOneZeroIn;
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
               first_digit = ch;
               parser_state = ParserPreemptivelyDecOneZeroIn;
            }
            else if ( isdigit(ch) )
            {
               first_digit = ch;
               parser_state = ParserOneDecDigit;
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
               parser_state = ParserTwoDecDigits;
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
            assert(false); // Something went wrong /w the parser_state or we
                           // forgot to account for a state!
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
   assert( !(assume_hex && assume_dec) );
   
   // Post state machine processing
   if ( loop_limit_counter >= MAX_NUM_LEN )
   {
      result = TooManyDigitsEntered;
   }
   else if ( exit_loop )
   {
      // Something erroneous was detected. Don't update ID!
   }
   else if ( (assume_hex || assume_dec) && ('\0' == first_digit) && ('\0' == second_digit) )
   {
      result = NoNumericalDigitsEnteredWithFormat;
   }

   else
   {
      // We got valid digits! Let's get that ID.
      uint8_t most_significant_digit = 0xFF;
      uint8_t least_significant_digit = 0xFF;

      // By this point, I assume we have at least one digit placed in first_digit.
      assert( (first_digit != '\0') );

      // Only one digit entered
      if ( second_digit == '\0' )
      {
         most_significant_digit = 0x00u;
#ifndef NDEBUG
         bool conv = MyAtoI( first_digit, &least_significant_digit );
#else
         (void)MyAtoI( first_digit, &least_significant_digit );
#endif
         assert( conv );
         assert( (most_significant_digit == 0x00) && (least_significant_digit <= 0x0F) );
      }

      // Two digits entered
      else
      {
#ifndef NDEBUG
         bool conv1 = MyAtoI( first_digit, &most_significant_digit );
         bool conv2 = MyAtoI( second_digit, &least_significant_digit );
#else
         (void)MyAtoI( first_digit, &most_significant_digit );
         (void)MyAtoI( second_digit, &least_significant_digit );
#endif
         assert( conv1 && conv2 );
         assert( (most_significant_digit <= 0x0F) && (least_significant_digit <= 0x0F) );
      }

      // Digits obtained should be <= 0xF
      assert( (most_significant_digit <= 0x0F) && (least_significant_digit <= 0x0F) );

      // Final step!
      if ( (0 == most_significant_digit) && (0 == least_significant_digit) )
      {
         *id = 0x00u;
      }
      else if ( assume_hex || !assume_dec )
      {
         *id = (uint8_t)( (most_significant_digit * 0x10) + least_significant_digit );
      }
      else  // Must be decimal
      {
         *id = (uint8_t)( (most_significant_digit * 10) + least_significant_digit );
      }
   }

   *ishex = assume_hex;
   *isdec = assume_dec;
   
   return result;
}

STATIC bool MyAtoI(char digit, uint8_t * converted_digit)
{
   assert( converted_digit != NULL );

   bool ret_val = false;

   // Check input validity
   if (
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

         "\n\t\033[;3mNote that single digit entires without \033[35m-d\033[0m \033[;3mor\033[0m \033[35m--dec\033[0m \033[;3mwill be taken as hexadecimal numbers no matter what.\033[0m\n"
         "\t\033[;3mFor example, the entry \"1d\" is interpreted as the hexadecimal number \"0x1D\", not a decimal number 1.\033[0m\n"
         "\t\033[;3mThe 'd' suffix there is indistinguishable from the hexadecimal digit 'd', and this program defaults to hex in these situations.\033[0m\n"

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

static void PrintErrMsg(enum LIN_PID_Result_E err)
{
   assert( (err >= (enum LIN_PID_Result_E)0) && (err < NUM_OF_EXCEPTIONS) );
   fprintf(stderr, "%.*s", MAX_ERR_MSG_LEN, ErrorMsgs[err]);
}

#ifndef NDEBUG

STATIC int UInt8_Cmp( const void * a, const void * b )
{
   assert( (a != NULL) && (b != NULL) );

   uint8_t * c = (uint8_t *)a;
   uint8_t * d = (uint8_t *)b;

   return ( *c - *d );
}

#endif

/**
 * @brief Determines the numeric format of the given string entry.
 *
 * This function analyzes the input string and identifies its numeric format,
 * based on a list of supported formats.
 * 
 * @note str is assumed to be a valid ID entry! Do not call this function before
 *       passing the string through GetID.
 *
 * @param str Pointer to the null-terminated string to be analyzed.
 * @return NumericFormat_E enunm indicating the detected format.
 */
STATIC enum NumericFormat_E DetermineEntryFormat( const char * str,
                                                  bool ishex,
                                                  bool isdec )
{
   assert( (str != NULL) && (str[0] != '\0') );

   // Let's try this /w regex, just for fun.
   for ( int i = 0; i < NUM_OF_NUMERIC_FORMATS; i++ )
   {
      if ( (ishex && NumericFormats[i].isdec) || (isdec && NumericFormats[i].ishex) )
      {
         continue;
      }
      int match_len; // don't care about this but tiny-regex-c expects it
      if ( re_match( NumericFormats[i].regex_pattern, str, &match_len) != -1 )
      {
         return (enum NumericFormat_E)i;
      }
   }

   return INVALID_NUMERIC_FORMAT;
}
