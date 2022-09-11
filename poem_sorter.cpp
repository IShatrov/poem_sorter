#include "poem_sorter.h"

void read_poem(char ***lines, int *n_lines, FILE *stream)
{
    assert(lines != NULL);
    assert(*lines != NULL);
    assert(n_lines != NULL);
    assert(stream != NULL);

    int lines_read = 0;

    *(*lines + lines_read) = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    assert(*(*lines + lines_read) != NULL);

    fgets(*(*lines + lines_read), MAX_LINE_LEN, stream);

    while (*(*lines + lines_read) != NULL)
    {
        ++lines_read;
        if (lines_read == *n_lines)
        {
            *n_lines *= 2;
            *lines = (char**) realloc(*lines, *n_lines * sizeof(char*));

            assert(*lines != NULL);
        }

        *(*lines + lines_read) = (char*) malloc(MAX_LINE_LEN * sizeof(char));

        assert(*(*lines + lines_read) != NULL);

        *(*lines + lines_read) = fgets(*(*lines + lines_read), MAX_LINE_LEN, stream);
    }

    if (*n_lines > lines_read)
    {
        *lines = (char**) realloc(*lines, lines_read * sizeof(char*));
        assert(*lines != NULL);
        *n_lines = lines_read;
    }

    return;
}

int ins_strcmp(const char *lhs, const char *rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    while(*lhs != '\0' && *rhs != '\0' && tolower(*lhs) == tolower(*rhs))
    {
        ++lhs;
        ++rhs;
    }

    return tolower(*lhs) - tolower(*rhs);
}

void sort_poem(char **poem, int n_lines)
{
    assert(poem != NULL);
    assert(n_lines != 0);

    int not_sorted = 1;
    char *temp = NULL;

    while (not_sorted)
    {
        not_sorted = 0;
        for (int i = 0; i < n_lines - 1; ++i)
        {
            if (ins_strcmp(poem[i], poem[i + 1]) > 0)
            {
                not_sorted = 1;

                temp = poem[i + 1];
                poem[i + 1] = poem[i] ;
                poem[i] = temp;
            }
        }
    }

    return;
}

void fprint_poem(char **poem, int n_lines, FILE *stream)
{
    for (int i = 0; i < n_lines; ++i) fputs(poem[i], stream);

    return;
}
