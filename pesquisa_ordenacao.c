#include <stdio.h>
#include <stdlib.h> // Para qsort (se usar) e malloc/free
#include <string.h>
#include "pesquisa_ordenacao.h"
#include "livro.h" // Para exibir_livro

// --- ESQUELETOS DETALHADOS (VOCÊ IMPLEMENTA) ---

NoLista* pesquisar_livro_por_titulo(const ColecaoLivros* colecao, const char* titulo_busca) {
    // Lógica: Verificar se a coleção ou o título são NULL.
    if (colecao == NULL || titulo_busca == NULL) {
        return NULL;
    }

    NoLista* atual = colecao->inicio;
    // Lógica: Percorrer a lista.
    while (atual != NULL) {
        // Lógica: Comparar o título do livro atual com o título_busca.
        //         Usar strstr() para busca de substring ou strcmp() para correspondência exata (case sensitive).
        //         Para case insensitive, você precisaria converter ambas as strings para minúsculas/maiúsculas antes de comparar.
        if (strstr(atual->dadosLivro.titulo, titulo_busca) != NULL) { // Exemplo com strstr (encontra se titulo_busca é parte do título do livro)
        // if (strcmp(atual->dadosLivro.titulo, titulo_busca) == 0) { // Para correspondência exata
            return atual; // Retorna o nó onde o livro foi encontrado
        }
        atual = atual->proximo;
    }
    // Lógica: Se não encontrar, retornar NULL.
    return NULL;
    // TODO: Se múltiplos livros puderem ter o mesmo título (ou parte dele),
    //       esta função só retorna o primeiro. Poderia ser modificada para
    //       retornar uma lista de resultados ou imprimir todos os encontrados.
}


// Função auxiliar para qsort, se for ordenar um vetor de ponteiros para Livro
int comparar_livros_por_titulo(const void* a, const void* b) {
    // Lógica: Converte os ponteiros void* para ponteiros para ponteiros para Livro (Livro**).
    //         Depois, acessa o campo 'titulo' e usa strcmp.
    Livro* livro_a = *(Livro**)a; // Dereferencia para obter o Livro*
    Livro* livro_b = *(Livro**)b;
    return strcmp(livro_a->titulo, livro_b->titulo);
}

int comparar_livros_por_ano(const void* a, const void* b) {
    // Lógica: Similar ao anterior, mas compara 'anoPublicacao'.
    Livro* livro_a = *(Livro**)a;
    Livro* livro_b = *(Livro**)b;
    if (livro_a->anoPublicacao < livro_b->anoPublicacao) return -1;
    if (livro_a->anoPublicacao > livro_b->anoPublicacao) return 1;
    return 0;
}


void ordenar_colecao_por_titulo(ColecaoLivros* colecao) {
    // Lógica: Verificar se a coleção é NULL ou tem menos de 2 elementos (já está ordenada).
    if (colecao == NULL || colecao->quantidade < 2) {
        return;
    }

    // Lógica: Abordagem 1: Copiar ponteiros para Livro para um vetor.
    //         Alocar um vetor de ponteiros para Livro (Livro**) com tamanho colecao->quantidade.
    Livro** array_de_ponteiros = (Livro**) malloc(colecao->quantidade * sizeof(Livro*));
    if (array_de_ponteiros == NULL) {
        perror("Erro ao alocar vetor para ordenacao");
        return;
    }

    // Lógica: Preencher este vetor com os endereços dos 'dadosLivro' de cada nó da lista.
    NoLista* atual = colecao->inicio;
    int i = 0;
    while (atual != NULL && i < colecao->quantidade) {
        array_de_ponteiros[i++] = &(atual->dadosLivro);
        atual = atual->proximo;
    }
    
    // Lógica: Usar qsort (da stdlib.h) para ordenar o vetor de ponteiros.
    //         Você precisará de uma função de comparação (ex: comparar_livros_por_titulo).
    qsort(array_de_ponteiros, colecao->quantidade, sizeof(Livro*), comparar_livros_por_titulo);

    // Lógica: Após ordenar o vetor, você pode:
    //         Opção A: Apenas exibir os livros na ordem do vetor (sem modificar a lista original).
    printf("\n--- Livros Ordenados por Titulo (sem alterar a lista original) ---\n");
    for (i = 0; i < colecao->quantidade; i++) {
        exibir_livro(array_de_ponteiros[i]);
    }
    printf("--- Fim da Lista Ordenada ---\n");

    //         Opção B (Mais complexo): Reconstruir a lista encadeada com base na ordem do vetor.
    //                  Isso envolveria realocar os nós ou, mais cuidadosamente,
    //                  pegar os dados dos Livros no vetor e colocá-los em nós
    //                  em uma nova lista ordenada, depois substituindo a original.
    //                  Ou, se os 'dadosLivro' fossem ponteiros para Livro,
    //                  poderia reordenar os nós da lista diretamente (muito complexo).
    //                  Para este projeto, a Opção A (exibir ordenado) é suficiente para demonstrar o conceito.

    // Lógica: Liberar a memória do vetor de ponteiros.
    free(array_de_ponteiros);

    // TODO: Se optar por modificar a lista original, a lógica aqui seria bem mais envolvida,
    //       possivelmente implementando um algoritmo de ordenação diretamente na lista encadeada
    //       (ex: selection sort adaptado, merge sort adaptado) ou a reconstrução mencionada.
    //       A abordagem com `qsort` em um array auxiliar é geralmente mais simples e eficiente
    //       para demonstrar o uso de um algoritmo de ordenação robusto.
}

void ordenar_colecao_por_ano(ColecaoLivros* colecao) {
    // Lógica: Similar a ordenar_colecao_por_titulo, mas usando comparar_livros_por_ano.
    if (colecao == NULL || colecao->quantidade < 2) {
        return;
    }

    Livro** array_de_ponteiros = (Livro**) malloc(colecao->quantidade * sizeof(Livro*));
    if (array_de_ponteiros == NULL) {
        perror("Erro ao alocar vetor para ordenacao por ano");
        return;
    }

    NoLista* atual = colecao->inicio;
    int i = 0;
    while (atual != NULL && i < colecao->quantidade) {
        array_de_ponteiros[i++] = &(atual->dadosLivro);
        atual = atual->proximo;
    }
    
    qsort(array_de_ponteiros, colecao->quantidade, sizeof(Livro*), comparar_livros_por_ano);

    printf("\n--- Livros Ordenados por Ano (sem alterar a lista original) ---\n");
    for (i = 0; i < colecao->quantidade; i++) {
        exibir_livro(array_de_ponteiros[i]);
    }
    printf("--- Fim da Lista Ordenada ---\n");

    free(array_de_ponteiros);
}