#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// =========================================================================
// 1. CRIAÇÃO DA STRUCT Componente
// =========================================================================

// Struct base para representar um componente da torre de fuga
typedef struct {
    char nome[30];
    char tipo[20]; // Ex: "controle", "suporte", "propulsão"
    int prioridade; // De 1 (baixa) a 10 (alta)
} Componente;

// =========================================================================
// VARIÁVEIS GLOBAIS E CONSTANTES
// =========================================================================

#define MAX_COMPONENTES 20 // Capacidade máxima de componentes
Componente componentes[MAX_COMPONENTES];
int numComponentes = 0;

// Variáveis de feedback de desempenho
long long contadorComparacoes = 0;
double tempoExecucao = 0.0;

// Flag para controle da busca binária: 0=desordenado, 1=ordenado por nome, 2=ordenado por tipo, 3=ordenado por prioridade
int vetorOrdenadoPor = 0;

// =========================================================================
// PROTÓTIPOS DE FUNÇÕES
// =========================================================================

void limparBuffer();
void exibirMenu();
void lerNovoComponente();
void mostrarComponentes(Componente array[], int tamanho);

// Algoritmos de Ordenação (funções obrigatórias)
void bubbleSortNome(Componente array[], int tamanho);
void insertionSortTipo(Componente array[], int tamanho);
void selectionSortPrioridade(Componente array[], int tamanho);

// Algoritmo de Busca (função obrigatória)
int buscaBinariaPorNome(Componente array[], int tamanho, const char chave[]);

// Função de Controle para executar ordenação e medição (medirTempo)
void medirEOrdenar(int opcao);

// Função auxiliar de troca
void trocar(Componente *a, Componente *b);

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
 * @brief Troca dois componentes de posição.
 */
void trocar(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Obtém os dados de um novo componente do usuário.
 */
void lerNovoComponente() {
    if (numComponentes >= MAX_COMPONENTES) {
        printf("\n[ALERTA] Limite de componentes (%d) atingido.\n", MAX_COMPONENTES);
        return;
    }

    Componente *novoComp = &componentes[numComponentes];
    limparBuffer();

    printf("\n--- CADASTRO DE COMPONENTE %d ---\n", numComponentes + 1);

    // Leitura do Nome
    printf("Nome do Componente (max 29): ");
    fgets(novoComp->nome, sizeof(novoComp->nome), stdin);
    novoComp->nome[strcspn(novoComp->nome, "\n")] = 0;

    // Leitura do Tipo
    printf("Tipo (ex: controle, suporte, propulsão, max 19): ");
    fgets(novoComp->tipo, sizeof(novoComp->tipo), stdin);
    novoComp->tipo[strcspn(novoComp->tipo, "\n")] = 0;

    // Leitura da Prioridade (1 a 10)
    printf("Prioridade (1 - 10, 10 é mais urgente): ");
    while (scanf("%d", &novoComp->prioridade) != 1 || novoComp->prioridade < 1 || novoComp->prioridade > 10) {
        printf("[ERRO] Prioridade inválida. Digite um valor entre 1 e 10: ");
        limparBuffer();
    }

    numComponentes++;
    vetorOrdenadoPor = 0; // Cadastro desorganiza
    printf("[SUCESSO] Componente '%s' cadastrado.\n", novoComp->nome);
}

/**
 * @brief Exibe o menu principal. (Interface Amigável)
 */
void exibirMenu() {
    printf("\n===================================================\n");
    printf("      MÓDULO DE MONTAGEM DA TORRE DE RESGATE\n");
    printf("===================================================\n");
    printf("Status: %d/%d Componentes Cadastrados.\n", numComponentes, MAX_COMPONENTES);
    printf("Estratégia Atual: ");
    if (vetorOrdenadoPor == 1) printf("Ordenado por Nome (Bubble Sort)\n");
    else if (vetorOrdenadoPor == 2) printf("Ordenado por Tipo (Insertion Sort)\n");
    else if (vetorOrdenadoPor == 3) printf("Ordenado por Prioridade (Selection Sort)\n");
    else printf("Nenhuma (Desordenado)\n");
    printf("---------------------------------------------------\n");
    printf("1. Cadastrar Novo Componente\n");
    printf("2. Listar Componentes Atuais\n");
    printf("3. Estratégia A: Ordenar por NOME (Bubble Sort) -> Permite Busca Binária\n");
    printf("4. Estratégia B: Ordenar por TIPO (Insertion Sort)\n");
    printf("5. Estratégia C: Ordenar por PRIORIDADE (Selection Sort)\n");
    printf("6. Buscar Componente-Chave (Busca Binária Otimizada)\n");
    printf("0. Finalizar Missão\n");
    printf("Opção: ");
}

/**
 * @brief Exibe o vetor de componentes formatado. (Função obrigatória)
 */
void mostrarComponentes(Componente array[], int tamanho) {
    if (tamanho == 0) {
        printf("\n[INFO] Nenhum componente cadastrado.\n");
        return;
    }

    printf("\n--- SEQUÊNCIA DE MONTAGEM ---\n");
    printf("+----+------------------------------+--------------------+------------+\n");
    printf("| #  | NOME                         | TIPO               | PRIORIDADE |\n");
    printf("+----+------------------------------+--------------------+------------+\n");
    for (int i = 0; i < tamanho; i++) {
        printf("| %-2d | %-28s | %-18s | %-10d |\n",
               i + 1, array[i].nome, array[i].tipo, array[i].prioridade);
    }
    printf("+----+------------------------------+--------------------+------------+\n");
}

// =========================================================================
// 3. ALGORITMOS DE ORDENAÇÃO E MEDIÇÃO DE DESEMPENHO
// =========================================================================

/**
 * @brief Ordena por nome usando Bubble Sort. (Função obrigatória)
 */
void bubbleSortNome(Componente array[], int tamanho) {
    contadorComparacoes = 0;
    
    for (int i = 0; i < tamanho - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < tamanho - 1 - i; j++) {
            // Compara o campo nome (String)
            contadorComparacoes++; 
            if (strcmp(array[j].nome, array[j + 1].nome) > 0) {
                trocar(&array[j], &array[j + 1]);
                trocou = 1;
            }
        }
        if (trocou == 0) break;
    }
}

