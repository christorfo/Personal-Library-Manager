#ifndef PILHA_HISTORICO_H
#define PILHA_HISTORICO_H

/**
 * @file pilha_historico.h
 * @brief Define as estruturas de dados e protótipos de funções para uma pilha de histórico de ISBNs.
 * A pilha é implementada como uma lista encadeada e armazena strings de ISBN.
 */

/**
 * @brief Tamanho máximo para uma string ISBN, incluindo o terminador nulo.
 * Usado para definir o buffer nos nós da pilha.
 */
#define TAM_ISBN 14

/**
 * @brief Nó da pilha de histórico.
 * Contém uma string ISBN e um ponteiro para o próximo nó na pilha.
 */
typedef struct NoPilha {
    char isbn_consultado[TAM_ISBN]; ///< Armazena a string do ISBN consultado.
    struct NoPilha* proximo;        ///< Ponteiro para o próximo nó na pilha.
} NoPilha;

/**
 * @brief Estrutura da Pilha de Histórico.
 * Mantém um ponteiro para o topo da pilha e a quantidade de elementos.
 * (Estrutura Complexa - Pilhas)
 */
typedef struct {
    NoPilha* topo;          ///< Ponteiro para o nó no topo da pilha (NULL se a pilha estiver vazia).
    int quantidade;         ///< Número de elementos atualmente na pilha.
} PilhaHistorico;

// --- Protótipos das Funções ---

/**
 * @brief Cria e inicializa uma nova pilha de histórico vazia.
 * Aloca memória para a estrutura PilhaHistorico e inicializa seus campos.
 * @return PilhaHistorico* Um ponteiro para a nova pilha alocada, ou NULL se a alocação de memória falhar.
 */
PilhaHistorico* criar_pilha_historico();

/**
 * @brief Adiciona um ISBN ao topo da pilha de histórico (push).
 *
 * @param pilha Ponteiro para a PilhaHistorico onde o ISBN será adicionado.
 * @param isbn String constante contendo o ISBN a ser empilhado. Não deve ser NULL.
 * @return int 1 se o ISBN foi adicionado com sucesso, 0 em caso de falha
 * (pilha/ISBN nulos ou falha de alocação de memória para o novo nó).
 */
int push_historico(PilhaHistorico* pilha, const char* isbn);

/**
 * @brief Remove e retorna o ISBN do topo da pilha de histórico (pop).
 * A função aloca nova memória para a string ISBN retornada.
 * **O chamador é responsável por liberar esta memória usando `free()`**.
 *
 * @param pilha Ponteiro para a PilhaHistorico de onde o ISBN será removido.
 * @return char* Ponteiro para uma nova string contendo o ISBN removido do topo.
 * Retorna NULL se a pilha estava vazia, se 'pilha' era NULL, ou se a alocação
 * para a string de retorno falhou.
 */
char* pop_historico(PilhaHistorico* pilha);

/**
 * @brief Retorna uma referência (ponteiro) ao ISBN no topo da pilha sem removê-lo (peek).
 *
 * @param pilha Ponteiro constante para a PilhaHistorico a ser consultada.
 * @return const char* Ponteiro constante para a string ISBN no topo da pilha.
 * Este ponteiro aponta para dados internos da pilha e **NÃO deve ser liberado
 * ou modificado pelo chamador**. Ele é válido apenas enquanto o nó correspondente
 * estiver no topo da pilha e a pilha não for modificada de forma a remover este nó.
 * Retorna NULL se a pilha estiver vazia ou se 'pilha' for NULL.
 */
const char* peek_historico(const PilhaHistorico* pilha);

/**
 * @brief Verifica se a pilha de histórico está vazia.
 *
 * @param pilha Ponteiro constante para a PilhaHistorico a ser verificada.
 * @return int 1 se a pilha estiver vazia ou se 'pilha' for NULL.
 * @return 0 caso contrário.
 */
int pilha_historico_vazia(const PilhaHistorico* pilha);

/**
 * @brief Libera toda a memória alocada para a pilha de histórico e seus nós.
 * Isso inclui as strings ISBN que seriam retornadas (e alocadas) por `pop_historico`
 * durante o processo de esvaziamento da pilha.
 *
 * @param pilha Ponteiro para a PilhaHistorico a ser destruída.
 * Se 'pilha' for NULL, a função não faz nada.
 * @note Após esta chamada, o ponteiro `pilha` no código chamador se tornará inválido
 * (dangling pointer) e deve ser idealmente atribuído a NULL pelo chamador.
 */
void destruir_pilha_historico(PilhaHistorico* pilha);

/**
 * @brief Retorna o número de elementos atualmente na pilha de histórico.
 * @param pilha Ponteiro constante para a PilhaHistorico.
 * @return int O número de elementos na pilha. Retorna 0 se 'pilha' for NULL.
 */
int tamanho_pilha_historico(const PilhaHistorico* pilha);

#endif // PILHA_HISTORICO_H