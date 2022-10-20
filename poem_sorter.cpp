#include "poem_sorter.h"

char* read_poem(FILE *stream)
{
    assert(stream != NULL);

    size_t len = get_file_size(stream);
    assert(len != 0);

    char* text = (char*) calloc(len + 2, sizeof(char));
    assert(text != NULL);

    //first character in text will be \0, useful for second comparator
    fread(text + 1, sizeof(char), len, stream);
    assert(!ferror(stream));

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

size_t split_poem(char *text, struct line_info **dest)
{
    assert(dest != NULL);
    assert(text != NULL);

    size_t n_lines = 2 + count_chr_in_str('\n', text + 1); //last line does not end with \n, and one more line for NULL

    *dest = (struct line_info*) realloc(*dest, n_lines * sizeof(struct line_info));
    assert(*dest != NULL);

    size_t lines_found = 0, chr = 1;

    char done = 0, is_line_empty = 0;
    char *prev_line = text + 1;

    do
    {
        while(*(text + chr) == ' ' || *(text + chr) == '\t') ++chr; //check if line is empty
        if(*(text + chr) == '\n')
        {
            ++chr;

            prev_line = text + chr;

            continue;
        }

        (*dest)[lines_found].line = text + chr;

        while(*(text + chr) != '\n')
        {
            ++chr;

            if(*(text + chr) == '\0')
            {
                done = 1;

                break;
            }
        }

        *(text + chr) = '\0';

        (*dest)[lines_found].len = text + chr - prev_line;
        prev_line = text + chr + 1;

        ++lines_found;
        ++chr;
    }
    while (!done);

    (*dest)[lines_found].line = NULL;
    (*dest)[lines_found].len = -1;

    return lines_found + 1;
}

int line_cmp(const void *struct_l_ptr, const void *struct_r_ptr)
{
    assert(struct_l_ptr != NULL);
    assert(struct_r_ptr != NULL);

    const char *line_l = (*((const struct line_info*) struct_l_ptr)).line;
    const char *line_r = (*((const struct line_info*) struct_r_ptr)).line;

    while((*line_l != '\0' && *line_r != '\0' && tolower(*line_l) == tolower(*line_r)) ||
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

    const char *line_l = (*((const struct line_info*) struct_l_ptr)).line + (*((const struct line_info*) struct_l_ptr)).len - 1;
    const char *line_r = (*((const struct line_info*) struct_r_ptr)).line + (*((const struct line_info*) struct_r_ptr)).len - 1;

    while((*line_l != '\0' && *line_r != '\0' && tolower(*line_l) == tolower(*line_r)) ||
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

void fprint_poem(const struct line_info *poem, FILE *stream)
{
    assert(poem != NULL);
    assert(stream != NULL);

    size_t lines_printed = 0;

    while((*(poem + lines_printed)).line != NULL)
    {
        fputs((*(poem + lines_printed)).line, stream);
        assert(!ferror(stream));

        putc('\n', stream);
        assert(!ferror(stream));

        ++lines_printed;
    }

    return;
}

size_t get_file_size(FILE *stream)
{
    assert(stream != NULL);

    size_t ans = 0;

    assert(!fseek(stream, 0, SEEK_END));   //fseek returns zero upon success and non-zero otherwise
    ans = ftell(stream);
    assert(!ferror(stream));

    assert(!fseek(stream, 0, SEEK_SET));

    return ans;
}

struct line_info* copy_struct_array(struct line_info *lines, size_t n_lines)
{
    assert(lines != NULL);
    assert(n_lines != 0);

    struct line_info *ans = (struct line_info*) calloc(n_lines, sizeof(struct line_info));
    assert(ans != NULL);

    for (size_t lines_copied = 0; lines_copied < n_lines; ++lines_copied)
    {
        ans[lines_copied].line = lines[lines_copied].line;
        ans[lines_copied].len = lines[lines_copied].len;
    }

    return ans;
}

void byte_swap(void *a, void *b, size_t size)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(size != 0);

    while(size >= sizeof(double))
    {
        double temp = *((double*)a);
        *((double*)a) = *((double*)b);
        *((double*)b) = temp;

        a = ((double*)a) + 1;
        b = ((double*)b) + 1;

        size -= sizeof(double);
    }

    while(size >= sizeof(int))
    {
        int temp = *((int*)a);
        *((int*)a) = *((int*)b);
        *((int*)b) = temp;

        a = ((int*)a) + 1;
        b = ((int*)b) + 1;

        size -= sizeof(int);
    }

    while(size > 0)
    {
        char temp = *((char*)a);
        *((char*)a) = *((char*)b);
        *((char*)b) = temp;

        a = ((char*)a) + 1;
        b = ((char*)b) + 1;

        size -= 1;
    }

    return;
}

void my_qsort(void *ptr, size_t count, size_t size, int (*cmp)(const void *a, const void *b))
{
    assert(ptr != NULL);
    assert(cmp != NULL);

    void *low = ptr, *high = (char*)ptr + (count - 1)*size;

    if(low < high)
    {
        void *pivot = high;

        void *l = (char*)low - size;

        for(void *r = low; r < high; r = (char*)r + size)
        {
            if(cmp(r, pivot) <= 0)
            {
                l = (char*)l + size;

                byte_swap(l, r, size);
            }
        }

        l = (char*)l + size;

        byte_swap(l, high, size);

        void *point = l;


        if (low < high && count > 1)
        {
            my_qsort(ptr, ((char*)point - (char*)low)/size, size, cmp);
            my_qsort((char*)ptr + count*size - ((char*)high - (char*)point), ((char*)high - (char*)point)/size, size, cmp);
        }
    }
}
