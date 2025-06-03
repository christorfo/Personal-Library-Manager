#ifndef LISTA_LIVROS_H
#define LISTA_LIVROS_H

#include "livro.h" // Necessário para a definição da struct Livro

/**
 * @file lista_livros.h
 * @brief Define as estruturas de dados e protótipos de funções para uma coleção de livros
 * implementada como uma lista encadeada.
 */

/**
 * @brief Nó da lista encadeada de livros.
 * Cada nó armazena os dados de um livro e um ponteiro para o próximo nó na lista.
 */
typedef struct NoLista {
    Livro dadosLivro;          ///< Os dados do livro armazenados neste nó.
    struct NoLista* proximo;   ///< Ponteiro para o próximo nó na lista (Referenciamento à memória - Ponteiros).
} NoLista;

/**
 * @brief Estrutura da coleção de livros.
 * Representa uma lista encadeada de livros, mantendo um ponteiro para o início
 * da lista e a quantidade total de livros.
 */
typedef struct {
    NoLista* inicio;           ///< Ponteiro para o primeiro nó da lista (ou NULL se a lista estiver vazia).
    int quantidade;            ///< Número total de livros na coleção.
} ColecaoLivros;

// --- Protótipos das Funções para Manipular a Coleção de Livros ---
// (Estrutura Complexa - Listas)

/**
 * @brief Cria e inicializa uma nova coleção de livros vazia.
 * Aloca memória para a estrutura ColecaoLivros e define seus campos iniciais
 * (início como NULL e quantidade como 0).
 * @return ColecaoLivros* Um ponteiro para a nova coleção alocada, ou NULL se a alocação de memória falhar.
 */
ColecaoLivros* criar_colecao();

/**
 * @brief Adiciona um novo livro à coleção.
 * O livro é tipicamente adicionado no início da lista para maior eficiência (O(1)).
 * Uma cópia da struct Livro fornecida é armazenada na lista.
 *
 * @param colecao Ponteiro para a ColecaoLivros onde o livro será adicionado.
 * @param novo_livro A struct Livro contendo os dados do livro a ser adicionado.
 * @return int 1 se o livro foi adicionado com sucesso, 0 em caso de falha
 * (coleção nula ou falha de alocação de memória para o novo nó).
 */
int adicionar_livro_colecao(ColecaoLivros* colecao, Livro novo_livro);

/**
 * @brief Remove um livro da coleção com base no seu ISBN.
 * Procura o livro pelo ISBN e, se encontrado, remove-o da lista e libera a memória do nó.
 *
 * @param colecao Ponteiro para a ColecaoLivros de onde o livro será removido.
 * @param isbn String constante contendo o ISBN do livro a ser removido.
 * @return int 1 se o livro foi removido com sucesso, 0 se o livro não foi encontrado,
 * a coleção é nula/vazia ou o ISBN é nulo.
 */
int remover_livro_colecao(ColecaoLivros* colecao, const char* isbn);

/**
 * @brief Lista todos os livros presentes na coleção.
 * Exibe os detalhes de cada livro na coleção no console, utilizando a função `exibir_livro`.
 *
 * @param colecao Ponteiro constante para a ColecaoLivros a ser listada.
 * A coleção não é modificada por esta função.
 */
void listar_todos_livros(const ColecaoLivros* colecao);

/**
 * @brief Busca um livro na coleção pelo seu ISBN.
 *
 * @param colecao Ponteiro constante para a ColecaoLivros onde a busca será realizada.
 * @param isbn String constante contendo o ISBN do livro a ser buscado.
 * @return const Livro* Um ponteiro constante para a struct Livro encontrada dentro da lista.
 * Retorna NULL se o livro não for encontrado, ou se a coleção/ISBN forem inválidos.
 * @warning O ponteiro retornado aponta para dados internos da lista. Este ponteiro
 * não deve ser usado para modificar os dados do livro (o tipo `const Livro*` ajuda a
 * impor isso) e não deve ser liberado pelo chamador. O ponteiro se torna inválido
 * se o livro for removido da coleção ou se a coleção for destruída.
 */
const Livro* buscar_livro_por_isbn_na_colecao(const ColecaoLivros* colecao, const char* isbn);

/**
 * @brief Libera toda a memória alocada para a coleção de livros.
 * Percorre a lista, liberando cada nó individualmente, e depois libera a própria
 * estrutura da coleção.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser destruída.
 * Se `colecao` for NULL, a função não faz nada.
 * @note Após esta chamada, o ponteiro `colecao` no código chamador se tornará inválido
 * (dangling pointer) e deve ser idealmente atribuído a NULL pelo chamador.
 */
void destruir_colecao(ColecaoLivros* colecao);

/**
 * @brief Retorna a quantidade de livros atualmente na coleção.
 * @param colecao Ponteiro constante para a ColecaoLivros.
 * @return int O número de livros na coleção. Retorna 0 se a coleção for NULL.
 */
int tamanho_colecao(const ColecaoLivros* colecao);

#endif // LISTA_LIVROS_H