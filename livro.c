#include <stdio.h>
#include <stdlib.h> // Para malloc e free
#include <string.h> // Para strcpy, strncpy, strcspn, strlen
#include <ctype.h>  // Para isdigit (se usado para validação mais avançada)
#include "livro.h"  // Assume que define Livro e TAM_TITULO, TAM_AUTOR, TAM_ISBN, TAM_GENERO

// --- FUNÇÕES COMPLETAS E APRIMORADAS ---

/**
 * @brief Cria uma nova instância de Livro e a inicializa com os dados fornecidos.
 * Aloca memória dinamicamente para a struct Livro. O chamador é responsável
 * por liberar essa memória usando destruir_livro().
 *
 * @param titulo Título do livro (não deve ser NULL).
 * @param autor Autor do livro (não deve ser NULL).
 * @param ano Ano de publicação do livro.
 * @param isbn ISBN do livro (identificador único, não deve ser NULL).
 * @param genero Gênero do livro (não deve ser NULL).
 * @return Livro* Um ponteiro para a nova struct Livro alocada, ou NULL se a alocação falhar
 * ou se algum dos parâmetros de string obrigatórios for NULL.
 */
Livro* criar_livro(const char* titulo, const char* autor, int ano, const char* isbn, const char* genero) {
    // Validação básica dos parâmetros de entrada
    if (titulo == NULL || autor == NULL || isbn == NULL || genero == NULL) {
        fprintf(stderr, "ERRO (criar_livro): Parametros de string nao podem ser NULL.\n");
        return NULL;
    }
    // Adicionar mais validações se necessário (ex: ano > 0, strlen(isbn) == X, etc.)

    Livro* novo_livro = (Livro*) malloc(sizeof(Livro));
    if (novo_livro == NULL) {
        perror("ERRO (criar_livro): Falha ao alocar memoria para novo livro");
        return NULL;
    }

    // Copiar dados para os campos da struct, garantindo terminação nula
    strncpy(novo_livro->titulo, titulo, TAM_TITULO - 1);
    novo_livro->titulo[TAM_TITULO - 1] = '\0';

    strncpy(novo_livro->autor, autor, TAM_AUTOR - 1);
    novo_livro->autor[TAM_AUTOR - 1] = '\0';

    novo_livro->anoPublicacao = ano;

    strncpy(novo_livro->isbn, isbn, TAM_ISBN - 1);
    novo_livro->isbn[TAM_ISBN - 1] = '\0';

    strncpy(novo_livro->genero, genero, TAM_GENERO - 1);
    novo_livro->genero[TAM_GENERO - 1] = '\0';

    return novo_livro;
}

/**
 * @brief Libera a memória alocada para uma instância de Livro.
 * Se o ponteiro do livro for NULL, a função não faz nada.
 *
 * @param livro Ponteiro para a struct Livro a ser destruída.
 */
void destruir_livro(Livro* livro) {
    if (livro != NULL) {
        free(livro);
    }
    // Nota: Se a struct Livro contivesse ponteiros para outras memórias alocadas
    // (ex: char* titulo_dinamico = malloc(...)), essas memórias internas
    // precisariam ser liberadas aqui antes de liberar 'livro'.
}

/**
 * @brief Exibe os detalhes de um livro no console.
 *
 * @param livro Ponteiro constante para a struct Livro a ser exibida.
 */
void exibir_livro(const Livro* livro) {
    if (livro == NULL) {
        printf("Nao e possivel exibir um livro NULO.\n");
        return;
    }

    printf("-------------------------\n");
    printf("Titulo: %s\n", livro->titulo);
    printf("Autor: %s\n", livro->autor);
    printf("Ano: %d\n", livro->anoPublicacao);
    printf("ISBN: %s\n", livro->isbn);
    printf("Genero: %s\n", livro->genero);
    printf("-------------------------\n");
}

