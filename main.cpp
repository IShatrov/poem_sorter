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

    char **lines = NULL;
    size_t n_lines = split_poem(text, &lines);

    char **sorted_lines = copy_poem(lines, n_lines);
    qsort(sorted_lines, n_lines - 1, sizeof(char*), &line_cmp);

    FILE *dest = NULL;
    dest = fopen(destination_filename, "w");
    assert(dest != NULL && "Destination file creation error");

    fprint_poem((const char**) sorted_lines, dest);
    putc('\n', dest);
    fprint_poem((const char**) lines, dest);

    fclose(poem);
    free(text);
    fclose(dest);
    free_lines(lines, n_lines);
    free(lines);

    return 0;
}
