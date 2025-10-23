#include "banco.h"
#include "io.h"
#include <stdio.h>    // Para printf, fopen, fclose, fprintf, sscanf
#include <string.h>   // Para strcpy, strcmp, strcspn
#include <stdlib.h>   // Para atof (se usar para converter saldo de string)
#include <time.h>     // Para registrar_movimento (data/hora)


// Inicializa o sistema bancário
void banco_init(Banco* b, const char* arq_clientes, const char* arq_movimentos) {
    lista_clientes_init(&b->clientes);
    strcpy(b->arquivo_clientes, arq_clientes);
    strcpy(b->arquivo_movimentos, arq_movimentos);
    printf("Banco Malvader inicializado com arquivos: %s e %s\n", arq_clientes, arq_movimentos);
}

// Carrega os dados do banco dos arquivos
int banco_carregar(Banco* b) {
    printf("Carregando clientes de %s...\n", b->arquivo_clientes);
    if (!carregar_clientes(b->arquivo_clientes, &b->clientes)) {
        fprintf(stderr, "Erro ao carregar clientes.\n");
        return 0;
    }
    printf("Clientes carregados: %zu\n", b->clientes.tam);
    // Aqui você poderia adicionar o carregamento de movimentos se implementado
    return 1;
}

// Salva os dados do banco nos arquivos
int banco_salvar(Banco* b) {
    printf("Salvando clientes em %s...\n", b->arquivo_clientes);
    if (!salvar_clientes(b->arquivo_clientes, &b->clientes)) {
        fprintf(stderr, "Erro ao salvar clientes.\n");
        return 0;
    }
    printf("Clientes salvos.\n");
    // Aqui você poderia adicionar o salvamento de movimentos se implementado
    return 1;
}

// Libera os recursos alocados pelo banco
void banco_free(Banco* b) {
    lista_clientes_free(&b->clientes);
    printf("Recursos do Banco Malvader liberados.\n");
}

// Abertura de conta
int banco_abrir_conta(Banco* b, Cliente novo_cliente) {
    // Verificar se o CPF já existe
    if (lista_clientes_buscar_por_cpf(&b->clientes, novo_cliente.cpf) != -1) {
        fprintf(stderr, "Erro: Cliente com CPF %s já existe.\n", novo_cliente.cpf);
        return 0;
    }
    // Verificar se a conta já existe
    if (lista_clientes_buscar_por_conta(&b->clientes, novo_cliente.conta) != -1) {
        fprintf(stderr, "Erro: Número da conta %s já está em uso.\n", novo_cliente.conta);
        return 0;
    }

    if (!lista_clientes_add(&b->clientes, novo_cliente)) {
        fprintf(stderr, "Erro: Falha ao adicionar novo cliente à lista.\n");
        return 0;
    }
    printf("Conta aberta com sucesso para %s.\n", novo_cliente.nome);
    return 1;
}

// Encerrar conta
int banco_encerrar_conta(Banco* b, const char* conta) {
    int idx = lista_clientes_buscar_por_conta(&b->clientes, conta);
    if (idx == -1) {
        fprintf(stderr, "Erro: Conta %s não encontrada.\n", conta);
        return 0;
    }

    if (b->clientes.dados[idx].saldo != 0.0) {
        fprintf(stderr, "Erro: Não é possível encerrar conta com saldo diferente de zero (%.2f).\n", b->clientes.dados[idx].saldo);
        return 0;
    }

    b->clientes.dados[idx].ativo = 0; // Marca como inativa
    printf("Conta %s de %s encerrada com sucesso.\n", conta, b->clientes.dados[idx].nome);
    return 1;
}

// Depósito
int banco_depositar(Banco* b, const char* conta, double valor) {
    if (valor <= 0) {
        fprintf(stderr, "Erro: Valor de depósito deve ser positivo.\n");
        return 0;
    }

    int idx = lista_clientes_buscar_por_conta(&b->clientes, conta);
    if (idx == -1) {
        fprintf(stderr, "Erro: Conta %s não encontrada.\n", conta);
        return 0;
    }
    if (!b->clientes.dados[idx].ativo) {
        fprintf(stderr, "Erro: Conta %s está inativa e não pode receber depósitos.\n", conta);
        return 0;
    }

    b->clientes.dados[idx].saldo += valor;
    printf("Depósito de %.2f realizado na conta %s. Novo saldo: %.2f\n", valor, conta, b->clientes.dados[idx].saldo);
    registrar_movimento(b->arquivo_movimentos, conta, "DEPOSITO", valor, b->clientes.dados[idx].saldo);
    return 1;
}