// Função auxiliar para limpar o buffer de entrada (stdin)
static void limpar_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Lê os dados de um livro a partir da entrada padrão (teclado)
 * e preenche uma struct Livro fornecida.
 *
 * @param livro_destino Ponteiro para a struct Livro que será preenchida com os dados lidos.
 * Não deve ser NULL.
 * @return int 1 se a leitura e o preenchimento foram bem-sucedidos, 0 caso contrário
 * (ex: livro_destino é NULL ou ocorreu um erro de leitura).
 */
int ler_dados_livro_teclado(Livro* livro_destino) {
    if (livro_destino == NULL) {
        fprintf(stderr, "ERRO (ler_dados_livro_teclado): livro_destino nao pode ser NULL.\n");
        return 0;
    }

    char buffer_ano[10]; // Buffer para ler o ano como string

    printf("Digite o titulo (max %d chars): ", TAM_TITULO -1);
    if (fgets(livro_destino->titulo, TAM_TITULO, stdin) == NULL) return 0; // Erro de leitura
    livro_destino->titulo[strcspn(livro_destino->titulo, "\n")] = '\0'; // Remove a quebra de linha
    if (strlen(livro_destino->titulo) == TAM_TITULO -1 && getchar()!='\n') limpar_buffer_stdin();


    printf("Digite o autor (max %d chars): ", TAM_AUTOR -1);
    if (fgets(livro_destino->autor, TAM_AUTOR, stdin) == NULL) return 0;
    livro_destino->autor[strcspn(livro_destino->autor, "\n")] = '\0';
    if (strlen(livro_destino->autor) == TAM_AUTOR -1 && getchar()!='\n') limpar_buffer_stdin();

    printf("Digite o ano de publicacao: ");
    if (fgets(buffer_ano, sizeof(buffer_ano), stdin) == NULL) return 0;
    buffer_ano[strcspn(buffer_ano, "\n")] = '\0';
    if (strlen(buffer_ano) == sizeof(buffer_ano) -1 && getchar()!='\n') limpar_buffer_stdin();
    
    // Validação simples para o ano (deve ser numérico)
    for (int i = 0; buffer_ano[i] != '\0'; i++) {
        if (!isdigit(buffer_ano[i]) && !(i == 0 && buffer_ano[i] == '-')) { // Permite negativo, mas ano geralmente é positivo
            fprintf(stderr, "AVISO: Ano de publicacao invalido. Usando 0.\n");
            livro_destino->anoPublicacao = 0; // Valor padrão em caso de erro
            goto ler_isbn; // Pula para o próximo campo
        }
    }
    livro_destino->anoPublicacao = atoi(buffer_ano);
    if (livro_destino->anoPublicacao == 0 && strcmp(buffer_ano, "0") != 0 && strcmp(buffer_ano, "-0") != 0) {
         fprintf(stderr, "AVISO: Falha ao converter o ano. Usando 0.\n");
         // atoi retorna 0 em falha, o que pode ser um ano válido.
         // Esta é uma verificação simples; para robustez total, use strtol.
    }


ler_isbn:
    printf("Digite o ISBN (max %d chars): ", TAM_ISBN -1);
    if (fgets(livro_destino->isbn, TAM_ISBN, stdin) == NULL) return 0;
    livro_destino->isbn[strcspn(livro_destino->isbn, "\n")] = '\0';
     if (strlen(livro_destino->isbn) == TAM_ISBN -1 && getchar()!='\n') limpar_buffer_stdin();
    
    printf("Digite o genero (max %d chars): ", TAM_GENERO -1);
    if (fgets(livro_destino->genero, TAM_GENERO, stdin) == NULL) return 0;
    livro_destino->genero[strcspn(livro_destino->genero, "\n")] = '\0';
    if (strlen(livro_destino->genero) == TAM_GENERO -1 && getchar()!='\n') limpar_buffer_stdin();

    return 1; // Sucesso
}