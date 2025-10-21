#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para medir o tempo das operações

// =========================================================================
// 1. CRIAÇÃO DA STRUCT Item
// =========================================================================

// Struct base para representar um item no inventário
typedef struct {
    char nome[30];
    char tipo[20]; // Ex: arma, munição, cura, ferramenta
    int quantidade;
} Item;

// =========================================================================
// VARIÁVEIS GLOBAIS E CONSTANTES (MOCHILA VETOR)
// =========================================================================

#define MAX_ITENS 10 // Capacidade máxima do inventário (Vetor)
Item inventario[MAX_ITENS];
int numItens = 0;
// Flag para indicar se o vetor está ordenado, essencial para a Busca Binária
int vetorOrdenado = 0;

// Contador de comparações para análise de desempenho
long long contadorComparacoes = 0;

// =========================================================================
// PROTÓTIPOS DAS FUNÇÕES
// =========================================================================

void limparBuffer();
Item lerNovoItem();
void exibirMenu();

// Funções de Inventário
void inserirItem();
void removerItem();
void listarItens();
void ordenarInventario();
void buscarSequencial();
void buscarBinaria();


// =========================================================================
// FUNÇÕES AUXILIARES
// =========================================================================

/**
 * @brief Limpa o buffer de entrada.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * @brief Obtém os dados de um novo item do usuário.
 * @return Item A struct Item preenchida.
 */
Item lerNovoItem() {
    Item novoItem;
    limparBuffer(); // Garante que o buffer esteja limpo

    printf("  Nome (max 29): ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0; // Remove o '\n'

    printf("  Tipo (ex: cura, arma, max 19): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0; // Remove o '\n'

    printf("  Quantidade: ");
    while (scanf("%d", &novoItem.quantidade) != 1 || novoItem.quantidade <= 0) {
        printf("  [ERRO] Quantidade inválida. Digite um inteiro positivo: ");
        limparBuffer();
    }
    return novoItem;
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\n--- MOCHILA DE COMBATE (Vetor / Lista Sequencial) ---\n");
    printf("1. Coletar Novo Item (Inserir)\n");
    printf("2. Descartar Item (Remover)\n");
    printf("3. Listar Inventário (%d/%d)\n", numItens, MAX_ITENS);
    printf("4. Organizar Itens (Ordenar por Nome)\n");
    printf("5. Buscar Item Sequencial (Busca Lenta - O(N))\n");
    printf("6. Buscar Item Crítico Binário (Busca Rápida - O(log N))\n");
    printf("0. Sair do Jogo\n");
    printf("Escolha uma opção: ");
}

// =========================================================================
// FUNÇÕES DE INVENTÁRIO (VETOR)
// =========================================================================

/**
 * @brief Insere um novo item no final do vetor (mochila).
 */
void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("\n[ALERTA] Mochila cheia (%d/%d). Impossível coletar mais itens.\n", numItens, MAX_ITENS);
        return;
    }

    printf("\n--- COLETAR ITEM ---\n");
    inventario[numItens] = lerNovoItem();
    numItens++;
    vetorOrdenado = 0; // Qualquer inserção desorganiza o vetor
    printf("[SUCESSO] Item '%s' coletado.\n", inventario[numItens - 1].nome);
}

/**
 * @brief Remove um item do vetor por nome (Busca Sequencial seguida de reorganização).
 */
