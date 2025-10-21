#include <stdio.h>
#include <stdlib.h> // Para exit
#include <string.h> // Para strcmp

#include "src/banco.h"
#include "src/io.h"
#include "src/cliente.h" // Apenas para novo_cliente_padrao e imprimir_dados_cliente, etc.

// Protótipos das funções do menu
void exibir_menu(void);
int ler_opcao(void);
void lidar_abrir_conta(Banco* b);
void lidar_encerrar_conta(Banco* b);
void lidar_depositar(Banco* b);
void lidar_sacar(Banco* b);
void lidar_consultar_cliente(const Banco* b);
// ... Adicione mais handlers conforme as funcionalidades ...


int main(void) {
    Banco meu_banco;
    // Nomes dos arquivos de persistência
    const char* arq_clientes = "data/clientes.txt";
    const char* arq_movimentos = "data/movimentos.txt";

    banco_init(&meu_banco, arq_clientes, arq_movimentos);
    banco_carregar(&meu_banco);

    int opcao;
    do {
        exibir_menu();
        opcao = ler_opcao();

        switch (opcao) {
            case 1: // Abrir conta
                lidar_abrir_conta(&meu_banco);
                break;
            case 2: // Encerrar conta
                lidar_encerrar_conta(&meu_banco);
                break;
            case 3: // Consultar cliente (por CPF ou Conta)
                lidar_consultar_cliente(&meu_banco);
                break;
            case 4: // Alterar dados (ainda não implementado neste exemplo)
                printf("Funcionalidade 'Alterar Dados' em desenvolvimento.\n");
                break;
            case 5: // Depósito
                lidar_depositar(&meu_banco);
                break;
            case 6: // Saque
                lidar_sacar(&meu_banco);
                break;
            case 7: // Listar clientes (ordem de cadastro)
                banco_listar_clientes(&meu_banco);
                break;
            case 8: // Listar clientes ordenados por nome
                banco_listar_clientes_ordenado_por_nome(&meu_banco);
                break;
            case 0: // Sair
                printf("Saindo do Banco Malvader. Salvando dados...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
        printf("\nPressione ENTER para continuar...");
        getchar(); // Consome o '\n' pendente
        getchar(); // Espera o usuário pressionar ENTER
    } while (opcao != 0);

    banco_salvar(&meu_banco);
    banco_free(&meu_banco);

    return 0;
}

// --- Funções Auxiliares do Menu ---

void exibir_menu(void) {
    printf("\n--- Banco Malvader ---\n");
    printf("1. Abrir nova conta\n");
    printf("2. Encerrar conta\n");
    printf("3. Consultar dados do cliente\n");
    printf("4. Alterar dados do cliente\n");
    printf("5. Realizar depósito\n");
    printf("6. Realizar saque\n");
    printf("7. Listar todos os clientes\n");
    printf("8. Listar clientes (Ordenado por Nome)\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}

int ler_opcao(void) {
    int op;
    // Limpa o buffer de entrada antes de ler um int para evitar problemas
    // com caracteres residuais de entradas anteriores.
    // Isso é uma forma comum, mas pode ser mais robusto dependendo do compilador.
    // Outra opção é ler a linha toda como string e depois converter para int.
    if (scanf("%d", &op) != 1) {
        // Se a leitura falhar (ex: usuário digitou texto), limpa o buffer
        while (getchar() != '\n');
        return -1; // Retorna um valor inválido
    }
    while (getchar() != '\n'); // Consome o '\n' deixado pelo scanf
    return op;
}

void lidar_abrir_conta(Banco* b) {
    Cliente novo = novo_cliente_padrao(); // Começa com um cliente padrão

    printf("\n--- Abrir Nova Conta ---\n");

    printf("Nome completo: ");
    ler_linha(novo.nome, sizeof(novo.nome));

    printf("CPF (apenas números): ");
    ler_linha(novo.cpf, sizeof(novo.cpf));

    printf("Número da Conta: "); // Simples por enquanto, sem geração automática
    ler_linha(novo.conta, sizeof(novo.conta));

    printf("Senha: ");
    ler_linha(novo.senha, sizeof(novo.senha));

    // Mais campos podem ser solicitados aqui, ou preenchidos com valores padrão
    printf("Data de Nascimento (DD-MM-AAAA): ");
    ler_linha(novo.data_nasc, sizeof(novo.data_nasc));

    printf("Telefone: ");
    ler_linha(novo.telefone, sizeof(novo.telefone));

    printf("Endereço (Rua, Número): ");
    ler_linha(novo.endereco, sizeof(novo.endereco));

    printf("Número da Casa: ");
    ler_linha(novo.numero_casa, sizeof(novo.numero_casa));

    printf("Bairro: ");
    ler_linha(novo.bairro, sizeof(novo.bairro));

    printf("CEP: ");
    ler_linha(novo.cep, sizeof(novo.cep));

    printf("Local (Ponto de referência): ");
    ler_linha(novo.local, sizeof(novo.local));

    printf("Cidade: ");
    ler_linha(novo.cidade, sizeof(novo.cidade));

    printf("Estado (UF): ");
    ler_linha(novo.estado, sizeof(novo.estado));

    // O saldo é iniciado em 0.0 pelo novo_cliente_padrao

    if (banco_abrir_conta(b, novo)) {
        printf("Conta aberta e cliente cadastrado com sucesso!\n");
    } else {
        printf("Falha ao abrir conta. Verifique os dados.\n");
    }
}

void lidar_encerrar_conta(Banco* b) {
    char conta[16];
    printf("\n--- Encerrar Conta ---\n");
    printf("Digite o número da conta a ser encerrada: ");
    ler_linha(conta, sizeof(conta));

    if (banco_encerrar_conta(b, conta)) {
        printf("Operação de encerramento de conta concluída.\n");
    } else {
        printf("Falha ao encerrar conta.\n");
    }
}

void lidar_depositar(Banco* b) {
    char conta[16];
    double valor;
    printf("\n--- Realizar Depósito ---\n");
    printf("Digite o número da conta: ");
    ler_linha(conta, sizeof(conta));
    printf("Digite o valor do depósito: ");
    if (scanf("%lf", &valor) != 1) {
        printf("Valor inválido.\n");
        while (getchar() != '\n'); // Limpa buffer
        return;
    }
    while (getchar() != '\n'); // Limpa buffer

    if (banco_depositar(b, conta, valor)) {
        printf("Depósito realizado com sucesso.\n");
    } else {
        printf("Falha ao realizar depósito.\n");
    }
}

void lidar_sacar(Banco* b) {
    char conta[16];
    double valor;
    printf("\n--- Realizar Saque ---\n");
    printf("Digite o número da conta: ");
    ler_linha(conta, sizeof(conta));
    printf("Digite o valor do saque: ");
    if (scanf("%lf", &valor) != 1) {
        printf("Valor inválido.\n");
        while (getchar() != '\n'); // Limpa buffer
        return;
    }
    while (getchar() != '\n'); // Limpa buffer

    // Aqui você poderia adicionar uma validação de senha antes do saque, como um desafio extra
    // char senha_digitada[20];
    // printf("Digite a senha da conta: ");
    // ler_linha(senha_digitada, sizeof(senha_digitada));
    // int idx = lista_clientes_buscar_por_conta(&b->clientes, conta);
    // if (idx != -1 && strcmp(b->clientes.dados[idx].senha, senha_digitada) == 0) { ... } else { ... }

    if (banco_sacar(b, conta, valor)) {
        printf("Saque realizado com sucesso.\n");
    } else {
        printf("Falha ao realizar saque.\n");
    }
}

void lidar_consultar_cliente(const Banco* b) {
    char termo_busca[100]; // Suficientemente grande para CPF ou Nome
    printf("\n--- Consultar Cliente ---\n");
    printf("Deseja buscar por CPF (1) ou Número da Conta (2)? ");
    int tipo_busca = ler_opcao();
    int idx = -1;

    if (tipo_busca == 1) {
        printf("Digite o CPF do cliente: ");
        ler_linha(termo_busca, sizeof(termo_busca));
        idx = lista_clientes_buscar_por_cpf(&b->clientes, termo_busca);
    } else if (tipo_busca == 2) {
        printf("Digite o Número da Conta do cliente: ");
        ler_linha(termo_busca, sizeof(termo_busca));
        idx = lista_clientes_buscar_por_conta(&b->clientes, termo_busca);
    } else {
        printf("Opção de busca inválida.\n");
        return;
    }

    if (idx != -1) {
        imprimir_dados_cliente(&b->clientes.dados[idx]);
    } else {
        printf("Cliente não encontrado.\n");
    }
}