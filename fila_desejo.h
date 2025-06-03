#ifndef FILA_DESEJOS_H
#define FILA_DESEJOS_H

#include "livro.h" // Necessário para a definição da struct Livro

/**
 * @file fila_desejos.h
 * @brief Define as estruturas de dados e protótipos de funções para uma fila de desejos de livros.
 *
 * A fila é implementada como uma lista encadeada e armazena cópias de structs Livro.
 */

/**
 * @brief Nó da fila de desejos.
 * Contém um livro desejado e um ponteiro para o próximo nó na fila.
 * Esta estrutura é usada internamente pela implementação da fila encadeada.
 */
typedef struct NoFila {
    /**
     * @brief Os dados do livro desejado.
     * Alternativamente, para economizar memória, poderia ser armazenado apenas o ISBN
     * e o livro completo seria buscado em uma coleção principal quando necessário.
     */
    Livro livro_desejado;
    struct NoFila* proximo; ///< Ponteiro para o próximo nó na fila.
} NoFila;

/**
 * @brief Estrutura da Fila de Desejos.
 * Mantém ponteiros para o início e o fim da fila,
 * além da quantidade de elementos presentes.
 * (Estrutura Complexa - Filas)
 */
typedef struct {
    NoFila* inicio;      ///< Ponteiro para o primeiro nó da fila (ou NULL se vazia).
    NoFila* fim;         ///< Ponteiro para o último nó da fila (ou NULL se vazia).
    int quantidade;      ///< Número de elementos atualmente na fila.
} FilaDesejos;

// --- Protótipos das Funções ---

/**
 * @brief Cria e inicializa uma nova fila de desejos vazia.
 * Aloca memória para a estrutura FilaDesejos e inicializa seus campos.
 * @return Ponteiro para a FilaDesejos recém-criada, ou NULL em caso de falha na alocação.
 */
FilaDesejos* criar_fila_desejos();

/**
 * @brief Adiciona um livro ao final da fila de desejos (enqueue).
 * Uma cópia da struct Livro fornecida é adicionada à fila.
 * @param fila Ponteiro para a FilaDesejos onde o livro será adicionado.
 * @param livro A struct Livro a ser enfileirada.
 */
void enqueue_desejo(FilaDesejos* fila, Livro livro);

/**
 * @brief Remove e retorna o livro do início da fila de desejos (dequeue).
 * Se a fila não estiver vazia, o livro no início é removido, seus dados são
 * copiados para a struct apontada por 'livro_removido', e o nó é liberado.
 * @param fila Ponteiro para a FilaDesejos de onde o livro será removido.
 * @param livro_removido Ponteiro para uma struct Livro onde os dados do livro
 * removido serão armazenados. Não deve ser NULL se o valor do livro for desejado.
 * @return 1 se um livro foi removido com sucesso.
 * @return 0 se a fila estava vazia, se 'fila' era NULL.
 */
int dequeue_desejo(FilaDesejos* fila, Livro* livro_removido);

/**
 * @brief Obtém os dados do livro no início da fila de desejos sem removê-lo (front/peek).
 * Se a fila não estiver vazia, os dados do livro no início são copiados
 * para a struct apontada por 'livro_frente'.
 * @param fila Ponteiro constante para a FilaDesejos a ser consultada.
 * @param livro_frente Ponteiro para uma struct Livro onde os dados do livro
 * da frente serão armazenados. Não deve ser NULL.
 * @return 1 se os dados do livro da frente foram obtidos com sucesso.
 * @return 0 se a fila estava vazia, se 'fila' era NULL, ou se 'livro_frente' era NULL.
 */
int front_desejo(const FilaDesejos* fila, Livro* livro_frente);

/**
 * @brief Verifica se a fila de desejos está vazia.
 * @param fila Ponteiro constante para a FilaDesejos a ser verificada.
 * @return 1 se a fila estiver vazia ou se 'fila' for NULL.
 * @return 0 caso contrário.
 */
int fila_desejos_vazia(const FilaDesejos* fila);

/**
 * @brief Libera toda a memória alocada para a fila de desejos e seus nós.
 * Após esta chamada, o ponteiro 'fila' não deve mais ser usado.
 * @param fila Ponteiro para a FilaDesejos a ser destruída.
 * Se 'fila' for NULL, a função não faz nada.
 */
void destruir_fila_desejos(FilaDesejos* fila);

/**
 * @brief Retorna o número de elementos atualmente na fila de desejos.
 * @param fila Ponteiro constante para a FilaDesejos.
 * @return O número de elementos na fila, ou 0 se 'fila' for NULL.
 */
int tamanho_fila_desejos(const FilaDesejos* fila);

#endif // FILA_DESEJOS_H