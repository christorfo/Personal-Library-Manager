#include "livro.h" // Para usar o tipo Livro

typedef struct NoLista {
    Livro dadosLivro;
    struct NoLista* proximo; // Ponteiro para o próximo nó
} NoLista;

typedef struct {
    NoLista* inicio;        // Ponteiro para o primeiro nó
    int quantidade;
} ColecaoLivros;