/**
 * @brief Ordena por tipo usando Insertion Sort. (Função obrigatória)
 */
void insertionSortTipo(Componente array[], int tamanho) {
    contadorComparacoes = 0;

    for (int i = 1; i < tamanho; i++) {
        Componente chave = array[i];
        int j = i - 1;

        // Compara o campo tipo (String)
        while (j >= 0) {
            contadorComparacoes++;
            if (strcmp(array[j].tipo, chave.tipo) > 0) {
                array[j + 1] = array[j]; // Desloca
                j = j - 1;
            } else {
                break; // Saída otimizada
            }
        }
        array[j + 1] = chave;
    }
}

/**
 * @brief Ordena por prioridade usando Selection Sort. (Função obrigatória)
 */
void selectionSortPrioridade(Componente array[], int tamanho) {
    contadorComparacoes = 0;

    for (int i = 0; i < tamanho - 1; i++) {
        int indiceMax = i; // Usamos Max para ordenação decrescente de Prioridade
        for (int j = i + 1; j < tamanho; j++) {
            // Compara o campo prioridade (Int)
            contadorComparacoes++; 
            if (array[j].prioridade > array[indiceMax].prioridade) { // Ordem decrescente de prioridade
                indiceMax = j;
            }
        }
        if (indiceMax != i) {
            trocar(&array[i], &array[indiceMax]);
        }
    }
}

/**
 * @brief Função de controle para medir e executar a ordenação escolhida. (Simula medirTempo)
 */
