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

 //                     Format Enum                                     Regex                      Print Format Specifier  IsHex    IsDec
LIN_PID_NUMERIC_FORMAT( DecNoPrefixOrSuffix_NoLeadingZeros,             "^[1-9][0-9]?$",            "%d",                  false,   true )
LIN_PID_NUMERIC_FORMAT( DecNoPrefixOrSuffix_LeadingZeros,               "^[0-9][0-9]?$",            "%02d",                false,   true )

LIN_PID_NUMERIC_FORMAT( HexNoPrefixOrSuffix_NoLeadingZeros_Uppercase,   "^[1-9A-F][0-9A-F]?$",      "%X",                  true,    false )
LIN_PID_NUMERIC_FORMAT( HexNoPrefixOrSuffix_NoLeadingZeros_Lowercase,   "^[1-9a-f][0-9a-f]?$",      "%x",                  true,    false )
LIN_PID_NUMERIC_FORMAT( HexNoPrefixOrSuffix_LeadingZeros_Uppercase,     "^[0-9A-F][0-9A-F]?$",      "%02X",                true,    false )
LIN_PID_NUMERIC_FORMAT( HexNoPrefixOrSuffix_LeadingZeros_Lowercase,     "^[0-9a-f][0-9a-f]?$",      "%02x",                true,    false )

LIN_PID_NUMERIC_FORMAT( ClassicHexPrefix_NoLeadingZeros_Uppercase,      "^0x[1-9A-F][0-9A-F]?$",    "0x%X",                true,    false )
LIN_PID_NUMERIC_FORMAT( ClassicHexPrefix_NoLeadingZeros_Lowercase,      "^0x[1-9a-f][0-9a-f]?$",    "0x%x",                true,    false )
LIN_PID_NUMERIC_FORMAT( ClassicHexPrefix_LeadingZeros_Uppercase,        "^0x[0-9A-F][0-9A-F]?$",    "0x%02X",              true,    false )
LIN_PID_NUMERIC_FORMAT( ClassicHexPrefix_LeadingZeros_Lowercase,        "^0x[0-9a-f][0-9a-f]?$",    "0x%02x",              true,    false )

LIN_PID_NUMERIC_FORMAT( LowercasexPrefix_NoLeadingZeros_Uppercase,      "^x[1-9A-F][0-9A-F]?$",     "x%X",                 true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasexPrefix_NoLeadingZeros_Lowercase,      "^x[1-9a-f][0-9a-f]?$",     "x%x",                 true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasexPrefix_LeadingZeros_Uppercase,        "^x[0-9A-F][0-9A-F]?$",     "x%02X",               true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasexPrefix_LeadingZeros_Lowercase,        "^x[0-9a-f][0-9a-f]?$",     "x%02x",               true,    false )

LIN_PID_NUMERIC_FORMAT( UppercaseXPrefix_NoLeadingZeros_Uppercase,      "^X[1-9A-F][0-9A-F]?$",     "X%X",                 true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseXPrefix_NoLeadingZeros_Lowercase,      "^X[1-9a-f][0-9a-f]?$",     "X%x",                 true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseXPrefix_LeadingZeros_Uppercase,        "^X[0-9A-F][0-9A-F]?$",     "X%02X",               true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseXPrefix_LeadingZeros_Lowercase,        "^X[0-9a-f][0-9a-f]?$",     "X%02x",               true,    false )

LIN_PID_NUMERIC_FORMAT( LowercasehSuffix_NoLeadingZeros_Uppercase,      "^[1-9A-F][0-9A-F]?h$",     "%Xh",                 true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasehSuffix_NoLeadingZeros_Lowercase,      "^[1-9a-f][0-9a-f]?h$",     "%xh",                 true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasehSuffix_LeadingZeros_Uppercase,        "^[0-9A-F][0-9A-F]?h$",     "%02Xh",               true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasehSuffix_LeadingZeros_Lowercase,        "^[0-9a-f][0-9a-f]?h$",     "%02xh",               true,    false )

LIN_PID_NUMERIC_FORMAT( UppercaseHSuffix_NoLeadingZeros_Uppercase,      "^[1-9A-F][0-9A-F]?H$",      "%XH",                true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseHSuffix_NoLeadingZeros_Lowercase,      "^[1-9a-f][0-9a-f]?H$",      "%xH",                true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseHSuffix_LeadingZeros_Uppercase,        "^[0-9A-F][0-9A-F]?H$",      "%02XH",              true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseHSuffix_LeadingZeros_Lowercase,        "^[0-9a-f][0-9a-f]?H$",      "%02xH",              true,    false )

LIN_PID_NUMERIC_FORMAT( LowercasexSuffix_NoLeadingZeros_Uppercase,      "^[1-9A-F][0-9A-F]?x$",     "%Xx",                 true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasexSuffix_NoLeadingZeros_Lowercase,      "^[1-9a-f][0-9a-f]?x$",     "%xx",                 true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasexSuffix_LeadingZeros_Uppercase,        "^[0-9A-F][0-9A-F]?x$",     "%02Xx",               true,    false )
LIN_PID_NUMERIC_FORMAT( LowercasexSuffix_LeadingZeros_Lowercase,        "^[0-9a-f][0-9a-f]?x$",     "%02xx",               true,    false )

LIN_PID_NUMERIC_FORMAT( UppercaseXSuffix_NoLeadingZeros_Uppercase,      "^[1-9A-F][0-9A-F]?X$",      "%XX",                true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseXSuffix_NoLeadingZeros_Lowercase,      "^[1-9a-f][0-9a-f]?X$",      "%xX",                true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseXSuffix_LeadingZeros_Uppercase,        "^[0-9A-F][0-9A-F]?X$",      "%02XX",              true,    false )
LIN_PID_NUMERIC_FORMAT( UppercaseXSuffix_LeadingZeros_Lowercase,        "^[0-9a-f][0-9a-f]?X$",      "%02xX",              true,    false )

LIN_PID_NUMERIC_FORMAT( LowercasedSuffix_NoLeadingZeros,                "^[1-9][0-9]?d$",            "%dd",                false,   true )
LIN_PID_NUMERIC_FORMAT( LowercasedSuffix_LeadingZeros,                  "^[0-9][0-9]?[0-9]?d$",      "%02dd",              false,   true )

LIN_PID_NUMERIC_FORMAT( UppercaseDSuffix_NoLeadingZeros,                "^[1-9][0-9]?D$",            "%dD",                false,   true )
LIN_PID_NUMERIC_FORMAT( UppercaseDSuffix_LeadingZeros,                  "^[0-9][0-9]?[0-9]?D$",      "%02dD",              false,   true )
