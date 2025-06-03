#ifndef LIVRO_H
#define LIVRO_H

// Definição da struct Livro (Estrutura de Dados Simples Heterogênea) [cite: 1]
typedef struct {
    char titulo[100];       // String (Vetor de char) [cite: 1]
    char autor[100];        // String (Vetor de char) [cite: 1]
    int anoPublicacao;
    char isbn[14];          // String (Vetor de char), usado como identificador único [cite: 1]
    char genero[50];        // String (Vetor de char) [cite: 1]
    // Você pode adicionar mais campos se desejar, ex: editora, num_paginas
} Livro;

// Protótipos das funções
Livro* criar_livro(const char* titulo, const char* autor, int ano, const char* isbn, const char* genero);
void exibir_livro(const Livro* livro);
// Funcao para ler os dados de um livro do teclado (pode ser útil)
// Livro ler_dados_livro_teclado();

#endif // LIVRO_H