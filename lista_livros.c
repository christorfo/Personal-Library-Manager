#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_livros.h" // Assume que define ColecaoLivros, NoLista e Livro, e protótipo de exibir_livro

// --- FUNÇÕES IMPLEMENTADAS E APRIMORADAS ---

/**
 * @brief Cria e inicializa uma nova coleção de livros (lista encadeada).
 * @return ColecaoLivros* Um ponteiro para a nova coleção alocada, ou NULL se a alocação falhar.
 */
ColecaoLivros* criar_colecao() {
    ColecaoLivros* nova_colecao = (ColecaoLivros*) malloc(sizeof(ColecaoLivros));

    if (nova_colecao == NULL) {
        perror("ERRO: Falha ao alocar memoria para a colecao de livros");
        return NULL;
    }

    nova_colecao->inicio = NULL;
    nova_colecao->quantidade = 0;

    return nova_colecao;
}

/**
 * @brief Adiciona um novo livro à coleção (lista encadeada).
 * O novo livro é adicionado no início da lista para eficiência (O(1)).
 * Uma cópia dos dados de 'novo_livro_dados' é armazenada na lista.
 *
 * @param colecao Ponteiro para a ColecaoLivros onde o livro será adicionado.
 * @param novo_livro_dados Struct Livro contendo os dados do livro a ser adicionado.
 * @return int 1 se o livro foi adicionado com sucesso, 0 em caso de falha
 * (coleção nula ou falha de alocação de memória para o novo nó).
 * @note Esta função permite a adição de livros com ISBNs duplicados.
 * Para inserção ordenada ou no final, funções adicionais seriam necessárias.
 */
int adicionar_livro_colecao(ColecaoLivros* colecao, Livro novo_livro_dados) {
    if (colecao == NULL) {
        fprintf(stderr, "ERRO: Colecao nao pode ser NULA para adicionar livro.\n");
        return 0; // Falha: coleção nula
    }

    NoLista* novo_no = (NoLista*) malloc(sizeof(NoLista));
    if (novo_no == NULL) {
        perror("ERRO: Falha ao alocar memoria para novo no da lista de livros");
        return 0; // Falha: alocação do nó
    }

    novo_no->dadosLivro = novo_livro_dados; // Copia a struct Livro inteira
    novo_no->proximo = colecao->inicio;
    colecao->inicio = novo_no;
    colecao->quantidade++;

    return 1; // Sucesso
}

/**
 * @brief Remove um livro da coleção com base no ISBN.
 * Percorre a lista, encontra o livro com o ISBN correspondente e o remove,
 * liberando a memória do nó.
 *
 * @param colecao Ponteiro para a ColecaoLivros.
 * @param isbn String constante contendo o ISBN do livro a ser removido.
 * @return int 1 se o livro foi removido com sucesso, 0 se o livro não foi encontrado,
 * a coleção é nula/vazia ou o ISBN é nulo.
 */
