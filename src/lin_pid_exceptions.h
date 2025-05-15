/**
 * @file lin_pid_exceptions.h
 * @brief LIN_PID_EXCEPTION X-macro declarations.
 *
 * @author Abdulla Almosalami (memphis242)
 * @date Wed Apr 30, 2025
 * @copyright MIT License
 */

//                 Exception Enum                                   Error String
LIN_PID_EXCEPTION( GoodResult,                                      "Good result ☺" )
LIN_PID_EXCEPTION( TooManyInputArgs,                                "Too many input arguments." )
LIN_PID_EXCEPTION( HexAndDecFlagsSimultaneouslyUsed,                "Can't use both hex and dec format flags simultaneously." )
LIN_PID_EXCEPTION( ID_OOR,                                          "ID is out of range. Enter a number between 0 and 63d (0x3F)." )
LIN_PID_EXCEPTION( WhiteSpaceOnlyIDArg,                             "Only encountered whitespace in input." )
LIN_PID_EXCEPTION( InvalidCharacterEncountered_FirstChar,           "Invalid character encountered (first character)." )
LIN_PID_EXCEPTION( InvalidCharacterEncountered_SecondChar,          "Invalid character encountered (first character)." )
LIN_PID_EXCEPTION( InvalidDigitEncountered_FirstDigit,              "Invalid digit encountered (first digit)." )
LIN_PID_EXCEPTION( InvalidDigitEncountered_SecondDigit,             "Invalid digit encountered (second digit)." )
LIN_PID_EXCEPTION( TooManyDigitsEntered,                            "Too many digits entered." )
LIN_PID_EXCEPTION( HexDigitEncounteredUnderDecSetting_FirstDigit,   "Hexadecimal digit encountered under decimal settings (first digit)." )
LIN_PID_EXCEPTION( HexDigitEncounteredUnderDecSetting_SecondDigit,  "Hexadecimal digit encountered under decimal settings (second digit)." )
LIN_PID_EXCEPTION( InvalidDecimalSuffixEncountered,                 "Invalid decimal suffix encountered. Possibly too many digits." )
LIN_PID_EXCEPTION( DuplicateFormatFlagsUsed,                        "Duplicate format flag detected. Please only specify (-d | --dec) or (-h | --hex) once." )
LIN_PID_EXCEPTION( InvalidFlagDetected,                             "Invalid flag detected. Please use only from the following: -d, --dec, -h, --hex, --no-new-line, --quiet, -q, -t, --table, --help" )
LIN_PID_EXCEPTION( InvalidPositionOfNumber,                         "Number was not placed properly. It needs to either be the first or second argument." )
LIN_PID_EXCEPTION( CantUseNoNewLineWithoutQuiet,                    "Can't use --no-new-line without (--quiet | -q)" )
LIN_PID_EXCEPTION( PrematureTerminatingCharEncounted,               "Premature terminating character encountered when a digit was expected." )
LIN_PID_EXCEPTION( NoNumericalDigitsEnteredWithFormat,              "No numerical digits entered wit." )
LIN_PID_EXCEPTION( HexPrefixAndSuffixEncountered,                   "Hexadecimal prefix and suffix encountered. That is not allowed." )
