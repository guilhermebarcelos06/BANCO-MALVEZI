#ifndef CLIENTE_H
#define CLIENTE_H

#include <stddef.h> // Para size_t

// Definindo a struct Cliente conforme o modelo
typedef struct {
    char agencia[8];
    char conta[16];
    char nome[200];
    char cpf[14];
    char data_nasc[14];
    char telefone[22];
    char endereco[120];
    char cep[12];
    char local[60];
    char numero_casa[6];
    char bairro[60];
    char cidade[60];
    char estado[4];
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