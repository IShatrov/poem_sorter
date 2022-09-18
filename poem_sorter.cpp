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
    assert(str != NULL);

    size_t ans = 0, chars_checked = 0;

    while (*(str + chars_checked) != '\0')
    {
        if (*(str + chars_checked) == chr) ++ans;

        ++chars_checked;
    }

    return ans;
}

size_t split_poem(char *text, struct line_info ***dest, struct line_info *struct_array)
{
    assert(dest != NULL);
    assert(text != NULL);

    size_t n_lines = 2 + count_chr_in_str('\n', text + 1); //last line does not end with \n, and one more line for NULL

    *dest = (line_info**) realloc(*dest, n_lines * sizeof(struct line_info*));
    struct_array = (struct line_info*) realloc(struct_array, n_lines * sizeof(struct line_info));

    struct_array[0].line = text + 1;

    **dest = &(struct_array[0]);

    size_t lines_found = 1, chr = 1; //have to start from 1 because text starts from \0
    const char *prev_line = text;

    while (*(text + chr) != '\0')
    {
        if (*(text + chr) == '\n')
        {
            struct_array[lines_found].line = text + chr + 1;
            struct_array[lines_found - 1].len = (text + chr) - prev_line - 1;
            prev_line = text + chr;

            *(*dest + lines_found) = &(struct_array[lines_found]);

            *(text + chr) = '\0';

            ++lines_found;
        }

        ++chr;
    }

    struct_array[lines_found - 1].len = (text + chr) - prev_line - 1;

    struct_array[n_lines - 1].line = NULL;
    struct_array[n_lines - 1].len = -1;
    *(*dest + n_lines - 1) = &(struct_array[n_lines - 1]);

    return n_lines;
}

int line_cmp(const void *struct_l_ptr, const void *struct_r_ptr)
{
    assert(struct_l_ptr != NULL);
    assert(struct_r_ptr != NULL);

    const char *line_l = (*((const struct line_info**) struct_l_ptr))->line;
    const char *line_r = (*((const struct line_info**) struct_r_ptr))->line;

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

int line_cmp_from_end(const void *struct_l_ptr, const void *struct_r_ptr)
{
    assert(struct_l_ptr != NULL);
    assert(struct_r_ptr != NULL);

    const char *line_l = (*((const struct line_info**) struct_l_ptr))->line + (*((const struct line_info**) struct_l_ptr))->len - 1;
    const char *line_r = (*((const struct line_info**) struct_r_ptr))->line + (*((const struct line_info**) struct_r_ptr))->len - 1;

    while(*line_l != '\0' && *line_r != '\0' && tolower(*line_l) == tolower(*line_r) ||
     (ispunct(*line_l) || isspace(*line_l)) || (ispunct(*line_r) || isspace(*line_r)))
    {

        if(ispunct(*line_l) || isspace(*line_l))
        {
            --line_l;
            continue;
        }

        if(ispunct(*line_r) || isspace(*line_r))
        {
            --line_r;
            continue;
        }

        --line_l;
        --line_r;
    }

    return tolower(*line_l) - tolower(*line_r);

}

void fprint_poem(const struct line_info **poem, FILE *stream)
{
    assert(poem != NULL);
    assert(stream != NULL);

    size_t lines_printed = 0;

    while((*(poem + lines_printed))->line != NULL)
    {
        fputs((*(poem + lines_printed))->line, stream);
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

struct line_info** copy_struct_array(struct line_info **lines, size_t n_lines)
{
    assert(lines != NULL);
    assert(n_lines != 0);

    struct line_info **ans = (struct line_info**) calloc(n_lines, sizeof(struct line_info*));

    for (size_t lines_copied = 0; lines_copied < n_lines; ++lines_copied)
    {
        *(ans + lines_copied) = *(lines + lines_copied);
    }

    return ans;
}

void byte_swap(void *a, void *b, size_t size)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(size != NULL);

    char temp = 0;

    for(size_t i = 0; i < size; ++i)
    {
        temp = *((char*)a + i);
        *((char*)a + i) = *((char*)b + i);
        *((char*)b + i) = temp;
    }

    return;
}

void my_sort(void *ptr, size_t count, size_t size, int (*cmp)(const void *a, const void *b))
{
    assert(ptr != NULL);
    assert(count != 0);
    assert(size != 0);
    assert(cmp != NULL);

    char not_sorted = 1;

    while(not_sorted)
    {
        not_sorted = 0;

        for(int i = 0; i < count - 1; ++i)
        {
            if((*cmp)(ptr + i*size, ptr + (i + 1)*size) > 0)
            {
                not_sorted = 1;

                byte_swap(ptr + i*size, ptr + (i + 1)*size, size);
            }
        }
    }

    return;
}
