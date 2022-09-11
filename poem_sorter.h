#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

const int MAX_LINE_LEN = 101;
const int DEFAULT_N_LINES = 100;

//! @param[in] lines Pointer to array of pointers to strings.
//! @param[in] n_lines Pointer to the size of initial array.
//! @param[in] stream File to read from.
//! @brief Reads strings from stream and stores them in lines. Keeps newline character. Resulting strings a null-terminated. Can`t read more than MAX_LINE_LEN symbols. Expands lines to fit all strings if required.
void read_poem(char ***lines, int *n_lines, FILE *stream);

//! @param[in] lhs Pointer to string.
//! @param[in] rhs Pointer to string.
//! @return Returns zero if lhs == rhs, a positive integer if rhs < lhs and a negative integer if rhs > lhs.
//! @brief Compares strings lexicographically. Case insensitive.
int ins_strcmp(const char *lhs, const char *rhs);

//! @param[in] poem Array of pointers to strings.
//! @param[in] n_lines Amount of strings in the array.
//! @brief Sorts strings from poem lexicographically.
void sort_poem(char **poem, int n_lines);

//! @param[in] poem Array of pointers to strings.
//! @param[in] stream File to print strings to.
//! @brief Prints strings from poem to stream.
void fprint_poem(char **poem, int n_lines, FILE *stream);

#endif
