/*                                                                 ARVORE AVL
3) (3,0 pontos) Faça um programa em C que cadastre: 
(a) Uma lista de Plataforma de Áudio e Videos: Nome plataforma, árvore de PodCast, a lista deve ser 
organizada pela ordem de inserção, ou seja, o último a cadastrar fica no final. 
(b) Uma árvore de PodCast: Nome PodCast, Nome entrevistador e uma árvore de Temas. A árvore deve ser 
organizada pelo nome do PodCast. 
(c) Uma árvore de Temas: Tema, árvore de entrevistas. A árvore deve ser organizada pelo Tema. 
(d) Uma árvore de entrevistas: título da entrevista, data, duração, nome entrevistado, especialidade do 
entrevistado. Exemplo de especialidade: médico, professor, analista de sistemas, ator ….. A árvore deve 
ser organizada pelo título da entrevista. 
Além de conter os cadastros no menu de opções, o mesmo de conter uma opção para cada item a seguir: 
i) Mostrar na tela todos os nomes dos PodCast em ordem alfabética de uma dada plataforma. 
ii) Mostrar na tela todos os temas de um PodCast de uma plataforma. 
iii) Mostrar na tela todos os títulos das entrevistas de um PodCast de uma plataforma dado o tema. 
iv) Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e título 
da entrevista. 
v) Mostrar na tela todos os entrevistados de um determinado tema de um PodCast de uma plataforma. 
vi) Permitir que o usuário possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento. 
vii)Permitir que o usuário possa excluir uma entrevista, um tema, ou um PodCast. Para excluir um PodCast, 
o mesmo não poderá ter nenhum tema cadastrado para ele. No caso do tema, ele só poderá ser excluído 
se não existir nenhuma entrevista cadastrada para ele. Para excluir uma plataforma a mesma não poderá 
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
    int altura; // Altura do nó na árvore
} Entrevista;

// Estrutura para um tema
typedef struct Tema {
    char tema[100];
    Entrevista *entrevistas_raiz; // Raiz da árvore de entrevistas
    struct Tema *esquerda;
    struct Tema *direita;
    int altura; // Altura do nó na árvore
} Tema;

// Estrutura para arvore de PodCast
typedef struct PodCast {
    char nome[100];
    char entrevistador[100];
    struct Tema *temas_raiz; // Raiz da árvore de temas
    struct PodCast *esquerda;
    struct PodCast *direita;
    int altura; // Altura do nó na árvore
} PodCast;

// Estrutura para uma Plataforma de Áudio e Vídeos
typedef struct Plataforma {
    char nome[100];
    PodCast *podcasts_raiz; // Raiz da árvore de PodCast
    struct Plataforma *anterior; // Ponteiro para o nó anterior na lista
    struct Plataforma *proximo; // Ponteiro para o próximo nó na lista
} Plataforma;



                                                             //BLOCO ARVORE AVL PARA ENTREVISTA

//CRIAR E ALOCAR
// Função para criar um novo nó de Entrevista
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
// Função para calcular o máximo entre dois números
int maior(int a, int b) {
    return (a > b) ? a : b;
}

// Função auxiliar para encontrar o nó com valor mínimo
Entrevista* encontrarMinimo(Entrevista *raiz) {
    if (raiz == NULL)
        return NULL;
    while (raiz->esquerda != NULL)
        raiz = raiz->esquerda;
    return raiz;
}

//ALTURA NO
// Função para calcular a altura de um nó
int altura(Entrevista *n) {
    if (n == NULL)
        return -1;
    else
        return n->altura;
}

//FATOR BALANCEAMENTO
// Função para calcular o fator de balanceamento de um nó
int fatorBalanceamento(Entrevista *n) {
    return abs(altura(n->esquerda) - altura(n->direita));
}

//ROTACAO ESQUERDA
// Função para rotacionar à esquerda
Entrevista* rotacaoEsquerda(Entrevista *n) {
    Entrevista *temp = n->direita;
    n->direita = temp->esquerda;
    temp->esquerda = n;
    n->altura = 1 + maior(altura(n->esquerda), altura(n->direita));
    temp->altura = 1 + maior(altura(temp->esquerda), altura(temp->direita));
    return temp;
}

//ROTACAO DIREITA
// Função para rotacionar à direita
Entrevista* rotacaoDireita(Entrevista *n) {
    Entrevista *temp = n->esquerda;
    n->esquerda = temp->direita;
    temp->direita = n;
    n->altura = 1 + maior(altura(n->esquerda), altura(n->direita));
    temp->altura = 1 + maior(altura(temp->esquerda), altura(temp->direita));
    return temp;
}

//ROTACOES DUPLAS
// Função para realizar a rotação dupla à esquerda e depois à direita
Entrevista* rotacaoDuplaEsquerdaDireita(Entrevista *n) {
    n->esquerda = rotacaoEsquerda(n->esquerda);
    return rotacaoDireita(n);
}

// Função para realizar a rotação dupla à direita e depois à esquerda
Entrevista* rotacaoDuplaDireitaEsquerda(Entrevista *n) {
    n->direita = rotacaoDireita(n->direita);
    return rotacaoEsquerda(n);
}

//INSERCAO
// Função para inserir uma entrevista na árvore AVL
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
        // Título duplicado
        return raiz;
    }

    raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita));

    int balanceamento = fatorBalanceamento(raiz);

    // Caso de rotação à esquerda
    if (balanceamento > 1 && strcmp(titulo, raiz->esquerda->titulo) < 0) {
        return rotacaoDireita(raiz);
    }

    // Caso de rotação à direita
    if (balanceamento < -1 && strcmp(titulo, raiz->direita->titulo) > 0) {
        return rotacaoEsquerda(raiz);
    }

    // Caso de rotação dupla à esquerda e depois à direita
    if (balanceamento > 1 && strcmp(titulo, raiz->esquerda->titulo) > 0) {
        return rotacaoDuplaEsquerdaDireita(raiz);
    }

    // Caso de rotação dupla à direita e depois à esquerda
    if (balanceamento < -1 && strcmp(titulo, raiz->direita->titulo) < 0) {
        return rotacaoDuplaDireitaEsquerda(raiz);
    }

    return raiz;
}


//BUSCA
// Função para buscar uma entrevista em uma árvore AVL
Entrevista* buscarEntrevista(Entrevista *raiz, char titulo[]) {
    if (raiz == NULL || strcmp(titulo, raiz->titulo) == 0)
        return raiz;

    if (strcmp(titulo, raiz->titulo) < 0)
        return buscarEntrevista(raiz->esquerda, titulo);

    return buscarEntrevista(raiz->direita, titulo);
}

//NO FOLHA
// Função auxiliar para verificar se um nó é folha em uma árvore de entrevistas
int ehFolhaEntrevista(Entrevista *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

//UNICO FILHO
// Função auxiliar para encontrar o único filho de um nó em uma árvore de entrevistas
Entrevista* unicoFilhoEntrevista(Entrevista* R) {
    if (R->esquerda != NULL && R->direita == NULL) {
        return R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        return R->direita;
    }
    return NULL;
}

//DOIS FILHOS
// Verifica se o nó possui dois filhos em uma árvore de Entrevistas
int doisFilhosEntrevista(Entrevista *R) {
    return (R->esquerda != NULL && R->direita != NULL);
}

//MAIOR INFORMACAO
// Função auxiliar para encontrar o nó com o maior título em uma subárvore
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
// Função para remover uma entrevista de uma árvore AVL
Entrevista* removerEntrevista(Entrevista *raiz, char titulo[]) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(titulo, raiz->titulo) < 0) {
        raiz->esquerda = removerEntrevista(raiz->esquerda, titulo);
    } else if (strcmp(titulo, raiz->titulo) > 0) {
        raiz->direita = removerEntrevista(raiz->direita, titulo);
    } else {
        // Nó encontrado, realizar a remoção
        if (ehFolhaEntrevista(raiz)) {
            free(raiz);
            return NULL;
        } else if (unicoFilhoEntrevista(raiz) != NULL) {
            Entrevista *temp = unicoFilhoEntrevista(raiz);
            free(raiz);
            return temp;
        } else if (doisFilhosEntrevista(raiz)) {
            // Nó possui dois filhos, substituir pelo menor nó na subárvore direita
            Entrevista *temp = menorInfoEntrevista(raiz->direita);
            strcpy(raiz->titulo, temp->titulo);
            raiz->direita = removerEntrevista(raiz->direita, temp->titulo);
        }
    }

    return raiz;
}

//LIBERAR MEMORIA
// Função para liberar a memória de uma árvore AVL
void liberarArvore(Entrevista *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

//IMPRIMIR
// Função para imprimir as entrevistas em ordem
void imprimirEntrevistas(Entrevista *raiz) {
    if (raiz != NULL) {
        imprimirEntrevistas(raiz->esquerda);
        printf("Título: %s\n", raiz->titulo);
        printf("Data: %s\n", raiz->data);
        printf("Duração: %d minutos\n", raiz->duracao);
        printf("Nome do entrevistado: %s\n", raiz->nome_entrevistado);
        printf("Especialidade do entrevistado: %s\n\n", raiz->especialidade);
        imprimirEntrevistas(raiz->direita);
    }
}

                          
                                                      ////BLOCO ARVORE AVL PARA TEMAS


//CRIAR E ALOCAR
// Função para criar um novo nó de Tema
Tema* criarTema(char tema[]) {
    Tema *novo_tema = (Tema*)malloc(sizeof(Tema));
    if (novo_tema != NULL) {
        strcpy(novo_tema->tema, tema);
        novo_tema->entrevistas_raiz = NULL; // Inicializa a raiz da árvore de entrevistas como NULL
        novo_tema->esquerda = NULL;
        novo_tema->direita = NULL;
        novo_tema->altura = 0;
    }
    return novo_tema;
}

//ALTURA NO
// Função para calcular a altura de um nó
int alturaTema(Tema *n) {
    if (n == NULL)
        return -1;
    else
        return n->altura;
}
//FATOR BALANCEAMENTO
// Função para calcular o fator de balanceamento de um nó
int fatorBalanceamentoTema(Tema *n) {
    return abs(alturaTema(n->esquerda) - alturaTema(n->direita));
}

//ROTAÇÃO ESQUERDA
// Função para rotacionar à esquerda
Tema* rotacaoEsquerdaTema(Tema *n) {
    Tema *temp = n->direita;
    n->direita = temp->esquerda;
    temp->esquerda = n;
    n->altura = 1 + maior(alturaTema(n->esquerda), alturaTema(n->direita));
    temp->altura = 1 + maior(alturaTema(temp->esquerda), alturaTema(temp->direita));
    return temp;
}

//ROTACAO DIREITA
// Função para rotacionar à direita
Tema* rotacaoDireitaTema(Tema *n) {
    Tema *temp = n->esquerda;
    n->esquerda = temp->direita;
    temp->direita = n;
    n->altura = 1 + maior(alturaTema(n->esquerda), alturaTema(n->direita));
    temp->altura = 1 + maior(alturaTema(temp->esquerda), alturaTema(temp->direita));
    return temp;
}

//ROTACOES DUPLAS
// Função para realizar a rotação dupla à esquerda e depois à direita
Tema* rotacaoDuplaEsquerdaDireitaTema(Tema *n) {
    n->esquerda = rotacaoEsquerdaTema(n->esquerda);
    return rotacaoDireitaTema(n);
}
// Função para realizar a rotação dupla à direita e depois à esquerda
Tema* rotacaoDuplaDireitaEsquerdaTema(Tema *n) {
    n->direita = rotacaoDireitaTema(n->direita);
    return rotacaoEsquerdaTema(n);
}

//INSERIR
// Função para inserir um tema em uma árvore AVL
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

    // Caso de rotação à esquerda
    if (balanceamento > 1 && strcmp(tema, raiz->esquerda->tema) < 0) {
        return rotacaoDireitaTema(raiz);
    }

    // Caso de rotação à direita
    if (balanceamento < -1 && strcmp(tema, raiz->direita->tema) > 0) {
        return rotacaoEsquerdaTema(raiz);
    }

    // Caso de rotação dupla à esquerda e depois à direita
    if (balanceamento > 1 && strcmp(tema, raiz->esquerda->tema) > 0) {
        return rotacaoDuplaEsquerdaDireitaTema(raiz);
    }

    // Caso de rotação dupla à direita e depois à esquerda
    if (balanceamento < -1 && strcmp(tema, raiz->direita->tema) < 0) {
        return rotacaoDuplaDireitaEsquerdaTema(raiz);
    }

    return raiz;
}

//BUSCA
// Função para buscar um tema em uma árvore AVL
Tema* buscarTemaAVL(Tema *raiz, char tema[]) {
    if (raiz == NULL || strcmp(tema, raiz->tema) == 0)
        return raiz;

    if (strcmp(tema, raiz->tema) < 0)
        return buscarTemaAVL(raiz->esquerda, tema);

    return buscarTemaAVL(raiz->direita, tema);
}

//NO FOLHA
// Função auxiliar para verificar se um nó é folha em uma árvore de temas
int ehFolhaTema(Tema *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

//UNICO FILHO
// Função auxiliar para encontrar o único filho de um nó em uma árvore de temas
Tema* unicoFilhoTema(Tema* R) {
    if (R->esquerda != NULL && R->direita == NULL) {
        return R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        return R->direita;
    }
    return NULL;
}

//DOIS FILHOS
// Verifica se o nó possui dois filhos em uma árvore de Temas
int doisFilhosTema(Tema *R) {
    return (R->esquerda != NULL && R->direita != NULL);
}

//MAIOR TEMA
// Função auxiliar para encontrar o tema com o maior nome em uma subárvore
Tema* maiorInfoTema(Tema *R) {
    while (R != NULL && R->direita != NULL) {
        R = R->direita;
    }
    return R;
}

//REMOVER
// Função para remover um tema de uma árvore AVL
Tema* removerTemaAVL(Tema *raiz, char tema[]) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(tema, raiz->tema) < 0) {
        raiz->esquerda = removerTemaAVL(raiz->esquerda, tema);
    } else if (strcmp(tema, raiz->tema) > 0) {
        raiz->direita = removerTemaAVL(raiz->direita, tema);
    } else {
        // Nó encontrado, realizar a remoção
        if (ehFolhaTema(raiz)) {
            free(raiz);
            return NULL;
        } else if (unicoFilhoTema(raiz) != NULL) {
            Tema *temp = unicoFilhoTema(raiz);
            free(raiz);
            return temp;
        } else {
            // Nó possui dois filhos, substituir pelo menor nó na subárvore direita
            Tema *temp = maiorInfoTema(raiz->esquerda);
            strcpy(raiz->tema, temp->tema);
            raiz->esquerda = removerTemaAVL(raiz->esquerda, temp->tema);
        }
    }

    return raiz;
}

//LIBERAR MEMORIA
// Função para liberar a memória de uma árvore AVL de temas
void liberarArvoreTema(Tema *raiz) {
    if (raiz != NULL) {
        liberarArvoreTema(raiz->esquerda);
        liberarArvoreTema(raiz->direita);
        free(raiz);
    }
}

//IMPRIMIR
// Função para imprimir os temas em ordem
void imprimirTemas(Tema *raiz) {
    if (raiz != NULL) {
        imprimirTemas(raiz->esquerda);
        printf("Tema: %s\n", raiz->tema);
        printf("\nEntrevistas:\n");
        imprimirEntrevistas(raiz->entrevistas_raiz); // Chama a função para imprimir as entrevistas do Tema
        printf("----------------------------------------\n");
        imprimirTemas(raiz->direita);
    }
}



                                                          //////BLOCO ARVORE AVL PARA PODCAST
                                                          
//CRIAR E ALOCAR
// Função para criar um novo nó de PodCast
PodCast* alocaPodCast(char nome[], char entrevistador[]) {
    PodCast *novo_podcast = (PodCast*)malloc(sizeof(PodCast));
    if (novo_podcast != NULL) {
        strcpy(novo_podcast->nome, nome);
        strcpy(novo_podcast->entrevistador, entrevistador);
        novo_podcast->temas_raiz = NULL; // Inicializa a árvore de temas como NULL
        novo_podcast->esquerda = NULL;
        novo_podcast->direita = NULL;
    }
    return novo_podcast;
}

//ALTURA NO
// Função auxiliar para calcular a altura de um nó PodCast
int alturaPodCast(PodCast *n) {
    if (n == NULL)
        return -1;
    else
        return n->altura;
}

//FATOR BALANCEAMENTO
// Função auxiliar para calcular o fator de balanceamento de um nó PodCast
int fatorBalanceamentoPodCast(PodCast *n) {
    return abs(alturaPodCast(n->esquerda) - alturaPodCast(n->direita));
}

//ROTACAO ESQUERDA
// Função para rotacionar à esquerda na árvore de PodCast
PodCast* rotacaoEsquerdaPodCast(PodCast *n) {
    PodCast *temp = n->direita;
    n->direita = temp->esquerda;
    temp->esquerda = n;
    n->altura = 1 + maior(alturaPodCast(n->esquerda), alturaPodCast(n->direita));
    temp->altura = 1 + maior(alturaPodCast(temp->esquerda), alturaPodCast(temp->direita));
    return temp;
}

//ROTACAO DIREITA
// Função para rotacionar à direita na árvore de PodCast
PodCast* rotacaoDireitaPodCast(PodCast *n) {
    PodCast *temp = n->esquerda;
    n->esquerda = temp->direita;
    temp->direita = n;
    n->altura = 1 + maior(alturaPodCast(n->esquerda), alturaPodCast(n->direita));
    temp->altura = 1 + maior(alturaPodCast(temp->esquerda), alturaPodCast(temp->direita));
    return temp;
}

//ROTACOES DUPLAS
// Função para realizar a rotação dupla à esquerda e depois à direita na árvore de PodCast
PodCast* rotacaoDuplaEsquerdaDireitaPodCast(PodCast *n) {
    n->esquerda = rotacaoEsquerdaPodCast(n->esquerda);
    return rotacaoDireitaPodCast(n);
}
// Função para realizar a rotação dupla à direita e depois à esquerda na árvore de PodCast
PodCast* rotacaoDuplaDireitaEsquerdaPodCast(PodCast *n) {
    n->direita = rotacaoDireitaPodCast(n->direita);
    return rotacaoEsquerdaPodCast(n);
}

//INSERIR
// Função para inserir um PodCast em uma árvore AVL de PodCast
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

    // Caso de rotação à esquerda
    if (balanceamento > 1 && strcmp(nome, raiz->esquerda->nome) < 0) {
        return rotacaoDireitaPodCast(raiz);
    }

    // Caso de rotação à direita
    if (balanceamento < -1 && strcmp(nome, raiz->direita->nome) > 0) {
        return rotacaoEsquerdaPodCast(raiz);
    }

    // Caso de rotação dupla à esquerda e depois à direita
    if (balanceamento > 1 && strcmp(nome, raiz->esquerda->nome) > 0) {
        return rotacaoDuplaEsquerdaDireitaPodCast(raiz);
    }

    // Caso de rotação dupla à direita e depois à esquerda
    if (balanceamento < -1 && strcmp(nome, raiz->direita->nome) < 0) {
        return rotacaoDuplaDireitaEsquerdaPodCast(raiz);
    }

    return raiz;
}

//BUSCAR
// Função para buscar um PodCast em uma árvore AVL de PodCast
PodCast* buscarPodCastAVL(PodCast *raiz, char nome[]) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        return buscarPodCastAVL(raiz->esquerda, nome);

    return buscarPodCastAVL(raiz->direita, nome);
}

//NO FOLHA
// Função auxiliar para verificar se um nó é folha em uma árvore de PodCast
int ehFolhaPodCast(PodCast *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

//UNICO FILHO
// Função auxiliar para encontrar o único filho de um nó em uma árvore de PodCast
PodCast* unicoFilhoPodCast(PodCast* R) {
    if (R->esquerda != NULL && R->direita == NULL) {
        return R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        return R->direita;
    }
    return NULL;
}

//DOIS FILHOS
// Verifica se o nó possui dois filhos em uma árvore de PodCast
int doisFilhosPodCast(PodCast *R) {
    return (R->esquerda != NULL && R->direita != NULL);
}

//MAIOR INFORMACAO
// Função auxiliar para encontrar o PodCast com o maior nome em uma subárvore
PodCast* maiorInfoPodCast(PodCast *R) {
    while (R != NULL && R->direita != NULL) {
        R = R->direita;
    }
    return R;
}

//REMOVER
// Função para remover um PodCast de uma árvore AVL de PodCast
PodCast* removerPodCastAVL(PodCast *raiz, char nome[]) {
    if (raiz == NULL) {
        return raiz;
    }

    if (strcmp(nome, raiz->nome) < 0) {
        raiz->esquerda = removerPodCastAVL(raiz->esquerda, nome);
    } else if (strcmp(nome, raiz->nome) > 0) {
        raiz->direita = removerPodCastAVL(raiz->direita, nome);
    } else {
        // Nó encontrado, realizar a remoção
        if (ehFolhaPodCast(raiz)) {
            free(raiz);
            return NULL;
        } else if (unicoFilhoPodCast(raiz) != NULL) {
            PodCast *temp = unicoFilhoPodCast(raiz);
            free(raiz);
            return temp;
        } else {
            // Nó possui dois filhos, substituir pelo menor nó na subárvore direita
            PodCast *temp = maiorInfoPodCast(raiz->esquerda);
            strcpy(raiz->nome, temp->nome);
            strcpy(raiz->entrevistador, temp->entrevistador);
            raiz->esquerda = removerPodCastAVL(raiz->esquerda, temp->nome);
        }
    }

    return raiz;
}

//LIBERAR MEMORIA
// Função para liberar a memória de uma árvore AVL de PodCast
void liberarArvorePodCast(PodCast *raiz) {
    if (raiz != NULL) {
        liberarArvorePodCast(raiz->esquerda);
        liberarArvorePodCast(raiz->direita);
        free(raiz);
    }
}

//IMPRIMIR
// Função para imprimir os PodCasts em ordem
void imprimirPodCasts(PodCast *raiz) {
    if (raiz != NULL) {
        imprimirPodCasts(raiz->esquerda);
        printf("Nome do PodCast: %s\n", raiz->nome);
        printf("Entrevistador: %s\n", raiz->entrevistador);
        printf("\nTemas:\n");
        imprimirTemas(raiz->temas_raiz); // Chama a função para imprimir os temas do PodCast
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
// Função para criar uma nova plataforma de áudio e vídeo
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
        printf("Erro ao alocar memória para a nova plataforma.\n");
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
    return NULL; // Retorna NULL se a plataforma não for encontrada
}

//REMOVER
// Remover uma plataforma pelo nome
void removerPlataforma(Plataforma **inicio, char nome[]) {
    Plataforma *plataforma_remover = buscarPlataforma(*inicio, nome);
    if (plataforma_remover == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nome);
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
// Função para imprimir a lista de Plataformas com seus PodCasts
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
            imprimirPodCasts(atual->podcasts_raiz); // Chama a função para imprimir os PodCasts da Plataforma
        }
        printf("----------------------------------------\n");
        atual = atual->proximo;
    }
}



                                                               //////////FUNCOES ESPECIFICAS

//i) Mostrar na tela todos os nomes dos PodCast em ordem alfabética de uma dada plataforma.
// Função auxiliar para percorrer a árvore de PodCasts em ordem e imprimir os nomes
void percorrerEmOrdemPodCasts(PodCast *raiz) {
    if (raiz != NULL) {
        percorrerEmOrdemPodCasts(raiz->esquerda);
        printf("- %s\n", raiz->nome);
        percorrerEmOrdemPodCasts(raiz->direita);
    }
}
// Função para mostrar na tela todos os nomes dos PodCasts em ordem alfabética de uma dada plataforma
void mostrarNomesPodCasts(Plataforma *inicio, char nomePlataforma[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
    }

    printf("Nomes dos PodCasts em ordem alfabética na plataforma \"%s\":\n", nomePlataforma);
    percorrerEmOrdemPodCasts(plataforma->podcasts_raiz);
}


//ii) Mostrar na tela todos os temas de um PodCast de uma plataforma.
// Função auxiliar para percorrer a árvore de temas em ordem e imprimir os nomes dos temas
void percorrerEmOrdemTemas(Tema *raiz) {
    if (raiz != NULL) {
        percorrerEmOrdemTemas(raiz->esquerda);
        printf("- %s\n", raiz->tema);
        percorrerEmOrdemTemas(raiz->direita);
    }
}

// Função para mostrar na tela todos os temas de um PodCast de uma plataforma
void mostrarTemasPodCast(Plataforma *inicio, char nomePlataforma[], char nomePodCast[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    printf("Temas do PodCast \"%s\" da plataforma \"%s\":\n", nomePodCast, nomePlataforma);
    percorrerEmOrdemTemas(podcast->temas_raiz);
}


//iii) Mostrar na tela todos os títulos das entrevistas de um PodCast de uma plataforma dado o tema.
// Função auxiliar para percorrer a árvore de entrevistas em ordem
void percorrerEmOrdemEntrevistas(Entrevista *raiz) {
    if (raiz != NULL) {
        percorrerEmOrdemEntrevistas(raiz->esquerda);
        printf("- %s\n", raiz->titulo);
        percorrerEmOrdemEntrevistas(raiz->direita);
    }
}
// Função para mostrar os títulos das entrevistas de um determinado tema de um PodCast de uma plataforma
void mostrarTitulosEntrevistas(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Buscar a plataforma
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    
    // Buscar o PodCast
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    
    // Buscar o tema
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" não encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    
    // Imprimir os títulos das entrevistas do tema
    printf("Títulos das entrevistas do tema \"%s\" do PodCast \"%s\" da plataforma \"%s\":\n", tema, nomePodCast, nomePlataforma);
    percorrerEmOrdemEntrevistas(temaEncontrado->entrevistas_raiz);
}

//iv) Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e título da entrevista.
void mostrarDadosEntrevista(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" não encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    
    Entrevista *entrevistaAtual = temaEncontrado->entrevistas_raiz;
    while (entrevistaAtual != NULL) {
        if (strcmp(entrevistaAtual->titulo, titulo) == 0) {
            printf("Dados da entrevista:\n");
            printf("Título: %s\n", entrevistaAtual->titulo);
            printf("Entrevistado: %s\n", entrevistaAtual->nome_entrevistado);
            printf("Duração: %d minutos\n", entrevistaAtual->duracao);
            return;
        }
        entrevistaAtual = entrevistaAtual->direita;
    }
    
    printf("Entrevista com o título \"%s\" não encontrada no tema \"%s\" do PodCast \"%s\" da plataforma \"%s\".\n", titulo, tema, nomePodCast, nomePlataforma);
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
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" não encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }

    printf("Entrevistados do tema \"%s\" do PodCast \"%s\" da plataforma \"%s\":\n", tema, nomePodCast, nomePlataforma);

    // Percorrer a árvore de entrevistas e imprimir os nomes dos entrevistados
    imprimirEntrevistados(temaEncontrado->entrevistas_raiz);
}

//vi) Permitir que o usuário possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento.
void adicionarPlataforma(Plataforma **inicio, char nome[]) {
    inserirPlataforma(inicio, nome);
    printf("Plataforma \"%s\" adicionada com sucesso.\n", nome);
}
void adicionarPodCast(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char entrevistador[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    plataforma->podcasts_raiz = inserirPodCastAVL(plataforma->podcasts_raiz, nomePodCast, entrevistador);
    printf("PodCast \"%s\" adicionado na plataforma \"%s\" com sucesso.\n", nomePodCast, nomePlataforma);
}
void adicionarTema(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    podcast->temas_raiz = inserirTemaAVL(podcast->temas_raiz, tema);
    printf("Tema \"%s\" adicionado ao PodCast \"%s\" da plataforma \"%s\" com sucesso.\n", tema, nomePodCast, nomePlataforma);
}
void adicionarEntrevista(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[], char data[], int duracao, char nome_entrevistado[], char especialidade[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" não encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    temaEncontrado->entrevistas_raiz = inserirEntrevista(temaEncontrado->entrevistas_raiz, titulo, "", 0, nome_entrevistado, especialidade);
}

 

/*  vii)Permitir que o usuário possa excluir uma entrevista, um tema, ou um PodCast. 
Para excluir um PodCast, o mesmo não poderá ter nenhum tema cadastrado para ele. 
No caso do tema, ele só poderá ser excluído se não existir nenhuma entrevista cadastrada para ele. 
Para excluir uma plataforma a mesma não poderá ter nenhum PodCast cadastrado para ele.    */
void excluirEntrevista(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaEncontrado = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaEncontrado == NULL) {
        printf("Tema \"%s\" não encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    Entrevista *entrevistaRemover = buscarEntrevista(temaEncontrado->entrevistas_raiz, titulo);
    if (entrevistaRemover == NULL) {
        printf("Entrevista \"%s\" não encontrada no tema \"%s\" do PodCast \"%s\" da plataforma \"%s\".\n", titulo, tema, nomePodCast, nomePlataforma);
        return;
    }
    temaEncontrado->entrevistas_raiz = removerEntrevista(temaEncontrado->entrevistas_raiz, titulo);
    printf("Entrevista \"%s\" do tema \"%s\" do PodCast \"%s\" da plataforma \"%s\" foi removida com sucesso.\n", titulo, tema, nomePodCast, nomePlataforma);
}
void excluirTema(Plataforma *inicio, char nomePlataforma[], char nomePodCast[], char tema[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcast = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcast == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    Tema *temaRemover = buscarTemaAVL(podcast->temas_raiz, tema);
    if (temaRemover == NULL) {
        printf("Tema \"%s\" não encontrado no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    if (temaRemover->entrevistas_raiz != NULL) {
        printf("Não é possível excluir o tema \"%s\" pois ainda há entrevistas associadas a ele no PodCast \"%s\" da plataforma \"%s\".\n", tema, nomePodCast, nomePlataforma);
        return;
    }
    podcast->temas_raiz = removerTemaAVL(podcast->temas_raiz, tema);
    printf("Tema \"%s\" do PodCast \"%s\" da plataforma \"%s\" foi removido com sucesso.\n", tema, nomePodCast, nomePlataforma);
}
void excluirPodCast(Plataforma *inicio, char nomePlataforma[], char nomePodCast[]) {
    Plataforma *plataforma = buscarPlataforma(inicio, nomePlataforma);
    if (plataforma == NULL) {
        printf("Plataforma \"%s\" não encontrada.\n", nomePlataforma);
        return;
    }
    PodCast *podcastRemover = buscarPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    if (podcastRemover == NULL) {
        printf("PodCast \"%s\" não encontrado na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    if (podcastRemover->temas_raiz != NULL) {
        printf("Não é possível excluir o PodCast \"%s\" pois ainda há temas associados a ele na plataforma \"%s\".\n", nomePodCast, nomePlataforma);
        return;
    }
    plataforma->podcasts_raiz = removerPodCastAVL(plataforma->podcasts_raiz, nomePodCast);
    printf("PodCast \"%s\" da plataforma \"%s\" foi removido com sucesso.\n", nomePodCast, nomePlataforma);
}


                                                        // Função principal com o menu de opções


int main() {
	// Inicialização da lista de plataformas
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
        printf("5. Mostrar os nomes dos PodCast em ordem alfabética\n");
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
            case 1: //vi) Permitir que o usuário possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento
            	 printf("Digite o nome da plataforma: ");
				 scanf(" %s", nomePlataforma);
		         //Chamada para adicionar plataforma
		         adicionarPlataforma(&inicio, nomePlataforma);
		         break;
		     case 2:  // Permitir que o usuário possa adicionar podcast e analisar o tempo de execução 30 vezes
			    // Solicita ao usuário que insira os dados
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
			        
			        // Soma os tempos para calcular a média posteriormente
			        soma += tempo_decorrido;
			    }
			    
			    // Calcula a média dos tempos decorridos
			    double media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
			    break;
			case 3:  //vi) Permitir que o usuário possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                //chamada da funcao para adicionar tema
                adicionarTema(inicio, nomePlataforma, nomePodCast, tema);
                break;
            case 4:  //vi) Permitir que o usuário possa adicionar plataforma, PodCast, Tema, Entrevistas a qualquer momento
            	printf("Digite o nome da plataforma: ");
			    scanf(" %s", nomePlataforma);
			    printf("Digite o nome do PodCast: ");
			    scanf(" %s", nomePodCast);
			    printf("Digite o nome do tema: ");
			    scanf(" %s", tema);
			    printf("Digite o título da entrevista: ");
			    scanf(" %[^\n]s", tituloEntrevista);
			    printf("Digite a data da entrevista (DD/MM/AAAA): ");
			    scanf(" %s", data);
			    printf("Digite a duração da entrevista (em minutos): ");
			    scanf(" %d", &duracao);
			    printf("Digite o nome do entrevistado: ");
			    scanf(" %s", nomeEntrevistador);
			    printf("Digite a especialidade do entrevistado: ");
			    scanf(" %s", especialidade);
			    // Chamada da função para adicionar a entrevista
			    adicionarEntrevista(inicio, nomePlataforma, nomePodCast, tema, tituloEntrevista, data, duracao, nomeEntrevistador, especialidade);
			    break;
            case 5:   //i) Mostrar na tela todos os nomes dos PodCast em ordem alfabética de uma dada plataforma
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                // Chamada da função para mostrar os nomes dos PodCasts em ordem alfabética
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
    			
	                // Chamada da função para mostrar os temas de um PodCast
	                mostrarTemasPodCast(inicio, nomePlataforma, nomePodCast);
	                
	                // Marca o tempo final
			        gettimeofday(&fim, NULL);
			        
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - _inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - _inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a média posteriormente
			        soma += tempo_decorrido;
			    }
			    
			    // Calcula a média dos tempos decorridos
			    media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
			    break;
                
            case 7:   //iii) Mostrar na tela todos os títulos das entrevistas de um PodCast de uma plataforma dado o tema
            	printf("Digite o tema: ");
                scanf(" %s", tema);
                // Chamada da função para mostrar os títulos das entrevistas
                mostrarTitulosEntrevistas(inicio, nomePlataforma, nomePodCast, tema); 
                break;
            case 8:  //iv) Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e título da entrevista
				printf("Digite o nome da plataforma: ");
			    scanf(" %s", nomePlataforma);
			    printf("Digite o nome do PodCast: ");
			    scanf(" %s", nomePodCast);
			    printf("Digite o nome do tema: ");
			    scanf(" %s", tema);
			    printf("Digite o título da entrevista: ");
			    scanf(" %[^\n]s", tituloEntrevista);
			    // Chamada da função para mostrar os dados de uma entrevista de um Podcast
			    mostrarDadosEntrevista(inicio, nomePlataforma, nomePodCast, tema, tituloEntrevista);
			    break;
            case 9:  //v) Mostrar na tela todos os entrevistados de um determinado tema de um PodCast de uma plataforma
            	printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                // Chamada da função para mostrar os entrevistados
                mostrarEntrevistadosTema(inicio, nomePlataforma, nomePodCast, tema);
                break;
            case 10:  //vii)Permitir que o usuário possa excluir uma entrevista, um tema, ou um PodCast
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                printf("Digite o título da entrevista: ");
                scanf(" %[^\n]s", tituloEntrevista);
                // Chamada da função para excluir uma entrevista
                excluirEntrevista(inicio, nomePlataforma, nomePodCast, tema, tituloEntrevista);
                break;
            case 11:  //vii)Permitir que o usuário possa excluir uma entrevista, um tema, ou um PodCast
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                // Chamada da função para excluir um tema
                excluirTema(inicio, nomePlataforma, nomePodCast, tema);
                break;
            case 12:  //vii)Permitir que o usuário possa excluir uma entrevista, um tema, ou um PodCast
            	printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                // Chamada da função para excluir um podcast
            	excluirPodCast(inicio, nomePlataforma, nomePodCast);
            	break;
            case 13:
            	removerPlataforma(&inicio, nomePlataforma);
            	break;
            case 0:
                printf("Encerrando o programa\n");
                break;
            default:
                printf("Opcao inválida! Tente novamente\n");
       }
    }while (opcao != 0);
    
    return 0;
}
    
    





