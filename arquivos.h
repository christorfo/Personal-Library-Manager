#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "lista_livros.h" // Precisa da definição de ColecaoLivros

// Manipulação de Arquivos [cite: 1, 3]
int salvar_colecao_texto(const ColecaoLivros* colecao, const char* nome_arquivo);
int carregar_colecao_texto(ColecaoLivros* colecao, const char* nome_arquivo);

int salvar_colecao_binario(const ColecaoLivros* colecao, const char* nome_arquivo);
int carregar_colecao_binario(ColecaoLivros* colecao, const char* nome_arquivo);

#endif // ARQUIVOS_H