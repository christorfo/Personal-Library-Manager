#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strncpy, se necessário, embora aqui copiamos a struct Livro inteira
#include "fila_desejos.h"

// --- FUNÇÕES COMPLETAS (EXEMPLOS) ---
FilaDesejos* criar_fila_desejos() {
    // Lógica: Alocar memória para a struct FilaDesejos.
    FilaDesejos* fila = (FilaDesejos*) malloc(sizeof(FilaDesejos));
    // Lógica: Verificar alocação.
    if (fila == NULL) {
        perror("Erro ao alocar memoria para a fila");
        return NULL;
    }
    // Lógica: Inicializar inicio e fim como NULL.
    fila->inicio = NULL;
    fila->fim = NULL;
    return fila;
}

int fila_desejos_vazia(const FilaDesejos* fila) {
    // Lógica: Verificar se a fila é NULL ou se o inicio (ou fim) é NULL.
    if (fila == NULL || fila->inicio == NULL) {
        return 1; // Vazia
    }
    return 0; // Não vazia
}

// --- ESQUELETOS DETALHADOS (VOCÊ IMPLEMENTA) ---
void enqueue_desejo(FilaDesejos* fila, Livro livro) {
    // Lógica: Verificar se a fila é NULL.
    if (fila == NULL) {
        fprintf(stderr, "Erro: Fila nula para enqueue.\n");
        return;
    }

    // Lógica: Alocar memória para um novo NoFila.
    NoFila* novo_no = (NoFila*) malloc(sizeof(NoFila));
    if (novo_no == NULL) {
        perror("Erro ao alocar memoria para novo no da fila");
        return;
    }

    // Lógica: Copiar os dados do 'livro' para o novo nó.
    novo_no->livro_desejado = livro;
    novo_no->proximo = NULL; // O novo nó será o último, então proximo é NULL.

    // Lógica: Se a fila estiver vazia (fila->inicio == NULL):
    //         O novo nó é tanto o 'inicio' quanto o 'fim'.
    if (fila_desejos_vazia(fila)) {
        fila->inicio = novo_no;
        fila->fim = novo_no;
    }
    // Lógica: Se a fila não estiver vazia:
    //         O 'proximo' do antigo 'fim' aponta para o novo nó.
    //         Atualizar o 'fim' da fila para o novo nó.
    else {
        fila->fim->proximo = novo_no;
        fila->fim = novo_no;
    }
}

Livro dequeue_desejo(FilaDesejos* fila) {
    Livro livro_removido_dados;
    // Lógica: Inicializar livro_removido_dados com valores padrão "vazios" ou de erro.
    // Isso é importante caso a fila esteja vazia.
    strcpy(livro_removido_dados.isbn, "VAZIO"); // Indicador de erro/fila vazia
    livro_removido_dados.titulo[0] = '\0';


    // Lógica: Verificar se a fila é NULL ou está vazia.
    if (fila_desejos_vazia(fila)) {
        fprintf(stderr, "Aviso: Fila de desejos vazia, nao e possivel fazer dequeue.\n");
        return livro_removido_dados; // Retorna o livro "vazio"
    }

    // Lógica: Guardar o nó do início atual.
    NoFila* no_removido = fila->inicio;

    // Lógica: Copiar os dados do livro do nó a ser removido para 'livro_removido_dados'.
    livro_removido_dados = no_removido->livro_desejado;

    // Lógica: Atualizar o 'inicio' da fila para o 'proximo' do nó do início.
    fila->inicio = no_removido->proximo;

    // Lógica: Se, após a remoção, o 'inicio' se tornar NULL (fila ficou vazia),
    //         Então o 'fim' também deve ser NULL.
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    // Lógica: Liberar a memória do nó removido.
    free(no_removido);

    // Lógica: Retornar os dados do livro que foi removido.
    return livro_removido_dados;
    // TODO: O chamador deve verificar se o ISBN do livro retornado é "VAZIO"
    //       para saber se o dequeue falhou por fila vazia.
    //       Uma abordagem melhor seria retornar um status (int) e passar Livro por referência.
}

Livro front_desejo(const FilaDesejos* fila) {
    Livro livro_frente_dados;
    strcpy(livro_frente_dados.isbn, "VAZIO"); // Indicador de erro/fila vazia
    livro_frente_dados.titulo[0] = '\0';

    // Lógica: Verificar se a fila é NULL ou está vazia.
    if (fila_desejos_vazia(fila)) {
        //fprintf(stderr, "Aviso: Fila de desejos vazia, nao e possivel ver o elemento da frente.\n");
        return livro_frente_dados;
    }

    // Lógica: Retornar os dados do livro do nó 'inicio' da fila.
    //         (Copia os dados, não retorna ponteiro para dentro da fila).
    return fila->inicio->livro_desejado;
    // TODO: Mesma consideração de erro do dequeue.
}

void destruir_fila_desejos(FilaDesejos* fila) {
    // Lógica: Verificar se a fila é NULL.
    if (fila == NULL) {
        return;
    }

    // Lógica: Enquanto a fila não estiver vazia, remover (dequeue) os elementos.
    //         Não precisamos nos preocupar com o valor retornado por dequeue aqui,
    //         apenas em esvaziar a fila e liberar os nós.
    while (!fila_desejos_vazia(fila)) {
        dequeue_desejo(fila); // dequeue já libera o nó
    }

    // Lógica: Liberar a estrutura da fila em si.
    free(fila);
}