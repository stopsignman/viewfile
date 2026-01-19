#include <stdio.h>
#include <errno.h>
#include <unistd.h>

void print_help_prompt() {
    fprintf(stderr, "Too few or too many arguments. Use ./viewfile -h for help.\n");
}

void print_help() {
    printf("Usage: ./viewfile [options] <filename>\n");
    printf("Options:\n");
    printf("  -n    Show line numbers\n");
    printf("  -b    Show byte count\n");
    printf("  -h    Show this help message\n");
}

int main(int argc, char *argv[]) {
    
    char data[4096];
    size_t bytesRead;
    size_t num = 1;
    size_t total_bytes = 0;
    int show_lines = 0;
    int show_bytes = 0;
    int opt;

    if (argc == 1) {
        print_help_prompt();
        return 1;
    }
    if (argc > 5) {
        print_help_prompt();
        return 1;
    }

    while ((opt = getopt(argc, argv, "nbh")) != -1) {
        switch (opt) {
            case 'n': show_lines = 1; break;
            case 'b': show_bytes = 1; break;
            case 'h': print_help(); return 0;
            default: print_help_prompt(); return 1;
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "No file provided.\n");
        print_help_prompt();
        return 1;
    }
    char *filename = argv[optind];
 
    FILE *file = fopen(filename, "r");
    if (!file) {
        int err = errno;
        if (err == EACCES) {
            fprintf(stderr, "Permission denied on file: %s\n", filename);
        } else if (err == ENOENT) {
            fprintf(stderr, "File not found: %s\n", filename);
        } else {
            fprintf(stderr, "Error opening file: %s\n", filename);
        }
        return 1;
    }

    while ((bytesRead = fread(data, sizeof(char), sizeof(data), file)) > 0) {
        if (show_lines) {printf("%zu ", num);}
        for (size_t i = 0; i < bytesRead; i++) {
            char ch = data[i];
            putchar(ch);
            if (ch == '\n' && show_lines) {
                printf("%zu ", num);
                num++;
            }
        }
        total_bytes += bytesRead;
    }

    if (show_bytes) {
        printf("\nTotal bytes read: %zu\n", total_bytes);
    }
    
    fclose(file);

    return 0;
}