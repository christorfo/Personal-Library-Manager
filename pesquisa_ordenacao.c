#include <stdio.h>
#include <stdlib.h> // Para qsort, malloc, free
#include <string.h> // Para strstr, strcmp
#include "pesquisa_ordenacao.h"
#include "livro.h"         // Para struct Livro e exibir_livro (se usado aqui)
#include "lista_livros.h"  // Para ColecaoLivros, NoLista

// --- FUNÇÕES DE PESQUISA E ORDENAÇÃO APRIMORADAS ---

/**
 * @brief Pesquisa o primeiro livro na coleção cujo título contém a string de busca.
 * A busca é case-sensitive e considera substrings (ex: buscar por "Senhor" pode encontrar "O Senhor dos Anéis").
 *
 * @param colecao Ponteiro constante para a ColecaoLivros a ser pesquisada.
 * @param titulo_busca String constante contendo o título (ou parte dele) a ser buscado.
 * @return const Livro* Ponteiro constante para os dados do primeiro livro encontrado,
 * ou NULL se nenhum livro corresponder, ou se os parâmetros forem inválidos.
 * @warning O ponteiro retornado aponta para dados internos da lista.
 * Não modifique o conteúdo apontado e não libere este ponteiro.
 * @note Se múltiplos livros corresponderem, apenas o primeiro encontrado na ordem da lista é retornado.
 * Para encontrar todos, seria necessária uma função diferente (ex: iterar e aplicar um callback).
 */
const Livro* pesquisar_livro_por_titulo(const ColecaoLivros* colecao, const char* titulo_busca) {
    if (colecao == NULL || titulo_busca == NULL) {
        return NULL;
    }

    const NoLista* atual = colecao->inicio;
    while (atual != NULL) {
        // Usar strstr para busca de substring (case-sensitive)
        if (strstr(atual->dadosLivro.titulo, titulo_busca) != NULL) {
            return &(atual->dadosLivro); // Retorna ponteiro para os dados do livro no nó
        }
        // Para correspondência exata (case-sensitive):
        // if (strcmp(atual->dadosLivro.titulo, titulo_busca) == 0) {
        //     return &(atual->dadosLivro);
        // }
        atual = atual->proximo;
    }
    return NULL; // Não encontrado
}

/**
 * @brief Função de comparação para qsort, para ordenar Livros por título (alfabética, case-sensitive).
 * Espera que 'a' e 'b' sejam ponteiros para structs Livro.
 *
 * @param a Ponteiro void para o primeiro Livro.
 * @param b Ponteiro void para o segundo Livro.
 * @return int <0 se o título de 'a' vem antes de 'b', 0 se são iguais, >0 se 'a' vem depois de 'b'.
 */
static int comparar_livros_qsort_por_titulo(const void* a, const void* b) {
    const Livro* livro_a = (const Livro*)a;
    const Livro* livro_b = (const Livro*)b;
    return strcmp(livro_a->titulo, livro_b->titulo);
}

/**
 * @brief Função de comparação para qsort, para ordenar Livros por ano de publicação (crescente).
 * Espera que 'a' e 'b' sejam ponteiros para structs Livro.
 *
 * @param a Ponteiro void para o primeiro Livro.
 * @param b Ponteiro void para o segundo Livro.
 * @return int <0 se o ano de 'a' é menor que 'b', 0 se são iguais, >0 se o ano de 'a' é maior que 'b'.
 */
static int comparar_livros_qsort_por_ano(const void* a, const void* b) {
    const Livro* livro_a = (const Livro*)a;
    const Livro* livro_b = (const Livro*)b;

    if (livro_a->anoPublicacao < livro_b->anoPublicacao) return -1;
    if (livro_a->anoPublicacao > livro_b->anoPublicacao) return 1;
    return 0;
}

/**
 * @brief Função de comparação para qsort, para ordenar Livros por autor (alfabética, case-sensitive).
 * Espera que 'a' e 'b' sejam ponteiros para structs Livro.
 *
 * @param a Ponteiro void para o primeiro Livro.
 * @param b Ponteiro void para o segundo Livro.
 * @return int <0 se o autor de 'a' vem antes de 'b', 0 se são iguais, >0 se 'a' vem depois de 'b'.
 */
static int comparar_livros_qsort_por_autor(const void* a, const void* b) {
    const Livro* livro_a = (const Livro*)a;
    const Livro* livro_b = (const Livro*)b;
    return strcmp(livro_a->autor, livro_b->autor);
}


/**
 * @brief Ordena os dados dos livros na coleção por título (ordem alfabética, case-sensitive).
 * A função modifica a ordem dos dados Livro DENTRO dos nós existentes da lista encadeada.
 * A estrutura da lista (os nós e seus ponteiros 'proximo') não é alterada, apenas o conteúdo 'dadosLivro'.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser ordenada.
 */
