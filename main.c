#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para manipulação de strings
#include <errno.h>  // Para strtol error checking

// Nossos cabeçalhos de módulo
#include "livro.h"
#include "lista_livros.h"
#include "pilha_historico.h"
#include "fila_desejos.h"
#include "arquivos.h"
#include "pesquisa_ordenacao.h"

/**
 * @brief Exibe o menu principal de opções para o usuário.
 */
void exibir_menu() {
    printf("\n--- Biblioteca Pessoal 📖 ---\n");
    printf("1. Adicionar Novo Livro\n");
    printf("2. Remover Livro (por ISBN)\n");
    printf("3. Listar Todos os Livros\n");
    printf("4. Buscar Livro por ISBN\n");
    printf("5. Buscar Livro por Titulo\n");
    printf("6. Ordenar Colecao por Titulo\n");    // Menu atualizado
    printf("7. Ordenar Colecao por Ano\n");       // Menu atualizado
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

/**
 * @brief Lê uma string do teclado de forma segura usando fgets.
 * Remove o caractere de nova linha do final, se presente.
 * Se a entrada for maior que o buffer, o restante da linha é consumido.
 * @param destino Ponteiro para o buffer onde a string será armazenada.
 * @param tamanho Tamanho do buffer de destino.
 */
void ler_string_segura(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            // Limpar o restante do buffer de entrada se a linha foi muito longa
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } else {
        destino[0] = '\0'; // Em caso de erro no fgets, string vazia
    }
}

/**
 * @brief Lê um inteiro do teclado de forma mais robusta.
 * @param prompt A mensagem a ser exibida para o usuário.
 * @param sucesso Ponteiro para um inteiro que será 1 se a leitura for bem-sucedida, 0 caso contrário.
 * @return O valor inteiro lido, ou 0 em caso de falha na conversão.
 */
int ler_inteiro_seguro(const char* prompt, int* sucesso) {
    char buffer[32];
    long valor;
    char* endptr;

    if (prompt) printf("%s", prompt);
    ler_string_segura(buffer, sizeof(buffer));

    errno = 0; // Resetar errno antes da chamada a strtol
    valor = strtol(buffer, &endptr, 10);

    if (endptr == buffer || *endptr != '\0' || errno == ERANGE) {
        // Nenhuma conversão, caracteres extras após o número, ou valor fora do range de long
        if (sucesso) *sucesso = 0;
        return 0; // Ou algum valor de erro específico
    }

    if (sucesso) *sucesso = 1;
    return (int)valor; // Cast para int (cuidado com overflow se long for muito grande)
}


/**
 * @brief Função principal que executa o loop do menu da biblioteca.
 * @return int 0 se o programa encerrar normalmente.
 */
