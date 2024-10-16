/*                                                                 ARVORE AVL
3) (3,0 pontos) Fa�a um programa em C que cadastre: 
(a) Uma lista de Plataforma de �udio e Videos: Nome plataforma, �rvore de PodCast, a lista deve ser 
organizada pela ordem de inser��o, ou seja, o �ltimo a cadastrar fica no final. 
(b) Uma �rvore de PodCast: Nome PodCast, Nome entrevistador e uma �rvore de Temas. A �rvore deve ser 
organizada pelo nome do PodCast. 
(c) Uma �rvore de Temas: Tema, �rvore de entrevistas. A �rvore deve ser organizada pelo Tema. 
(d) Uma �rvore de entrevistas: t�tulo da entrevista, data, dura��o, nome entrevistado, especialidade do 
entrevistado. Exemplo de especialidade: m�dico, professor, analista de sistemas, ator �.. A �rvore deve 
ser organizada pelo t�tulo da entrevista. 
Al�m de conter os cadastros no menu de op��es, o mesmo de conter uma op��o para cada item a seguir: 
i) Mostrar na tela todos os nomes dos PodCast em ordem alfab�tica de uma dada plataforma. 
ii) Mostrar na tela todos os temas de um PodCast de uma plataforma. 
iii) Mostrar na tela todos os t�tulos das entrevistas de um PodCast de uma plataforma dado o tema. 
iv) Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e t�tulo 
da entrevista. 
v) Mostrar na tela todos os entrevistados de um determinado tema de um PodCast de uma plataforma. 
vi) Permitir que o usu�rio possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento. 
vii)Permitir que o usu�rio possa excluir uma entrevista, um tema, ou um PodCast. Para excluir um PodCast, 
o mesmo n�o poder� ter nenhum tema cadastrado para ele. No caso do tema, ele s� poder� ser exclu�do 
se n�o existir nenhuma entrevista cadastrada para ele. Para excluir uma plataforma a mesma n�o poder� 
ter nenhum PodCast cadastrado para ele.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// Estrutura para uma entrevista
typedef struct Entrevista {
    char titulo[100];
    char data[20];
    int duracao;
    char nome_entrevistado[100];
    char especialidade[100];
    struct Entrevista *esquerda;
    struct Entrevista *direita;
    int altura; // Altura do n� na �rvore
} Entrevista;

// Estrutura para um tema
typedef struct Tema {
    char tema[100];
    Entrevista *entrevistas_raiz; // Raiz da �rvore de entrevistas
    struct Tema *esquerda;
    struct Tema *direita;
    int altura; // Altura do n� na �rvore
} Tema;

// Estrutura para arvore de PodCast
typedef struct PodCast {
    char nome[100];
    char entrevistador[100];
    struct Tema *temas_raiz; // Raiz da �rvore de temas
    struct PodCast *esquerda;
    struct PodCast *direita;
    int altura; // Altura do n� na �rvore
} PodCast;

// Estrutura para uma Plataforma de �udio e V�deos
typedef struct Plataforma {
    char nome[100];
    PodCast *podcasts_raiz; // Raiz da �rvore de PodCast
    struct Plataforma *anterior; // Ponteiro para o n� anterior na lista
    struct Plataforma *proximo; // Ponteiro para o pr�ximo n� na lista
} Plataforma;



                                                             //BLOCO ARVORE AVL PARA ENTREVISTA

//CRIAR E ALOCAR
// Fun��o para criar um novo n� de Entrevista
Entrevista* alocaEntrevista(char titulo[], char data[], int duracao, char nome_entrevistado[], char especialidade[]) {
    Entrevista *nova_entrevista = (Entrevista*)malloc(sizeof(Entrevista));
    if (nova_entrevista != NULL) {
        strcpy(nova_entrevista->titulo, titulo);
        strcpy(nova_entrevista->data, data);
        nova_entrevista->duracao = duracao;
        strcpy(nova_entrevista->nome_entrevistado, nome_entrevistado);
        strcpy(nova_entrevista->especialidade, especialidade);
        nova_entrevista->esquerda = NULL;
        nova_entrevista->direita = NULL;
        nova_entrevista->altura = 0;
    }
    return nova_entrevista;
}

//MAIOR SUBARVORE
// Fun��o para calcular o m�ximo entre dois n�meros
int maior(int a, int b) {
    return (a > b) ? a : b;
}

// Fun��o auxiliar para encontrar o n� com valor m�nimo
Entrevista* encontrarMinimo(Entrevista *raiz) {
    if (raiz == NULL)
        return NULL;
    while (raiz->esquerda != NULL)
        raiz = raiz->esquerda;
    return raiz;
}

//ALTURA NO
// Fun��o para calcular a altura de um n�
int altura(Entrevista *n) {
    if (n == NULL)
        return -1;
    else
        return n->altura;
}

//FATOR BALANCEAMENTO
// Fun��o para calcular o fator de balanceamento de um n�
int fatorBalanceamento(Entrevista *n) {
    return abs(altura(n->esquerda) - altura(n->direita));
}

//ROTACAO ESQUERDA
// Fun��o para rotacionar � esquerda
Entrevista* rotacaoEsquerda(Entrevista *n) {
    Entrevista *temp = n->direita;
    n->direita = temp->esquerda;
    temp->esquerda = n;
    n->altura = 1 + maior(altura(n->esquerda), altura(n->direita));
    temp->altura = 1 + maior(altura(temp->esquerda), altura(temp->direita));
    return temp;
}

//ROTACAO DIREITA
// Fun��o para rotacionar � direita
Entrevista* rotacaoDireita(Entrevista *n) {
    Entrevista *temp = n->esquerda;
    n->esquerda = temp->direita;
    temp->direita = n;
    n->altura = 1 + maior(altura(n->esquerda), altura(n->direita));
    temp->altura = 1 + maior(altura(temp->esquerda), altura(temp->direita));
    return temp;
}

//ROTACOES DUPLAS
// Fun��o para realizar a rota��o dupla � esquerda e depois � direita
Entrevista* rotacaoDuplaEsquerdaDireita(Entrevista *n) {
    n->esquerda = rotacaoEsquerda(n->esquerda);
    return rotacaoDireita(n);
}

// Fun��o para realizar a rota��o dupla � direita e depois � esquerda
Entrevista* rotacaoDuplaDireitaEsquerda(Entrevista *n) {
    n->direita = rotacaoDireita(n->direita);
    return rotacaoEsquerda(n);
}

//INSERCAO
// Fun��o para inserir uma entrevista na �rvore AVL
Entrevista* inserirEntrevista(Entrevista *raiz, char titulo[], char data[], int duracao, char nome_entrevistado[], char especialidade[]) {
    if (raiz == NULL) {
        Entrevista *nova_entrevista = alocaEntrevista(titulo, data, duracao, nome_entrevistado, especialidade);
        nova_entrevista->altura = 0;
        return nova_entrevista;
    }

    if (strcmp(titulo, raiz->titulo) < 0) {
        raiz->esquerda = inserirEntrevista(raiz->esquerda, titulo, data, duracao, nome_entrevistado, especialidade);
    } else if (strcmp(titulo, raiz->titulo) > 0) {
        raiz->direita = inserirEntrevista(raiz->direita, titulo, data, duracao, nome_entrevistado, especialidade);
    } else {
        // T�tulo duplicado
        return raiz;
    }

    raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita));

    int balanceamento = fatorBalanceamento(raiz);

    // Caso de rota��o � esquerda
    if (balanceamento > 1 && strcmp(titulo, raiz->esquerda->titulo) < 0) {
        return rotacaoDireita(raiz);
    }

    // Caso de rota��o � direita
    if (balanceamento < -1 && strcmp(titulo, raiz->direita->titulo) > 0) {
        return rotacaoEsquerda(raiz);
    }

    // Caso de rota��o dupla � esquerda e depois � direita
    if (balanceamento > 1 && strcmp(titulo, raiz->esquerda->titulo) > 0) {
        return rotacaoDuplaEsquerdaDireita(raiz);
    }

    // Caso de rota��o dupla � direita e depois � esquerda
    if (balanceamento < -1 && strcmp(titulo, raiz->direita->titulo) < 0) {
        return rotacaoDuplaDireitaEsquerda(raiz);
    }

    return raiz;
}


//BUSCA
// Fun��o para buscar uma entrevista em uma �rvore AVL
Entrevista* buscarEntrevista(Entrevista *raiz, char titulo[]) {
    if (raiz == NULL || strcmp(titulo, raiz->titulo) == 0)
        return raiz;

    if (strcmp(titulo, raiz->titulo) < 0)
        return buscarEntrevista(raiz->esquerda, titulo);

    return buscarEntrevista(raiz->direita, titulo);
}

//NO FOLHA
// Fun��o auxiliar para verificar se um n� � folha em uma �rvore de entrevistas
int ehFolhaEntrevista(Entrevista *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

//UNICO FILHO
// Fun��o auxiliar para encontrar o �nico filho de um n� em uma �rvore de entrevistas
Entrevista* unicoFilhoEntrevista(Entrevista* R) {
    if (R->esquerda != NULL && R->direita == NULL) {
        return R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        return R->direita;
    }
    return NULL;
}

//DOIS FILHOS
// Verifica se o n� possui dois filhos em uma �rvore de Entrevistas
int doisFilhosEntrevista(Entrevista *R) {
    return (R->esquerda != NULL && R->direita != NULL);
}

//MAIOR INFORMACAO
// Fun��o auxiliar para encontrar o n� com o maior t�tulo em uma sub�rvore
Entrevista* maiorInfoEntrevista(Entrevista *R) {
    while (R != NULL && R->direita != NULL) {
        R = R->direita;
    }
    return R;
}

//MENOR INFORMACAO
Entrevista* menorInfoEntrevista(Entrevista* no) {
    Entrevista* atual = no;
    while (atual && atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}


//REMOCAO
// Fun��o para remover uma entrevista de uma �rvore AVL
Entrevista* removerEntrevista(Entrevista *raiz, char titulo[]) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(titulo, raiz->titulo) < 0) {
        raiz->esquerda = removerEntrevista(raiz->esquerda, titulo);
    } else if (strcmp(titulo, raiz->titulo) > 0) {
        raiz->direita = removerEntrevista(raiz->direita, titulo);
    } else {
        // N� encontrado, realizar a remo��o
        if (ehFolhaEntrevista(raiz)) {
            free(raiz);
            return NULL;
        } else if (unicoFilhoEntrevista(raiz) != NULL) {
            Entrevista *temp = unicoFilhoEntrevista(raiz);
            free(raiz);
            return temp;
        } else if (doisFilhosEntrevista(raiz)) {
            // N� possui dois filhos, substituir pelo menor n� na sub�rvore direita
            Entrevista *temp = menorInfoEntrevista(raiz->direita);
            strcpy(raiz->titulo, temp->titulo);
            raiz->direita = removerEntrevista(raiz->direita, temp->titulo);
        }
    }

    return raiz;
}

//LIBERAR MEMORIA
// Fun��o para liberar a mem�ria de uma �rvore AVL
void liberarArvore(Entrevista *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

//IMPRIMIR
// Fun��o para imprimir as entrevistas em ordem
void imprimirEntrevistas(Entrevista *raiz) {
    if (raiz != NULL) {
        imprimirEntrevistas(raiz->esquerda);
        printf("T�tulo: %s\n", raiz->titulo);
        printf("Data: %s\n", raiz->data);
        printf("Dura��o: %d minutos\n", raiz->duracao);
        printf("Nome do entrevistado: %s\n", raiz->nome_entrevistado);
        printf("Especialidade do entrevistado: %s\n\n", raiz->especialidade);
        imprimirEntrevistas(raiz->direita);
    }
}

                          
                                                      ////BLOCO ARVORE AVL PARA TEMAS


//CRIAR E ALOCAR
// Fun��o para criar um novo n� de Tema
Tema* criarTema(char tema[]) {
    Tema *novo_tema = (Tema*)malloc(sizeof(Tema));
    if (novo_tema != NULL) {
        strcpy(novo_tema->tema, tema);
        novo_tema->entrevistas_raiz = NULL; // Inicializa a raiz da �rvore de entrevistas como NULL
        novo_tema->esquerda = NULL;
        novo_tema->direita = NULL;
        novo_tema->altura = 0;
    }
    return novo_tema;
}

//ALTURA NO
// Fun��o para calcular a altura de um n�
int alturaTema(Tema *n) {
    if (n == NULL)
        return -1;
    else
        return n->altura;
}
//FATOR BALANCEAMENTO
// Fun��o para calcular o fator de balanceamento de um n�
int fatorBalanceamentoTema(Tema *n) {
    return abs(alturaTema(n->esquerda) - alturaTema(n->direita));
}

//ROTA��O ESQUERDA
// Fun��o para rotacionar � esquerda
Tema* rotacaoEsquerdaTema(Tema *n) {
    Tema *temp = n->direita;
    n->direita = temp->esquerda;
    temp->esquerda = n;
    n->altura = 1 + maior(alturaTema(n->esquerda), alturaTema(n->direita));
    temp->altura = 1 + maior(alturaTema(temp->esquerda), alturaTema(temp->direita));
    return temp;
}

//ROTACAO DIREITA
// Fun��o para rotacionar � direita
Tema* rotacaoDireitaTema(Tema *n) {
    Tema *temp = n->esquerda;
    n->esquerda = temp->direita;
    temp->direita = n;
    n->altura = 1 + maior(alturaTema(n->esquerda), alturaTema(n->direita));
    temp->altura = 1 + maior(alturaTema(temp->esquerda), alturaTema(temp->direita));
    return temp;
}

//ROTACOES DUPLAS
// Fun��o para realizar a rota��o dupla � esquerda e depois � direita
Tema* rotacaoDuplaEsquerdaDireitaTema(Tema *n) {
    n->esquerda = rotacaoEsquerdaTema(n->esquerda);
    return rotacaoDireitaTema(n);
}
// Fun��o para realizar a rota��o dupla � direita e depois � esquerda
Tema* rotacaoDuplaDireitaEsquerdaTema(Tema *n) {
    n->direita = rotacaoDireitaTema(n->direita);
    return rotacaoEsquerdaTema(n);
}

//INSERIR
// Fun��o para inserir um tema em uma �rvore AVL
Tema* inserirTemaAVL(Tema *raiz, char tema[]) {
    if (raiz == NULL) {
        Tema *novo_tema = criarTema(tema);
        novo_tema->altura = 0;
        return novo_tema;
    }

    if (strcmp(tema, raiz->tema) < 0) {
        raiz->esquerda = inserirTemaAVL(raiz->esquerda, tema);
    } else if (strcmp(tema, raiz->tema) > 0) {
        raiz->direita = inserirTemaAVL(raiz->direita, tema);
    } else {
        // Tema duplicado
        return raiz;
    }

    raiz->altura = 1 + maior(alturaTema(raiz->esquerda), alturaTema(raiz->direita));

    int balanceamento = fatorBalanceamentoTema(raiz);

    // Caso de rota��o � esquerda
    if (balanceamento > 1 && strcmp(tema, raiz->esquerda->tema) < 0) {
        return rotacaoDireitaTema(raiz);
    }

    // Caso de rota��o � direita
    if (balanceamento < -1 && strcmp(tema, raiz->direita->tema) > 0) {
        return rotacaoEsquerdaTema(raiz);
    }

    // Caso de rota��o dupla � esquerda e depois � direita
    if (balanceamento > 1 && strcmp(tema, raiz->esquerda->tema) > 0) {
        return rotacaoDuplaEsquerdaDireitaTema(raiz);
    }

    // Caso de rota��o dupla � direita e depois � esquerda
    if (balanceamento < -1 && strcmp(tema, raiz->direita->tema) < 0) {
        return rotacaoDuplaDireitaEsquerdaTema(raiz);
    }

    return raiz;
}

//BUSCA
// Fun��o para buscar um tema em uma �rvore AVL
Tema* buscarTemaAVL(Tema *raiz, char tema[]) {
    if (raiz == NULL || strcmp(tema, raiz->tema) == 0)
        return raiz;

    if (strcmp(tema, raiz->tema) < 0)
        return buscarTemaAVL(raiz->esquerda, tema);

    return buscarTemaAVL(raiz->direita, tema);
}

//NO FOLHA
// Fun��o auxiliar para verificar se um n� � folha em uma �rvore de temas
int ehFolhaTema(Tema *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

//UNICO FILHO
// Fun��o auxiliar para encontrar o �nico filho de um n� em uma �rvore de temas
Tema* unicoFilhoTema(Tema* R) {
    if (R->esquerda != NULL && R->direita == NULL) {
        return R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        return R->direita;
    }
    return NULL;
}

//DOIS FILHOS
// Verifica se o n� possui dois filhos em uma �rvore de Temas
int doisFilhosTema(Tema *R) {
    return (R->esquerda != NULL && R->direita != NULL);
}

//MAIOR TEMA
// Fun��o auxiliar para encontrar o tema com o maior nome em uma sub�rvore
Tema* maiorInfoTema(Tema *R) {
    while (R != NULL && R->direita != NULL) {
        R = R->direita;
    }
    return R;
}

//REMOVER
// Fun��o para remover um tema de uma �rvore AVL
Tema* removerTemaAVL(Tema *raiz, char tema[]) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(tema, raiz->tema) < 0) {
        raiz->esquerda = removerTemaAVL(raiz->esquerda, tema);
    } else if (strcmp(tema, raiz->tema) > 0) {
        raiz->direita = removerTemaAVL(raiz->direita, tema);
    } else {
        // N� encontrado, realizar a remo��o
        if (ehFolhaTema(raiz)) {
            free(raiz);
            return NULL;
        } else if (unicoFilhoTema(raiz) != NULL) {
            Tema *temp = unicoFilhoTema(raiz);
            free(raiz);
            return temp;
        } else {
            // N� possui dois filhos, substituir pelo menor n� na sub�rvore direita
            Tema *temp = maiorInfoTema(raiz->esquerda);
            strcpy(raiz->tema, temp->tema);
            raiz->esquerda = removerTemaAVL(raiz->esquerda, temp->tema);
        }
    }

    return raiz;
}

//LIBERAR MEMORIA
// Fun��o para liberar a mem�ria de uma �rvore AVL de temas
void liberarArvoreTema(Tema *raiz) {
    if (raiz != NULL) {
        liberarArvoreTema(raiz->esquerda);
        liberarArvoreTema(raiz->direita);
        free(raiz);
    }
}

//IMPRIMIR
// Fun��o para imprimir os temas em ordem
void imprimirTemas(Tema *raiz) {
    if (raiz != NULL) {
        imprimirTemas(raiz->esquerda);
        printf("Tema: %s\n", raiz->tema);
        printf("\nEntrevistas:\n");
        imprimirEntrevistas(raiz->entrevistas_raiz); // Chama a fun��o para imprimir as entrevistas do Tema
        printf("----------------------------------------\n");
        imprimirTemas(raiz->direita);
    }
}



                                                          //////BLOCO ARVORE AVL PARA PODCAST
                                                          
//CRIAR E ALOCAR
// Fun��o para criar um novo n� de PodCast
PodCast* alocaPodCast(char nome[], char entrevistador[]) {
    PodCast *novo_podcast = (PodCast*)malloc(sizeof(PodCast));
    if (novo_podcast != NULL) {
        strcpy(novo_podcast->nome, nome);
        strcpy(novo_podcast->entrevistador, entrevistador);
        novo_podcast->temas_raiz = NULL; // Inicializa a �rvore de temas como NULL
        novo_podcast->esquerda = NULL;
        novo_podcast->direita = NULL;
    }
    return novo_podcast;
}

//ALTURA NO
// Fun��o auxiliar para calcular a altura de um n� PodCast
int alturaPodCast(PodCast *n) {
    if (n == NULL)
        return -1;
    else
        return n->altura;
}

//FATOR BALANCEAMENTO
// Fun��o auxiliar para calcular o fator de balanceamento de um n� PodCast
int fatorBalanceamentoPodCast(PodCast *n) {
    return abs(alturaPodCast(n->esquerda) - alturaPodCast(n->direita));
}

//ROTACAO ESQUERDA
// Fun��o para rotacionar � esquerda na �rvore de PodCast
PodCast* rotacaoEsquerdaPodCast(PodCast *n) {
    PodCast *temp = n->direita;
    n->direita = temp->esquerda;
    temp->esquerda = n;
    n->altura = 1 + maior(alturaPodCast(n->esquerda), alturaPodCast(n->direita));
    temp->altura = 1 + maior(alturaPodCast(temp->esquerda), alturaPodCast(temp->direita));
    return temp;
}

//ROTACAO DIREITA
// Fun��o para rotacionar � direita na �rvore de PodCast
PodCast* rotacaoDireitaPodCast(PodCast *n) {
    PodCast *temp = n->esquerda;
    n->esquerda = temp->direita;
    temp->direita = n;
    n->altura = 1 + maior(alturaPodCast(n->esquerda), alturaPodCast(n->direita));
    temp->altura = 1 + maior(alturaPodCast(temp->esquerda), alturaPodCast(temp->direita));
    return temp;
}

//ROTACOES DUPLAS
// Fun��o para realizar a rota��o dupla � esquerda e depois � direita na �rvore de PodCast
PodCast* rotacaoDuplaEsquerdaDireitaPodCast(PodCast *n) {
    n->esquerda = rotacaoEsquerdaPodCast(n->esquerda);
    return rotacaoDireitaPodCast(n);
}
// Fun��o para realizar a rota��o dupla � direita e depois � esquerda na �rvore de PodCast
PodCast* rotacaoDuplaDireitaEsquerdaPodCast(PodCast *n) {
    n->direita = rotacaoDireitaPodCast(n->direita);
    return rotacaoEsquerdaPodCast(n);
}

//INSERIR
// Fun��o para inserir um PodCast em uma �rvore AVL de PodCast
PodCast* inserirPodCastAVL(PodCast *raiz, char nome[], char entrevistador[]) {
    if (raiz == NULL) {
        PodCast *novo_podcast = (PodCast*)malloc(sizeof(PodCast));
        if (novo_podcast != NULL) {
            strcpy(novo_podcast->nome, nome);
            strcpy(novo_podcast->entrevistador, entrevistador);
            novo_podcast->temas_raiz = NULL;
            novo_podcast->esquerda = NULL;
            novo_podcast->direita = NULL;
            novo_podcast->altura = 0;
        }
        return novo_podcast;
    }

    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = inserirPodCastAVL(raiz->esquerda, nome, entrevistador);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = inserirPodCastAVL(raiz->direita, nome, entrevistador);
    } else {
        // PodCast duplicado
        return raiz;
    }

    raiz->altura = 1 + maior(alturaPodCast(raiz->esquerda), alturaPodCast(raiz->direita));

    int balanceamento = fatorBalanceamentoPodCast(raiz);

    // Caso de rota��o � esquerda
    if (balanceamento > 1 && strcmp(nome, raiz->esquerda->nome) < 0) {
        return rotacaoDireitaPodCast(raiz);
    }

    // Caso de rota��o � direita
    if (balanceamento < -1 && strcmp(nome, raiz->direita->nome) > 0) {
        return rotacaoEsquerdaPodCast(raiz);
    }

    // Caso de rota��o dupla � esquerda e depois � direita
    if (balanceamento > 1 && strcmp(nome, raiz->esquerda->nome) > 0) {
        return rotacaoDuplaEsquerdaDireitaPodCast(raiz);
    }

    // Caso de rota��o dupla � direita e depois � esquerda
    if (balanceamento < -1 && strcmp(nome, raiz->direita->nome) < 0) {
        return rotacaoDuplaDireitaEsquerdaPodCast(raiz);
    }

    return raiz;
}

//BUSCAR
// Fun��o para buscar um PodCast em uma �rvore AVL de PodCast
PodCast* buscarPodCastAVL(PodCast *raiz, char nome[]) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        return buscarPodCastAVL(raiz->esquerda, nome);

    return buscarPodCastAVL(raiz->direita, nome);
}

//NO FOLHA
// Fun��o auxiliar para verificar se um n� � folha em uma �rvore de PodCast
int ehFolhaPodCast(PodCast *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

//UNICO FILHO
// Fun��o auxiliar para encontrar o �nico filho de um n� em uma �rvore de PodCast
PodCast* unicoFilhoPodCast(PodCast* R) {
    if (R->esquerda != NULL && R->direita == NULL) {
        return R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        return R->direita;
    }
    return NULL;
}

//DOIS FILHOS
// Verifica se o n� possui dois filhos em uma �rvore de PodCast
int doisFilhosPodCast(PodCast *R) {
    return (R->esquerda != NULL && R->direita != NULL);
}

//MAIOR INFORMACAO
// Fun��o auxiliar para encontrar o PodCast com o maior nome em uma sub�rvore
PodCast* maiorInfoPodCast(PodCast *R) {
    while (R != NULL && R->direita != NULL) {
        R = R->direita;
    }
    return R;
}

//REMOVER
// Fun��o para remover um PodCast de uma �rvore AVL de PodCast
PodCast* removerPodCastAVL(PodCast *raiz, char nome[]) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = removerPodCastAVL(raiz->esquerda, nome);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = removerPodCastAVL(raiz->direita, nome);
    } else {
        // N� encontrado, realizar a remo��o
        if (ehFolhaPodCast(raiz)) {
            free(raiz);
            return NULL;
        } else if (unicoFilhoPodCast(raiz) != NULL) {
            PodCast *temp = unicoFilhoPodCast(raiz);
            free(raiz);
            return temp;
        } else {
            // N� possui dois filhos, substituir pelo menor n� na sub�rvore direita
            PodCast *temp = maiorInfoPodCast(raiz->esquerda);
            strcpy(raiz->nome, temp->nome);
            strcpy(raiz->entrevistador, temp->entrevistador);
            raiz->esquerda = removerPodCastAVL(raiz->esquerda, temp->nome);
        }
    }

    return raiz;
}

//LIBERAR MEMORIA
// Fun��o para liberar a mem�ria de uma �rvore AVL de PodCast
void liberarArvorePodCast(PodCast *raiz) {
    if (raiz != NULL) {
        liberarArvorePodCast(raiz->esquerda);
        liberarArvorePodCast(raiz->direita);
        free(raiz);
    }
}

//IMPRIMIR
// Fun��o para imprimir os PodCasts em ordem
void imprimirPodCasts(PodCast *raiz) {
    if (raiz != NULL) {
        imprimirPodCasts(raiz->esquerda);
        printf("Nome do PodCast: %s\n", raiz->nome);
        printf("Entrevistador: %s\n", raiz->entrevistador);
        printf("\nTemas:\n");
        imprimirTemas(raiz->temas_raiz); // Chama a fun��o para imprimir os temas do PodCast
        printf("----------------------------------------\n");
        imprimirPodCasts(raiz->direita);
    }
}
void imprimirArvAVLPodCasts(PodCast *raiz, int nivel) {
	int i;
    if (raiz != NULL) {
        imprimirArvAVLPodCasts(raiz->direita, nivel + 1);
        printf("\n\n");

        for (i = 0; i < nivel; i++)
            printf("\t");

        printf("%s - %s", raiz->nome, raiz->entrevistador);
        
        // Imprimir os temas associados a este PodCast
        printf("\nTemas:\n");
        imprimirTemas(raiz->temas_raiz);

        imprimirArvAVLPodCasts(raiz->esquerda, nivel + 1);
    }
}


                                            ////////BLOCO LISTA ENCADEADA PARA PLATAFORMA DE AUDIO E VIDEO
																																																				
  
//CRIA LISTA ENCADEADA PARA PLATAFORMA
// Fun��o para criar uma nova plataforma de �udio e v�deo
Plataforma* criarPlataforma(char nome[]) {
    Plataforma *nova_plataforma = (Plataforma*)malloc(sizeof(Plataforma));
    if (nova_plataforma != NULL) {
        strcpy(nova_plataforma->nome, nome);
        nova_plataforma->podcasts_raiz = NULL;
        nova_plataforma->anterior = NULL;
        nova_plataforma->proximo = NULL;
    }
    return nova_plataforma;
}

//INSERIR
// Inserir uma nova plataforma no final da lista
void inserirPlataforma(Plataforma **inicio, char nome[]) {
    Plataforma *nova_plataforma = criarPlataforma(nome);
    if (nova_plataforma == NULL) {
        printf("Erro ao alocar mem�ria para a nova plataforma.\n");
        return;
    }

    if (*inicio == NULL) {
        *inicio = nova_plataforma;
    } else {
        Plataforma *atual = *inicio;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = nova_plataforma;
        nova_plataforma->anterior = atual;
    }
}

//BUSCAR
// Buscar uma plataforma pelo nome
Plataforma* buscarPlataforma(Plataforma *inicio, char nome[]) {
    Plataforma *atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual; // Retorna a plataforma se encontrada
        }
        atual = atual->proximo;
    }
    return NULL; // Retorna NULL se a plataforma n�o for encontrada
}

//REMOVER
// Remover uma plataforma pelo nome
void removerPlataforma(Plataforma **inicio, char nome[]) {
    Plataforma *plataforma_remover = buscarPlataforma(*inicio, nome);
    if (plataforma_remover == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nome);
        return;
    }

    if (plataforma_remover->anterior != NULL) {
        plataforma_remover->anterior->proximo = plataforma_remover->proximo;
    } else {
        *inicio = plataforma_remover->proximo;
    }

    if (plataforma_remover->proximo != NULL) {
        plataforma_remover->proximo->anterior = plataforma_remover->anterior;
    }

    free(plataforma_remover);
    printf("Plataforma \"%s\" removida com sucesso.\n", nome);
}

//IMPRIMIR
// Fun��o para imprimir a lista de Plataformas com seus PodCasts
void imprimirPlataformas(Plataforma *inicio) {
    if (inicio == NULL) {
        printf("Lista de Plataformas vazia.\n");
        return;
    }

    Plataforma *atual = inicio;
    while (atual != NULL) {
        printf("Nome da Plataforma: %s\n", atual->nome);
        printf("PodCasts:\n");
        if (atual->podcasts_raiz == NULL) {
            printf("Nenhum PodCast cadastrado nesta Plataforma.\n");
        } else {
            imprimirPodCasts(atual->podcasts_raiz); // Chama a fun��o para imprimir os PodCasts da Plataforma
        }
        printf("----------------------------------------\n");
        atual = atual->proximo;
    }
}



                                                               //////////FUNCOES ESPECIFICAS

//i) Mostrar na tela todos os nomes dos PodCast em ordem alfab�tica de uma dada plataforma.
// Fun��o auxiliar para percorrer a �rvore de PodCasts em ordem e imprimir os nomes
void percorrerEmOrdemPodCasts(PodCast *raiz) {
    if (raiz != NULL) {
        percorrerEmOrdemPodCasts(raiz->esquerda);
        printf("- %s\n", raiz->nome);
        percorrerEmOrdemPodCasts(raiz->direita);
    }
}
// Fun��o para mostrar na tela todos os nomes dos PodCasts em ordem alfab�tica de uma dada plataforma
void mostrarNomesPodCasts(Plataforma *inicio, char nomePlataforma[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
    }

    printf("Nomes dos PodCasts em ordem alfab�tica na plataforma \"%s\":\n", nomePlataforma);
    percorrerEmOrdemPodCasts(plataforma->podcasts_raiz);
}


//ii) Mostrar na tela todos os temas de um PodCast de uma plataforma.
// Fun��o auxiliar para percorrer a �rvore de temas em ordem e imprimir os nomes dos temas
void percorrerEmOrdemTemas(Tema *raiz) {
    if (raiz != NULL) {
        percorrerEmOrdemTemas(raiz->esquerda);
        printf("- %s\n", raiz->tema);
        percorrerEmOrdemTemas(raiz->direita);
    }
}

// Fun��o para mostrar na tela todos os temas de um PodCast de uma plataforma
void mostrarTemasPodCast(Plataforma *inicio, char nomePlataforma[], char nomePodCast[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    printf("Temas do PodCast \"%s\" da plataforma \"%s\":\n", nomePodCast, nomePlataforma);
    percorrerEmOrdemTemas(podcast->temas_raiz);
}


//iii) Mostrar na tela todos os t�tulos das entrevistas de um PodCast de uma plataforma dado o tema.
// Fun��o auxiliar para percorrer a �rvore de entrevistas em ordem
void percorrerEmOrdemEntrevistas(Entrevista *raiz) {
    if (raiz != NULL) {
        percorrerEmOrdemEntrevistas(raiz->esquerda);
        printf("- %s\n", raiz->titulo);
        percorrerEmOrdemEntrevistas(raiz->direita);
    }
}
// Fun��o para mostrar os t�tulos das entrevistas de um determinado tema de um PodCast de uma plataforma
void mostrarTitulosEntrevistas(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Buscar a plataforma
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    
    // Buscar o PodCast
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    
    // Buscar o tema
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" n�o encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    
    // Imprimir os t�tulos das entrevistas do tema
    printf("T�tulos das entrevistas do tema \"%s\" do PodCast \"%s\" da plataforma \"%s\":\n", tema, nomePodCast, nomePlataforma);
    percorrerEmOrdemEntrevistas(temaEncontrado->entrevistas_raiz);
}

//iv) Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e t�tulo da entrevista.
void mostrarDadosEntrevista(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" n�o encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    
    Entrevista *entrevistaAtual = temaEncontrado->entrevistas_raiz;
    while (entrevistaAtual != NULL) {
        if (strcmp(entrevistaAtual->titulo, titulo) == 0) {
            printf("Dados da entrevista:\n");
            printf("T�tulo: %s\n", entrevistaAtual->titulo);
            printf("Entrevistado: %s\n", entrevistaAtual->nome_entrevistado);
            printf("Dura��o: %d minutos\n", entrevistaAtual->duracao);
            return;
        }
        entrevistaAtual = entrevistaAtual->direita;
    }
    
    printf("Entrevista com o t�tulo \"%s\" n�o encontrada no tema \"%s\" do PodCast \"%s\" da plataforma \"%s\".\n", titulo, tema, nomePodCast, nomePlataforma);
}



//v) Mostrar na tela todos os entrevistados de um determinado tema de um PodCast de uma plataforma.
void imprimirEntrevistados(Entrevista *raiz) {
    if (raiz != NULL) {
        imprimirEntrevistados(raiz->esquerda);
        printf("- %s\n", raiz->nome_entrevistado);
        imprimirEntrevistados(raiz->direita);
    }
}
void mostrarEntrevistadosTema(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" n�o encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }

    printf("Entrevistados do tema \"%s\" do PodCast \"%s\" da plataforma \"%s\":\n", tema, nomePodCast, nomePlataforma);

    // Percorrer a �rvore de entrevistas e imprimir os nomes dos entrevistados
    imprimirEntrevistados(temaEncontrado->entrevistas_raiz);
}

//vi) Permitir que o usu�rio possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento.
void adicionarPlataforma(Plataforma **inicio, char nome[]) {
    inserirPlataforma(inicio, nome);
    printf("Plataforma \"%s\" adicionada com sucesso.\n", nome);
}
void adicionarPodCast(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char entrevistador[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    plataforma->podcasts_raiz = inserirPodCastAVL(plataforma->podcasts_raiz, nomePodCast, entrevistador);
    printf("PodCast \"%s\" adicionado na plataforma \"%s\" com sucesso.\n", nomePodCast, nomePlataforma);
}
void adicionarTema(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    podcast->temas_raiz = inserirTemaAVL(podcast->temas_raiz, tema);
    printf("Tema \"%s\" adicionado ao PodCast \"%s\" da plataforma \"%s\" com sucesso.\n", tema, nomePodCast, nomePlataforma);
}
void adicionarEntrevista(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[], char data[], int duracao, char nome_entrevistado[], char especialidade[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" n�o encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    temaEncontrado->entrevistas_raiz = inserirEntrevista(temaEncontrado->entrevistas_raiz, titulo, "", 0, nome_entrevistado, especialidade);
}

 

/*  vii)Permitir que o usu�rio possa excluir uma entrevista, um tema, ou um PodCast. 
Para excluir um PodCast, o mesmo n�o poder� ter nenhum tema cadastrado para ele. 
No caso do tema, ele s� poder� ser exclu�do se n�o existir nenhuma entrevista cadastrada para ele. 
Para excluir uma plataforma a mesma n�o poder� ter nenhum PodCast cadastrado para ele.    */
void excluirEntrevista(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" n�o encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    Entrevista *entrevistaRemover = buscarEntrevista(temaEncontrado->entrevistas_raiz, titulo);
    if (entrevistaRemover == NULL) {
        printf("Entrevista \"%s\" n�o encontrada no tema \"%s\" do PodCast \"%s\" da plataforma \"%s\".\n", titulo, tema, nomePodCast, nomePlataforma);
        return;
    }
    temaEncontrado->entrevistas_raiz = removerEntrevista(temaEncontrado->entrevistas_raiz, titulo);
    printf("Entrevista \"%s\" do tema \"%s\" do PodCast \"%s\" da plataforma \"%s\" foi removida com sucesso.\n", titulo, tema, nomePodCast, nomePlataforma);
}
void excluirTema(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaRemover = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaRemover == NULL) {
        printf("Tema \"%s\" n�o encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    if (temaRemover->entrevistas_raiz != NULL) {
        printf("N�o � poss�vel excluir o tema \"%s\" pois ainda h� entrevistas associadas a ele no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    podcast->temas_raiz = removerTemaAVL(podcast->temas_raiz, tema);
    printf("Tema \"%s\" do PodCast \"%s\" da plataforma \"%s\" foi removido com sucesso.\n", tema, nomePodCast, nomePlataforma);
}
void excluirPodCast(Plataforma *inicio, char nomePlataforma[], char nomePodCast[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" n�o encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcastRemover = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcastRemover == NULL) {
        printf("PodCast \"%s\" n�o encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    if (podcastRemover->temas_raiz != NULL) {
        printf("N�o � poss�vel excluir o PodCast \"%s\" pois ainda h� temas associados a ele na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    plataforma->podcasts_raiz = removerPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    printf("PodCast \"%s\" da plataforma \"%s\" foi removido com sucesso.\n", nomePodCast, nomePlataforma);
}


                                                        // Fun��o principal com o menu de op��es


int main() {
	// Inicializa��o da lista de plataformas
    Plataforma *inicio = NULL;
    char nomePlataforma[100], nomePodCast[100], tema[100], tituloEntrevista[100], data[20], nomeEntrevistador[100], especialidade[100]; 
    int opcao, duracao, i=0;
    double soma, tempo_decorrido, tempDecorrido[30];
    struct timeval _inicio, fim;
    
    do{
    	printf("\nMenu:\n");
        printf("1. Adicionar Plataforma\n");
        printf("2. Adicionar PodCast\n");
        printf("3. Adicionar Tema\n");
        printf("4. Adicionar Entrevista\n");
        printf("5. Mostrar os nomes dos PodCast em ordem alfab�tica\n");
        printf("6. Mostrar os temas de um PodCast\n");
        printf("7. Mostrar os titulos das entrevistas de um PodCast\n");
        printf("8. Mostrar os dados de uma entrevista\n");
        printf("9. Mostrar os entrevistados por tema\n");
        printf("10. Excluir Entrevista\n");
        printf("11. Excluir Tema\n");
        printf("12. Excluir PodCast\n");
        printf("13. Excluir Plataforma\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: //vi) Permitir que o usu�rio possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento
            	 printf("Digite o nome da plataforma: ");
				 scanf(" %s", nomePlataforma);
		         //Chamada para adicionar plataforma
		         adicionarPlataforma(&inicio, nomePlataforma);
		         break;
		     case 2:  // Permitir que o usu�rio possa adicionar podcast e analisar o tempo de execu��o 30 vezes
			    // Solicita ao usu�rio que insira os dados
			    printf("Digite o nome da plataforma: ");
			    scanf(" %s", nomePlataforma);
			    printf("Digite o nome do PodCast: ");
			    scanf(" %s", nomePodCast);
			    printf("Digite o nome do entrevistador: ");
			    scanf(" %s", nomeEntrevistador);
			    
			    for (i = 0; i < 30; i++) {
			        // Marca o tempo inicial
			        gettimeofday(&_inicio, NULL);
			        
			        // Chamada para adicionar podcast
			        adicionarPodCast(inicio, nomePlataforma, nomePodCast, nomeEntrevistador);
			        
			        // Marca o tempo final
			        gettimeofday(&fim, NULL);
			        
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - _inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - _inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a m�dia posteriormente
			        soma += tempo_decorrido;
			    }
			    
			    // Calcula a m�dia dos tempos decorridos
			    double media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
			    break;
			case 3:  //vi) Permitir que o usu�rio possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                //chamada da funcao para adicionar tema
                adicionarTema(inicio, nomePlataforma, nomePodCast, tema);
                break;
            case 4:  //vi) Permitir que o usu�rio possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento
            	printf("Digite o nome da plataforma: ");
			    scanf(" %s", nomePlataforma);
			    printf("Digite o nome do PodCast: ");
			    scanf(" %s", nomePodCast);
			    printf("Digite o nome do tema: ");
			    scanf(" %s", tema);
			    printf("Digite o t�tulo da entrevista: ");
			    scanf(" %[^\n]s", tituloEntrevista);
			    printf("Digite a data da entrevista (DD/MM/AAAA): ");
			    scanf(" %s", data);
			    printf("Digite a dura��o da entrevista (em minutos): ");
			    scanf(" %d", &duracao);
			    printf("Digite o nome do entrevistado: ");
			    scanf(" %s", nomeEntrevistador);
			    printf("Digite a especialidade do entrevistado: ");
			    scanf(" %s", especialidade);
			    // Chamada da fun��o para adicionar a entrevista
			    adicionarEntrevista(inicio, nomePlataforma, nomePodCast, tema, tituloEntrevista, data, duracao, nomeEntrevistador, especialidade);
			    break;
            case 5:   //i) Mostrar na tela todos os nomes dos PodCast em ordem alfab�tica de uma dada plataforma
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                // Chamada da fun��o para mostrar os nomes dos PodCasts em ordem alfab�tica
                mostrarNomesPodCasts(inicio, nomePlataforma);
                break;
            case 6:   //ii) Mostrar na tela todos os temas de um PodCast de uma plataforma                
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                 for (i = 0; i < 30; i++) {
			        // Marca o tempo inicial
			        gettimeofday(&_inicio, NULL);
    			
	                // Chamada da fun��o para mostrar os temas de um PodCast
	                mostrarTemasPodCast(inicio, nomePlataforma, nomePodCast);
	                
	                // Marca o tempo final
			        gettimeofday(&fim, NULL);
			        
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - _inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - _inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a m�dia posteriormente
			        soma += tempo_decorrido;
			    }
			    
			    // Calcula a m�dia dos tempos decorridos
			    media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
			    break;
                
            case 7:   //iii) Mostrar na tela todos os t�tulos das entrevistas de um PodCast de uma plataforma dado o tema
            	printf("Digite o tema: ");
                scanf(" %s", tema);
                // Chamada da fun��o para mostrar os t�tulos das entrevistas
                mostrarTitulosEntrevistas(inicio, nomePlataforma, nomePodCast, tema); 
                break;
            case 8:  //iv) Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e t�tulo da entrevista
				printf("Digite o nome da plataforma: ");
			    scanf(" %s", nomePlataforma);
			    printf("Digite o nome do PodCast: ");
			    scanf(" %s", nomePodCast);
			    printf("Digite o nome do tema: ");
			    scanf(" %s", tema);
			    printf("Digite o t�tulo da entrevista: ");
			    scanf(" %[^\n]s", tituloEntrevista);
			    // Chamada da fun��o para mostrar os dados de uma entrevista de um Podcast
			    mostrarDadosEntrevista(inicio, nomePlataforma, nomePodCast, tema, tituloEntrevista);
			    break;
            case 9:  //v) Mostrar na tela todos os entrevistados de um determinado tema de um PodCast de uma plataforma
            	printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                // Chamada da fun��o para mostrar os entrevistados
                mostrarEntrevistadosTema(inicio, nomePlataforma, nomePodCast, tema);
                break;
            case 10:  //vii)Permitir que o usu�rio possa excluir uma entrevista, um tema, ou um PodCast
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                printf("Digite o t�tulo da entrevista: ");
                scanf(" %[^\n]s", tituloEntrevista);
                // Chamada da fun��o para excluir uma entrevista
                excluirEntrevista(inicio, nomePlataforma, nomePodCast, tema, tituloEntrevista);
                break;
            case 11:  //vii)Permitir que o usu�rio possa excluir uma entrevista, um tema, ou um PodCast
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                // Chamada da fun��o para excluir um tema
                excluirTema(inicio, nomePlataforma, nomePodCast, tema);
                break;
            case 12:  //vii)Permitir que o usu�rio possa excluir uma entrevista, um tema, ou um PodCast
            	printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                // Chamada da fun��o para excluir um podcast
            	excluirPodCast(inicio, nomePlataforma, nomePodCast);
            	break;
            case 13:
            	removerPlataforma(&inicio, nomePlataforma);
            	break;
            case 0:
                printf("Encerrando o programa\n");
                break;
            default:
                printf("Opcao inv�lida! Tente novamente\n");
       }
    }while (opcao != 0);
    
    return 0;
}
    
    





