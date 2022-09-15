#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

//! @param[in] stream File to read from.
//! @return Returns pointer to char array.
//! @brief Reads text from stream and stores it as char array. First character in array is \0.
char* read_poem(FILE *stream);

//! @param[in] chr Character to count.
//! @param[in] str Pointer to string to count at.
//! @return Returns amount of occurrences of chr in str.
size_t count_chr_in_str(char chr, const char *str);

//! @param[in] text Text to split.
//! @param[in] dest Pointer to array of pointers to save pointers to lines.
//! @return Returns amount of poem lines in text.
//! @brief Replaces \n with \0 in text and saves pointers to beginnings if poem lines in dest. The last pointer points to NULL.
size_t split_poem(char *text, char ***dest);

//! @param[in] lhs Pointer to string.
//! @param[in] rhs Pointer to string.
//! @return Returns zero if lhs == rhs, a positive integer if rhs < lhs and a negative integer if rhs > lhs.
//! @brief Compares strings lexicographically. Case insensitive. Ignores punctuation.
int line_cmp(const void *line_l, const void *line_r);

//! @param[in] poem Array of pointers to strings.
//! @param[in] stream File to print strings to.
//! @brief Prints strings from poem to stream.
void fprint_poem(const char **poem, FILE *stream);

//! @param[in] stream File to examine.
//! @return Returns amount of characters in stream.
//! @brief Gets amount of characters in stream.
size_t get_file_size(FILE *stream);

//! @param[in] lines Pointer to array of pointers to poem lines.
//! @param[in] n_lines Amount of lines in lines.
//! @return Returns pointer to copy of lines.
//! @brief Copies lines.
char** copy_poem(char **lines, size_t n_lines);

void free_lines(char **lines, size_t n_lines);

#endif
