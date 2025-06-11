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

// --- Constantes Globais ---
#define ARQUIVO_BINARIO "biblioteca.dat"
#define ARQUIVO_TEXTO "biblioteca.txt"

// --- Protótipos das Funções de Gerenciamento do Menu ---
void limpar_tela();
void pausar_e_continuar();
void exibir_menu_completo();
void gerenciar_adicao_livro(ColecaoLivros* colecao, PilhaHistorico* historico);
void gerenciar_remocao_livro(ColecaoLivros* colecao);
void gerenciar_busca_isbn(const ColecaoLivros* colecao, PilhaHistorico* historico);
void gerenciar_busca_titulo(const ColecaoLivros* colecao, PilhaHistorico* historico);
void gerenciar_adicao_desejo(FilaDesejos* fila);
void gerenciar_processar_desejo(FilaDesejos* fila);
void gerenciar_ver_historico(const PilhaHistorico* historico);
void ler_string_segura(char* destino, int tamanho);


// --- Implementação das Funções Auxiliares ---

void limpar_tela() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pausar_e_continuar() {
    printf("\nPressione Enter para continuar...");
    getchar(); 
}

void exibir_menu_completo() {
    printf("\n--- Biblioteca Pessoal 📖 ---\n");
    printf("1. Adicionar Novo Livro\n");
    printf("2. Remover Livro (por ISBN)\n");
    printf("3. Listar Todos os Livros\n");
    printf("4. Buscar Livro por ISBN\n");
    printf("5. Buscar Livro por Titulo\n");
    printf("6. Ordenar Colecao por Titulo\n");
    printf("7. Ordenar Colecao por Ano\n");
    printf("8. Ordenar Colecao por Autor\n");
    printf("9. Adicionar Livro a Lista de Desejos\n");
    printf("10. Ver Proximo Livro da Lista de Desejos\n");
    printf("11. Ver Topo do Historico de Consultas\n");
    printf("12. Salvar Colecao em Arquivo Texto\n");
    printf("13. Carregar Colecao de Arquivo Texto\n");
    printf("14. Salvar Colecao em Arquivo Binario\n");
    printf("15. Carregar Colecao de Arquivo Binario\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

void ler_string_segura(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        // Remove a quebra de linha do final, se presente.
        destino[strcspn(destino, "\n")] = '\0';
        // Se a entrada foi maior que o buffer, o restante da linha precisa ser limpo.
        // Verificamos se o último caractere lido não foi '\n'.
        size_t len = strlen(destino);
        if (len == tamanho - 1 && destino[len-1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } else {
        destino[0] = '\0'; // Em caso de erro no fgets, string vazia
    }
}

void gerenciar_adicao_livro(ColecaoLivros* colecao, PilhaHistorico* historico) {
    Livro livro_temp; // Cria uma struct Livro temporária na stack
    printf("--- Adicionar Novo Livro ---\n");
    if (ler_dados_livro_teclado(&livro_temp)) { // Usa a função centralizada para ler dados
        // Verifica se já existe livro com este ISBN
        if (buscar_livro_por_isbn_na_colecao(colecao, livro_temp.isbn) != NULL) {
            printf("ERRO: Ja existe um livro com o ISBN '%s'.\n", livro_temp.isbn);
        } else {
            // A função adicionar_livro_colecao cria o nó dinamicamente (malloc)
            // e copia os dados da nossa 'livro_temp'.
            if (adicionar_livro_colecao(colecao, livro_temp)) {
                printf("Livro '%s' adicionado com sucesso! 👍\n", livro_temp.titulo);
                // Adiciona ao histórico a operação bem-sucedida
                push_historico(historico, livro_temp.isbn);
            } else {
                printf("ERRO: Nao foi possivel adicionar o livro (falha de memoria?).\n");
            }
        }
    } else {
        printf("ERRO: Falha ao ler dados do livro.\n");
    }
}

void gerenciar_remocao_livro(ColecaoLivros* colecao) {
    char buffer_isbn[TAM_ISBN];
    printf("Digite o ISBN do livro a remover: ");
    ler_string_segura(buffer_isbn, sizeof(buffer_isbn));

    const Livro* livro_para_remover = buscar_livro_por_isbn_na_colecao(colecao, buffer_isbn);

    if (livro_para_remover != NULL) {
        char confirmacao_buffer[4];
        printf("Voce tem certeza que deseja remover o livro '%s'? (S/N): ", livro_para_remover->titulo);
        ler_string_segura(confirmacao_buffer, sizeof(confirmacao_buffer));

        if (confirmacao_buffer[0] == 'S' || confirmacao_buffer[0] == 's') {
            if (remover_livro_colecao(colecao, buffer_isbn)) {
                printf("Livro com ISBN '%s' removido com sucesso. 🗑️\n", buffer_isbn);
            } else {
                printf("Ocorreu um erro inesperado ao tentar remover o livro.\n");
            }
        } else {
            printf("Remocao cancelada.\n");
        }
    } else {
        printf("Livro com ISBN '%s' nao encontrado para remocao.\n", buffer_isbn);
    }
}

void gerenciar_busca_isbn(const ColecaoLivros* colecao, PilhaHistorico* historico) {
    char buffer_isbn[TAM_ISBN];
    printf("Digite o ISBN a buscar: ");
    ler_string_segura(buffer_isbn, sizeof(buffer_isbn));
    const Livro* encontrado = buscar_livro_por_isbn_na_colecao(colecao, buffer_isbn);
    if (encontrado) {
        printf("Livro encontrado: 🔍\n");
        exibir_livro(encontrado);
        push_historico(historico, encontrado->isbn);
    } else {
        printf("Livro com ISBN '%s' nao encontrado.\n", buffer_isbn);
    }
}

void gerenciar_busca_titulo(const ColecaoLivros* colecao, PilhaHistorico* historico) {
    char buffer_titulo[TAM_TITULO];
    printf("Digite parte do Titulo a buscar: ");
    ler_string_segura(buffer_titulo, sizeof(buffer_titulo));
    const Livro* encontrado = pesquisar_livro_por_titulo(colecao, buffer_titulo);
    if (encontrado) {
        printf("Primeiro livro encontrado: 🔍\n");
        exibir_livro(encontrado);
        push_historico(historico, encontrado->isbn);
    } else {
        printf("Nenhum livro encontrado com o titulo contendo '%s'.\n", buffer_titulo);
    }
}

void gerenciar_adicao_desejo(FilaDesejos* fila) {
    Livro livro_desejo;
    printf("--- Adicionar Livro a Lista de Desejos ❤️ ---\n");
    if (ler_dados_livro_teclado(&livro_desejo)) {
        enqueue_desejo(fila, livro_desejo);
        printf("Livro '%s' adicionado a lista de desejos.\n", livro_desejo.titulo);
    }
}

void gerenciar_processar_desejo(FilaDesejos* fila) {
    Livro livro_desejado;
    if (fila_desejos_vazia(fila)) {
        printf("Lista de desejos esta vazia. 텅\n");
    } else if (dequeue_desejo(fila, &livro_desejado)) { // Usa a nova assinatura de dequeue
        printf("Proximo livro da lista de desejos (removido):\n");
        exibir_livro(&livro_desejado);
    }
}

void gerenciar_ver_historico(const PilhaHistorico* historico) {
    const char* isbn_topo = peek_historico(historico);
    if (isbn_topo) {
        printf("Ultimo ISBN consultado/adicionado no historico: %s 📜\n", isbn_topo);
        // Não precisamos dar free em isbn_topo pois peek_historico agora retorna
        // um ponteiro constante para os dados internos da pilha.
    } else {
        printf("Historico de consultas vazio.\n");
    }
}


// --- FUNÇÃO PRINCIPAL ---
int main() {
    ColecaoLivros* minha_colecao = criar_colecao();
    PilhaHistorico* meu_historico = criar_pilha_historico();
    FilaDesejos* minha_fila_desejos = criar_fila_desejos();

    if (!minha_colecao || !meu_historico || !minha_fila_desejos) {
        fprintf(stderr, "ERRO FATAL: Falha ao alocar estruturas principais. Saindo.\n");
        if (minha_colecao) destruir_colecao(minha_colecao);
        if (meu_historico) destruir_pilha_historico(meu_historico);
        if (minha_fila_desejos) destruir_fila_desejos(minha_fila_desejos);
        return 1;
    }

    limpar_tela();
    // Tenta carregar dados do arquivo binário ao iniciar
    if (carregar_colecao_binario(minha_colecao, ARQUIVO_BINARIO)) {
        printf("Dados carregados de %s! 🎉\n", ARQUIVO_BINARIO);
    } else if (carregar_colecao_texto(minha_colecao, ARQUIVO_TEXTO)) { // Tenta carregar do texto se o binário falhar
        printf("Dados carregados de %s! 📄\n", ARQUIVO_TEXTO);
    } else {
        printf("Nenhum arquivo de dados encontrado. Iniciando com colecao vazia.\n");
    }
    pausar_e_continuar();

    int opcao;
    char buffer_opcao[16];

    do {
        limpar_tela();
        exibir_menu_completo();
        ler_string_segura(buffer_opcao, sizeof(buffer_opcao));
        opcao = atoi(buffer_opcao); // Converte a string lida para inteiro

        limpar_tela();
        switch (opcao) {
            case 1: gerenciar_adicao_livro(minha_colecao, meu_historico); break;
            case 2: gerenciar_remocao_livro(minha_colecao); break;
            case 3: listar_todos_livros(minha_colecao); break;
            case 4: gerenciar_busca_isbn(minha_colecao, meu_historico); break;
            case 5: gerenciar_busca_titulo(minha_colecao, meu_historico); break;
            case 6: case 7: case 8:
                if (tamanho_colecao(minha_colecao) > 0) {
                    if (opcao == 6) {
                        ordenar_colecao_por_titulo(minha_colecao);
                        printf("Colecao ordenada por titulo.\n");
                    } else if (opcao == 7) {
                        ordenar_colecao_por_ano(minha_colecao);
                        printf("Colecao ordenada por ano.\n");
                    } else {
                        ordenar_colecao_por_autor(minha_colecao);
                        printf("Colecao ordenada por autor.\n");
                    }
                    // A função de ordenação agora modifica os dados da coleção.
                    // Exibimos o resultado automaticamente para melhor UX.
                    listar_todos_livros(minha_colecao);
                } else {
                    printf("Colecao vazia, nada para ordenar.\n");
                }
                break;
            case 9: gerenciar_adicao_desejo(minha_fila_desejos); break;
            case 10: gerenciar_processar_desejo(minha_fila_desejos); break;
            case 11: gerenciar_ver_historico(meu_historico); break;
            case 12: // Salvar Texto
                if (salvar_colecao_texto(minha_colecao, ARQUIVO_TEXTO)) printf("Colecao salva em %s ✅\n", ARQUIVO_TEXTO);
                else printf("ERRO ao salvar colecao em %s ❌\n", ARQUIVO_TEXTO);
                break;
            case 13: // Carregar Texto
                // A lógica de carregar adiciona à coleção existente.
                printf("Carregando de arquivo texto. A colecao atual sera incrementada.\n");
                if (carregar_colecao_texto(minha_colecao, ARQUIVO_TEXTO)) printf("Colecao carregada/incrementada de %s ✅\n", ARQUIVO_TEXTO);
                else printf("ERRO ou arquivo %s nao encontrado. 📄\n", ARQUIVO_TEXTO);
                break;
            case 14: // Salvar Binário
                if (salvar_colecao_binario(minha_colecao, ARQUIVO_BINARIO)) printf("Colecao salva em %s ✅\n", ARQUIVO_BINARIO);
                else printf("ERRO ao salvar colecao em %s ❌\n", ARQUIVO_BINARIO);
                break;
            case 15: // Carregar Binário
                printf("Carregando de arquivo binario. A colecao atual sera incrementada.\n");
                if (carregar_colecao_binario(minha_colecao, ARQUIVO_BINARIO)) printf("Colecao carregada/incrementada de %s ✅\n", ARQUIVO_BINARIO);
                else printf("ERRO ou arquivo %s nao encontrado. 💾\n", ARQUIVO_BINARIO);
                break;
            case 0:
                printf("Salvando dados antes de sair...\n");
                // Tenta salvar em binário por padrão
                if (salvar_colecao_binario(minha_colecao, ARQUIVO_BINARIO)) {
                     printf("Dados salvos em %s. ✅\n", ARQUIVO_BINARIO);
                } else {
                    // Fallback para texto se o salvamento binário falhar
                    printf("AVISO: Falha ao salvar em binario. Tentando salvar em texto...\n");
                    if (salvar_colecao_texto(minha_colecao, ARQUIVO_TEXTO)) {
                        printf("Dados salvos em %s como fallback. ✅\n", ARQUIVO_TEXTO);
                    } else {
                        printf("ERRO: Falha ao salvar em texto tambem. Dados podem ter sido perdidos. ❌\n");
                    }
                }
                printf("Saindo... Ate logo! 👋\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente. 🚫\n");
        }
        if (opcao != 0) {
            pausar_e_continuar();
        }
    } while (opcao != 0);

    // Liberar toda a memória alocada antes de encerrar
    destruir_colecao(minha_colecao);
    destruir_pilha_historico(meu_historico);
    destruir_fila_desejos(minha_fila_desejos);

    printf("Memoria liberada. Programa encerrado.\n");
    return 0;
}