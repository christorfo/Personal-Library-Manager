// Exemplo com lista encadeada para a pilha
typedef struct NoPilha {
    char isbn_livro_visualizado[14]; // Armazena ISBN do livro
    struct NoPilha* proximo;
} NoPilha;

typedef struct {
    NoPilha* topo;
} PilhaHistorico;