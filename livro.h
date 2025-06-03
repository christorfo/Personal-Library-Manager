typedef struct {
    char titulo[100];       // String (Vetor de char)
    char autor[100];        // String (Vetor de char)
    int anoPublicacao;
    char isbn[14];          // String (Vetor de char), chave única
    char genero[50];        // String (Vetor de char)
    // Outros campos que desejar, ex: editora, numeroPaginas
} Livro;