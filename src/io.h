#ifndef IO_H
#define IO_H

#include <stddef.h> // Para size_t

// Função para ler uma linha de texto de forma segura
void ler_linha(char* buffer, size_t tamanho_maximo);

#endif // IO_H