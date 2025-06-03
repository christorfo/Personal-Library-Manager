#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "lista_livros.h" // Contém a definição de ColecaoLivros

// Manipulação de Arquivos

/**
 * @brief Salva a coleção de livros em um arquivo de texto.
 *
 * Cada livro na coleção é escrito em uma nova linha no arquivo,
 * geralmente em um formato como CSV (Comma Separated Values),
 * onde os campos do livro (título, autor, ano, ISBN, gênero)
 * são separados por um delimitador e strings são usualmente envoltas em aspas.
 *
 * @param colecao Um ponteiro constante para a struct ColecaoLivros que será salva.
 * A função não modifica a coleção.
 * @param nome_arquivo Uma string constante contendo o nome (e caminho, se necessário)
 * do arquivo onde os dados da coleção serão armazenados.
 * @return Retorna 1 em caso de sucesso (todos os livros salvos corretamente e arquivo fechado).
 * @return Retorna 0 em caso de falha (ex: coleção nula, nome de arquivo nulo,
 * erro ao abrir o arquivo para escrita, erro durante a escrita).
 */
int salvar_colecao_texto(const ColecaoLivros* colecao, const char* nome_arquivo);

/**
 * @brief Carrega uma coleção de livros a partir de um arquivo de texto.
 *
 * Lê os dados dos livros de um arquivo de texto (previamente salvo
 * pela função salvar_colecao_texto ou em formato compatível) e os
 * adiciona à coleção fornecida.
 *
 * @param colecao Um ponteiro para a struct ColecaoLivros onde os livros carregados
 * serão adicionados. A coleção será modificada.
 * @param nome_arquivo Uma string constante contendo o nome (e caminho, se necessário)
 * do arquivo de onde os dados da coleção serão lidos.
 * @return Retorna 1 se o carregamento foi bem-sucedido (mesmo que o arquivo esteja vazio
 * ou não exista, resultando em nenhuma adição à coleção mas sem erro de I/O fatal).
 * @return Retorna 0 em caso de falha crítica (ex: coleção nula, nome de arquivo nulo,
 * erro irrecuperável ao ler o arquivo).
 * Nota: Se o arquivo não existir, a função pode retornar 1 (indicando que tentou carregar
 * de um arquivo vazio) ou 0 dependendo da implementação específica de tratamento de erro
 * (no código C fornecido anteriormente, retorna 0 se `fopen` falha).
 */
int carregar_colecao_texto(ColecaoLivros* colecao, const char* nome_arquivo);

/**
 * @brief Salva a coleção de livros em um arquivo binário.
 *
 * Escreve os dados da coleção de livros diretamente em formato binário.
 * Isso geralmente resulta em arquivos menores e operações de leitura/escrita
 * mais rápidas comparado ao formato de texto, mas o arquivo não será
 * legível por humanos diretamente.
 *
 * @param colecao Um ponteiro constante para a struct ColecaoLivros que será salva.
 * A função não modifica a coleção.
 * @param nome_arquivo Uma string constante contendo o nome (e caminho, se necessário)
 * do arquivo binário onde os dados serão armazenados.
 * @return Retorna 1 em caso de sucesso.
 * @return Retorna 0 em caso de falha (ex: coleção nula, nome de arquivo nulo,
 * erro ao abrir o arquivo, erro durante a escrita).
 */
int salvar_colecao_binario(const ColecaoLivros* colecao, const char* nome_arquivo);

/**
 * @brief Carrega uma coleção de livros a partir de um arquivo binário.
 *
 * Lê os dados dos livros de um arquivo binário (previamente salvo
 * pela função salvar_colecao_binario) e os adiciona à coleção fornecida.
 *
 * @param colecao Um ponteiro para a struct ColecaoLivros onde os livros carregados
 * serão adicionados. A coleção será modificada.
 * @param nome_arquivo Uma string constante contendo o nome (e caminho, se necessário)
 * do arquivo binário de onde os dados serão lidos.
 * @return Retorna 1 se o carregamento foi bem-sucedido (similar ao carregar_colecao_texto,
 * pode incluir o caso de arquivo vazio ou não existente).
 * @return Retorna 0 em caso de falha crítica.
 * Nota: Se o arquivo não existir, a função pode retornar 1 ou 0
 * (no código C fornecido anteriormente, retorna 0 se `fopen` falha).
 */
int carregar_colecao_binario(ColecaoLivros* colecao, const char* nome_arquivo);

#endif // ARQUIVOS_H