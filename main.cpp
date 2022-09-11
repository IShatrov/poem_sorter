#include "poem_sorter.h"

int main(int argc, const char *argv[])
{
    const char *poem_filename = "poem.txt";
    const char *destination_filename = "des.txt";

    if (argc > 1) poem_filename = argv[1];
    if (argc > 2) destination_filename = argv[2];

    FILE *poem = NULL;
    poem = fopen(poem_filename, "r");
    assert(poem != NULL && "Poem file not found");

    FILE *des = NULL;
    des = fopen(destination_filename, "w");
    assert(des != NULL && "Destination file creation error");

    char **lines = (char**) malloc(DEFAULT_N_LINES * sizeof(char*));
    int n_lines = DEFAULT_N_LINES;

    read_poem(&lines, &n_lines, poem);

    sort_poem(lines, n_lines);

    fprint_poem(lines, n_lines, des);

    fclose(poem);
    fclose(des);
    for (int i = 0; i < n_lines; ++i) free(lines[i]);
    free(lines);

    return 0;
}