int main() {
    ColecaoLivros* minha_colecao = criar_colecao();
    PilhaHistorico* meu_historico = criar_pilha_historico();
    FilaDesejos* minha_fila_desejos = criar_fila_desejos();

    if (!minha_colecao || !meu_historico || !minha_fila_desejos) {
        fprintf(stderr, "ERRO FATAL: Falha ao alocar estruturas principais. Saindo.\n");
        // Liberar o que foi alocado com sucesso antes de sair
        if (minha_colecao) destruir_colecao(minha_colecao);
        if (meu_historico) destruir_pilha_historico(meu_historico);
        if (minha_fila_desejos) destruir_fila_desejos(minha_fila_desejos);
        return 1;
    }

    // Tenta carregar dados do arquivo binário ao iniciar
    if (carregar_colecao_binario(minha_colecao, "biblioteca.dat")) {
        printf("Dados carregados de biblioteca.dat! 🎉\n");
    } else {
        // Tenta carregar do arquivo de texto se o binário falhar ou não existir
        if (carregar_colecao_texto(minha_colecao, "biblioteca.txt")) {
            printf("Dados carregados de biblioteca.txt! 📄\n");
        } else {
            printf("Nao foi possivel carregar dados existentes ou nenhum arquivo de dados encontrado. Iniciando com colecao vazia.\n");
        }
    }

    int opcao;
    char buffer_isbn[TAM_ISBN];     // Usa TAM_ISBN conforme definido em livro.h ou similar
    char buffer_titulo[TAM_TITULO]; // Usa TAM_TITULO
    Livro livro_temp_op;          // Para operações de adicionar/buscar/desejo

    do {
        exibir_menu();
        int leitura_sucesso;
        opcao = ler_inteiro_seguro("", &leitura_sucesso); // "" pois exibir_menu já imprime "Escolha uma opcao: "
                                                       // getchar() para consumir newline de scanf não é mais necessário
                                                       // se ler_inteiro_seguro usar fgets e consumir a linha.
        if (!leitura_sucesso) {
            printf("Entrada invalida para opcao. Por favor, digite um numero.\n");
            opcao = -1; // Força o loop a continuar e exibir o menu novamente ou tratar como inválido
        }


        switch (opcao) {
            case 1: // Adicionar Novo Livro
                printf("--- Adicionar Novo Livro ---\n");
                if (ler_dados_livro_teclado(&livro_temp_op)) {
                    if (buscar_livro_por_isbn_na_colecao(minha_colecao, livro_temp_op.isbn) != NULL) {
                        printf("ERRO: Ja existe um livro com o ISBN '%s'.\n", livro_temp_op.isbn);
                    } else {
                        if (adicionar_livro_colecao(minha_colecao, livro_temp_op)) {
                            printf("Livro '%s' adicionado com sucesso! 👍\n", livro_temp_op.titulo);
                            if (!push_historico(meu_historico, livro_temp_op.isbn)) {
                                fprintf(stderr, "AVISO: Falha ao adicionar ISBN ao historico.\n");
                            }
                        } else {
                            printf("ERRO: Nao foi possivel adicionar o livro (falha de memoria?).\n");
                        }
                    }
                } else {
                    printf("ERRO: Falha ao ler dados do livro.\n");
                }
                break;

            case 2: // Remover Livro
                printf("Digite o ISBN do livro a remover: ");
                ler_string_segura(buffer_isbn, sizeof(buffer_isbn));
                if (remover_livro_colecao(minha_colecao, buffer_isbn)) {
                    printf("Livro com ISBN '%s' removido com sucesso. 🗑️\n", buffer_isbn);
                } else {
                    printf("Livro com ISBN '%s' nao encontrado para remocao.\n", buffer_isbn);
                }
                break;

            case 3: // Listar Todos os Livros
                listar_todos_livros(minha_colecao);
                break;

            case 4: // Buscar Livro por ISBN
                printf("Digite o ISBN a buscar: ");
                ler_string_segura(buffer_isbn, sizeof(buffer_isbn));
                const Livro* encontrado_isbn = buscar_livro_por_isbn_na_colecao(minha_colecao, buffer_isbn);
                if (encontrado_isbn != NULL) {
                    printf("Livro encontrado: 🔍\n");
                    exibir_livro(encontrado_isbn);
                    if (!push_historico(meu_historico, encontrado_isbn->isbn)) {
                         fprintf(stderr, "AVISO: Falha ao adicionar ISBN ao historico.\n");
                    }
                } else {
                    printf("Livro com ISBN '%s' nao encontrado.\n", buffer_isbn);
                }
                break;

            case 5: // Buscar Livro por Titulo
                printf("Digite parte do Titulo a buscar: ");
                ler_string_segura(buffer_titulo, sizeof(buffer_titulo));
                const Livro* encontrado_titulo = pesquisar_livro_por_titulo(minha_colecao, buffer_titulo); // Assinatura atualizada
                if (encontrado_titulo != NULL) {
                    printf("Primeiro livro encontrado contendo '%s' no titulo: 🔍\n", buffer_titulo);
                    exibir_livro(encontrado_titulo);
                    if (!push_historico(meu_historico, encontrado_titulo->isbn)) {
                        fprintf(stderr, "AVISO: Falha ao adicionar ISBN ao historico.\n");
                    }
                } else {
                    printf("Nenhum livro encontrado com o titulo contendo '%s'.\n", buffer_titulo);
                }
                break;

            case 6: // Ordenar por Título
                if (tamanho_colecao(minha_colecao) > 0) {
                    ordenar_colecao_por_titulo(minha_colecao); // Agora modifica a coleção
                    printf("Colecao ordenada por titulo. Use a opcao 3 para listar.\n");
                } else {
                    printf("Colecao vazia, nada para ordenar.\n");
                }
                break;

            case 7: // Ordenar por Ano
                 if (tamanho_colecao(minha_colecao) > 0) {
                    ordenar_colecao_por_ano(minha_colecao); // Agora modifica a coleção
                    printf("Colecao ordenada por ano. Use a opcao 3 para listar.\n");
                } else {
                    printf("Colecao vazia, nada para ordenar.\n");
                }
                break;

            case 8: // Adicionar à Lista de Desejos
                printf("--- Adicionar Livro a Lista de Desejos ❤️ ---\n");
                 if (ler_dados_livro_teclado(&livro_temp_op)) { // Usa a função centralizada
                    enqueue_desejo(minha_fila_desejos, livro_temp_op); // Assume que enqueue não falha por memória aqui (ou deveria retornar status)
                    printf("Livro '%s' adicionado a lista de desejos.\n", livro_temp_op.titulo);
                } else {
                    printf("ERRO: Falha ao ler dados do livro para lista de desejos.\n");
                }
                break;

            case 9: // Ver Próximo da Lista de Desejos
                if (fila_desejos_vazia(minha_fila_desejos)) {
                    printf("Lista de desejos esta vazia. 텅\n");
                } else {
                    // Usa a assinatura atualizada de dequeue_desejo
                    if (dequeue_desejo(minha_fila_desejos, &livro_temp_op)) {
                        printf("Proximo livro da lista de desejos (removido):\n");
                        exibir_livro(&livro_temp_op);
                    } else {
                        // Esta situação não deveria ocorrer se fila_desejos_vazia() foi falsa,
                        // a menos que haja um problema interno em dequeue_desejo.
                        printf("Nao foi possivel obter o proximo livro da lista de desejos.\n");
                    }
                }
                break;

            case 10: // Ver Topo do Histórico
                { // Bloco para variável local
                    const char* isbn_topo = peek_historico(meu_historico); // Assinatura atualizada
                    if (isbn_topo != NULL) {
                        printf("Ultimo ISBN consultado/adicionado no historico: %s 📜\n", isbn_topo);
                        // Não há 'free' pois peek_historico retorna ponteiro interno constante
                    } else {
                        printf("Historico de consultas vazio.\n");
                    }
                }
                break;

            case 11: // Salvar em Texto
                if (salvar_colecao_texto(minha_colecao, "biblioteca.txt")) {
                    printf("Colecao salva em biblioteca.txt ✅\n");
                } else {
                    printf("ERRO ao salvar colecao em biblioteca.txt ❌\n");
                }
                break;

            case 12: // Carregar de Texto
                printf("Carregando de arquivo texto. A colecao atual sera incrementada.\n");
                printf("Para substituir, primeiro limpe a colecao (opcao futura?) ou apague os arquivos de dados e reinicie.\n");
                if (carregar_colecao_texto(minha_colecao, "biblioteca.txt")) {
                    printf("Colecao carregada/incrementada de biblioteca.txt ✅\n");
                } else {
                    printf("ERRO ou arquivo biblioteca.txt nao encontrado. 📄\n");
                }
                break;

            case 13: // Salvar em Binário
                if (salvar_colecao_binario(minha_colecao, "biblioteca.dat")) {
                    printf("Colecao salva em biblioteca.dat ✅\n");
                } else {
                    printf("ERRO ao salvar colecao em biblioteca.dat ❌\n");
                }
                break;

            case 14: // Carregar de Binário
                printf("Carregando de arquivo binario. A colecao atual sera incrementada.\n");
                printf("Para substituir, primeiro limpe a colecao (opcao futura?) ou apague os arquivos de dados e reinicie.\n");
                if (carregar_colecao_binario(minha_colecao, "biblioteca.dat")) {
                    printf("Colecao carregada/incrementada de biblioteca.dat ✅\n");
                } else {
                    printf("ERRO ou arquivo biblioteca.dat nao encontrado. 💾\n");
                }
                break;

            case 0:
                printf("Salvando dados antes de sair...\n");
                if (!salvar_colecao_binario(minha_colecao, "biblioteca.dat")){ // Tenta salvar em binário por padrão
                    printf("AVISO: Falha ao salvar em binario. Tentando salvar em texto...\n");
                    if (!salvar_colecao_texto(minha_colecao, "biblioteca.txt")) { // Fallback para texto
                        printf("ERRO: Falha ao salvar em texto tambem. Dados podem ter sido perdidos. ❌\n");
                    } else {
                        printf("Dados salvos em biblioteca.txt como fallback. ✅\n");
                    }
                } else {
                     printf("Dados salvos em biblioteca.dat. ✅\n");
                }
                printf("Saindo... Até logo! 👋\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente. 🚫\n");
        }
        if (opcao != 0 && leitura_sucesso) { // Apenas pausa se uma opção válida foi processada
            printf("\nPressione Enter para continuar...");
            // Consumir qualquer entrada restante e esperar pelo Enter
            int ch;
            // Primeiro, limpa o que pode ter sobrado da leitura da opção se ler_inteiro_seguro não consumiu tudo
            // (embora ler_string_segura dentro de ler_inteiro_seguro deva ter feito isso).
            // Em seguida, espera por um novo Enter.
            while ((ch = getchar()) != '\n' && ch != EOF); // Limpa o buffer até o newline da opção, se houver
            // getchar(); // Espera pelo Enter do usuário
        }
    } while (opcao != 0);

    // Liberar toda a memória alocada
    destruir_colecao(minha_colecao);
    destruir_pilha_historico(meu_historico);
    destruir_fila_desejos(minha_fila_desejos);

    printf("Memoria liberada. Programa encerrado.\n");
    return 0;
}