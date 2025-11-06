#include "cliente.h"
#include <stdio.h>   // Para printf
#include <string.h>  // Para strcpy

// Inicializa um novo cliente com valores padrão
Cliente novo_cliente_padrao(void) {
    Cliente c = {0}; // Inicializa todos os membros com zero/null
    strcpy(c.agencia, "0001");
    strcpy(c.estado, "DF"); // Exemplo de estado padrão
    c.saldo = 0.0;
    c.ativo = 1; // Ativo por padrão
    return c;
}

// Imprime os dados de um cliente
void imprimir_dados_cliente(const Cliente* c) {
    printf("--- Dados do Cliente ---\n");
    printf("Ag�ncia: %s\n", c->agencia);
    printf("Conta: %s\n", c->conta);
    printf("Nome: %s\n", c->nome);
    printf("CPF: %s\n", c->cpf);
    printf("Data Nasc.: %s\n", c->data_nasc);
    printf("Telefone: %s\n", c->telefone);
    printf("Endere�o: %s, %s - %s, %s - %s/%s\n", c->endereco, c->numero_casa, c->bairro, c->local, c->cidade, c->estado);
    printf("CEP: %s\n", c->cep);
    printf("Saldo: %.2f\n", c->saldo);
    printf("Status: %s\n", c->ativo ? "Ativa" : "Encerrada");
    printf("------------------------\n");
}

// Compara dois clientes pelo nome
// Retorna < 0 se a.nome for "menor" que b.nome, 0 se iguais, > 0 se "maior"
int nome_menor(const Cliente* a, const Cliente* b) {
    return strcmp(a->nome, b->nome);
}

// Troca dois clientes de posição na memória
void trocar_clientes(Cliente* a, Cliente* b) {
    Cliente temp = *a;
    *a = *b;
    *b = temp;
}
