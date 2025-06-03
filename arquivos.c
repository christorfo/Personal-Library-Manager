#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "livro.h" // Para struct Livro

// --- ESQUELETOS DETALHADOS (VOCÊ IMPLEMENTA) ---

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
        //         Exemplo de formato (CSV - Comma Separated Values):
        //         "Titulo","Autor",Ano,"ISBN","Genero"\n
        //         Use fprintf. Cuidado com títulos/autores que podem ter vírgulas.
        //         Uma forma mais segura para strings é usar aspas ou outro delimitador menos comum.
        //         Para simplicidade, vamos assumir que não têm caracteres problemáticos ou usar aspas.
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
     // Lógica: Considerar limpar a coleção atual antes de carregar novos dados,
     //         ou adicionar aos existentes. Para este esqueleto, vamos assumir que
     //         a coleção está vazia ou que os novos livros serão adicionados.
     //         Se for para substituir: destruir_colecao(colecao); colecao = criar_colecao();
     //         Mas como 'colecao' é passado por valor (ponteiro), isso não recriaria no chamador.
     //         O chamador teria que cuidar disso, ou esta função teria que limpar os nós existentes.
     //         Para simplificar o esqueleto: apenas adiciona.

    // Lógica: Abrir o arquivo em modo de leitura ("r").
    FILE* arquivo = fopen(nome_arquivo, "r");
    // Lógica: Verificar se o arquivo foi aberto com sucesso.
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para leitura (pode nao existir)");
        return 0; // Falha (arquivo pode não existir, o que não é necessariamente um erro fatal)
    }

    Livro livro_temp;
    char linha[512]; // Buffer para ler uma linha inteira

    // Lógica: Ler o arquivo linha por linha.
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Lógica: Para cada linha, parsear os dados para preencher uma struct Livro temporária.
        //         Usar sscanf para parsear a linha de acordo com o formato usado em salvar_colecao_texto.
        //         Ex: sscanf(linha, "\"%[^\"]\",\"%[^\"]\",%d,\"%[^\"]\",\"%[^\"]\"", ...);
        //         %[^\"] lê até encontrar uma aspa.
        //         Cuidado com o buffer overflow nos campos de livro_temp.
        int campos_lidos = sscanf(linha, "\"%99[^\"]\",\"%99[^\"]\",%d,\"%13[^\"]\",\"%49[^\"]\"",
               livro_temp.titulo,
               livro_temp.autor,
               &livro_temp.anoPublicacao, // sscanf precisa do endereço para int
               livro_temp.isbn,
               livro_temp.genero);
        
        if (campos_lidos == 5) { // Verifica se todos os 5 campos foram lidos corretamente
            // Lógica: Adicionar o livro_temp à coleção usando adicionar_livro_colecao.
            adicionar_livro_colecao(colecao, livro_temp);
        } else {
            //fprintf(stderr, "Aviso: Linha mal formatada no arquivo: %s", linha);
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

    // Lógica: Primeiro, pode ser útil salvar a quantidade de livros.
    // fwrite(&(colecao->quantidade), sizeof(int), 1, arquivo);

    NoLista* atual = colecao->inicio;
    // Lógica: Percorrer a lista.
    while (atual != NULL) {
        // Lógica: Para cada livro, escrever a struct Livro inteira no arquivo usando fwrite.
        // size_t itens_escritos = fwrite(&(atual->dadosLivro), sizeof(Livro), 1, arquivo);
        // if (itens_escritos != 1) {
        //     perror("Erro ao escrever livro em arquivo binario");
        //     fclose(arquivo);
        //     return 0; // Falha na escrita
        // }
        // TODO: Implementar a escrita do struct Livro.
         if (fwrite(&(atual->dadosLivro), sizeof(Livro), 1, arquivo) != 1) {
            perror("Erro ao escrever livro em arquivo binario");
            fclose(arquivo);
            return 0;
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
    // Lógica: Mesma consideração sobre limpar a coleção atual que em carregar_colecao_texto.

    // Lógica: Abrir o arquivo em modo de leitura binária ("rb").
    FILE* arquivo = fopen(nome_arquivo, "rb");
    // Lógica: Verificar abertura.
    if (arquivo == NULL) {
        // perror("Erro ao abrir arquivo binario para leitura (pode nao existir)");
        // Não é um erro fatal se o arquivo não existe na primeira vez.
        return 0; 
    }

    // Lógica: Se você salvou a quantidade de livros, leia-a primeiro.
    // int quantidade_a_ler;
    // if (fread(&quantidade_a_ler, sizeof(int), 1, arquivo) != 1) { ... }

    Livro livro_temp;
    // Lógica: Ler os livros do arquivo um por um usando fread, até o fim do arquivo.
    //         fread retorna o número de itens lidos com sucesso.
    // while (fread(&livro_temp, sizeof(Livro), 1, arquivo) == 1) {
    //     adicionar_livro_colecao(colecao, livro_temp);
    // }
    // TODO: Implementar a leitura do struct Livro e adicionar à coleção.
    while (fread(&livro_temp, sizeof(Livro), 1, arquivo) == 1) {
        adicionar_livro_colecao(colecao, livro_temp);
    }


    // Lógica: Fechar o arquivo.
    fclose(arquivo);
    return 1; // Sucesso
}