void medirEOrdenar(int opcao) {
    if (numComponentes == 0) {
        printf("\n[ALERTA] Cadastre componentes antes de ordenar.\n");
        return;
    }
    
    // Início da medição de tempo
    clock_t inicio = clock();
    
    printf("\n--- EXECUTANDO ESTRATÉGIA DE ORGANIZAÇÃO ---\n");

    if (opcao == 3) {
        bubbleSortNome(componentes, numComponentes);
        printf("[ESTRATÉGIA A] Montagem ordenada por NOME (Bubble Sort).\n");
        vetorOrdenadoPor = 1;
    } else if (opcao == 4) {
        insertionSortTipo(componentes, numComponentes);
        printf("[ESTRATÉGIA B] Montagem ordenada por TIPO (Insertion Sort).\n");
        vetorOrdenadoPor = 2;
    } else if (opcao == 5) {
        selectionSortPrioridade(componentes, numComponentes);
        printf("[ESTRATÉGIA C] Montagem ordenada por PRIORIDADE (Selection Sort).\n");
        vetorOrdenadoPor = 3;
    } else {
        return;
    }

    // Fim da medição de tempo
    clock_t fim = clock();
    tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Feedback educacional e desempenho (Requisito: Desempenho Educacional)
    printf("\n--- RELATÓRIO DE IMPACTO DA ESTRATÉGIA ---\n");
    printf("Comparações Realizadas: %lld\n", contadorComparacoes);
    printf("Tempo de Execução (s): %.6f\n", tempoExecucao);
    printf("Impacto: Quanto menor o número de comparações e o tempo, mais rápido é o plano de fuga!\n");

    mostrarComponentes(componentes, numComponentes);
}

// =========================================================================
// 4. BUSCA BINÁRIA OTIMIZADA
// =========================================================================

/**
 * @brief Realiza uma Busca Binária otimizada por nome. (Função obrigatória)
 * @return int Posição do item encontrado (base 1), ou 0 se não encontrado.
 */
int buscaBinariaPorNome(Componente array[], int tamanho, const char chave[]) {
    if (vetorOrdenadoPor != 1) {
        printf("\n[ERRO FATAL] A Busca Binária por NOME requer a Estratégia A (Opção 3) para ordenar por nome primeiro.\n");
        return 0;
    }

    if (tamanho == 0) return 0;

    contadorComparacoes = 0;
    clock_t inicio = clock();
    
    int baixo = 0;
    int alto = tamanho - 1;
    int resultado = 0; 

    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        int cmp = strcmp(array[meio].nome, chave);
        contadorComparacoes++;

        if (cmp == 0) {
            resultado = meio + 1; // Item encontrado (posição base 1)
            break;
        } else if (cmp < 0) {
            baixo = meio + 1; // Item está na metade superior
        } else {
            alto = meio - 1; // Item está na metade inferior
        }
    }

    clock_t fim = clock();
    tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Relatório de Busca
    printf("\n--- RELATÓRIO DE BUSCA BINÁRIA ---\n");
    printf("Comparações: %lld\n", contadorComparacoes);
    printf("Tempo de Execução: %.6f segundos.\n", tempoExecucao);
    
    return resultado;
}

// =========================================================================
// FUNÇÃO PRINCIPAL
// =========================================================================

int main() {
    int opcao;

    do {
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Entrada inválida. Por favor, digite um número.\n");
            limparBuffer();
            opcao = -1; 
            continue;
        }

        switch (opcao) {
            case 1:
                lerNovoComponente();
                break;
            case 2:
                mostrarComponentes(componentes, numComponentes);
                break;
            case 3:
            case 4:
            case 5:
                medirEOrdenar(opcao);
                break;
            case 6: {
                char chave[30];
                printf("\n--- ATIVAR TORRE DE FUGA ---\n");
                printf("Digite o NOME do componente-chave para destravar a torre: ");
                limparBuffer();
                fgets(chave, sizeof(chave), stdin);
                chave[strcspn(chave, "\n")] = 0;

                int pos = buscaBinariaPorNome(componentes, numComponentes, chave);
                
                if (pos > 0) {
                    printf("\n[CRÍTICO] Componente-Chave '%s' encontrado na Posição %d.\n", chave, pos);
                    printf(">>>>> TORRE DE RESGATE ATIVADA! <<<<<\n");
                } else {
                    printf("\n[ALERTA] Componente-Chave '%s' não encontrado. Reorganize o inventário (Opção 3) e tente novamente.\n", chave);
                }
                break;
            }
            case 0:
                printf("\n[SUCESSO] Missão Finalizada! Você escapou da ilha.\n");
                break;
            default:
                printf("\n[ALERTA] Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}