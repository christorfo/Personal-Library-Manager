#ifndef PILHA_HISTORICO_H
#define PILHA_HISTORICO_H

#define TAM_ISBN 14

// Nó da pilha (se implementação encadeada)
typedef struct NoPilha {
    char isbn_consultado[TAM_ISBN]; // Armazena o ISBN
    struct NoPilha* proximo;        // Ponteiro (se encadeada) [cite: 1]
} NoPilha;

// Estrutura da Pilha (Estrutura Complexa - Pilhas) [cite: 1]
typedef struct {
    NoPilha* topo;
    // Se for implementação com vetor:
    // char itens[MAX_PILHA][TAM_ISBN];
    // int topo_idx;
} PilhaHistorico;

// Protótipos
PilhaHistorico* criar_pilha_historico();
void push_historico(PilhaHistorico* pilha, const char* isbn);
char* pop_historico(PilhaHistorico* pilha); // Retorna o ISBN ou NULL se vazia
char* peek_historico(const PilhaHistorico* pilha); // Retorna o ISBN do topo ou NULL
int pilha_historico_vazia(const PilhaHistorico* pilha);
void destruir_pilha_historico(PilhaHistorico* pilha);

#endif // PILHA_HISTORICO_H