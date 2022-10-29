#include "poem_sorter.h"

int main(int argc, const char *argv[])
{
    const char *poem_filename = "poem.txt";
    const char *destination_filename = "dest.txt";

    get_cmd_line_args(&poem_filename, &destination_filename, argc, argv);

    FILE *poem = NULL;
    poem = fopen(poem_filename, "r");
    assert(poem != NULL && "Poem file not found");

    char *text = read_poem(poem);

    struct line_info *lines = NULL;
    size_t n_lines = split_poem(text, &lines);

    struct line_info *sorted_lines = copy_struct_array(lines, n_lines);

    my_qsort(sorted_lines, n_lines - 1, sizeof(struct line_info), &line_cmp);

    FILE *dest = NULL;
    dest = fopen(destination_filename, "w");
    assert(dest != NULL && "Destination file creation error");

    fprint_poem((const struct line_info*) sorted_lines, dest);
    putc('\n', dest);

    my_qsort(sorted_lines, n_lines - 1, sizeof(struct line_info), &line_cmp_from_end);

    fprint_poem((const struct line_info*) sorted_lines, dest);
    putc('\n', dest);

    fprint_poem((const struct line_info*) lines, dest);

    fclose(poem);
    fclose(dest);

    free(text);
    free(lines);
    free(sorted_lines);

    return 0;
}
