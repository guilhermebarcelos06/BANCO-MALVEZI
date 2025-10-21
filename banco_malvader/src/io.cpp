#include "io.h"
#include <stdio.h>  // Para fgets, stdin
#include <string.h> // Para strcspn

// Lê uma linha de texto de forma segura, removendo o '\n' se presente
void ler_linha(char* buffer, size_t tamanho_maximo) {
    if (fgets(buffer, (int)tamanho_maximo, stdin)) {
        // Remove o '\n' se ele foi lido
        buffer[strcspn(buffer, "\n")] = 0;
    }
}