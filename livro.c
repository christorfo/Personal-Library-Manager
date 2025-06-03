#include <stdio.h>
#include <stdlib.h> // Para malloc e free
#include <string.h> // Para strcpy
#include "livro.h"

// --- FUNÇÕES COMPLETAS (EXEMPLOS) ---

/**
 * @brief Cria uma nova instância de Livro e a inicializa com os dados fornecidos.
 * * @param titulo Título do livro.
 * @param autor Autor do livro.
 * @param ano Ano de publicação do livro.
 * @param isbn ISBN do livro (identificador único).
 * @param genero Gênero do livro.
 * @return Livro* Um ponteiro para a nova struct Livro alocada, ou NULL se a alocação falhar.
 */
Livro* criar_livro(const char* titulo, const char* autor, int ano, const char* isbn, const char* genero) {
    // Lógica: Alocar memória para a struct Livro.
    Livro* novo_livro = (Livro*) malloc(sizeof(Livro));

    // Lógica: Verificar se a alocação foi bem-sucedida.
    if (novo_livro == NULL) {
        perror("Erro ao alocar memoria para novo livro");
        return NULL; // Retorna NULL em caso de falha na alocação.
    }

    // Lógica: Copiar os dados fornecidos para os campos da struct.
    // Usar strncpy para evitar buffer overflow é uma boa prática.
    strncpy(novo_livro->titulo, titulo, sizeof(novo_livro->titulo) - 1);
    novo_livro->titulo[sizeof(novo_livro->titulo) - 1] = '\0'; // Garante terminação nula

    strncpy(novo_livro->autor, autor, sizeof(novo_livro->autor) - 1);
    novo_livro->autor[sizeof(novo_livro->autor) - 1] = '\0';

    novo_livro->anoPublicacao = ano;

    strncpy(novo_livro->isbn, isbn, sizeof(novo_livro->isbn) - 1);
    novo_livro->isbn[sizeof(novo_livro->isbn) - 1] = '\0';

    strncpy(novo_livro->genero, genero, sizeof(novo_livro->genero) - 1);
    novo_livro->genero[sizeof(novo_livro->genero) - 1] = '\0';

    // Lógica: Retornar o ponteiro para o livro criado.
    return novo_livro;
}

/**
 * @brief Exibe os detalhes de um livro no console.
 * * @param livro Ponteiro para a struct Livro a ser exibida.
 */
void exibir_livro(const Livro* livro) {
    // Lógica: Verificar se o ponteiro do livro não é NULL.
    if (livro == NULL) {
        printf("Nao e possivel exibir um livro NULO.\n");
        return;
    }

    // Lógica: Imprimir cada campo do livro de forma formatada.
    printf("-------------------------\n");
    printf("Titulo: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Ano: %d\n", livro->anoPublicacao);
    printf("ISBN: %s\n", livro->isbn);
    printf("Genero: %s\n", livro->genero);
    printf("-------------------------\n");
}

/*
// Exemplo de como você poderia implementar uma função para ler dados do teclado:
Livro ler_dados_livro_teclado() {
    Livro l;
    char buffer[100]; // Buffer temporário para leitura

    printf("Digite o titulo: ");
    fgets(l.titulo, sizeof(l.titulo), stdin);
    l.titulo[strcspn(l.titulo, "\n")] = 0; // Remove a quebra de linha

    printf("Digite o autor: ");
    fgets(l.autor, sizeof(l.autor), stdin);
    l.autor[strcspn(l.autor, "\n")] = 0;

    printf("Digite o ano de publicacao: ");
    fgets(buffer, sizeof(buffer), stdin);
    l.anoPublicacao = atoi(buffer); // Converte string para int

    printf("Digite o ISBN: ");
    fgets(l.isbn, sizeof(l.isbn), stdin);
    l.isbn[strcspn(l.isbn, "\n")] = 0;
    
    printf("Digite o genero: ");
    fgets(l.genero, sizeof(l.genero), stdin);
    l.genero[strcspn(l.genero, "\n")] = 0;

    return l;
}
*/