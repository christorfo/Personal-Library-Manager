#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_livros.h"

// --- FUNÇÃO COMPLETA (EXEMPLO) ---
/**
 * @brief Cria e inicializa uma nova coleção de livros (lista encadeada).
 * * @return ColecaoLivros* Um ponteiro para a nova coleção alocada, ou NULL se a alocação falhar.
 */
ColecaoLivros* criar_colecao() {
    // Lógica: Alocar memória para a struct ColecaoLivros.
    ColecaoLivros* nova_colecao = (ColecaoLivros*) malloc(sizeof(ColecaoLivros));

    // Lógica: Verificar se a alocação foi bem-sucedida.
    if (nova_colecao == NULL) {
        perror("Erro ao alocar memoria para a colecao de livros");
        return NULL;
    }

    // Lógica: Inicializar os campos da coleção (inicio para NULL, quantidade para 0).
    nova_colecao->inicio = NULL;
    nova_colecao->quantidade = 0;

    // Lógica: Retornar o ponteiro para a coleção criada.
    return nova_colecao;
}

// --- ESQUELETOS DETALHADOS (VOCÊ IMPLEMENTA) ---

/**
 * @brief Adiciona um novo livro à coleção (lista encadeada).
 * O novo livro é geralmente adicionado no início da lista para simplicidade.
 * * @param colecao Ponteiro para a ColecaoLivros onde o livro será adicionado.
 * @param novo_livro Struct Livro contendo os dados do livro a ser adicionado.
 * Note que uma cópia dos dados de novo_livro será armazenada na lista.
 */
void adicionar_livro_colecao(ColecaoLivros* colecao, Livro novo_livro_dados) {
    // Lógica: Verificar se o ponteiro da coleção é NULL. Se for, não fazer nada e talvez imprimir um erro.
    if (colecao == NULL) {
        fprintf(stderr, "Erro: Colecao nao pode ser NULA para adicionar livro.\n");
        return;
    }

    // Lógica: Alocar memória para um novo NoLista (nó da lista).
    NoLista* novo_no = (NoLista*) malloc(sizeof(NoLista));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para novo no da lista");
        return; // Falha na alocação do nó
    }

    // Lógica: Copiar os dados de 'novo_livro_dados' para o campo 'dadosLivro' do novo nó.
    // Como 'dadosLivro' é uma struct Livro (e não um ponteiro), a cópia é direta.
    novo_no->dadosLivro = novo_livro_dados;

    // Lógica: Fazer o campo 'proximo' do novo nó apontar para o antigo 'inicio' da lista.
    novo_no->proximo = colecao->inicio;

    // Lógica: Atualizar o 'inicio' da lista para que aponte para o novo nó.
    colecao->inicio = novo_no;

    // Lógica: Incrementar a 'quantidade' de livros na coleção.
    colecao->quantidade++;

    // TODO: Considerar se é melhor adicionar no final ou de forma ordenada (mais complexo).
    //       Adicionar no início é mais simples e eficiente (O(1)).
}

/**
 * @brief Remove um livro da coleção com base no ISBN.
 * * @param colecao Ponteiro para a ColecaoLivros.
 * @param isbn ISBN do livro a ser removido.
 * @return int 1 se o livro foi removido com sucesso, 0 se o livro não foi encontrado.
 */
