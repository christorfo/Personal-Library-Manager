#ifndef PESQUISA_ORDENACAO_H
#define PESQUISA_ORDENACAO_H

#include "lista_livros.h" // Necessário para a definição de ColecaoLivros e Livro (indiretamente)

/**
 * @file pesquisa_ordenacao.h
 * @brief Define os protótipos das funções para pesquisar e ordenar coleções de livros.
 * Estas funções operam sobre a estrutura ColecaoLivros definida em "lista_livros.h".
 */

// --- Métodos de Pesquisa ---

/**
 * @brief Pesquisa o primeiro livro na coleção cujo título contém a string de busca.
 * A busca é case-sensitive e considera substrings. Por exemplo, buscar por "Senhor"
 * pode encontrar "O Senhor dos Anéis".
 *
 * @note A busca por ISBN, por ser um identificador único, geralmente está implementada
 * diretamente nas funções de manipulação da lista (ex: `buscar_livro_por_isbn_na_colecao`
 * em `lista_livros.c`).
 *
 * @param colecao Ponteiro constante para a ColecaoLivros a ser pesquisada.
 * @param titulo_busca String constante contendo o título (ou parte dele) a ser buscado.
 * @return const Livro* Ponteiro constante para os dados do primeiro livro encontrado.
 * Retorna NULL se nenhum livro corresponder aos critérios de busca, ou se os
 * parâmetros de entrada forem inválidos (ex: colecao ou titulo_busca nulos).
 * @warning O ponteiro retornado aponta para dados internos da lista. Não modifique
 * o conteúdo apontado e não libere este ponteiro.
 * @note Se múltiplos livros corresponderem aos critérios, apenas o primeiro encontrado
 * (na ordem atual da lista) é retornado. Para obter todos os resultados, uma
 * abordagem diferente seria necessária (ex: uma função que retorna uma nova coleção
 * com os resultados ou que utiliza uma função de callback para processar cada achado).
 */
const Livro* pesquisar_livro_por_titulo(const ColecaoLivros* colecao, const char* titulo_busca);

// --- Métodos de Classificação (Ordenação) ---

/**
 * @brief Ordena os dados dos livros na coleção por título (ordem alfabética, case-sensitive).
 * Esta função modifica a ordem dos dados Livro DENTRO dos nós existentes da lista encadeada.
 * A estrutura da lista (os nós e seus ponteiros 'proximo') não é alterada, apenas o conteúdo 'dadosLivro'.
 * A ordenação é geralmente implementada copiando os dados para um array temporário,
 * ordenando o array com `qsort`, e depois copiando os dados ordenados de volta para a lista.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser ordenada.
 * Se a coleção for NULL ou tiver menos de 2 elementos, nenhuma ação é tomada.
 */
void ordenar_colecao_por_titulo(ColecaoLivros* colecao);

/**
 * @brief Ordena os dados dos livros na coleção por ano de publicação (ordem crescente).
 * Modifica a ordem dos dados Livro DENTRO dos nós existentes da lista, similarmente
 * à função `ordenar_colecao_por_titulo`.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser ordenada.
 * Se a coleção for NULL ou tiver menos de 2 elementos, nenhuma ação é tomada.
 */
void ordenar_colecao_por_ano(ColecaoLivros* colecao);

/**
 * @brief Ordena os dados dos livros na coleção por autor (ordem alfabética, case-sensitive).
 * Modifica a ordem dos dados Livro DENTRO dos nós existentes da lista.
 * A implementação desta função seguiria um padrão similar ao de
 * `ordenar_colecao_por_titulo`, utilizando uma função de comparação apropriada
 * para autores com `qsort`.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser ordenada.
 * Se a coleção for NULL ou tiver menos de 2 elementos, nenhuma ação é tomada.
 */
void ordenar_colecao_por_autor(ColecaoLivros* colecao);


#endif // PESQUISA_ORDENACAO_H