#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "livro.h" // Para struct Livro, NoLista, ColecaoLivros, adicionar_livro_colecao

// --- FUNÇÕES IMPLEMENTADAS ---

int salvar_colecao_texto(const ColecaoLivros* colecao, const char* nome_arquivo) {
    // Lógica: Verificar se a coleção ou o nome do arquivo são NULL.
    if (colecao == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Erro: Colecao ou nome de arquivo nulos para salvar.\n");
        return 0; // Falha
    }

    // Lógica: Abrir o arquivo em modo de escrita ("w").
    FILE* arquivo = fopen(nome_arquivo, "w");
    // Lógica: Verificar se o arquivo foi aberto com sucesso.
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para escrita");
        return 0; // Falha
    }

    NoLista* atual = colecao->inicio;
    // Lógica: Percorrer a lista de livros da coleção.
    while (atual != NULL) {
        // Lógica: Para cada livro, escrever seus dados formatados no arquivo.
        // Formato CSV: "Titulo","Autor",Ano,"ISBN","Genero"\n
        fprintf(arquivo, "\"%s\",\"%s\",%d,\"%s\",\"%s\"\n",
                atual->dadosLivro.titulo,
                atual->dadosLivro.autor,
                atual->dadosLivro.anoPublicacao,
                atual->dadosLivro.isbn,
                atual->dadosLivro.genero);
        atual = atual->proximo;
    }

    // Lógica: Fechar o arquivo.
    fclose(arquivo);
    return 1; // Sucesso
}

int carregar_colecao_texto(ColecaoLivros* colecao, const char* nome_arquivo) {
    // Lógica: Verificar se a coleção ou o nome do arquivo são NULL.
    if (colecao == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Erro: Colecao ou nome de arquivo nulos para carregar.\n");
        return 0; // Falha
    }
    // Nota: Considerar limpar a coleção atual antes de carregar.
    // Para este exemplo, apenas adiciona os livros à coleção existente.

    // Lógica: Abrir o arquivo em modo de leitura ("r").
    FILE* arquivo = fopen(nome_arquivo, "r");
    // Lógica: Verificar se o arquivo foi aberto com sucesso.
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para leitura (pode nao existir)");
        return 0; // Falha (arquivo pode não existir, tratado como coleção vazia)
    }

    Livro livro_temp;
    char linha[512]; // Buffer para ler uma linha inteira

    // Lógica: Ler o arquivo linha por linha.
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Lógica: Para cada linha, parsear os dados para preencher uma struct Livro temporária.
        // Usar sscanf para parsear a linha de acordo com o formato usado em salvar_colecao_texto.
        // %[^\"] lê até encontrar uma aspa.
        // Os números (99, 99, 13, 49) são para prevenir buffer overflow nos campos de livro_temp,
        // assumindo que os campos string na struct Livro têm tamanho 100, 100, 14, 50 respectivamente.
        int campos_lidos = sscanf(linha, "\"%99[^\"]\",\"%99[^\"]\",%d,\"%13[^\"]\",\"%49[^\"]\"",
               livro_temp.titulo,
               livro_temp.autor,
               &livro_temp.anoPublicacao, // sscanf precisa do endereço para int
               livro_temp.isbn,
               livro_temp.genero);
        
        if (campos_lidos == 5) { // Verifica se todos os 5 campos foram lidos corretamente
            // Lógica: Adicionar o livro_temp à coleção usando adicionar_livro_colecao.
            // Assumindo que adicionar_livro_colecao é uma função definida em livro.h/livro.c
            adicionar_livro_colecao(colecao, livro_temp);
        } else {
            //fprintf(stderr, "Aviso: Linha mal formatada ou em branco no arquivo: %s", linha);
            // Pode ser uma linha em branco no final do arquivo, ou um erro de formatação.
        }
    }

    // Lógica: Fechar o arquivo.
    fclose(arquivo);
    return 1; // Sucesso (ou pelo menos tentativa de leitura concluída)
}

int salvar_colecao_binario(const ColecaoLivros* colecao, const char* nome_arquivo) {
    // Lógica: Verificar se a coleção ou o nome do arquivo são NULL.
    if (colecao == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Erro: Colecao ou nome de arquivo nulos para salvar binario.\n");
        return 0;
    }

    // Lógica: Abrir o arquivo em modo de escrita binária ("wb").
    FILE* arquivo = fopen(nome_arquivo, "wb");
    // Lógica: Verificar abertura.
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo binario para escrita");
        return 0;
    }

    // Opcional: Salvar a quantidade de livros primeiro.
    // fwrite(&(colecao->quantidade), sizeof(int), 1, arquivo);

    NoLista* atual = colecao->inicio;
    // Lógica: Percorrer a lista.
    while (atual != NULL) {
        // Lógica: Para cada livro, escrever a struct Livro inteira no arquivo usando fwrite.
        if (fwrite(&(atual->dadosLivro), sizeof(Livro), 1, arquivo) != 1) {
            perror("Erro ao escrever livro em arquivo binario");
            fclose(arquivo);
            return 0; // Falha na escrita
        }
        atual = atual->proximo;
    }

    // Lógica: Fechar o arquivo.
    fclose(arquivo);
    return 1; // Sucesso
}

int carregar_colecao_binario(ColecaoLivros* colecao, const char* nome_arquivo) {
    // Lógica: Verificar se a coleção ou o nome do arquivo são NULL.
    if (colecao == NULL || nome_arquivo == NULL) {
        fprintf(stderr, "Erro: Colecao ou nome de arquivo nulos para carregar binario.\n");
        return 0;
    }
    // Nota: Mesma consideração sobre limpar a coleção atual que em carregar_colecao_texto.

    // Lógica: Abrir o arquivo em modo de leitura binária ("rb").
    FILE* arquivo = fopen(nome_arquivo, "rb");
    // Lógica: Verificar abertura.
    if (arquivo == NULL) {
        // perror("Erro ao abrir arquivo binario para leitura (pode nao existir)");
        // Não é um erro fatal se o arquivo não existe na primeira vez.
        return 0; 
    }

    // Opcional: Se você salvou a quantidade de livros, leia-a primeiro.
    // int quantidade_a_ler;
    // if (fread(&quantidade_a_ler, sizeof(int), 1, arquivo) != 1) { /* erro */ }
    // for (int i = 0; i < quantidade_a_ler; ++i) { /* ler livro */ }

    Livro livro_temp;
    // Lógica: Ler os livros do arquivo um por um usando fread, até o fim do arquivo.
    // fread retorna o número de itens lidos com sucesso.
    while (fread(&livro_temp, sizeof(Livro), 1, arquivo) == 1) {
        // Assumindo que adicionar_livro_colecao é uma função definida em livro.h/livro.c
        adicionar_livro_colecao(colecao, livro_temp);
    }

    // Lógica: Fechar o arquivo.
    fclose(arquivo);
    return 1; // Sucesso
}