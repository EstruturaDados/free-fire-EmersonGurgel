#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da struct Item, conforme o requisito.
// Armazena informações essenciais de cada objeto coletado.
typedef struct {
    char nome[30];
    char tipo[20]; // Ex: arma, munição, cura, ferramenta
    int quantidade;
} Item;

// Constante para o tamanho máximo do inventário (mochila)
#define MAX_ITENS 10

// Variável global para o vetor de structs que representa o inventário
Item inventario[MAX_ITENS];
// Variável global para rastrear o número atual de itens no inventário
int num_itens = 0;

// Protótipos das funções obrigatórias
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void exibirMenu();

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\n--- MOCHILA DE LOOT INICIAL ---\n");
    printf("1. Cadastrar novo item (Coletar)\n");
    printf("2. Remover item (Descartar)\n");
    printf("3. Listar todos os itens\n");
    printf("4. Buscar item por nome\n");
    printf("0. Sair do Jogo\n");
    printf("Escolha uma opção: ");
}

/**
 * @brief Cadastra um novo item no inventário.
 * O sistema deve permitir que o jogador cadastre até 10 itens.
 */
void inserirItem() {
    // Verifica se o inventário está cheio
    if (num_itens >= MAX_ITENS) {
        printf("\n[ALERTA] Mochila cheia! Não é possível coletar mais itens (%d/%d).\n", num_itens, MAX_ITENS);
        return;
    }

    printf("\n--- CADASTRO DE NOVO ITEM ---\n");

    // Cadastro do Nome
    printf("Digite o nome do item (max 29 caracteres): ");
    // Uso de scanf com %29s para evitar buffer overflow,
    // pois a entrada do nome não deve conter espaços para esta implementação simples.
    // Para entradas com espaços, seria ideal usar fgets() e limpar o '\n'.
    if (scanf("%29s", inventario[num_itens].nome) != 1) {
        printf("[ERRO] Falha ao ler o nome. Abortando cadastro.\n");
        return;
    }

    // Cadastro do Tipo
    printf("Digite o tipo do item (ex: arma, munição, cura, ferramenta) (max 19 caracteres): ");
    if (scanf("%19s", inventario[num_itens].tipo) != 1) {
        printf("[ERRO] Falha ao ler o tipo. Abortando cadastro.\n");
        return;
    }

    // Cadastro da Quantidade
    printf("Digite a quantidade do item: ");
    if (scanf("%d", &inventario[num_itens].quantidade) != 1 || inventario[num_itens].quantidade <= 0) {
        printf("[ERRO] Quantidade inválida. A quantidade deve ser um número inteiro positivo. Abortando cadastro.\n");
        return;
    }

    // Incrementa o contador de itens
    num_itens++;
    printf("\n[SUCESSO] Item '%s' cadastrado na mochila!\n", inventario[num_itens - 1].nome);

    // Requisito: Listar itens após cada operação.
    listarItens();
}

/**
 * @brief Remove um item do inventário, buscando-o pelo nome.
 * O sistema reorganiza o vetor após a remoção.
 */
void removerItem() {
    if (num_itens == 0) {
        printf("\n[ALERTA] A mochila está vazia. Não há itens para remover.\n");
        return;
    }

    char nome_remover[30];
    printf("\n--- REMOÇÃO DE ITEM ---\n");
    printf("Digite o nome do item que deseja descartar (remover): ");

    // Limpa o buffer de entrada antes de ler a string, caso haja '\n' pendente
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    
    // Leitura do nome com fgets para ser mais robusto, limitando o tamanho
    if (fgets(nome_remover, sizeof(nome_remover), stdin) == NULL) {
        printf("[ERRO] Falha ao ler a entrada.\n");
        return;
    }
    // Remove o '\n' que fgets adiciona no final da string, se houver
    nome_remover[strcspn(nome_remover, "\n")] = 0;

    int encontrado = 0;
    int i;
    for (i = 0; i < num_itens; i++) {
        // Compara o nome digitado com o nome do item no inventário
        if (strcmp(inventario[i].nome, nome_remover) == 0) {
            // Item encontrado
            encontrado = 1;

            // Desloca os elementos subsequentes para preencher a lacuna (reorganização)
            for (int j = i; j < num_itens - 1; j++) {
                inventario[j] = inventario[j + 1];
            }

            // Decrementa o contador de itens
            num_itens--;
            printf("\n[SUCESSO] O item '%s' foi descartado da mochila.\n", nome_remover);
            break; // Sai do loop após a remoção
        }
    }

    if (!encontrado) {
        printf("\n[ALERTA] Item com o nome '%s' não encontrado na mochila.\n", nome_remover);
    }

    // Requisito: Listar itens após cada operação.
    listarItens();
}

