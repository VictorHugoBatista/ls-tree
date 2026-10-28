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

void listdir(char *path, char level) {
    DIR *dir = opendir(path);

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

            // Cria a string com o tamanho preciso que vai tomar. Evida segerror por overflow.
            char new_path[strlen(path) + strlen(current_dirname) + 1];
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, current_dirname);
            
            // Recursão.
            listdir(new_path, level + 1);
        }
    }

    // Livra a string de tabs criada com malloc e fecha o diretório.
    free(tabs);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    char *path = argv[1];
    char level_zero = 0;

    listdir(path, level_zero);

    return EXIT_SUCCESS;
}
