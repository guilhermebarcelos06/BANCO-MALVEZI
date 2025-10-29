#include <stdio.h>
#include <stdlib.h> // Para exit
#include <string.h> // Para strcmp
#include <locale.h>

#include "banco.h"
#include "io.h"
#include "cliente.h" // Apenas para novo_cliente_padrao e imprimir_dados_cliente, etc.

// ProtÃ³tipos das funÃ§Ãµes do menu
void exibir_menu(void);
int ler_opcao(void);
void lidar_abrir_conta(Banco* b);
void lidar_encerrar_conta(Banco* b);
void lidar_depositar(Banco* b);
void lidar_sacar(Banco* b);
void lidar_consultar_cliente(const Banco* b);
// ... Adicione mais handlers conforme as funcionalidades ...


int main(void) {
	setlocale(LC_ALL, "Portuguese");
    Banco meu_banco;
    // Nomes dos arquivos de persistÃªncia
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
            case 4: // Alterar dados (ainda nÃ£o implementado neste exemplo)
                printf("Funcionalidade 'Alterar Dados' em desenvolvimento.\n");
                break;
            case 5: // DepÃ³sito
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
        getchar(); // Espera o usuÃ¡rio pressionar ENTER
    } while (opcao != 0);

    banco_salvar(&meu_banco);
    banco_free(&meu_banco);

    return 0;
}

// --- FunÃ§Ãµes Auxiliares do Menu ---

void exibir_menu(void) {
    printf("\n--- Banco Malvader ---\n");
    printf("1. Abrir nova conta\n");
    printf("2. Encerrar conta\n");
    printf("3. Consultar dados do cliente\n");
    printf("4. Alterar dados do cliente\n");
    printf("5. Realizar deposito\n");
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
    // Isso Ã© uma forma comum, mas pode ser mais robusto dependendo do compilador.
    // Outra opÃ§Ã£o Ã© ler a linha toda como string e depois converter para int.
    if (scanf("%d", &op) != 1) {
        // Se a leitura falhar (ex: usuÃ¡rio digitou texto), limpa o buffer
        while (getchar() != '\n');
        return -1; // Retorna um valor invÃ¡lido
    }
    while (getchar() != '\n'); // Consome o '\n' deixado pelo scanf
    return op;
}

void lidar_abrir_conta(Banco* b) {
    Cliente novo = novo_cliente_padrao(); // ComeÃ§a com um cliente padrÃ£o

    printf("\n--- Abrir Nova Conta ---\n");

    printf("Nome completo: ");
    ler_linha(novo.nome, sizeof(novo.nome));

    printf("CPF (apenas números): ");
    ler_linha(novo.cpf, sizeof(novo.cpf));
	
	int proximo_numero = (int)b->clientes.tam + 1;
    gerar_numero_conta(novo.conta, proximo_numero);
    printf("Número da Conta gerado: %s\n", novo.conta);

    printf("Senha: ");
    ler_linha(novo.senha, sizeof(novo.senha));

    printf("Data de Nascimento (DD-MM-AAAA): ");
    ler_linha(novo.data_nasc, sizeof(novo.data_nasc));

    printf("Telefone: ");
    ler_linha(novo.telefone, sizeof(novo.telefone));

    printf("Endereço (Rua, Numero): ");
    ler_linha(novo.endereco, sizeof(novo.endereco));

    printf("Numero da Casa: ");
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

    // O saldo Ã© iniciado em 0.0 pelo novo_cliente_padrao

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
        printf("Operação de encerramento de conta concluida.\n");
    } else {
        printf("Falha ao encerrar conta.\n");
    }
}

void lidar_depositar(Banco* b) {
    char conta[16];
    double valor;
    printf("\n--- Realizar Deposito ---\n");
    printf("Digite o número da conta: ");
    ler_linha(conta, sizeof(conta));
    printf("Digite o valor do deposito: ");
    if (scanf("%lf", &valor) != 1) {
        printf("Valor inválido.\n");
        while (getchar() != '\n'); // Limpa buffer
        return;
    }
    while (getchar() != '\n'); // Limpa buffer

    if (banco_depositar(b, conta, valor)) {
        printf("Deposito realizado com sucesso.\n");
    } else {
        printf("Falha ao realizar deposito.\n");
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
        printf("Valor invalido.\n");
        while (getchar() != '\n'); // Limpa buffer
        return;
    }
    while (getchar() != '\n'); // Limpa buffer

    // Local para adicionar senha para acesso da conta, Gui.

    if (banco_sacar(b, conta, valor)) {
        printf("Saque realizado com sucesso.\n");
    } else {
        printf("Falha ao realizar saque.\n");
    }
}

void lidar_consultar_cliente(const Banco* b) {
    char termo_busca[100];
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
