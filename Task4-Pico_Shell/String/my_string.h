#include "../std_types.h"
#include <stdio.h>
#include <stdlib.h>

/* --------------------- Functions Declarations --------------------- */

/**
 * @brief Calculate length of a string.
 *
 * @param str Pointer to the first char of the string.
 *
 * @return return the length of string.
 */
uint32_t str_length(const char* str);

/**
 * @brief Compares two strings and returns a flag.
 *        1-> Strings are the same.
 *        0-> Not the same.  
 *
 * @param str1 Pointer to the first char of a string1.
 * @param str2 Pointer to the first char of a string2.
 *
 * @return returns a flag (1 or 0).
 */
char compare_str(const char* str1, const char* str2);

/**
 * @brief parse the string to put the part from left to right from that string in another
 *
 * @param src_str Pointer to the first char of the string.
 * @param left the first character you will start from.
 * @param right the lat character you will end at.
 *
 * @return returns a pointer to the generated string.
 */
char* substring(const char* src_str, uint32_t left, uint32_t right);

/**
 * @brief checks if a string is in an array of strings or no.
 *
 * @param str Pointer to the first char of the string.
 * @param arr an array of strings(pointer to char).
 * @param length length of array.
 *
 * @return returns a flag determining if the string in array or no.
 *         1-> in array
 *         0-> Not in array
 */
uint8_t is_str_in_arr(const char* str, const char* arr[], uint32_t length);

/**
 * @brief checks if a character is in an array of characters or no.
 *
 * @param ch the character we want to check about.
 * @param arr an array of characters.
 * @param length length of array.
 *
 * @return returns a flag determining if the char in array or no.
 *         1-> in array
 *         0-> Not in array
 */
uint8_t is_char_in_arr(const char ch, const char arr[], uint32_t length);