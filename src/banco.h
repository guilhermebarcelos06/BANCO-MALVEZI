#ifndef BANCO_H
#define BANCO_H

#include "lista_clientes.h" // Inclui a definição de ListaClientes

// Estrutura para representar o sistema bancário
typedef struct {
    ListaClientes clientes;
    char arquivo_clientes[256];   // Nome do arquivo para persistir clientes
    char arquivo_movimentos[256]; // Nome do arquivo para persistir movimentos
} Banco;

// Inicializa o sistema bancário
void banco_init(Banco* b, const char* arq_clientes, const char* arq_movimentos);

// Carrega os dados do banco dos arquivos
int banco_carregar(Banco* b);

// Salva os dados do banco nos arquivos
int banco_salvar(Banco* b);

// Libera os recursos alocados pelo banco
void banco_free(Banco* b);

// Operações bancárias principais
int banco_abrir_conta(Banco* b, Cliente novo_cliente);
int banco_encerrar_conta(Banco* b, const char* conta);
int banco_depositar(Banco* b, const char* conta, double valor);
int banco_sacar(Banco* b, const char* conta, double valor);
// int banco_alterar_dados_cliente(Banco* b, const char* cpf, Cliente novos_dados); // Exemplo futuro
// void banco_consultar_cliente(const Banco* b, const char* cpf); // Exemplo futuro

// Funções de relatório/listagem
void banco_listar_clientes(const Banco* b);
void banco_listar_clientes_ordenado_por_nome(Banco* b);

// Funções de persistência
int salvar_clientes(const char* arq, const ListaClientes* L);
int carregar_clientes(const char* arq, ListaClientes* L);

// Função para registrar movimentos
int registrar_movimento(const char* arq_movimentos, const char* conta, const char* tipo, double valor, double saldo_apos);


#endif // BANCO_H