/**
 * @brief Lista todos os itens atualmente registrados na mochila.
 */
void listarItens() {
    printf("\n--- INVENTÁRIO ATUAL (%d/%d) ---\n", num_itens, MAX_ITENS);

    if (num_itens == 0) {
        printf("A mochila está vazia. Colete itens essenciais!\n");
        return;
    }

    printf("+------+------------------------------+--------------------+--------------+\n");
    printf("| POS. | NOME                         | TIPO               | QUANTIDADE   |\n");
    printf("+------+------------------------------+--------------------+--------------+\n");
    for (int i = 0; i < num_itens; i++) {
        // Formatação para garantir alinhamento (manutenibilidade e usabilidade)
        printf("| %-4d | %-28s | %-18s | %-12d |\n",
               i + 1, inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
    }
    printf("+------+------------------------------+--------------------+--------------+\n");
}

/**
 * @brief Implementa uma função de busca sequencial.
 * Localiza um item na mochila com base no nome e exibe seus dados.
 */
void buscarItem() {
    if (num_itens == 0) {
        printf("\n[ALERTA] A mochila está vazia. Não há itens para buscar.\n");
        return;
    }

    char nome_buscar[30];
    printf("\n--- BUSCA SEQUENCIAL DE ITEM ---\n");
    printf("Digite o nome do item que deseja buscar: ");

    // Limpa o buffer de entrada antes de ler a string, caso haja '\n' pendente
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    // Leitura do nome com fgets
    if (fgets(nome_buscar, sizeof(nome_buscar), stdin) == NULL) {
        printf("[ERRO] Falha ao ler a entrada.\n");
        return;
    }
    // Remove o '\n'
    nome_buscar[strcspn(nome_buscar, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < num_itens; i++) {
        // Busca sequencial: compara o nome digitado com o nome do item
        if (strcmp(inventario[i].nome, nome_buscar) == 0) {
            encontrado = 1;
            printf("\n[ITEM ENCONTRADO] Dados do item '%s':\n", nome_buscar);
            printf("  -> Posição na mochila: %d\n", i + 1);
            printf("  -> Tipo: %s\n", inventario[i].tipo);
            printf("  -> Quantidade: %d\n", inventario[i].quantidade);
            break; // O desempenho é melhorado ao sair assim que o item é encontrado
        }
    }

    if (!encontrado) {
        printf("\n[INFO] O item '%s' não foi encontrado na mochila.\n", nome_buscar);
    }
}

/**
 * @brief Função principal, controlando o fluxo do programa.
 */
int main() {
    int opcao;

    printf("============================================\n");
    printf("       SISTEMA DE INVENTÁRIO DO JOGADOR\n");
    printf("  (Mochila de Loot Inicial - Capacidade %d)\n", MAX_ITENS);
    printf("============================================\n");

    do {
        // Exibe o menu para usabilidade
        exibirMenu();

        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada inválida. Por favor, digite um número.\n");
            // Limpa o buffer para evitar loop infinito
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1; // Força a repetição do loop
            continue;
        }

        // Fluxo de execução de acordo com a escolha
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\n[FIM] Organização do inventário concluída. Boa sorte no jogo!\n");
                break;
            default:
                printf("\n[ALERTA] Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}