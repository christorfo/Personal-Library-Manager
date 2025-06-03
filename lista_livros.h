#ifndef LISTA_LIVROS_H
#define LISTA_LIVROS_H

#include "livro.h" // Para usar o tipo Livro

// Nó da lista de livros
typedef struct NoLista {
    Livro dadosLivro;
    struct NoLista* proximo; // Ponteiro para o próximo nó (Referenciamento à memória - Ponteiros) [cite: 1]
} NoLista;

// Estrutura da coleção de livros (Lista Encadeada)
typedef struct {
    NoLista* inicio;        // Ponteiro para o primeiro nó da lista [cite: 1]
    int quantidade;
} ColecaoLivros;

// Protótipos das funções para manipular a coleção de livros (Estrutura Complexa - Listas) [cite: 1]
ColecaoLivros* criar_colecao();
void adicionar_livro_colecao(ColecaoLivros* colecao, Livro novo_livro);
int remover_livro_colecao(ColecaoLivros* colecao, const char* isbn); // Retorna 1 se sucesso, 0 se não encontrado
void listar_todos_livros(const ColecaoLivros* colecao);
Livro* buscar_livro_por_isbn_na_colecao(const ColecaoLivros* colecao, const char* isbn); // Pesquisa
void destruir_colecao(ColecaoLivros* colecao);

#endif // LISTA_LIVROS_H