#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para manipulação de strings

#include "livro.h"
#include "lista_livros.h"
#include "pilha_historico.h"
#include "fila_desejos.h"
#include "arquivos.h"
#include "pesquisa_ordenacao.h"

void exibir_menu() {
    printf("\n--- Biblioteca Pessoal ---\n");
    printf("1. Adicionar Novo Livro\n");
    printf("2. Remover Livro (por ISBN)\n");
    printf("3. Listar Todos os Livros\n");
    printf("4. Buscar Livro por ISBN\n");
    printf("5. Buscar Livro por Titulo\n");
    printf("6. Ordenar Livros por Titulo (Exibe ordenado)\n");
    printf("7. Ordenar Livros por Ano (Exibe ordenado)\n");
    printf("8. Adicionar Livro a Lista de Desejos\n");
    printf("9. Ver Proximo Livro da Lista de Desejos (e remover)\n");
    printf("10. Ver Topo do Historico de Consultas (ISBN)\n");
    printf("11. Salvar Colecao em Arquivo Texto\n");
    printf("12. Carregar Colecao de Arquivo Texto\n");
    printf("13. Salvar Colecao em Arquivo Binario\n");
    printf("14. Carregar Colecao de Arquivo Binario\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função auxiliar para ler string do teclado de forma segura
void ler_string(char* destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = 0; // Remove a quebra de linha
}


int main() {
    ColecaoLivros* minha_colecao = criar_colecao();
    PilhaHistorico* meu_historico = criar_pilha_historico();
    FilaDesejos* minha_fila_desejos = criar_fila_desejos();

    // Tenta carregar dados do arquivo binário ao iniciar
    if (carregar_colecao_binario(minha_colecao, "biblioteca.dat")) {
        printf("Dados carregados de biblioteca.dat!\n");
    } else {
        // Tenta carregar do arquivo de texto se o binário falhar ou não existir
        if (carregar_colecao_texto(minha_colecao, "biblioteca.txt")) {
            printf("Dados carregados de biblioteca.txt!\n");
        } else {
            printf("Nao foi possivel carregar dados existentes ou nenhum arquivo de dados encontrado.\n");
        }
    }


    int opcao;
    char buffer_isbn[TAM_ISBN + 2]; // +2 para newline e null terminator
    char buffer_titulo[102];
    char buffer_autor[102];
    char buffer_genero[52];
    char buffer_ano[10];
    int ano_int;

    Livro livro_temp_op; // Para operações de adicionar/buscar

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpar buffer de entrada
            opcao = -1; 
        }
        getchar(); // Consumir o newline deixado pelo scanf

        switch (opcao) {
            case 1: // Adicionar Novo Livro
                printf("--- Adicionar Novo Livro ---\n");
                printf("Titulo: "); ler_string(buffer_titulo, sizeof(buffer_titulo));
                printf("Autor: "); ler_string(buffer_autor, sizeof(buffer_autor));
                printf("Ano de Publicacao: "); ler_string(buffer_ano, sizeof(buffer_ano)); ano_int = atoi(buffer_ano);
                printf("ISBN: "); ler_string(buffer_isbn, sizeof(buffer_isbn));
                printf("Genero: "); ler_string(buffer_genero, sizeof(buffer_genero));

                // Verifica se já existe livro com este ISBN
                if (buscar_livro_por_isbn_na_colecao(minha_colecao, buffer_isbn) != NULL) {
                    printf("Erro: Ja existe um livro com o ISBN '%s'.\n", buffer_isbn);
                } else {
                    // Cria uma struct Livro temporária com os dados lidos
                    // Não precisa de malloc aqui, pois será copiado para dentro do nó da lista
                    strncpy(livro_temp_op.titulo, buffer_titulo, sizeof(livro_temp_op.titulo)-1);
                    livro_temp_op.titulo[sizeof(livro_temp_op.titulo)-1] = '\0';
                    strncpy(livro_temp_op.autor, buffer_autor, sizeof(livro_temp_op.autor)-1);
                    livro_temp_op.autor[sizeof(livro_temp_op.autor)-1] = '\0';
                    livro_temp_op.anoPublicacao = ano_int;
                    strncpy(livro_temp_op.isbn, buffer_isbn, sizeof(livro_temp_op.isbn)-1);
                    livro_temp_op.isbn[sizeof(livro_temp_op.isbn)-1] = '\0';
                    strncpy(livro_temp_op.genero, buffer_genero, sizeof(livro_temp_op.genero)-1);
                    livro_temp_op.genero[sizeof(livro_temp_op.genero)-1] = '\0';
                    
                    adicionar_livro_colecao(minha_colecao, livro_temp_op);
                    printf("Livro '%s' adicionado com sucesso!\n", livro_temp_op.titulo);
                    push_historico(meu_historico, livro_temp_op.isbn); // Adiciona ao histórico
                }
                break;
            case 2: // Remover Livro
                printf("Digite o ISBN do livro a remover: ");
                ler_string(buffer_isbn, sizeof(buffer_isbn));
                if (remover_livro_colecao(minha_colecao, buffer_isbn)) {
                    printf("Livro com ISBN '%s' removido com sucesso.\n", buffer_isbn);
                } else {
                    printf("Livro com ISBN '%s' nao encontrado para remocao.\n", buffer_isbn);
                }
                break;
            case 3: // Listar Todos os Livros
                listar_todos_livros(minha_colecao);
                break;
            case 4: // Buscar Livro por ISBN
                printf("Digite o ISBN a buscar: ");
                ler_string(buffer_isbn, sizeof(buffer_isbn));
                Livro* encontrado_isbn = buscar_livro_por_isbn_na_colecao(minha_colecao, buffer_isbn);
                if (encontrado_isbn != NULL) {
                    printf("Livro encontrado:\n");
                    exibir_livro(encontrado_isbn);
                    push_historico(meu_historico, encontrado_isbn->isbn);
                } else {
                    printf("Livro com ISBN '%s' nao encontrado.\n", buffer_isbn);
                }
                break;
            case 5: // Buscar Livro por Titulo
                 printf("Digite parte do Titulo a buscar: ");
                 ler_string(buffer_titulo, sizeof(buffer_titulo));
                 NoLista* no_encontrado_titulo = pesquisar_livro_por_titulo(minha_colecao, buffer_titulo);
                 if (no_encontrado_titulo != NULL) {
                     printf("Primeiro livro encontrado contendo '%s' no titulo:\n", buffer_titulo);
                     exibir_livro(&(no_encontrado_titulo->dadosLivro));
                     push_historico(meu_historico, no_encontrado_titulo->dadosLivro.isbn);
                     // TODO: Adicionar lógica para encontrar próximos, se desejar.
                 } else {
                     printf("Nenhum livro encontrado com o titulo contendo '%s'.\n", buffer_titulo);
                 }
                break;
            case 6: // Ordenar por Título
                ordenar_colecao_por_titulo(minha_colecao);
                // A função de ordenação agora apenas exibe, não modifica a lista original.
                break;
            case 7: // Ordenar por Ano
                ordenar_colecao_por_ano(minha_colecao);
                // A função de ordenação agora apenas exibe, não modifica a lista original.
                break;
            case 8: // Adicionar à Lista de Desejos
                printf("--- Adicionar Livro a Lista de Desejos ---\n");
                printf("Titulo: "); ler_string(buffer_titulo, sizeof(buffer_titulo));
                printf("Autor: "); ler_string(buffer_autor, sizeof(buffer_autor));
                printf("Ano de Publicacao: "); ler_string(buffer_ano, sizeof(buffer_ano)); ano_int = atoi(buffer_ano);
                printf("ISBN (opcional, mas util para referencia): "); ler_string(buffer_isbn, sizeof(buffer_isbn));
                printf("Genero: "); ler_string(buffer_genero, sizeof(buffer_genero));
                
                strncpy(livro_temp_op.titulo, buffer_titulo, sizeof(livro_temp_op.titulo)-1);
                livro_temp_op.titulo[sizeof(livro_temp_op.titulo)-1] = '\0';
                strncpy(livro_temp_op.autor, buffer_autor, sizeof(livro_temp_op.autor)-1);
                livro_temp_op.autor[sizeof(livro_temp_op.autor)-1] = '\0';
                livro_temp_op.anoPublicacao = ano_int;
                strncpy(livro_temp_op.isbn, buffer_isbn, sizeof(livro_temp_op.isbn)-1);
                livro_temp_op.isbn[sizeof(livro_temp_op.isbn)-1] = '\0';
                strncpy(livro_temp_op.genero, buffer_genero, sizeof(livro_temp_op.genero)-1);
                livro_temp_op.genero[sizeof(livro_temp_op.genero)-1] = '\0';

                enqueue_desejo(minha_fila_desejos, livro_temp_op);
                printf("Livro '%s' adicionado a lista de desejos.\n", livro_temp_op.titulo);
                break;
            case 9: // Ver Próximo da Lista de Desejos
                if (fila_desejos_vazia(minha_fila_desejos)) {
                    printf("Lista de desejos esta vazia.\n");
                } else {
                    livro_temp_op = dequeue_desejo(minha_fila_desejos);
                    // A função dequeue_desejo agora retorna um livro "VAZIO" no ISBN se a fila estava vazia
                    // no momento da chamada, mas já verificamos com fila_desejos_vazia antes.
                    printf("Proximo livro da lista de desejos (removido):\n");
                    exibir_livro(&livro_temp_op);
                }
                break;
            case 10: // Ver Topo do Histórico
                {
                    char* isbn_topo = peek_historico(meu_historico);
                    if (isbn_topo != NULL) {
                        printf("Ultimo ISBN consultado/adicionado no historico: %s\n", isbn_topo);
                        // Não precisamos dar free em isbn_topo pois peek_historico retorna ponteiro interno
                        // (ou deveria ser uma cópia se quiséssemos mais segurança e o chamador liberasse).
                        // A implementação atual de peek_historico retorna ponteiro interno, o que é ok para visualização rápida.
                    } else {
                        printf("Historico de consultas vazio.\n");
                    }
                }
                break;
            case 11: // Salvar em Texto
                if (salvar_colecao_texto(minha_colecao, "biblioteca.txt")) {
                    printf("Colecao salva em biblioteca.txt\n");
                } else {
                    printf("Erro ao salvar colecao em biblioteca.txt\n");
                }
                break;
            case 12: // Carregar de Texto
                // Lógica de limpar coleção antes de carregar:
                // destruir_colecao(minha_colecao);
                // minha_colecao = criar_colecao(); // Recria a coleção vazia
                printf("Carregando de arquivo texto. A colecao atual sera incrementada.\n");
                printf("Para substituir, apague os arquivos .dat e .txt e reinicie, ou implemente a limpeza aqui.\n");
                if (carregar_colecao_texto(minha_colecao, "biblioteca.txt")) {
                    printf("Colecao carregada de biblioteca.txt\n");
                } else {
                    printf("Erro ou arquivo biblioteca.txt nao encontrado.\n");
                }
                break;
            case 13: // Salvar em Binário
                if (salvar_colecao_binario(minha_colecao, "biblioteca.dat")) {
                    printf("Colecao salva em biblioteca.dat\n");
                } else {
                    printf("Erro ao salvar colecao em biblioteca.dat\n");
                }
                break;
            case 14: // Carregar de Binário
                // destruir_colecao(minha_colecao);
                // minha_colecao = criar_colecao();
                printf("Carregando de arquivo binario. A colecao atual sera incrementada.\n");
                printf("Para substituir, apague os arquivos .dat e .txt e reinicie, ou implemente a limpeza aqui.\n");
                if (carregar_colecao_binario(minha_colecao, "biblioteca.dat")) {
                    printf("Colecao carregada de biblioteca.dat\n");
                } else {
                    printf("Erro ou arquivo biblioteca.dat nao encontrado.\n");
                }
                break;
            case 0:
                printf("Salvando dados antes de sair...\n");
                if (!salvar_colecao_binario(minha_colecao, "biblioteca.dat")){ // Tenta salvar em binário por padrão
                     salvar_colecao_texto(minha_colecao, "biblioteca.txt"); // Fallback para texto
                }
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            // getchar(); // Já tem um getchar() após o scanf da opção. Se precisar de mais uma pausa, descomente.
        }
    } while (opcao != 0);

    // Liberar toda a memória alocada
    destruir_colecao(minha_colecao);
    destruir_pilha_historico(meu_historico);
    destruir_fila_desejos(minha_fila_desejos);

    printf("Memoria liberada. Programa encerrado.\n");
    return 0;
}