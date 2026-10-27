#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// Cria os espaços dos níveis de diretórios.
char* maketabs(char size) {
    // Aloca o espaço de memória para cada espaço mais o caractere inicial da string (\0).
    char *tabs = malloc(size + 1);

    if (tabs == NULL) {
        printf("ERROR: INSUFICIENT MEMORY");
    }

    // Inicia string, removendo caracteres estranhos.
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
        // Não lista o diretório atual e o anterior.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Printa o arquivo atual.
        printf("%s - (%x)%s - %lu\n", tabs, entry->d_type, entry->d_name, entry->d_ino);
        
        // Monta o path do próximo diretório e o chama novamente.
        if (entry->d_type == 4) {
            char *current_dirname = entry-> d_name;
            char *path = realpath(current_dirname, NULL);

            // Cria a string com o tamanho preciso que vai tomar. Evida segerror por overflow.
            char new_dirname[strlen(dirname) + strlen(current_dirname) + 1];
            strcpy(new_dirname, dirname);
            strcat(new_dirname, "/");
            strcat(new_dirname, current_dirname);
            
            // Recursão.
            listdir(new_dirname, level + 1);
        }
    }

    // Livra a string de tabs criada com malloc e fecha o diretório.
    free(tabs);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    char *dirname = argv[1];
    char level_zero = 0;

    listdir(dirname, level_zero);

    return EXIT_SUCCESS;
}