int remover_livro_colecao(ColecaoLivros* colecao, const char* isbn) {
    // Lógica: Verificar se a coleção ou o ISBN são NULL, ou se a lista está vazia.
    if (colecao == NULL || colecao->inicio == NULL || isbn == NULL) {
        return 0; // Nada a remover ou ISBN inválido
    }

    NoLista* atual = colecao->inicio;
    NoLista* anterior = NULL;

    // Lógica: Percorrer a lista para encontrar o livro com o ISBN fornecido.
    //         Manter um ponteiro para o nó anterior para facilitar a remoção.
    while (atual != NULL && strcmp(atual->dadosLivro.isbn, isbn) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // Lógica: Se o livro não for encontrado (atual == NULL), retornar 0.
    if (atual == NULL) {
        return 0; // Livro não encontrado
    }

    // Lógica: Se o livro for encontrado:
    //         Caso 1: O livro a ser removido é o primeiro da lista (anterior == NULL).
    if (anterior == NULL) {
        colecao->inicio = atual->proximo;
    }
    //         Caso 2: O livro a ser removido está no meio ou no fim da lista.
    else {
        anterior->proximo = atual->proximo;
    }

    // Lógica: Liberar a memória do nó removido ('atual').
    free(atual);

    // Lógica: Decrementar a 'quantidade' de livros na coleção.
    colecao->quantidade--;

    // Lógica: Retornar 1 indicando sucesso.
    return 1;
}

/**
 * @brief Lista todos os livros presentes na coleção, exibindo seus detalhes.
 * * @param colecao Ponteiro para a ColecaoLivros.
 */
void listar_todos_livros(const ColecaoLivros* colecao) {
    // Lógica: Verificar se a coleção é NULL.
    if (colecao == NULL) {
        printf("Colecao de livros nao inicializada.\n");
        return;
    }

    // Lógica: Verificar se a lista está vazia (colecao->inicio == NULL ou colecao->quantidade == 0).
    if (colecao->inicio == NULL) {
        printf("A colecao de livros esta vazia.\n");
        return;
    }

    printf("\n--- Listando Todos os Livros (%d) ---\n", colecao->quantidade);
    NoLista* atual = colecao->inicio;

    // Lógica: Percorrer a lista do início ao fim.
    while (atual != NULL) {
        // Lógica: Para cada nó, chamar a função exibir_livro() para mostrar os dados do livro.
        exibir_livro(&(atual->dadosLivro)); // Passa o endereço da struct Livro
        atual = atual->proximo;
    }
    printf("--- Fim da Lista ---\n");
}

/**
 * @brief Busca um livro na coleção pelo ISBN.
 * * @param colecao Ponteiro para a ColecaoLivros.
 * @param isbn ISBN do livro a ser buscado.
 * @return Livro* Um ponteiro para a struct Livro encontrada (dentro do nó da lista),
 * ou NULL se o livro não for encontrado ou a coleção for inválida.
 * CUIDADO: Não libere este ponteiro diretamente, pois ele aponta para dados dentro da lista.
 */
Livro* buscar_livro_por_isbn_na_colecao(const ColecaoLivros* colecao, const char* isbn) {
    // Lógica: Verificar se a coleção ou o ISBN são NULL.
    if (colecao == NULL || isbn == NULL) {
        return NULL;
    }

    NoLista* atual = colecao->inicio;

    // Lógica: Percorrer a lista.
    while (atual != NULL) {
        // Lógica: Em cada nó, comparar o ISBN do livro com o ISBN procurado.
        if (strcmp(atual->dadosLivro.isbn, isbn) == 0) {
            // Lógica: Se encontrar, retornar um ponteiro para 'atual->dadosLivro'.
            return &(atual->dadosLivro);
        }
        atual = atual->proximo;
    }

    // Lógica: Se percorrer toda a lista e não encontrar, retornar NULL.
    return NULL;
}

/**
 * @brief Libera toda a memória alocada para a coleção de livros, incluindo todos os nós.
 * * @param colecao Ponteiro para a ColecaoLivros a ser destruída.
 */
void destruir_colecao(ColecaoLivros* colecao) {
    // Lógica: Verificar se a coleção é NULL.
    if (colecao == NULL) {
        return;
    }

    NoLista* atual = colecao->inicio;
    NoLista* proximo_no;

    // Lógica: Percorrer a lista, liberando cada nó.
    while (atual != NULL) {
        proximo_no = atual->proximo; // Guarda o próximo antes de liberar o atual
        // Lógica: Liberar o nó 'atual'.
        // Se 'dadosLivro' dentro de 'NoLista' contivesse ponteiros que também precisassem
        // ser liberados (ex: se Livro tivesse campos alocados dinamicamente),
        // seria necessário liberá-los aqui primeiro. No nosso caso, Livro só tem dados estáticos.
        free(atual);
        atual = proximo_no;
    }

    // Lógica: Finalmente, liberar a própria estrutura da coleção.
    free(colecao);

    // TODO: Após chamar esta função, o ponteiro 'colecao' no código chamador
    //       deve ser idealmente setado para NULL para evitar "dangling pointers".
}