void removerItem() {
    if (numItens == 0) {
        printf("\n[ALERTA] Mochila vazia. Nada a remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\n--- DESCARTAR ITEM ---\n");
    printf("Digite o nome do item para descartar: ");
    limparBuffer();
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = 0;

    int i, encontrado = -1;
    for (i = 0; i < numItens; i++) {
        if (strcmp(inventario[i].nome, nomeRemover) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Reorganização: desloca elementos subsequentes para a esquerda
        for (i = encontrado; i < numItens - 1; i++) {
            inventario[i] = inventario[i + 1];
        }
        numItens--;
        vetorOrdenado = 0; // A remoção anula a ordenação prévia (para segurança)
        printf("[SUCESSO] Item '%s' descartado.\n", nomeRemover);
    } else {
        printf("[ALERTA] Item '%s' não encontrado.\n", nomeRemover);
    }
}

/**
 * @brief Lista todos os itens do vetor.
 */
void listarItens() {
    printf("\n--- INVENTÁRIO ATUAL (%d/%d) ---\n", numItens, MAX_ITENS);
    printf("[STATUS] Ordenação: %s\n", vetorOrdenado ? "ORDENADO (Busca Binária disponível)" : "DESORDENADO");

    if (numItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    printf("+----+--------------------+-----------+------+\n");
    printf("| #  | NOME               | TIPO      | QTD  |\n");
    printf("+----+--------------------+-----------+------+\n");
    for (int i = 0; i < numItens; i++) {
        printf("| %-2d | %-18s | %-9s | %-4d |\n",
               i + 1, inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
    }
    printf("+----+--------------------+-----------+------+\n");
}

/**
 * @brief Ordena o vetor pelo nome (Selection Sort).
 */
void ordenarInventario() {
    if (numItens <= 1) {
        printf("\n[INFO] Inventário já ordenado (0 ou 1 item).\n");
        vetorOrdenado = 1;
        return;
    }

    // Algoritmo Selection Sort
    for (int i = 0; i < numItens - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < numItens; j++) {
            // Comparação por nome
            if (strcmp(inventario[j].nome, inventario[min_idx].nome) < 0) {
                min_idx = j;
            }
        }
        // Troca o elemento mínimo encontrado com o elemento atual (posição i)
        if (min_idx != i) {
            Item temp = inventario[i];
            inventario[i] = inventario[min_idx];
            inventario[min_idx] = temp;
        }
    }

    vetorOrdenado = 1;
    printf("\n[SUCESSO] Inventário ORGANIZADO com sucesso (Selection Sort).\n");
}

/**
 * @brief Busca sequencial de um item por nome. (Simula busca sob pressão, ineficiente)
 */
void buscarSequencial() {
    if (numItens == 0) {
        printf("\n[ALERTA] Mochila vazia. Nada a buscar.\n");
        return;
    }

    char nomeBuscar[30];
    printf("\n--- BUSCA SEQUENCIAL (Zona de Perigo) ---\n");
    printf("Digite o nome do item para buscar: ");
    limparBuffer();
    fgets(nomeBuscar, sizeof(nomeBuscar), stdin);
    nomeBuscar[strcspn(nomeBuscar, "\n")] = 0;

    contadorComparacoes = 0;
    int encontrado = 0;
    double inicio = (double)clock() / CLOCKS_PER_SEC; // Início da medição de tempo

    for (int i = 0; i < numItens; i++) {
        contadorComparacoes++; // Conta a comparação
        if (strcmp(inventario[i].nome, nomeBuscar) == 0) {
            encontrado = 1;
            printf("\n[ENCONTRADO] Item '%s' localizado na posição %d.\n", nomeBuscar, i + 1);
            printf("  Tipo: %s, Quantidade: %d\n", inventario[i].tipo, inventario[i].quantidade);
            break;
        }
    }

    double fim = (double)clock() / CLOCKS_PER_SEC; // Fim da medição de tempo

    if (!encontrado) {
        printf("[ALERTA] Item '%s' não encontrado.\n", nomeBuscar);
    }
    
    // Demonstração de desempenho
    printf("\n--- ANÁLISE DE DESEMPENHO (Busca Sequencial) ---\n");
    printf("Comparações (Esforço): %lld\n", contadorComparacoes);
    printf("Tempo de Execução: %.6f segundos.\n", fim - inicio);
}

/**
 * @brief Busca binária de um item por nome (Requer ordenação prévia). (Simula acesso rápido e eficiente)
 */
void buscarBinaria() {
    if (!vetorOrdenado) {
        printf("\n[ALERTA] Inventário DESORGANIZADO! É necessário Organizar (Opção 4) para a Busca Binária.\n");
        return;
    }
    if (numItens == 0) {
        printf("\n[ALERTA] Mochila vazia. Nada a buscar.\n");
        return;
    }

    char nomeBuscar[30];
    printf("\n--- BUSCA BINÁRIA (Acesso Rápido) ---\n");
    printf("Digite o nome do item crítico para buscar: ");
    limparBuffer();
    fgets(nomeBuscar, sizeof(nomeBuscar), stdin);
    nomeBuscar[strcspn(nomeBuscar, "\n")] = 0;

    contadorComparacoes = 0;
    int inicio = 0;
    int fim = numItens - 1;
    int encontrado = 0;
    double tempoInicio = (double)clock() / CLOCKS_PER_SEC; // Início da medição

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int cmp = strcmp(inventario[meio].nome, nomeBuscar);
        contadorComparacoes++; // Conta a comparação

        if (cmp == 0) {
            encontrado = 1;
            printf("\n[CRÍTICO ENCONTRADO] Item '%s' (Crítico) localizado na posição %d.\n", nomeBuscar, meio + 1);
            printf("  Tipo: %s, Quantidade: %d\n", inventario[meio].tipo, inventario[meio].quantidade);
            break;
        } else if (cmp < 0) {
            inicio = meio + 1; // Item está na metade direita
        } else {
            fim = meio - 1; // Item está na metade esquerda
        }
    }

    double tempoFim = (double)clock() / CLOCKS_PER_SEC; // Fim da medição

    if (!encontrado) {
        printf("[ALERTA] Item '%s' não encontrado.\n", nomeBuscar);
    }

    // Demonstração de desempenho
    printf("\n--- ANÁLISE DE DESEMPENHO (Busca Binária) ---\n");
    printf("Comparações (Esforço): %lld\n", contadorComparacoes);
    printf("Tempo de Execução: %.6f segundos.\n", tempoFim - tempoInicio);
}

// =========================================================================
// FUNÇÃO PRINCIPAL
// =========================================================================

int main() {
    int opcao;

    printf("===================================================\n");
    printf("       SISTEMA DE INVENTÁRIO DE COMBATE (VETOR)\n");
    printf("  Análise de Estruturas: Busca Sequencial vs Binária\n");
    printf("===================================================\n");

    do {
        // Exibe o menu para usabilidade
        exibirMenu();

        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada inválida. Por favor, digite um número.\n");
            limparBuffer();
            opcao = -1; // Força a repetição do loop
            continue;
        }

        // Fluxo de execução
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
                ordenarInventario();
                break;
            case 5:
                buscarSequencial();
                break;
            case 6:
                buscarBinaria();
                break;
            case 0:
                printf("\n[FIM DE JOGO] Inventário otimizado. Boa sorte na missão!\n");
                break;
            default:
                printf("\n[ALERTA] Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}