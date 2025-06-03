#include <stdio.h>
#include <stdlib.h> // Para malloc e free
#include <string.h> // Para strncpy
#include "pilha_historico.h" // Assume que define PilhaHistorico, NoPilha, TAM_ISBN

// --- FUNÇÕES COMPLETAS E APRIMORADAS ---

/**
 * @brief Cria e inicializa uma nova pilha de histórico.
 * Aloca memória para a estrutura PilhaHistorico e define seu topo como NULL
 * e quantidade como 0.
 *
 * @return PilhaHistorico* Ponteiro para a PilhaHistorico recém-criada ou NULL se a alocação falhar.
 */
PilhaHistorico* criar_pilha_historico() {
    PilhaHistorico* pilha = (PilhaHistorico*) malloc(sizeof(PilhaHistorico));
    if (pilha == NULL) {
        perror("ERRO (criar_pilha_historico): Falha ao alocar memoria para a pilha");
        return NULL;
    }
    pilha->topo = NULL;
    pilha->quantidade = 0; // Inicializa a quantidade de elementos
    return pilha;
}

/**
 * @brief Verifica se a pilha de histórico está vazia.
 * Uma pilha é considerada vazia se seu topo for NULL ou se a quantidade for 0.
 * A função também trata o caso de uma pilha não inicializada (NULL).
 *
 * @param pilha Ponteiro constante para a PilhaHistorico a ser verificada.
 * @return int 1 se a pilha estiver vazia ou for NULL, 0 caso contrário.
 */
int pilha_historico_vazia(const PilhaHistorico* pilha) {
    if (pilha == NULL || pilha->topo == NULL) { // Ou pilha->quantidade == 0
        return 1; // Vazia
    }
    return 0; // Não vazia
}

/**
 * @brief Adiciona um ISBN ao topo da pilha de histórico (push).
 *
 * @param pilha Ponteiro para a PilhaHistorico onde o ISBN será adicionado.
 * @param isbn String constante contendo o ISBN a ser adicionado. Não deve ser NULL.
 * @return int 1 se o ISBN foi adicionado com sucesso, 0 em caso de falha
 * (pilha/ISBN nulos ou falha de alocação de memória).
 */
int push_historico(PilhaHistorico* pilha, const char* isbn) {
    if (pilha == NULL || isbn == NULL) {
        fprintf(stderr, "ERRO (push_historico): Pilha ou ISBN nulos para push.\n");
        return 0;
    }

    NoPilha* novo_no = (NoPilha*) malloc(sizeof(NoPilha));
    if (novo_no == NULL) {
        perror("ERRO (push_historico): Falha ao alocar memoria para novo no da pilha");
        return 0;
    }

    strncpy(novo_no->isbn_consultado, isbn, TAM_ISBN - 1);
    novo_no->isbn_consultado[TAM_ISBN - 1] = '\0'; // Garante terminação nula

    novo_no->proximo = pilha->topo;
    pilha->topo = novo_no;
    pilha->quantidade++; // Incrementa a quantidade

    return 1; // Sucesso
}

/**
 * @brief Remove e retorna o ISBN do topo da pilha de histórico (pop).
 * A função aloca nova memória para a string ISBN retornada.
 * O CHAMADOR É RESPONSÁVEL POR LIBERAR ESTA MEMÓRIA USANDO free().
 *
 * @param pilha Ponteiro para a PilhaHistorico de onde o ISBN será removido.
 * @return char* Ponteiro para uma nova string contendo o ISBN removido,
 * ou NULL se a pilha estava vazia, era nula, ou se a alocação para a
 * string de retorno falhou.
 */
char* pop_historico(PilhaHistorico* pilha) {
    if (pilha_historico_vazia(pilha)) {
        fprintf(stderr, "AVISO (pop_historico): Pilha vazia, nao e possivel fazer pop.\n");
        return NULL;
    }

    NoPilha* no_removido = pilha->topo;

    // Alocar memória para copiar o ISBN a ser retornado.
    char* isbn_retorno = (char*) malloc(TAM_ISBN * sizeof(char));
    if (isbn_retorno == NULL) {
        perror("ERRO (pop_historico): Falha ao alocar memoria para retornar ISBN do pop");
        // A pilha permanece consistente, pois o nó não foi removido.
        return NULL;
    }
    // Usar strncpy para copiar de forma segura
    strncpy(isbn_retorno, no_removido->isbn_consultado, TAM_ISBN);
    isbn_retorno[TAM_ISBN - 1] = '\0'; // Garante terminação nula, especialmente se o original não tinha ou era maior

    pilha->topo = no_removido->proximo;
    free(no_removido);
    pilha->quantidade--; // Decrementa a quantidade

    return isbn_retorno;
}

/**
 * @brief Retorna uma referência ao ISBN no topo da pilha de histórico sem removê-lo (peek).
 *
 * @param pilha Ponteiro constante para a PilhaHistorico a ser consultada.
 * @return const char* Ponteiro constante para a string ISBN no topo da pilha.
 * Este ponteiro aponta para dados internos da pilha e NÃO deve ser liberado
 * ou modificado pelo chamador. Ele é válido apenas enquanto o nó estiver no topo
 * da pilha. Retorna NULL se a pilha estiver vazia ou for nula.
 */
const char* peek_historico(const PilhaHistorico* pilha) {
    if (pilha_historico_vazia(pilha)) {
        //fprintf(stderr, "AVISO (peek_historico): Pilha vazia.\n"); // Opcional
        return NULL;
    }
    // Retorna um ponteiro constante para a string dentro do nó.
    // Nenhuma nova memória é alocada.
    return pilha->topo->isbn_consultado;
}

/**
 * @brief Libera toda a memória alocada para a pilha de histórico.
 * Remove todos os elementos restantes (liberando a string ISBN alocada por pop_historico
 * durante este processo) e, em seguida, libera a própria estrutura da pilha.
 *
 * @param pilha Ponteiro para a PilhaHistorico a ser destruída.
 * Se a pilha for NULL, a função não faz nada.
 */
void destruir_pilha_historico(PilhaHistorico* pilha) {
    if (pilha == NULL) {
        return;
    }

    char* isbn_temp;
    while (!pilha_historico_vazia(pilha)) {
        isbn_temp = pop_historico(pilha); // pop_historico já libera o NoPilha
        if (isbn_temp != NULL) {
            free(isbn_temp); // Libera a string ISBN que foi alocada e retornada por pop_historico
        }
    }

    free(pilha); // Libera a estrutura da pilha em si
}

/**
 * @brief Retorna a quantidade de elementos na pilha de histórico.
 *
 * @param pilha Ponteiro constante para a PilhaHistorico.
 * @return int O número de elementos na pilha, ou 0 se a pilha for NULL.
 */
int tamanho_pilha_historico(const PilhaHistorico* pilha) {
    if (pilha == NULL) {
        return 0;
    }
    return pilha->quantidade;
}