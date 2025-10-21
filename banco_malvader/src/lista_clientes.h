#ifndef LISTA_CLIENTES_H
#define LISTA_CLIENTES_H

#include "cliente.h" // Inclui a definição de Cliente
#include <stddef.h>  // Para size_t

// Estrutura para gerenciar uma lista dinâmica de clientes
typedef struct {
    Cliente* dados; // Ponteiro para o vetor de clientes
    size_t tam;     // Número atual de clientes na lista
    size_t cap;     // Capacidade máxima atual do vetor
} ListaClientes;

// Inicializa a lista de clientes
void lista_clientes_init(ListaClientes* L);

// Adiciona um cliente à lista. Retorna 1 em caso de sucesso, 0 em caso de falha (ex: falha de realocação)
int lista_clientes_add(ListaClientes* L, Cliente c);

// Libera a memória alocada pela lista de clientes
void lista_clientes_free(ListaClientes* L);

// Busca um cliente pelo CPF
// Retorna o índice do cliente se encontrado, -1 caso contrário
int lista_clientes_buscar_por_cpf(const ListaClientes* L, const char* cpf);

// Busca um cliente pelo número da conta
// Retorna o índice do cliente se encontrado, -1 caso contrário
int lista_clientes_buscar_por_conta(const ListaClientes* L, const char* conta);

// Implementação do Quick Sort para ordenar clientes por nome
void quicksort_clientes_por_nome(Cliente v[], int l, int r);

// Partição para o Quick Sort
int quicksort_particao_nome(Cliente v[], int l, int r);

#endif // LISTA_CLIENTES_H