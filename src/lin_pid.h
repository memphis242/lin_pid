/**
 * @file lin_pid.h
 * @brief API for a dynamic array (vector) in C.
 *
 * This file provides the interface for computing the LIN 2.1 Protected ID
 * and exposing a testable main-like function for terminal-based testing.
 *
 * @author Abdulla Almosalami (memphis242)
 * @date Tues Apr 15, 2025
 * @copyright MIT License
 */

/* File Inclusions */
#include <stdlib.h>
#include <stdint.h>
// TODO: Exceptions

/* Public Macro Definitions */
#define LIN_2p0_MAX_ID  0x3Fu
#define MAX_ID_ALLOWED  LIN_2p0_MAX_ID

/* Public Datatypes */

/* Public API */

/**
 * @brief Print out the LIN 2.1 based Protected ID given an ID.
 *
 * This function is exposed for testing purposes. It simulates a main-like
 * function that takes command-line arguments, computes the LIN Protected ID,
 * and prints the result to the terminal.
 *
 * @param[in] argc The number of command-line arguments.
 * @param[in] argv The array of command-line arguments.
 * @return Returns 0 on success, or a non-zero value on failure.
 */
#ifdef TEST
int lin_pid_cli(int argc, char * argv[]);
#endif

/**
 * @brief Compute the LIN 2.1 Protected Identifier (PID) for a given ID.
 *
 * This function calculates the Protected Identifier (PID) for the LIN protocol
 * based on the provided 6-bit frame identifier. The PID includes the original
 * identifier and two parity bits (P0 and P1) as defined in the LIN 2.1 specification.
 *
 * @param[in] id The 6-bit frame identifier (0x00 to 0x3F).
 * @return The 8-bit Protected Identifier (PID), including the parity bits.
 * @note The input ID must not exceed the maximum allowed value (0x3F).
 */
uint8_t ComputePID(uint8_t id);