int remover_livro_colecao(ColecaoLivros* colecao, const char* isbn) {
    if (colecao == NULL || colecao->inicio == NULL || isbn == NULL || colecao->quantidade == 0) {
        if (colecao == NULL || isbn == NULL) {
            fprintf(stderr, "ERRO: Colecao ou ISBN nulos para remocao.\n");
        } else if (colecao->inicio == NULL) {
            //fprintf(stderr, "AVISO: Tentativa de remover de colecao vazia.\n");
        }
        return 0; // Nada a remover ou parâmetros inválidos
    }

    NoLista* atual = colecao->inicio;
    NoLista* anterior = NULL;

    // Percorrer a lista para encontrar o livro
    while (atual != NULL && strcmp(atual->dadosLivro.isbn, isbn) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Se o livro não for encontrado
    if (atual == NULL) {
        return 0; // Livro não encontrado
    }

    // Remover o livro
    if (anterior == NULL) { // O livro a ser removido é o primeiro da lista
        colecao->inicio = atual->proximo;
    } else { // O livro a ser removido está no meio ou no fim da lista
        anterior->proximo = atual->proximo;
    }

    // Liberar a memória do nó removido
    // Se Livro tivesse campos alocados dinamicamente, precisariam ser liberados aqui primeiro.
    free(atual);
    colecao->quantidade--;

    return 1; // Sucesso
}

/**
 * @brief Lista todos os livros presentes na coleção, exibindo seus detalhes.
 * Utiliza a função 'exibir_livro' (supostamente definida em outro lugar)
 * para formatar a saída de cada livro.
 *
 * @param colecao Ponteiro constante para a ColecaoLivros.
 */
void listar_todos_livros(const ColecaoLivros* colecao) {
    if (colecao == NULL) {
        printf("Colecao de livros nao inicializada.\n");
        return;
    }

    if (colecao->quantidade == 0) { // ou colecao->inicio == NULL
        printf("A colecao de livros esta vazia.\n");
        return;
    }

    printf("\n--- Listando Todos os Livros (%d) ---\n", colecao->quantidade);
    NoLista* atual = colecao->inicio;

    int contador = 1;
    while (atual != NULL) {
        printf("Livro %d:\n", contador++);
        exibir_livro(&(atual->dadosLivro)); // Assumindo que exibir_livro() existe e lida bem com const Livro* se necessário
        printf("---\n");
        atual = atual->proximo;
    }
    printf("--- Fim da Lista (%d livros listados) ---\n", colecao->quantidade);
}

/**
 * @brief Busca um livro na coleção pelo ISBN.
 *
 * @param colecao Ponteiro constante para a ColecaoLivros.
 * @param isbn String constante contendo o ISBN do livro a ser buscado.
 * @return const Livro* Um ponteiro constante para a struct Livro encontrada (dentro do nó da lista),
 * ou NULL se o livro não for encontrado ou se a coleção/ISBN forem inválidos.
 * @warning O ponteiro retornado aponta para dados internos da lista.
 * Não modifique o conteúdo apontado (o tipo de retorno `const Livro*` ajuda a impor isso)
 * e não libere este ponteiro. Ele se torna inválido se o livro for removido
 * ou a coleção destruída.
 */
const Livro* buscar_livro_por_isbn_na_colecao(const ColecaoLivros* colecao, const char* isbn) {
    if (colecao == NULL || isbn == NULL) {
        return NULL;
    }

    NoLista* atual = colecao->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dadosLivro.isbn, isbn) == 0) {
            return &(atual->dadosLivro); // Retorna ponteiro para o livro dentro do nó
        }
        atual = atual->proximo;
    }

    return NULL; // Livro não encontrado
}

/**
 * @brief Retorna a quantidade de livros na coleção.
 * @param colecao Ponteiro constante para a ColecaoLivros.
 * @return int O número de livros na coleção, ou 0 se a coleção for NULL.
 */
int tamanho_colecao(const ColecaoLivros* colecao) {
    if (colecao == NULL) {
        return 0;
    }
    return colecao->quantidade;
}

/**
 * @brief Libera toda a memória alocada para a coleção de livros, incluindo todos os nós e a própria estrutura da coleção.
 * Após chamar esta função, o ponteiro para 'colecao' no código chamador se tornará um "dangling pointer"
 * e deve, idealmente, ser atribuído a NULL pelo chamador para evitar seu uso acidental.
 *
 * @param colecao Ponteiro para a ColecaoLivros a ser destruída. Se NULL, a função não faz nada.
 * @note Para que esta função pudesse definir o ponteiro original do chamador como NULL,
 * ela precisaria receber um ponteiro para ponteiro (ColecaoLivros**).
 */
void destruir_colecao(ColecaoLivros* colecao) {
    if (colecao == NULL) {
        return;
    }

    NoLista* atual = colecao->inicio;
    NoLista* proximo_no;

    while (atual != NULL) {
        proximo_no = atual->proximo;
        // Se 'dadosLivro' dentro de 'NoLista' contivesse ponteiros que também precisassem
        // ser liberados (ex: se Livro tivesse campos alocados dinamicamente que a struct 'Livro' "possui"),
        // seria necessário liberá-los aqui primeiro, antes de liberar 'atual'.
        // Assumindo que a struct Livro em si não gerencia memória dinâmica dessa forma.
        free(atual);
        atual = proximo_no;
    }

    // Finalmente, liberar a própria estrutura da coleção.
    free(colecao);
    // O chamador é responsável por atribuir seu ponteiro original para NULL, se desejar.
    // Ex: colecao_ptr = NULL; após chamar destruir_colecao(colecao_ptr);
}