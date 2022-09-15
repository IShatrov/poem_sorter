#include "poem_sorter.h"

char* read_poem(FILE *stream)
{
    assert(stream != NULL);

    size_t len = get_file_size(stream);

    char* text = (char*) calloc(len + 2, sizeof(char));

    //first character in text will be \0, can be useful for another task
    fread(text + 1*sizeof(char), sizeof(char), len, stream);

    return text;
}

size_t count_chr_in_str(char chr, const char *str)
{
    size_t ans = 0, chars_checked = 0;

    while (*(str + chars_checked) != '\0')
    {
        if (*(str + chars_checked) == chr) ++ans;

        ++chars_checked;
    }

    return ans;
}

size_t split_poem(char *text, char ***dest)
{
    assert(dest != NULL);
    assert(text != NULL);

    size_t n_lines = 2 + count_chr_in_str('\n', text + 1); //last line does not end with \n, and one more line for NULL

    *dest = (char**) realloc(*dest, n_lines * sizeof(char*));

    **dest = text + 1;

    size_t lines_found = 1, chr = 1; //have to start from 1 because text starts from \0

    while (*(text + chr) != '\0')
    {
        if (*(text + chr) == '\n')
        {
            *(*dest + lines_found) = text + chr + 1;

            *(text + chr) = '\0';

            ++lines_found;
        }

        ++chr;
    }

    *(*dest + n_lines - 1) = NULL;

    return n_lines;
}

int line_cmp(const void *line_l_ptr, const void *line_r_ptr)
{
    assert(line_l_ptr != NULL);
    assert(line_r_ptr != NULL);

    const char *line_l = *((const char**) line_l_ptr);
    const char *line_r = *((const char**) line_r_ptr);

    while(*line_l != '\0' && *line_r != '\0' && tolower(*line_l) == tolower(*line_r) ||
     (ispunct(*line_l) || isspace(*line_l)) || (ispunct(*line_r) || isspace(*line_r)))
    {

        if(ispunct(*line_l) || isspace(*line_l))
        {
            ++line_l;
            continue;
        }

        if(ispunct(*line_r) || isspace(*line_r))
        {
            ++line_r;
            continue;
        }

        ++line_l;
        ++line_r;
    }

    return tolower(*line_l) - tolower(*line_r);
}

void fprint_poem(const char **poem, FILE *stream)
{
    size_t lines_printed = 0;

    while(*(poem + lines_printed) != NULL)
    {
        fputs(*(poem + lines_printed), stream);
        putc('\n', stream);

        ++lines_printed;
    }

    return;
}

size_t get_file_size(FILE *stream)
{
    assert(stream != NULL);

    size_t ans = 0;

    fseek(stream, 0, SEEK_END);
    ans = ftell(stream);

    fseek(stream, 0, SEEK_SET);

    return ans;
}

char** copy_poem(char **lines, size_t n_lines)
{
    char **ans = (char**) calloc(n_lines, sizeof(char*));

    for (size_t lines_copied = 0; lines_copied < n_lines; ++lines_copied)
    {
        *(ans + lines_copied) = *(lines + lines_copied);
    }

    return ans;
}

void free_lines(char **lines, size_t n_lines)
{
    for (size_t lines_freed= 0; lines_freed < n_lines; ++lines_freed) free(lines[lines_freed]);

    return;
}
