#include "poem_sorter.h"

int main(int argc, const char *argv[])
{
    const char *poem_filename = "poem.txt";
    const char *destination_filename = "dest.txt";

    if (argc > 1) poem_filename = argv[1];
    if (argc > 2) destination_filename = argv[2];

    FILE *poem = NULL;
    poem = fopen(poem_filename, "r");
    assert(poem != NULL && "Poem file not found");

    char *text = read_poem(poem);

    struct line_info **lines = NULL, *struct_array = NULL;
    size_t n_lines = split_poem(text, &lines, struct_array);

    struct line_info **sorted_lines = copy_struct_array(lines, n_lines);

    my_sort(sorted_lines, n_lines - 1, sizeof(struct line_info*), &line_cmp);

    FILE *dest = NULL;
    dest = fopen(destination_filename, "w");
    assert(dest != NULL && "Destination file creation error");

    fprint_poem((const struct line_info**) sorted_lines, dest);
    putc('\n', dest);

    my_sort(sorted_lines, n_lines - 1, sizeof(struct line_info*), &line_cmp_from_end);

    fprint_poem((const struct line_info**) sorted_lines, dest);
    putc('\n', dest);

    fprint_poem((const struct line_info**) lines, dest);

    fclose(poem);
    free(text);
    fclose(dest);
    free(struct_array);
    free(lines);

    return 0;
}
