#include "lista_clientes.h"
#include <stdlib.h> // Para malloc, realloc, free
#include <stdio.h>  // Para fprintf
#include <string.h> // Para strcmp

#define TAMANHO_BLOCO_INICIAL 4 // Capacidade inicial para o vetor dinâmico

// Inicializa a lista de clientes
void lista_clientes_init(ListaClientes* L) {
    L->dados = NULL;
    L->tam = 0;
    L->cap = 0;
}

// Adiciona um cliente à lista, realocando se necessário
int lista_clientes_add(ListaClientes* L, Cliente c) {
    // Se o tamanho atual for igual à capacidade, precisamos expandir o vetor
    if (L->tam == L->cap) {
        size_t nova_capacidade = (L->cap == 0) ? TAMANHO_BLOCO_INICIAL : L->cap * 2;
        Cliente* temp = (Cliente*)realloc(L->dados, nova_capacidade * sizeof(Cliente));

        if (temp == NULL) {
            fprintf(stderr, "Erro: Falha ao realocar memória para a lista de clientes.\n");
            return 0; // Falha na alocação
        }
        L->dados = temp;
        L->cap = nova_capacidade;
    }

    L->dados[L->tam] = c; // Adiciona o cliente no final
    L->tam++;            // Incrementa o tamanho
    return 1;            // Sucesso
}

// Libera a memória alocada pela lista de clientes
void lista_clientes_free(ListaClientes* L) {
    free(L->dados); // Libera o bloco de memória principal
    L->dados = NULL;
    L->tam = 0;
    L->cap = 0;
}

// Busca um cliente pelo CPF
int lista_clientes_buscar_por_cpf(const ListaClientes* L, const char* cpf) {
    for (size_t i = 0; i < L->tam; i++) {
        if (strcmp(L->dados[i].cpf, cpf) == 0) {
            return (int)i; // Encontrou, retorna o índice
        }
    }
    return -1; // Não encontrou
}

// Busca um cliente pelo número da conta
int lista_clientes_buscar_por_conta(const ListaClientes* L, const char* conta) {
    for (size_t i = 0; i < L->tam; i++) {
        if (strcmp(L->dados[i].conta, conta) == 0) {
            return (int)i; // Encontrou, retorna o índice
        }
    }
    return -1; // Não encontrou
}

// Partição para o Quick Sort (por nome)
int quicksort_particao_nome(Cliente v[], int l, int r) {
    Cliente pivot = v[r]; // Escolhe o último elemento como pivô
    int i = (l - 1);       // Índice do menor elemento

    for (int j = l; j <= r - 1; j++) {
        // Se o elemento atual for "menor" ou igual ao pivô
        if (strcmp(v[j].nome, pivot.nome) <= 0) {
            i++; // Incrementa o índice do menor elemento
            trocar_clientes(&v[i], &v[j]);
        }
    }
    trocar_clientes(&v[i + 1], &v[r]);
    return (i + 1);
}

// Implementação do Quick Sort para ordenar clientes por nome
void quicksort_clientes_por_nome(Cliente v[], int l, int r) {
    if (l < r) {
        int pi = quicksort_particao_nome(v, l, r); // pi é o índice de partição
        quicksort_clientes_por_nome(v, l, pi - 1);  // Ordena os elementos antes da partição
        quicksort_clientes_por_nome(v, pi + 1, r);  // Ordena os elementos depois da partição
    }
}