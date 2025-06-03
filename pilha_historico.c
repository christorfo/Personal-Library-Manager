#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha_historico.h"

// --- FUNÇÕES COMPLETAS (EXEMPLOS) ---
PilhaHistorico* criar_pilha_historico() {
    // Lógica: Alocar memória para a struct PilhaHistorico.
    PilhaHistorico* pilha = (PilhaHistorico*) malloc(sizeof(PilhaHistorico));
    // Lógica: Verificar alocação.
    if (pilha == NULL) {
        perror("Erro ao alocar memoria para a pilha");
        return NULL;
    }
    // Lógica: Inicializar o topo da pilha como NULL.
    pilha->topo = NULL;
    return pilha;
}

int pilha_historico_vazia(const PilhaHistorico* pilha) {
    // Lógica: Verificar se a pilha é NULL ou se o topo é NULL.
    if (pilha == NULL || pilha->topo == NULL) {
        return 1; // Vazia
    }
    return 0; // Não vazia
}

// --- ESQUELETOS DETALHADOS (VOCÊ IMPLEMENTA) ---
void push_historico(PilhaHistorico* pilha, const char* isbn) {
    // Lógica: Verificar se a pilha ou o ISBN são NULL.
    if (pilha == NULL || isbn == NULL) {
        fprintf(stderr, "Erro: Pilha ou ISBN nulos para push.\n");
        return;
    }

    // Lógica: Alocar memória para um novo NoPilha.
    NoPilha* novo_no = (NoPilha*) malloc(sizeof(NoPilha));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para novo no da pilha");
        return;
    }

    // Lógica: Copiar o ISBN para o novo nó.
    strncpy(novo_no->isbn_consultado, isbn, TAM_ISBN -1);
    novo_no->isbn_consultado[TAM_ISBN -1] = '\0'; // Garantir terminação nula

    // Lógica: Fazer o 'proximo' do novo nó apontar para o antigo 'topo' da pilha.
    novo_no->proximo = pilha->topo;

    // Lógica: Atualizar o 'topo' da pilha para o novo nó.
    pilha->topo = novo_no;
}

char* pop_historico(PilhaHistorico* pilha) {
    // Lógica: Verificar se a pilha é NULL ou se está vazia. Se sim, retornar NULL.
    if (pilha_historico_vazia(pilha)) {
        //fprintf(stderr, "Erro: Pilha vazia, nao e possivel fazer pop.\n");
        return NULL;
    }

    // Lógica: Guardar o nó do topo atual.
    NoPilha* no_removido = pilha->topo;

    // Lógica: Alocar memória para copiar o ISBN a ser retornado.
    //         O chamador desta função será responsável por liberar esta memória.
    char* isbn_retorno = (char*) malloc(TAM_ISBN * sizeof(char));
    if (isbn_retorno == NULL) {
        perror("Erro ao alocar memoria para retornar ISBN do pop");
        // O nó não foi removido da pilha ainda, então a pilha permanece consistente
        return NULL;
    }
    strncpy(isbn_retorno, no_removido->isbn_consultado, TAM_ISBN -1);
    isbn_retorno[TAM_ISBN-1] = '\0';


    // Lógica: Atualizar o 'topo' da pilha para o 'proximo' do nó do topo.
    pilha->topo = no_removido->proximo;

    // Lógica: Liberar a memória do nó removido.
    free(no_removido);

    // Lógica: Retornar o ISBN copiado.
    return isbn_retorno;
    // TODO: O chamador deve liberar a memória de isbn_retorno usando free().
}

char* peek_historico(const PilhaHistorico* pilha) {
    // Lógica: Verificar se a pilha é NULL ou se está vazia. Se sim, retornar NULL.
    if (pilha_historico_vazia(pilha)) {
        return NULL;
    }
    // Lógica: Retornar o 'isbn_consultado' do nó do topo.
    //         Não alocar nova memória, apenas retornar o ponteiro para a string dentro do nó.
    //         CUIDADO: este ponteiro é válido apenas enquanto o nó estiver na pilha e não for alterado.
    return (char*)pilha->topo->isbn_consultado; // Cast para char* (de const char*) se necessário, dependendo do uso.
                                               // Para ser seguro, e se a string puder ser modificada ou o nó liberado,
                                               // uma cópia deveria ser retornada (como em pop_historico).
                                               // Para apenas visualização, isso é aceitável.
}

void destruir_pilha_historico(PilhaHistorico* pilha) {
    // Lógica: Verificar se a pilha é NULL.
    if (pilha == NULL) {
        return;
    }

    // Lógica: Enquanto a pilha não estiver vazia, remover (pop) os elementos.
    //         Lembre-se que pop_historico aloca memória para o ISBN retornado.
    //         Portanto, precisamos liberar essa memória após cada pop.
    char* isbn_temp;
    while (!pilha_historico_vazia(pilha)) {
        isbn_temp = pop_historico(pilha); // pop já libera o nó da pilha
        if (isbn_temp != NULL) {
            free(isbn_temp); // Libera a string ISBN que foi alocada por pop_historico
        }
    }

    // Lógica: Liberar a estrutura da pilha em si.
    free(pilha);
}