// Saque
int banco_sacar(Banco* b, const char* conta, double valor) {
    if (valor <= 0) {
        fprintf(stderr, "Erro: Valor de saque deve ser positivo.\n");
        return 0;
    }

    int idx = lista_clientes_buscar_por_conta(&b->clientes, conta);
    if (idx == -1) {
        fprintf(stderr, "Erro: Conta %s não encontrada.\n", conta);
        return 0;
    }
    if (!b->clientes.dados[idx].ativo) {
        fprintf(stderr, "Erro: Conta %s está inativa e não pode realizar saques.\n", conta);
        return 0;
    }

    if (b->clientes.dados[idx].saldo < valor) {
        fprintf(stderr, "Erro: Saldo insuficiente na conta %s. Saldo atual: %.2f\n", conta, b->clientes.dados[idx].saldo);
        return 0;
    }

    b->clientes.dados[idx].saldo -= valor;
    printf("Saque de %.2f realizado da conta %s. Novo saldo: %.2f\n", valor, conta, b->clientes.dados[idx].saldo);
    registrar_movimento(b->arquivo_movimentos, conta, "SAQUE", valor, b->clientes.dados[idx].saldo);
    return 1;
}

// Lista todos os clientes na ordem atual
void banco_listar_clientes(const Banco* b) {
    printf("\n--- Lista de Clientes ---\n");
    if (b->clientes.tam == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    for (size_t i = 0; i < b->clientes.tam; i++) {
        imprimir_dados_cliente(&b->clientes.dados[i]);
    }
    printf("------------------------\n");
}

// Lista clientes ordenados por nome (cria uma cópia para ordenar e não alterar a ordem original)
void banco_listar_clientes_ordenado_por_nome(Banco* b) {
    if (b->clientes.tam == 0) {
        printf("Nenhum cliente cadastrado para ordenar.\n");
        return;
    }

    // Criar uma cópia do vetor de clientes para ordenar
    Cliente* copia_clientes = (Cliente*)malloc(b->clientes.tam * sizeof(Cliente));
    if (copia_clientes == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para cópia de clientes.\n");
        return;
    }
    memcpy(copia_clientes, b->clientes.dados, b->clientes.tam * sizeof(Cliente));

    // Ordenar a cópia
    quicksort_clientes_por_nome(copia_clientes, 0, (int)b->clientes.tam - 1);

    printf("\n--- Lista de Clientes (Ordenada por Nome) ---\n");
    for (size_t i = 0; i < b->clientes.tam; i++) {
        imprimir_dados_cliente(&copia_clientes[i]);
    }
    printf("---------------------------------------------\n");

    free(copia_clientes); // Liberar a memória da cópia
}


// --- Funções de Persistência ---

// Salva os clientes em um arquivo de texto
int salvar_clientes(const char* arq, const ListaClientes* L) {
    FILE* f = fopen(arq, "w");
    if (!f) {
        perror("Erro ao abrir arquivo de clientes para escrita");
        return 0;
    }

    for (size_t i = 0; i < L->tam; i++) {
        const Cliente* c = &L->dados[i];
        fprintf(f, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%.2f;%d\n",
                c->agencia, c->conta, c->nome, c->cpf, c->data_nasc, c->telefone,
                c->endereco, c->cep, c->local, c->numero_casa, c->bairro,
                c->cidade, c->estado, c->senha, c->saldo, c->ativo);
    }

    fclose(f);
    return 1;
}

// Carrega os clientes de um arquivo de texto
int carregar_clientes(const char* arq, ListaClientes* L) {
    FILE* f = fopen(arq, "r");
    if (!f) {
        perror("Erro ao abrir arquivo de clientes para leitura (pode não existir ainda)");
        return 1; // Não é um erro fatal se o arquivo ainda não existe
    }

    char linha[512];
    Cliente c;
    int ativo_temp; // Para ler o int ativo

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = 0; // Remove o '\n'

        // Tenta parsear a linha
        if (sscanf(linha, "%7[^;];%15[^;];%99[^;];%14[^;];%10[^;];%19[^;];%119[^;];%9[^;];%59[^;];%9[^;];%59[^;];%59[^;];%2[^;];%19[^;];%lf;%d",
                   c.agencia, c.conta, c.nome, c.cpf, c.data_nasc, c.telefone,
                   c.endereco, c.cep, c.local, c.numero_casa, c.bairro,
                   c.cidade, c.estado, c.senha, &c.saldo, &ativo_temp) == 15) {
            c.ativo = ativo_temp;
            if (!lista_clientes_add(L, c)) {
                fprintf(stderr, "Erro ao adicionar cliente carregado à lista.\n");
                fclose(f);
                return 0;
            }
        } else {
            fprintf(stderr, "Erro ao ler linha de cliente do arquivo: %s\n", linha);
        }
    }

    fclose(f);
    return 1;
}

// Registra um movimento bancário em um arquivo de texto
int registrar_movimento(const char* arq_movimentos, const char* conta, const char* tipo, double valor, double saldo_apos) {
    FILE* f = fopen(arq_movimentos, "a"); // Abre em modo append
    if (!f) {
        perror("Erro ao abrir arquivo de movimentos para escrita");
        return 0;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(f, "%d-%02d-%02d;%s;%s;%.2f;saldo=%.2f\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            conta, tipo, valor, saldo_apos);

    fclose(f);
    return 1;
}