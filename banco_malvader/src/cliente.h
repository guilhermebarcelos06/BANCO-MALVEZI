#ifndef CLIENTE_H
#define CLIENTE_H

#include <stddef.h> // Para size_t

// Definindo a struct Cliente conforme o modelo
typedef struct {
    char agencia[8];
    char conta[16];
    char nome[100];
    char cpf[15];
    char data_nasc[11];
    char telefone[20];
    char endereco[120];
    char cep[10];
    char local[60];
    char numero_casa[10];
    char bairro[60];
    char cidade[60];
    char estado[3];
    char senha[20];
    double saldo;
    int ativo; // 1 para ativa, 0 para encerrada
} Cliente;

// Funções para gerenciar um único cliente
// Exemplo: Inicializa um novo cliente com valores padrão
Cliente novo_cliente_padrao(void);

// Exemplo: Imprime os dados de um cliente
void imprimir_dados_cliente(const Cliente* c);

// Exemplo: Compara dois clientes pelo nome (para Quick Sort)
int nome_menor(const Cliente* a, const Cliente* b);

// Exemplo: Troca dois clientes de posição (para Quick Sort)
void trocar_clientes(Cliente* a, Cliente* b);


#endif // CLIENTE_H