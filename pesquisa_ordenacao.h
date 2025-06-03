#ifndef PESQUISA_ORDENACAO_H
#define PESQUISA_ORDENACAO_H

#include "lista_livros.h"

// Métodos de Pesquisa [cite: 2]
// A busca por ISBN já está em lista_livros.h, mas pode adicionar outras aqui
NoLista* pesquisar_livro_por_titulo(const ColecaoLivros* colecao, const char* titulo_busca);
// Poderia ter uma função que retorna uma lista de resultados se vários livros corresponderem

// Métodos de Classificação (Ordenação) [cite: 2]
// Estas funções podem modificar a lista diretamente ou criar uma cópia ordenada
// Adaptar algoritmos como Bubble Sort, Selection Sort, Insertion Sort para lista encadeada
// ou copiar para um vetor de ponteiros Livro*, ordenar o vetor, e reconstruir/exibir a lista.
void ordenar_colecao_por_titulo(ColecaoLivros* colecao);
void ordenar_colecao_por_ano(ColecaoLivros* colecao);
// void ordenar_colecao_por_autor(ColecaoLivros* colecao);


#endif // PESQUISA_ORDENACAO_H