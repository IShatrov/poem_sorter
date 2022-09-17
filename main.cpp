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


    //printf("%d\n", struct_array[0].line);

//    for(int i = 0; i < n_lines; ++i)
//    {
//        printf("%-50s\t%d\t%d\n", (*lines + i)->line, (*lines + i)->len, /*strlen((*lines + i)->line)*/5);
//    }

    struct line_info **sorted_lines = copy_struct_array(lines, n_lines);

    for(int i = 0; i < n_lines; ++i)
    {
        printf("%d %-50s\t%d\t%d\n", i, (*sorted_lines + i)->line, (*sorted_lines + i)->len, /*strlen((*lines + i)->line)*/5);
    }
//    int a = 4, b = 0;
//    printf("%s\n%s\n\n", sorted_lines[a]->line, sorted_lines[b]->line);
//    printf("%d", line_cmp(&sorted_lines[a], &sorted_lines[b]));
    qsort(sorted_lines, n_lines - 1, sizeof(struct line_info*), &line_cmp);

//    for(int i = 0; i < n_lines; ++i)
//    {
//        printf("%d %-50s\t%d\t%d\n", i, (*sorted_lines + i)->line, (*sorted_lines + i)->len, /*strlen((*lines + i)->line)*/5);
//    }

    FILE *dest = NULL;
    dest = fopen(destination_filename, "w");
    assert(dest != NULL && "Destination file creation error");

//    fprint_poem((const char**) sorted_lines, dest);
//putc('\n', dest);
//    fprint_poem((const char**) lines, dest);

    fclose(poem);
    free(text);
    fclose(dest);
//    free_lines(lines, n_lines);
    free(lines);

    return 0;
}
