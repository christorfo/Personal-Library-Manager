// Exemplo com lista encadeada para a fila
typedef struct NoFila {
    Livro livro_desejado; // Pode armazenar o livro inteiro ou só o ISBN
    struct NoFila* proximo;
} NoFila;

typedef struct {
    NoFila* inicio;
    NoFila* fim;
} FilaLeitura;