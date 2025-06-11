# Personal Library Manager 📚

Este programa é um gerenciador de biblioteca pessoal desenvolvido em C, utilizando estruturas de dados como **Listas Encadeadas, Pilhas e Filas**. Ele permite ao usuário catalogar seus livros, realizar buscas, ordená-los por diferentes critérios, manter um histórico de livros consultados e uma lista de desejos. Além disso, oferece a funcionalidade de salvar e carregar os dados da biblioteca em arquivos.

## Funcionalidades Principais

* **Gerenciamento de Livros**:
    * Adicionar novos livros à coleção (com verificação de ISBN duplicado).
    * Remover livros existentes (via ISBN) com diálogo de confirmação.
    * Listar todos os livros catalogados.
* **Pesquisa**:
    * Buscar livros por ISBN (identificador único).
    * Buscar livros por título (busca por substring, case-sensitive).
* **Ordenação**:
    * Ordenar a coleção de livros por título.
    * Ordenar a coleção de livros por ano de publicação.
    * Ordenar a coleção de livros por autor.
* **Recursos Adicionais**:
    * **Histórico de Consultas**: Mantém uma pilha (LIFO) dos ISBNs dos livros recentemente adicionados ou consultados.
    * **Lista de Desejos**: Permite ao usuário manter uma fila (FIFO) de livros que deseja adquirir.
* **Persistência de Dados**:
    * Salvar a coleção de livros em arquivo de texto (`biblioteca.txt`).
    * Carregar a coleção de livros de um arquivo de texto.
    * Salvar a coleção de livros em arquivo binário (`biblioteca.dat`) para maior eficiência.
    * Carregar a coleção de livros de um arquivo binário.
    * Carregamento automático de dados ao iniciar (priorizando o arquivo binário) e salvamento automático ao sair.
* **Interface**:
    * Menu interativo via console para fácil utilização, com limpeza de tela para melhor experiência.

## Estrutura do Projeto

O projeto é modularizado em diferentes arquivos para melhor organização:

* `livro.c`/`livro.h`: Define a estrutura `Livro` e funções básicas para sua manipulação.
* `lista_livros.c`/`lista_livros.h`: Implementa a coleção principal de livros usando uma lista encadeada.
* `pilha_historico.c`/`pilha_historico.h`: Implementa a pilha para o histórico de consultas.
* `fila_desejos.c`/`fila_desejos.h`: Implementa a fila para a lista de desejos.
* `arquivos.c`/`arquivos.h`: Contém as funções para salvar e carregar a coleção de/para arquivos.
* `pesquisa_ordenacao.c`/`pesquisa_ordenacao.h`: Agrupa as funções de busca avançada e ordenação da coleção.
* `main.c`: Ponto de entrada do programa, controla o menu e a interação com o usuário.

## Como Compilar e Executar

Para compilar o projeto com o GCC, utilize o seguinte comando. As flags recomendadas ativam avisos úteis e incluem informações de debug.

```bash
# Comando de compilação
gcc -o biblioteca_pessoal main.c livro.c lista_livros.c pilha_historico.c fila_desejos.c arquivos.c pesquisa_ordenacao.c -Wall -Wextra -g

# Para executar o programa
./biblioteca_pessoal