void ordenar_colecao_por_titulo(ColecaoLivros* colecao) {
    if (colecao == NULL || colecao->quantidade < 2) {
        return; // Nada a ordenar ou coleção inválida
    }

    // 1. Alocar um array temporário para armazenar as structs Livro
    Livro* array_de_livros = (Livro*) malloc(colecao->quantidade * sizeof(Livro));
    if (array_de_livros == NULL) {
        perror("ERRO (ordenar_colecao_por_titulo): Falha ao alocar array temporario");
        return;
    }

    // 2. Copiar os dados dos livros da lista para o array
    NoLista* atual = colecao->inicio;
    for (int i = 0; i < colecao->quantidade; i++) {
        if (atual == NULL) { // Verificação de segurança, não deveria acontecer se colecao->quantidade está correto
            fprintf(stderr, "ERRO (ordenar_colecao_por_titulo): Inconsistencia na quantidade de livros.\n");
            free(array_de_livros);
            return;
        }
        array_de_livros[i] = atual->dadosLivro; // Copia a struct
        atual = atual->proximo;
    }

    // 3. Ordenar o array de structs Livro usando qsort
    qsort(array_de_livros, colecao->quantidade, sizeof(Livro), comparar_livros_qsort_por_titulo);

    // 4. Copiar os dados ordenados de volta para os nós da lista original
    atual = colecao->inicio;
    for (int i = 0; i < colecao->quantidade; i++) {
         if (atual == NULL) { // Verificação de segurança
            fprintf(stderr, "ERRO (ordenar_colecao_por_titulo): Inconsistencia ao copiar dados de volta.\n");
            break; 
        }
        atual->dadosLivro = array_de_livros[i]; // Copia a struct ordenada
        atual = atual->proximo;
    }

    // 5. Liberar a memória do array temporário
    free(array_de_livros);

    // printf("INFO: Colecao ordenada por titulo.\n"); // Opcional: feedback
}

/**
 * @brief Ordena os dados dos livros na coleção por ano de publicação (ordem crescente).
 * A função modifica a ordem dos dados Livro DENTRO dos nós existentes da lista encadeada.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser ordenada.
 */
void ordenar_colecao_por_ano(ColecaoLivros* colecao) {
    if (colecao == NULL || colecao->quantidade < 2) {
        return; // Nada a ordenar ou coleção inválida
    }

    // 1. Alocar array temporário
    Livro* array_de_livros = (Livro*) malloc(colecao->quantidade * sizeof(Livro));
    if (array_de_livros == NULL) {
        perror("ERRO (ordenar_colecao_por_ano): Falha ao alocar array temporario");
        return;
    }

    // 2. Copiar da lista para o array
    NoLista* atual = colecao->inicio;
    for (int i = 0; i < colecao->quantidade; i++) {
        if (atual == NULL) {
            fprintf(stderr, "ERRO (ordenar_colecao_por_ano): Inconsistencia na quantidade de livros.\n");
            free(array_de_livros);
            return;
        }
        array_de_livros[i] = atual->dadosLivro;
        atual = atual->proximo;
    }

    // 3. Ordenar o array
    qsort(array_de_livros, colecao->quantidade, sizeof(Livro), comparar_livros_qsort_por_ano);

    // 4. Copiar de volta para a lista
    atual = colecao->inicio;
    for (int i = 0; i < colecao->quantidade; i++) {
        if (atual == NULL) {
            fprintf(stderr, "ERRO (ordenar_colecao_por_ano): Inconsistencia ao copiar dados de volta.\n");
            break;
        }
        atual->dadosLivro = array_de_livros[i];
        atual = atual->proximo;
    }

    // 5. Liberar array
    free(array_de_livros);

    // printf("INFO: Colecao ordenada por ano.\n"); // Opcional: feedback
}

/**
 * @brief Ordena os dados dos livros na coleção por autor (ordem alfabética, case-sensitive).
 * A função modifica a ordem dos dados Livro DENTRO dos nós existentes da lista encadeada.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser ordenada.
 */
void ordenar_colecao_por_autor(ColecaoLivros* colecao) {
    if (colecao == NULL || colecao->quantidade < 2) {
        return; // Nada a ordenar ou coleção inválida
    }

    // 1. Alocar array temporário
    Livro* array_de_livros = (Livro*) malloc(colecao->quantidade * sizeof(Livro));
    if (array_de_livros == NULL) {
        perror("ERRO (ordenar_colecao_por_autor): Falha ao alocar array temporario");
        return;
    }

    // 2. Copiar da lista para o array
    NoLista* atual = colecao->inicio;
    for (int i = 0; i < colecao->quantidade; i++) {
        if (atual == NULL) {
            fprintf(stderr, "ERRO (ordenar_colecao_por_autor): Inconsistencia na quantidade de livros.\n");
            free(array_de_livros);
            return;
        }
        array_de_livros[i] = atual->dadosLivro;
        atual = atual->proximo;
    }

    // 3. Ordenar o array
    qsort(array_de_livros, colecao->quantidade, sizeof(Livro), comparar_livros_qsort_por_autor);

    // 4. Copiar de volta para a lista
    atual = colecao->inicio;
    for (int i = 0; i < colecao->quantidade; i++) {
        if (atual == NULL) {
            fprintf(stderr, "ERRO (ordenar_colecao_por_autor): Inconsistencia ao copiar dados de volta.\n");
            break;
        }
        atual->dadosLivro = array_de_livros[i];
        atual = atual->proximo;
    }

    // 5. Liberar array
    free(array_de_livros);

    // printf("INFO: Colecao ordenada por autor.\n"); // Opcional: feedback
}