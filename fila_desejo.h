#ifndef FILA_DESEJOS_H
#define FILA_DESEJOS_H

#include "livro.h" // Para poder enfileirar um Livro (ou apenas o ISBN)

// Nó da fila (se implementação encadeada)
typedef struct NoFila {
    Livro livro_desejado; // Ou apenas o ISBN para economizar memória e buscar na coleção principal
    struct NoFila* proximo; // Ponteiro (se encadeada) [cite: 1]
} NoFila;

// Estrutura da Fila (Estrutura Complexa - Filas) [cite: 1]
typedef struct {
    NoFila* inicio;
    NoFila* fim;
    // Se for implementação com vetor circular:
    // Livro itens[MAX_FILA];
    // int inicio_idx;
    // int fim_idx;
    // int contador;
} FilaDesejos;

// Protótipos
FilaDesejos* criar_fila_desejos();
void enqueue_desejo(FilaDesejos* fila, Livro livro);
Livro dequeue_desejo(FilaDesejos* fila); // Retorna o Livro (pode precisar de um sinalizador para fila vazia)
Livro front_desejo(const FilaDesejos* fila);  // Retorna o Livro do início (sinalizar se vazia)
int fila_desejos_vazia(const FilaDesejos* fila);
void destruir_fila_desejos(FilaDesejos* fila);

#endif // FILA_DESEJOS_H