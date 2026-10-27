#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

char* maketabs(char size) {
    char *tabs = malloc(size + 1);

    if (tabs == NULL) {
        printf("ERROR: INSUFICIENT MEMORY");
    }

    tabs[0] = '\0';

    for (int i = 0; i < size; i++) {
        strcat(tabs, " ");
    }

    return tabs;
}

void listdir(char *dirname, char level) {
    DIR *dir = opendir(dirname);

    struct dirent *entry;

    char *tabs = maketabs(level);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        printf("%s - (%x)%s - %lu\n", tabs, entry->d_type, entry->d_name, entry->d_ino);
        
        if (entry->d_type == 4) {
            char *current_dirname = entry-> d_name;
            char *path = realpath(current_dirname, NULL);

            char new_dirname[40];
            strcpy(new_dirname, dirname);
            strcat(new_dirname, "/");
            strcat(new_dirname, current_dirname);
            
            listdir(new_dirname, level + 1);
        }
    }

    free(tabs);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    char *dirname = argv[1];
    char level_zero = 0;

    listdir(dirname, level_zero);

    return EXIT_SUCCESS;
}
