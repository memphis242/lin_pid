/**
 * @file lin_pid_supported_formats.h
 * @brief LIN_PID_NUMERIC_FORMAT X-macro declarations.
 * 
 * @note Acceptable formats:
 *          Hex:     0xZZ, ZZ, Z, ZZh, ZZH, ZZx, ZZX, xZZ, XZZ
 *          Decimal: ZZd, ZZD
 *
 * @author Abdulla Almosalami (memphis242)
 * @date Wed May 14, 2025
 * @copyright MIT License
 */

 //                     Format Enum                        Format String
 //                                                        Uppercase                         Lowercase
 //                                                        Leading Zero | No Leading Zeros   Leading Zero | No Leading Zeros
LIN_PID_NUMERIC_FORMAT( HexNoPrefixOrSuffix,               "%02X",        "%X"               "%02x",        "%x"   )
LIN_PID_NUMERIC_FORMAT( DecNoPrefixOrSuffix,               "%02d",        "%d"               "%02d",        "%d"   )
LIN_PID_NUMERIC_FORMAT( ClassicHexPrefix,                  "0x%02X",      "0x%X"             "0x%02x",      "0x%x" )
LIN_PID_NUMERIC_FORMAT( LowercasexPrefix,                  "x%02X",       "x%X"              "x%02x",       "x%x"  )
LIN_PID_NUMERIC_FORMAT( UppercaseXPrefix,                  "X%02X",       "X%X"              "X%02x",       "X%x"  )
LIN_PID_NUMERIC_FORMAT( LowercasehSuffix,                  "%02Xh",       "%Xh"              "%02xh",       "%xh"  )
LIN_PID_NUMERIC_FORMAT( UppercaseHSuffix,                  "%02XH",       "%XH"              "%02xH",       "%xH"  )
LIN_PID_NUMERIC_FORMAT( LowercasedSuffix,                  "%02dd",       "%dd"              "%02dd",       "%dd"  )
LIN_PID_NUMERIC_FORMAT( UppercaseDSuffix,                  "%02dD",       "%dD"              "%02dD",       "%dD"  )
