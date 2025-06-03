#ifndef LIVRO_H
#define LIVRO_H

/**
 * @file livro.h
 * @brief Define a estrutura de dados Livro e os protótipos das funções para manipulá-la.
 * Este arquivo é fundamental para representar e operar sobre informações de livros
 * no sistema de gerenciamento da biblioteca.
 */

// --- Constantes para Tamanhos dos Campos String ---

/** @brief Tamanho máximo para o título do livro, incluindo o terminador nulo. */
#define TAM_TITULO 100
/** @brief Tamanho máximo para o nome do autor do livro, incluindo o terminador nulo. */
#define TAM_AUTOR 100
/** @brief Tamanho máximo para o ISBN do livro (ex: 13 caracteres + terminador nulo). */
#define TAM_ISBN 14
/** @brief Tamanho máximo para o gênero do livro, incluindo o terminador nulo. */
#define TAM_GENERO 50

/**
 * @brief Estrutura para armazenar informações de um livro.
 * (Estrutura de Dados Simples Heterogênea)
 */
typedef struct {
    char titulo[TAM_TITULO];      ///< Título do livro (String - Vetor de char).
    char autor[TAM_AUTOR];        ///< Autor do livro (String - Vetor de char).
    int anoPublicacao;            ///< Ano de publicação do livro.
    char isbn[TAM_ISBN];          ///< ISBN do livro, usado como identificador único (String - Vetor de char).
    char genero[TAM_GENERO];      ///< Gênero do livro (String - Vetor de char).
    // Você pode adicionar mais campos se desejar, ex: editora, num_paginas.
} Livro;

// --- Protótipos das Funções ---

/**
 * @brief Cria dinamicamente uma nova instância de Livro e a inicializa com os dados fornecidos.
 * O chamador é responsável por liberar a memória alocada usando `destruir_livro()`.
 *
 * @param titulo Título do livro (não deve ser NULL).
 * @param autor Autor do livro (não deve ser NULL).
 * @param ano Ano de publicação do livro.
 * @param isbn ISBN do livro (identificador único, não deve ser NULL).
 * @param genero Gênero do livro (não deve ser NULL).
 * @return Livro* Um ponteiro para a nova struct Livro alocada, ou NULL se a alocação falhar
 * ou se algum dos parâmetros de string obrigatórios for NULL.
 */
Livro* criar_livro(const char* titulo, const char* autor, int ano, const char* isbn, const char* genero);

/**
 * @brief Libera a memória alocada para uma instância de Livro.
 * Se o ponteiro do livro for NULL, a função não faz nada.
 *
 * @param livro Ponteiro para a struct Livro cuja memória será liberada.
 */
void destruir_livro(Livro* livro);

/**
 * @brief Exibe os detalhes de um livro no console de forma formatada.
 *
 * @param livro Ponteiro constante para a struct Livro a ser exibida.
 * Se o ponteiro for NULL, uma mensagem indicando isso será exibida.
 */
void exibir_livro(const Livro* livro);

/**
 * @brief Lê os dados de um livro a partir da entrada padrão (teclado)
 * e preenche uma struct Livro fornecida.
 *
 * @param livro_destino Ponteiro para a struct Livro que será preenchida com os dados lidos.
 * Este ponteiro não deve ser NULL.
 * @return int 1 se a leitura e o preenchimento foram bem-sucedidos, 0 caso contrário
 * (ex: livro_destino é NULL ou ocorreu um erro/EOF durante a leitura).
 */
int ler_dados_livro_teclado(Livro* livro_destino);

#endif // LIVRO_H