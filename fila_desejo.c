#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strcpy, strlen
#include "fila_desejos.h" // Assume que define FilaDesejos, NoFila e Livro

// --- FUNÇÕES DE EXEMPLO (JÁ COMPLETAS E BOAS) ---

/**
 * @brief Cria e inicializa uma nova fila de desejos.
 *
 * Aloca memória para a estrutura FilaDesejos e define seus ponteiros
 * 'inicio' e 'fim' como NULL, indicando uma fila vazia.
 *
 * @return Ponteiro para a FilaDesejos recém-criada ou NULL se a alocação falhar.
 */
FilaDesejos* criar_fila_desejos() {
    FilaDesejos* fila = (FilaDesejos*) malloc(sizeof(FilaDesejos));
    if (fila == NULL) {
        perror("ERRO: Falha ao alocar memoria para a fila de desejos");
        return NULL;
    }
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->quantidade = 0; // Aprimoramento: adicionar contador de elementos
    return fila;
}

/**
 * @brief Verifica se a fila de desejos está vazia.
 *
 * Uma fila é considerada vazia se seu ponteiro 'inicio' for NULL
 * ou se a quantidade de elementos for 0.
 * A função também trata o caso de uma fila não inicializada (NULL).
 *
 * @param fila Ponteiro constante para a FilaDesejos a ser verificada.
 * @return 1 se a fila estiver vazia ou for NULL, 0 caso contrário.
 */
int fila_desejos_vazia(const FilaDesejos* fila) {
    if (fila == NULL || fila->inicio == NULL) { // Ou fila->quantidade == 0
        return 1; // Vazia
    }
    return 0; // Não vazia
}

/**
 * @brief Adiciona um livro à fila de desejos (enqueue).
 *
 * Cria um novo nó para armazenar o livro e o insere no final da fila.
 * Atualiza os ponteiros 'inicio' e 'fim' da fila conforme necessário.
 *
 * @param fila Ponteiro para a FilaDesejos onde o livro será adicionado.
 * @param livro O struct Livro a ser copiado para a fila de desejos.
 */
void enqueue_desejo(FilaDesejos* fila, Livro livro) {
    if (fila == NULL) {
        fprintf(stderr, "ERRO: Fila de desejos nula. Nao e possivel adicionar livro.\n");
        return;
    }

    NoFila* novo_no = (NoFila*) malloc(sizeof(NoFila));
    if (novo_no == NULL) {
        perror("ERRO: Falha ao alocar memoria para novo no da fila de desejos");
        return;
    }

    novo_no->livro_desejado = livro; // Copia a struct inteira
    novo_no->proximo = NULL;

    if (fila_desejos_vazia(fila)) {
        fila->inicio = novo_no;
        fila->fim = novo_no;
    } else {
        fila->fim->proximo = novo_no;
        fila->fim = novo_no;
    }
    fila->quantidade++; // Aprimoramento: incrementar contador
}

/**
 * @brief Remove e retorna o livro do início da fila de desejos (dequeue).
 *
 * O livro no início da fila é removido e seus dados são copiados
 * para a estrutura apontada por 'livro_removido'.
 * O nó correspondente é liberado da memória.
 *
 * @param fila Ponteiro para a FilaDesejos de onde o livro será removido.
 * @param livro_removido Ponteiro para uma struct Livro onde os dados do livro
 * removido serão armazenados.
 * @return 1 se um livro foi removido com sucesso, 0 se a fila estava vazia ou era nula.
 */
int dequeue_desejo(FilaDesejos* fila, Livro* livro_removido) {
    if (fila_desejos_vazia(fila)) {
        fprintf(stderr, "AVISO: Fila de desejos vazia, nao e possivel remover (dequeue).\n");
        // Opcional: Limpar a struct livro_removido se desejado, mas o status de retorno é o principal.
        if (livro_removido != NULL) { // Boa prática: verificar ponteiro antes de usar
            livro_removido->titulo[0] = '\0';
            // Defina outros campos para indicar "vazio" se necessário
        }
        return 0; // Falha: fila vazia ou nula
    }
    
    // Se livro_removido for NULL, não podemos armazenar o resultado, mas ainda removemos da fila.
    // Isso pode ser um caso de uso válido se o chamador só quer descartar o elemento.
    // Contudo, é mais comum que o chamador queira o valor.
    if (livro_removido == NULL) {
         fprintf(stderr, "AVISO: Ponteiro para livro_removido e nulo em dequeue_desejo. O elemento sera removido da fila, mas nao retornado.\n");
    }


    NoFila* no_removido_ptr = fila->inicio;

    if (livro_removido != NULL) {
        *livro_removido = no_removido_ptr->livro_desejado; // Copia a struct
    }

    fila->inicio = no_removido_ptr->proximo;

    if (fila->inicio == NULL) { // A fila ficou vazia
        fila->fim = NULL;
    }

    free(no_removido_ptr);
    fila->quantidade--; // Aprimoramento: decrementar contador
    return 1; // Sucesso
}

/**
 * @brief Obtém os dados do livro no início da fila de desejos sem removê-lo (front/peek).
 *
 * Copia os dados do livro no início da fila para a estrutura
 * apontada por 'livro_frente'.
 *
 * @param fila Ponteiro constante para a FilaDesejos a ser consultada.
 * @param livro_frente Ponteiro para uma struct Livro onde os dados do livro
 * da frente serão armazenados.
 * @return 1 se os dados do livro da frente foram obtidos com sucesso,
 * 0 se a fila estava vazia, era nula, ou se 'livro_frente' era NULL.
 */
int front_desejo(const FilaDesejos* fila, Livro* livro_frente) {
    if (livro_frente == NULL) {
        fprintf(stderr, "ERRO: Ponteiro para livro_frente nulo em front_desejo.\n");
        return 0; // Falha: destino nulo
    }
    if (fila_desejos_vazia(fila)) {
        fprintf(stderr, "AVISO: Fila de desejos vazia, nao e possivel ver o elemento da frente.\n");
        livro_frente->titulo[0] = '\0'; // Limpa para segurança
        return 0; // Falha: fila vazia ou nula
    }

    *livro_frente = fila->inicio->livro_desejado; // Copia a struct
    return 1; // Sucesso
}

/**
 * @brief Libera toda a memória alocada para a fila de desejos.
 *
 * Remove todos os elementos restantes da fila (liberando cada nó)
 * e, em seguida, libera a própria estrutura da fila.
 * Após esta chamada, o ponteiro 'fila' não deve mais ser usado.
 *
 * @param fila Ponteiro para a FilaDesejos a ser destruída.
 * Se a fila for NULL, a função não faz nada.
 */
void destruir_fila_desejos(FilaDesejos* fila) {
    if (fila == NULL) {
        return;
    }

    Livro livro_temp; // Variável temporária para o dequeue
    while (!fila_desejos_vazia(fila)) {
        dequeue_desejo(fila, &livro_temp); // Descarta o livro, apenas libera o nó
    }

    free(fila);
}

/**
 * @brief Retorna a quantidade de elementos na fila de desejos.
 *
 * @param fila Ponteiro constante para a FilaDesejos.
 * @return O número de elementos na fila, ou 0 se a fila for NULL.
 */
int tamanho_fila_desejos(const FilaDesejos* fila) {
    if (fila == NULL) {
        return 0;
    }
    return fila->quantidade;
}