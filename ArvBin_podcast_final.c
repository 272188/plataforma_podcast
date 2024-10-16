/*
1) (3,0 pontos) Faça um programa em C que cadastre: 
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
} Entrevista;


// Estrutura para um tema
typedef struct Tema {
    char tema[100];
    Entrevista *entrevistas_raiz; // Raiz da árvore de entrevistas
    struct Tema *esquerda;
    struct Tema *direita;
} Tema;

// Estrutura para arvore de PodCast
typedef struct PodCast {
    char nome[100];
    char entrevistador[100];
    struct Tema *temas_raiz; // Raiz da árvore de temas
    struct PodCast *esquerda;
    struct PodCast *direita;
} PodCast;


// Estrutura para uma Plataforma de Áudio e Vídeos
typedef struct Plataforma {
    char nome[100];
    PodCast *podcasts;
    struct Plataforma *prox;
} Plataforma;


                                                                //Bloco arvore binária de Entrevista


// Função para alocar espaço dinamicamente para uma entrevista
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
    }
    return nova_entrevista;
} 

// Função para inserir uma entrevista em uma árvore
int insereEntrevista(Entrevista **R, Entrevista *nova_entrevista) {
    int inseriu = 1;
    if (*R == NULL)
        *R = nova_entrevista;
    else if (strcmp(nova_entrevista->titulo, (*R)->titulo) < 0)
        inseriu = insereEntrevista(&((*R)->esquerda), nova_entrevista);
    else if (strcmp(nova_entrevista->titulo, (*R)->titulo) > 0)
        inseriu = insereEntrevista(&((*R)->direita), nova_entrevista);
    else
        inseriu = 0;
    return inseriu;
}

// Função para buscar uma entrevista em uma árvore
Entrevista* buscarEntrevista(Entrevista *R, char titulo[]) {
    Entrevista *achou = NULL;
    if (R != NULL) {
        if (strcmp(titulo, R->titulo) == 0)
            achou = R;
        else if (strcmp(titulo, R->titulo) < 0)
            achou = buscarEntrevista(R->esquerda, titulo);
        else
            achou = buscarEntrevista(R->direita, titulo);
    }
    return achou;
}


// Função para verificar se um nó é folha em uma árvore de entrevistas
int ehFolhaEntrevista(Entrevista *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

// Verifica se o nó possui um único filho em uma árvore de Entrevistas
Entrevista* unicoFilhoEntrevista(Entrevista* R) {
    Entrevista* filho = NULL;
    if (R->esquerda != NULL && R->direita == NULL) {
        filho = R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        filho = R->direita;
    }
    return filho;
}


// Função auxiliar para encontrar a entrevista com o maior título em uma subárvore
Entrevista* maiorInfoEntrevista(Entrevista *R) {
    Entrevista *maior = NULL;
    if (R->direita != NULL)
        maior = maiorInfoEntrevista(R->direita);
    else
        maior = R;
    return maior;
}

//REMOCAO
// Função para remover uma entrevista de uma árvore
int removeEntrevista(Entrevista **R, char titulo[]) {
    int removeu = 1;
    if (*R != NULL) {
        if (strcmp(titulo, (*R)->titulo) == 0) {
            Entrevista *aux, *filho;
            if ((filho = unicoFilhoEntrevista(*R)) != NULL) {
                aux = *R;
                *R = filho;
                free(aux);
            } else {
                // Encontra a entrevista com o maior título na subárvore esquerda
                Entrevista *maior = maiorInfoEntrevista((*R)->esquerda);
                strcpy((*R)->titulo, maior->titulo);
                removeEntrevista(&((*R)->esquerda), maior->titulo);
            }
        } else if (strcmp(titulo, (*R)->titulo) < 0)
            removeu = removeEntrevista(&((*R)->esquerda), titulo);
        else
            removeu = removeEntrevista(&((*R)->direita), titulo);
    } else
        removeu = 0;
    return removeu;
}

                                                               //Bloco arvore binária de Temas
																


// Função para alocar espaço dinamicamente para um tema
void alocaTema(Tema **N, char tema[]) {
    *N = (Tema*)malloc(sizeof(Tema));
    if (*N != NULL) {
        strcpy((*N)->tema, tema);
        (*N)->entrevistas_raiz = NULL; // Inicializa a raiz da árvore de entrevistas como NULL
        (*N)->esquerda = NULL;
        (*N)->direita = NULL;
    }
}

// Função para inserir um tema em uma árvore
int insereTema(Tema **R, Tema *novo_tema) {
    int inseriu = 1;
    if (*R == NULL)
        *R = novo_tema;
    else if (strcmp(novo_tema->tema, (*R)->tema) < 0)
        inseriu = insereTema(&((*R)->esquerda), novo_tema);
    else if (strcmp(novo_tema->tema, (*R)->tema) > 0)
        inseriu = insereTema(&((*R)->direita), novo_tema);
    else
        inseriu = 0;
    return inseriu;
}


// Função para buscar um tema em uma árvore
Tema* buscarTemaArv(Tema *R, char tema[]) {
    Tema *achou = NULL;
    if (R != NULL) {
        if (strcmp(tema, R->tema) == 0)
            achou = R;
        else if (strcmp(tema, R->tema) < 0)
            achou = buscarTemaArv(R->esquerda, tema);
        else
            achou = buscarTemaArv(R->direita, tema);
    }
    return achou;
}


// Função para verificar se um nó é folha em uma árvore de temas
int ehFolhaTema(Tema *R) {
    return (R->esquerda == NULL && R->direita == NULL);
}

// Verifica se o nó possui um único filho em uma árvore de Temas
Tema* unicoFilhoTema(Tema* R) {
    Tema* filho = NULL;
    if (R->esquerda != NULL && R->direita == NULL) {
        filho = R->esquerda;
    } else if (R->esquerda == NULL && R->direita != NULL) {
        filho = R->direita;
    }
    return filho;
}


// Função auxiliar para encontrar o tema com o maior nome em uma subárvore
Tema* maiorInfoTema(Tema *R) {
    Tema *maior = NULL;
    if (R->direita != NULL)
        maior = maiorInfoTema(R->direita);
    else
        maior = R;
    return maior;
}

//REMOCAO
// Função para remover um tema de uma árvore
int removeTema(Tema **R, char tema[]) {
    int removeu = 1;
    if (*R != NULL) {
        if (strcmp(tema, (*R)->tema) == 0) {
            Tema *aux, *filho;
            if ((*R)->entrevistas_raiz == NULL) { // Verifica se não há entrevistas associadas ao tema
                aux = *R;
                *R = NULL;
                free(aux);
            } else if ((filho = unicoFilhoTema(*R)) != NULL) {
                aux = *R;
                *R = filho;
                free(aux);
            } else {
                // Encontra o tema com o maior nome na subárvore esquerda
                Tema *maior = maiorInfoTema((*R)->esquerda);
                strcpy((*R)->tema, maior->tema);
                removeTema(&((*R)->esquerda), maior->tema);
            }
        } else if (strcmp(tema, (*R)->tema) < 0)
            removeu = removeTema(&((*R)->esquerda), tema);
        else
            removeu = removeTema(&((*R)->direita), tema);
    } else
        removeu = 0;
    return removeu;
}
													

                                                               //Bloco arvore binária de PodCast
// Lógica para alocar espaço dinamicamente em uma árvore
void alocaPodCast (PodCast **N, char nome[], char entrevistador[]){
	*N = (PodCast *) malloc (sizeof(PodCast));
	if (*N != NULL){
		strcpy((*N)->nome, nome);
		strcpy((*N)->entrevistador, entrevistador);
		(*N)->temas_raiz = NULL; // Inicializa a árvore de temas como NULL
		(*N)->esquerda = NULL;
		(*N)->direita = NULL;	
	}
}

// Lógica para inserir PodCasts em uma árvore
int inserePodCast(PodCast **R, PodCast *No){
	int inseriu = 1;
	if (*R == NULL)
		*R = No;
	else if (strcmp(No->nome, (*R)->nome) < 0)
		inseriu = inserePodCast(&((*R)->esquerda), No);
	else if (strcmp(No->nome, (*R)->nome) > 0)
		inseriu = inserePodCast(&((*R)->direita), No);
	else
		inseriu = 0;
	return(inseriu);
}

// Lógica para buscar PodCasts em uma árvore
PodCast *BuscarPodCast(PodCast *R, char nome[]){
	PodCast *Achou = NULL;
	if (R != NULL){
		if (strcmp(nome, R->nome) == 0)
			Achou = R;
		else if (strcmp(nome, R->nome) < 0)
			Achou = BuscarPodCast(R->esquerda, nome);
		else
			Achou = BuscarPodCast(R->direita, nome);
	}
	return(Achou);
}



// Função auxiliar recursiva para mostrar os nomes dos PodCasts em ordem alfabética
void mostrarNomesPodCastsRecursivo(PodCast *raiz) {
    if (raiz != NULL) {
        mostrarNomesPodCastsRecursivo(raiz->esquerda); // Exibe os nomes na subárvore esquerda
        printf("- %s\n", raiz->nome); // Exibe o nome do PodCast
        mostrarNomesPodCastsRecursivo(raiz->direita); // Exibe os nomes na subárvore direita
    }
}


// Função auxiliar recursiva para mostrar os temas de um PodCast
void mostrarTemasRecursivo(Tema *raiz) {
    if (raiz != NULL) {
        mostrarTemasRecursivo(raiz->esquerda); // Exibe os temas na subárvore esquerda
        printf("- %s\n", raiz->tema); // Exibe o tema
        mostrarTemasRecursivo(raiz->direita); // Exibe os temas na subárvore direita
    }
}


// Função auxiliar recursiva para listar todos os PodCasts em ordem alfabética
void listarPodCastsRecursivo(PodCast *raiz) {
    if (raiz != NULL) {
        listarPodCastsRecursivo(raiz->esquerda); // Lista os PodCasts na subárvore esquerda
        printf("- %s\n", raiz->nome); // Exibe o nome do PodCast
        listarPodCastsRecursivo(raiz->direita); // Lista os PodCasts na subárvore direita
    }
}


// Função para listar todas as entrevistas de um tema
void listarEntrevistasSubarvore(Entrevista *raiz) {
    if (raiz != NULL) {
        listarEntrevistasSubarvore(raiz->esquerda); // Lista as entrevistas na subárvore esquerda
        printf("- Título: %s, Data: %s, Duração: %d minutos, Entrevistado: %s, Especialidade: %s\n", raiz->titulo, raiz->data, raiz->duracao, raiz->nome_entrevistado, raiz->especialidade); // Exibe as informações da entrevista
        listarEntrevistasSubarvore(raiz->direita); // Lista as entrevistas na subárvore direita
    }
}


// Verifica se é nó folha em uma árvore de PodCasts
int ehfolhaPodCast(PodCast *R){
	int aux = 0;
	if (R->esquerda == NULL && R->direita == NULL){
		aux = 1;
	}
	return aux;
}

// Verifica se o nó possui um único filho em uma árvore de PodCasts
PodCast *unicoFilhoPodCast(PodCast *R){
	PodCast *filho = NULL;
	if (R->esquerda != NULL && R->direita == NULL){
		filho = R->esquerda;
	}
	else if (R->esquerda == NULL && R->direita != NULL){
		filho = R->direita;
	}
	return filho;
}

// Encontra o PodCast com o maior nome/informacao em uma árvore de PodCasts
PodCast *maiorInfoPodCast(PodCast *R){
	PodCast *maior = NULL;
	if (R->direita != NULL){
		maior = maiorInfoPodCast(R->direita);
	}
	else{
		maior = R;
	}
	return maior;
}


//REMOCAO
// Lógica para remover PodCasts de uma árvore
int removePodCast(PodCast **R, char nome[]){
	int removeu = 1;
	if (*R != NULL){
		if (strcmp(nome, (*R)->nome) == 0){
			PodCast *aux, *filho;
			if ((*R)->temas_raiz == NULL){ // Verifica se não há temas associados ao PodCast
				aux = *R;
				*R = NULL;
				free(aux);
			}
			else if ((filho = unicoFilhoPodCast(*R)) != NULL){
				aux = *R;
				*R = filho;
				free(aux);
			}
			else{
				// Encontra o PodCast com o maior nome na subárvore esquerda
				PodCast *maior = maiorInfoPodCast((*R)->esquerda);
				strcpy((*R)->nome, maior->nome);
				removePodCast(&((*R)->esquerda), maior->nome);
			}
		}
		else if (strcmp(nome, (*R)->nome) < 0)
			removeu = removePodCast(&((*R)->esquerda), nome);
		else
			removeu = removePodCast(&((*R)->direita), nome);
	}
	else
		removeu = 0;
	return(removeu);
}

                                                      //Bloco lista encadeada para uma Plataforma de audio e video


// Função para alocar espaço dinamicamente para uma plataforma
void alocaPlataforma(Plataforma **N, char nome[]) {
    *N = (Plataforma*)malloc(sizeof(Plataforma));
    if (*N != NULL) {
        strcpy((*N)->nome, nome);
        (*N)->podcasts = NULL; // Inicializa a raiz da árvore de PodCasts como NULL
        (*N)->prox = NULL;
    }
}

// Função para inserir uma nova plataforma no final da lista
void inserePlataforma(Plataforma **cabeca, Plataforma *nova_plataforma) {
    if (*cabeca == NULL)
        *cabeca = nova_plataforma;
    else {
        Plataforma *temp = *cabeca;
        while (temp->prox != NULL)
            temp = temp->prox;
        temp->prox = nova_plataforma;
    }
}


// Função para buscar uma plataforma na lista
Plataforma* buscarPlataforma(Plataforma *cabeca, char nome[]) {
    Plataforma *achou = NULL;
    Plataforma *temp = cabeca;
    while (temp != NULL) {
        if (strcmp(nome, temp->nome) == 0) {
            achou = temp;
            break;
        }
        temp = temp->prox;
    }
    return achou;
}

// Função para buscar um tema em um PodCast de uma plataforma
void buscarTema(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                printf("Tema '%s' encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            } else {
                printf("Tema '%s' não encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}

// Função para buscar um PodCast em uma plataforma
void buscarPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            printf("PodCast '%s' encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}


// Função auxiliar recursiva para listar todas as entrevistas de um PodCast
void listarEntrevistasRecursivo(Tema *raiz) {
    if (raiz != NULL) {
        listarEntrevistasRecursivo(raiz->esquerda); // Lista as entrevistas na subárvore esquerda
        printf("Tema: %s\n", raiz->tema);
        listarEntrevistasSubarvore(raiz->entrevistas_raiz); // Chama a função para listar as entrevistas do tema
        listarEntrevistasRecursivo(raiz->direita); // Lista as entrevistas na subárvore direita
    }
}

// Função para listar todas as entrevistas de um PodCast de uma plataforma
void listarEntrevistasPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            printf("Entrevistas do PodCast '%s' na plataforma '%s':\n", nomePodCast, nomePlataforma);
            listarEntrevistasRecursivo(podcast->temas_raiz); // Chama a função auxiliar para listar as entrevistas recursivamente
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}

// Função para listar todos os PodCasts de uma plataforma em ordem alfabética
void listarPodCastsOrdemAlfabetica(Plataforma *cabeca, char nomePlataforma[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        printf("PodCasts em ordem alfabética na plataforma '%s':\n", nomePlataforma);
        listarPodCastsRecursivo(plataforma->podcasts); // Chama a função auxiliar para listar os PodCasts recursivamente
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}

// Função para listar todas as plataformas
void listarPlataformas(Plataforma *plataformas) {
    if (plataformas == NULL) {
        printf("Não há plataformas cadastradas.\n");
    } else {
        Plataforma *atual = plataformas;
        printf("Lista de Plataformas:\n");
        while (atual != NULL) {
            printf("- %s\n", atual->nome);
            atual = atual->prox;
        }
    }
}


//REMOCAO 
// Função para excluir uma entrevista de um determinado tema em um PodCast de uma plataforma
void excluirEntrevista(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[], char tituloEntrevista[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                // Busca a entrevista na árvore de entrevistas do tema
                Entrevista *entrevista = buscarEntrevista(temaEncontrado->entrevistas_raiz, tituloEntrevista);
                
                if (entrevista != NULL) {
                    // Remove a entrevista da árvore de entrevistas do tema
                    if (removeEntrevista(&(temaEncontrado->entrevistas_raiz), tituloEntrevista)) {
                        printf("Entrevista '%s' removida com sucesso do tema '%s' no PodCast '%s' na plataforma '%s'.\n", tituloEntrevista, tema, nomePodCast, nomePlataforma);
                    } else {
                        printf("Falha ao remover a entrevista '%s' do tema '%s' no PodCast '%s' na plataforma '%s'.\n", tituloEntrevista, tema, nomePodCast, nomePlataforma);
                    }
                } else {
                    printf("Entrevista '%s' não encontrada no tema '%s' no PodCast '%s' na plataforma '%s'.\n", tituloEntrevista, tema, nomePodCast, nomePlataforma);
                }
            } else {
                printf("Tema '%s' não encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}


// Função para excluir um PodCast de uma plataforma
void excluirPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Remove o PodCast da árvore de PodCasts da plataforma
        if (removePodCast(&(plataforma->podcasts), nomePodCast)) {
            printf("PodCast '%s' removido com sucesso da plataforma '%s'.\n", nomePodCast, nomePlataforma);
        } else {
            printf("Falha ao remover o PodCast '%s' da plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}


// Função para verificar se a lista de PodCasts de uma plataforma está vazia
int listaPodCastsVazia(Plataforma *plataforma) {
    return (plataforma->podcasts == NULL);
}


                                                                 //FUNCOES ESPECIFICAS DA QUESTAO 1

//i) Funcao para Mostrar na tela todos os nomes dos PodCast em ordem alfabética de uma dada plataforma.
void mostrarNomesPodCasts(Plataforma *cabeca, char nomePlataforma[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);
    
    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        printf("Nomes dos PodCasts em ordem alfabetica na plataforma %s:\n", nomePlataforma);
        mostrarNomesPodCastsRecursivo(plataforma->podcasts); // Chamada recursiva para exibir os nomes em ordem
    } else {
        printf("Plataforma %s nao encontrada.\n", nomePlataforma);
    }
}

//ii) Funcao para Mostrar na tela todos os temas de um PodCast de uma plataforma.
void mostrarTemasPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            printf("Temas do PodCast %s na plataforma %s:\n", nomePodCast, nomePlataforma);
            mostrarTemasRecursivo(podcast->temas_raiz); // Chamada recursiva para exibir os temas
        } else {
            printf("PodCast %s nao encontrado na plataforma %s.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma %s nao encontrada.\n", nomePlataforma);
    }
}


//iii) Funcao para Mostrar na tela todos os títulos das entrevistas de um PodCast de uma plataforma dado o tema.
// Função auxiliar recursiva para mostrar os títulos das entrevistas de um tema
void mostrarTitulosEntrevistasRecursivo(Entrevista *raiz) {
    if (raiz != NULL) {
        mostrarTitulosEntrevistasRecursivo(raiz->esquerda); // Exibe os títulos na subárvore esquerda
        printf("- %s\n", raiz->titulo); // Exibe o título da entrevista
        mostrarTitulosEntrevistasRecursivo(raiz->direita); // Exibe os títulos na subárvore direita
    }
}
void mostrarTitulosEntrevistas(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                printf("Titulos das entrevistas do tema %s no PodCast %s na plataforma %s:\n", tema, nomePodCast, nomePlataforma);
                mostrarTitulosEntrevistasRecursivo(temaEncontrado->entrevistas_raiz); // Chamada recursiva para exibir os títulos
            } else {
                printf("Tema %s nao encontrado no PodCast %s na plataforma %s.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast %s nao encontrado na plataforma %s.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma %s nao encontrada.\n", nomePlataforma);
    }
}


//iv) Funcao p/ Mostrar na tela todos os dados de uma entrevista de um Podcast de uma dada plataforma, tema e título da entrevista.
void mostrarDadosEntrevista(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                // Busca a entrevista no tema
                Entrevista *entrevista = buscarEntrevista(temaEncontrado->entrevistas_raiz, titulo);
                
                if (entrevista != NULL) {
                    // Exibe os dados da entrevista
                    printf("Dados da entrevista:\n");
                    printf("Título: %s\n", entrevista->titulo);
                    printf("Data: %s\n", entrevista->data);
                    printf("Duração: %d minutos\n", entrevista->duracao);
                    printf("Nome do entrevistado: %s\n", entrevista->nome_entrevistado);
                    printf("Especialidade do entrevistado: %s\n", entrevista->especialidade);
                } else {
                    printf("Entrevista '%s' não encontrada no tema '%s' do PodCast '%s' na plataforma '%s'.\n", titulo, tema, nomePodCast, nomePlataforma);
                }
            } else {
                printf("Tema '%s' não encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}

// Função para buscar uma entrevista em um determinado tema de um PodCast de uma plataforma

void buscarEntrevistaTemaPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[], char tituloEntrevista[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                // Busca a entrevista na árvore de entrevistas do tema
                Entrevista *entrevista = buscarEntrevista(temaEncontrado->entrevistas_raiz, tituloEntrevista);
                
                if (entrevista != NULL) {
                    printf("Entrevista encontrada:\n");
                    printf("Titulo: %s\n", entrevista->titulo);
                    printf("Data: %s\n", entrevista->data);
                    printf("Duração: %d minutos\n", entrevista->duracao);
                    printf("Nome do entrevistado: %s\n", entrevista->nome_entrevistado);
                    printf("Especialidade do entrevistado: %s\n", entrevista->especialidade);
                } else {
                    printf("Entrevista '%s' não encontrada no tema '%s' no PodCast '%s' na plataforma '%s'.\n", tituloEntrevista, tema, nomePodCast, nomePlataforma);
                }
            } else {
                printf("Tema '%s' não encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}
//v) Funcao p/ Mostrar na tela todos os entrevistados de um determinado tema de um PodCast de uma plataforma.
// Função auxiliar para mostrar todos os entrevistados de um tema
void mostrarEntrevistados(Entrevista *raiz) {
    if (raiz != NULL) {
        // Exibe os dados da entrevista
        printf("Nome do entrevistado: %s\n", raiz->nome_entrevistado);
        // Recursivamente, mostra os entrevistados da subárvore esquerda e depois da subárvore direita
        mostrarEntrevistados(raiz->esquerda);
        mostrarEntrevistados(raiz->direita);
    }
}
void mostrarDadosEntrevistasTema(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                if (temaEncontrado->entrevistas_raiz != NULL) {
                    printf("Entrevistados do tema '%s' no PodCast '%s' na plataforma '%s':\n", tema, nomePodCast, nomePlataforma);
                    // Percorre a árvore de entrevistas do tema e exibe os dados de cada entrevista
                    mostrarEntrevistados(temaEncontrado->entrevistas_raiz);
                } else {
                    printf("Não há entrevistas cadastradas para o tema '%s' no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
                }
            } else {
                printf("Tema '%s' não encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}


//vi) Funcao que Permitie ao usuário adicione uma nova entrevista a um tema de um Podcast de uma dada plataforma
void adicionarEntrevista(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[], char data[], int duracao, char nomeEntrevistado[], char especialidade[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
    	printf("ok");
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Busca o tema na árvore de temas do PodCast
            Tema *temaEncontrado = buscarTemaArv(podcast->temas_raiz, tema);
            
            if (temaEncontrado != NULL) {
                // Verifica se a entrevista já existe no tema
                if (buscarEntrevista(temaEncontrado->entrevistas_raiz, titulo) == NULL) {
                    // Aloca espaço para a nova entrevista
                    Entrevista *novaEntrevista = alocaEntrevista(titulo, data, duracao, nomeEntrevistado, especialidade);
                    
                    if (novaEntrevista != NULL) {
                        // Insere a nova entrevista na árvore de entrevistas do tema
                        insereEntrevista(&(temaEncontrado->entrevistas_raiz), novaEntrevista);
                        printf("Entrevista '%s' adicionada com sucesso ao tema '%s' do PodCast '%s' na plataforma '%s'.\n", titulo, tema, nomePodCast, nomePlataforma);
                    } else {
                        printf("Erro ao alocar memória para a nova entrevista.\n");
                    }
                } else {
                    printf("A entrevista '%s' já existe no tema '%s' do PodCast '%s' na plataforma '%s'.\n", titulo, tema, nomePodCast, nomePlataforma);
                }
            } else {
                printf("Tema '%s' não encontrado no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}
void adicionarTema(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Verifica se o tema já existe na árvore de temas do PodCast
            if (buscarTemaArv(podcast->temas_raiz, tema) == NULL) {
                // Aloca espaço para o novo tema
                Tema *novoTema = NULL;
                alocaTema(&novoTema, tema);
                
                if (novoTema != NULL) {
                    // Insere o novo tema na árvore de temas do PodCast
                    insereTema(&(podcast->temas_raiz), novoTema);
                    printf("Tema '%s' adicionado com sucesso ao PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
                } else {
                    printf("Erro ao alocar memória para o novo tema.\n");
                }
            } else {
                printf("O tema '%s' já existe no PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}
void adicionarPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char entrevistador[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Verifica se o PodCast já existe na plataforma
        if (BuscarPodCast(plataforma->podcasts, nomePodCast) == NULL) {
            PodCast *novoPodCast;
            alocaPodCast(&novoPodCast, nomePodCast, entrevistador); // Aloca espaço para o novo PodCast
            if (novoPodCast != NULL) {
                // Insere o novo PodCast na árvore de PodCasts da plataforma
                if (inserePodCast(&(plataforma->podcasts), novoPodCast)) {
                    printf("PodCast %s adicionado com sucesso na plataforma %s!\n", nomePodCast, nomePlataforma);
                } else {
                    printf("Falha ao adicionar o PodCast %s na plataforma %s.\n", nomePodCast, nomePlataforma);
                }
            } else {
                printf("Falha ao alocar espaço para o PodCast %s.\n", nomePodCast);
            }
        } else {
            printf("O PodCast %s já existe na plataforma %s.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma %s não encontrada.\n", nomePlataforma);
    }
}


//vi) funcao que Permitir ao usuário padicione plataforma a qualquer momento.
void adicionarPlataforma(Plataforma **cabeca, char nomePlataforma[]) {
    Plataforma *novaPlataforma;
    alocaPlataforma(&novaPlataforma, nomePlataforma); // Aloca espaço para a nova plataforma
    if (novaPlataforma != NULL) {
        inserePlataforma(cabeca, novaPlataforma); // Insere a nova plataforma na lista
        printf("Plataforma %s adicionada com sucesso!\n", nomePlataforma);
    } else {
        printf("Falha ao adicionar a plataforma %s.\n", nomePlataforma);
    }
}


/*VII)Permitir que o usuário possa excluir uma entrevista, um tema, ou um PodCast. Para excluir um PodCast, 
o mesmo não poderá ter nenhum tema cadastrado para ele. No caso do tema, ele só poderá ser excluído 
se não existir nenhuma entrevista cadastrada para ele. Para excluir uma plataforma a mesma não poderá 
ter nenhum PodCast cadastrado para ele.*/

// Função para excluir um tema de um PodCast de uma plataforma
void excluirTema(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]) {
    // Busca a plataforma na lista
    Plataforma *plataforma = buscarPlataforma(cabeca, nomePlataforma);

    // Verifica se a plataforma foi encontrada
    if (plataforma != NULL) {
        // Busca o PodCast na árvore de PodCasts da plataforma
        PodCast *podcast = BuscarPodCast(plataforma->podcasts, nomePodCast);
        
        if (podcast != NULL) {
            // Verifica se há entrevistas associadas ao tema
            if (podcast->temas_raiz != NULL) {
                printf("Não é possível excluir o tema '%s' do PodCast '%s' na plataforma '%s' porque existem entrevistas associadas a ele.\n", tema, nomePodCast, nomePlataforma);
                return;
            }

            // Remove o tema da árvore de temas do PodCast
            if (removeTema(&(podcast->temas_raiz), tema)) {
                printf("Tema '%s' removido com sucesso do PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            } else {
                printf("Falha ao remover o tema '%s' do PodCast '%s' na plataforma '%s'.\n", tema, nomePodCast, nomePlataforma);
            }
        } else {
            printf("PodCast '%s' não encontrado na plataforma '%s'.\n", nomePodCast, nomePlataforma);
        }
    } else {
        printf("Plataforma '%s' não encontrada.\n", nomePlataforma);
    }
}


// Função para remover uma plataforma da lista
int removePlataforma(Plataforma **cabeca, char nome[]) {
    int removeu = 0;
    Plataforma *temp = *cabeca;
    Plataforma *anterior = NULL;

    // Busca a plataforma na lista
    while (temp != NULL && strcmp(temp->nome, nome) != 0) {
        anterior = temp;
        temp = temp->prox;
    }

    // Se a plataforma foi encontrada
    if (temp != NULL) {
        // Verifica se há Podcasts associados à plataforma
        if (temp->podcasts != NULL) {
            printf("Não é possível excluir a plataforma '%s' porque existem Podcasts associados a ela.\n", nome);
            return 0;
        }

        // Remove a plataforma da lista
        if (anterior == NULL)
            *cabeca = temp->prox;
        else
            anterior->prox = temp->prox;
        free(temp);
        removeu = 1;
    } else {
        printf("Plataforma '%s' não encontrada.\n", nome);
    }
    return removeu;
}

    // Declaração escopo das funções
    void adicionarEntrevista(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[], char data[], int duracao, char nomeEntrevistado[], char especialidade[]);
    void adicionarTema(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]);
    void adicionarPodCast(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char entrevistador[]);
    void adicionarPlataforma(Plataforma **cabeca, char nomePlataforma[]);
    void mostrarTitulosEntrevistas(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]);
    void mostrarDadosEntrevista(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]);
    void mostrarDadosEntrevistasTema(Plataforma *cabeca, char nomePlataforma[], char nomePodCast[], char tema[]);
    int removePlataforma(Plataforma **cabeca, char nome[]);
    void mostrarEntrevistasTema(Plataforma *plataformas, char nomePlataforma[], char nomePodCast[], char tema[]);
    void mostrarDetalhesEntrevista(Plataforma *plataformas, char nomePlataforma[], char nomePodCast[], char tema[], char titulo[]);
    void listarPlataformas(Plataforma *plataformas);
    

                                                        ///////////Funcao principal

int main() {
	Plataforma *plataformas = NULL;
	
	char nomePlataforma[100], nomePodCast[100], nomeEntrevistador[100], tema[100], tituloEntrevista[100], data[20], especialidade[100]; 
	int duracao, i=0, opcao;
    double soma, media, tempo_decorrido, tempDecorrido[30];
    struct timeval inicio, fim;

    do {
        printf("\nMenu:\n");
        printf("1. Adicionar Plataforma\n");
        printf("2. Adicionar PodCast\n");
        printf("3. Adicionar Tema\n");
        printf("4. Adicionar Entrevista\n");
        printf("5. Mostrar PodCasts de uma Plataforma\n");
        printf("6. Mostrar Temas de um PodCast\n");
        printf("7. Mostrar Entrevistas de um Tema\n");
        printf("8. Mostrar Detalhes de uma Entrevista\n");
        printf("9. Mostrar Entrevistados de um Tema\n");
        printf("10. Excluir Entrevista\n");
        printf("11. Excluir Tema\n");
        printf("12. Excluir PodCast\n");
        printf("13. Excluir Plataforma\n");
        printf("14. Listar Todas as Plataformas\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf(" %d", &opcao);

        switch (opcao) {
            case 1:
            	printf("Digite o nome da plataforma: ");
		        scanf(" %s", nomePlataforma);
		        // Chamada para adicionar plataforma
		        adicionarPlataforma(&plataformas, nomePlataforma);
		        break;
			case 2:
            	// Solicitação do nome da plataforma e do PodCast dentro do case
    			printf("Digite o nome da plataforma: ");
			    scanf(" %s", nomePlataforma);
			    printf("Digite o nome do PodCast: ");
			    scanf(" %s", nomePodCast);
			    printf("Digite o nome do entrevistador: ");
			    scanf(" %s", nomeEntrevistador);
			    
			    for (i = 0; i < 30; i++) {
			        // Marca o tempo inicial
			        gettimeofday(&inicio, NULL);
    			
				    // Chamada para adicionar PodCast
				    adicionarPodCast(plataformas, nomePlataforma, nomePodCast, nomeEntrevistador);
			        
			        // Marca o tempo final
			        gettimeofday(&fim, NULL);
			        
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a média posteriormente
			        soma += tempo_decorrido;
			    }
			    
			    // Calcula a média dos tempos decorridos
			    media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
			    break;
			case 3:
            	// Implementação da opção 3 (Adicionar Tema)
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                adicionarTema(plataformas, nomePlataforma, nomePodCast, tema);
                break;
			case 4:
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                printf("Digite o titulo da entrevista: ");
                scanf(" %[^\n]s", tituloEntrevista);
                printf("Digite a data da entrevista (DD/MM/AAAA): ");
                scanf(" %s", data);
                printf("Digite a duracao da entrevista (em minutos): ");
                scanf(" %d", &duracao);
                printf("Digite o nome do entrevistado: ");
                scanf(" %s", nomeEntrevistador);
                printf("Digite a especialidade do entrevistado: ");
                scanf(" %s", especialidade);
                adicionarEntrevista(plataformas, nomePlataforma, nomePodCast, tema, tituloEntrevista, data, duracao, nomeEntrevistador, especialidade);
                break;  
            case 5:
                // Implementação p/ Mostrar PodCasts de uma Plataforma)
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                mostrarNomesPodCasts(plataformas, nomePlataforma);
                break;
            case 6: 
                // Implementação p/ Mostrar Temas de um PodCast ii)                
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                
                 for (i = 0; i < 30; i++) {
			        // Marca o tempo inicial
			        gettimeofday(&inicio, NULL);
			        
			        // Chamada da funcao para mostrar temas de podcast
                	mostrarTemasPodCast(plataformas, nomePlataforma, nomePodCast);
                	
                	// Marca o tempo final
			        gettimeofday(&fim, NULL);
			        
			        // Calcula o tempo decorrido em microssegundos
			        tempo_decorrido = (double)(fim.tv_sec - inicio.tv_sec) * 1000000 + (double)(fim.tv_usec - inicio.tv_usec);
			        
			        // Armazena o tempo decorrido
			        tempDecorrido[i] = tempo_decorrido;
			        
			        // Soma os tempos para calcular a média posteriormente
			        soma += tempo_decorrido;
			    }
			    
			    // Calcula a média dos tempos decorridos
			    media = soma / 30;
			    printf("A media de tempos eh: %lf microssegundos\n", media);
                break;
            case 7: 
                // Implementação da opção iii) (Mostrar Entrevistas de um Tema)
                printf("Digite o tema: ");
                scanf(" %s", tema);
                mostrarTitulosEntrevistas(plataformas, nomePlataforma, nomePodCast, tema);
                break;
            case 8: 
                // Implementação da opção iv) (Mostrar Detalhes de uma Entrevista)
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                printf("Digite o título da entrevista: ");
                scanf(" %[^\n]s", tituloEntrevista);
                mostrarDadosEntrevista(plataformas, nomePlataforma, nomePodCast, tema, tituloEntrevista);
                break;
            case 9:
                // Implementação da opção v) (Mostrar Entrevistados de um Tema)
                
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                mostrarDadosEntrevistasTema(plataformas, nomePlataforma, nomePodCast, tema);
                break;
            case 10:
                // Implementação da opção 10 (Excluir Entrevista)
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                printf("Digite o título da entrevista: ");
                scanf(" %[^\n]s", tituloEntrevista);
                excluirEntrevista(plataformas, nomePlataforma, nomePodCast, tema, tituloEntrevista);
                break;
            case 11: 
                // Implementação da opção 11 (Excluir Tema)
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                printf("Digite o nome do tema: ");
                scanf(" %s", tema);
                excluirTema(plataformas, nomePlataforma, nomePodCast, tema);
                break;   
            case 12:
                // Implementação da opção 12 (Excluir PodCast)
                printf("Digite o nome da plataforma: ");
                scanf(" %s", nomePlataforma);
                printf("Digite o nome do PodCast: ");
                scanf(" %s", nomePodCast);
                excluirPodCast(plataformas, nomePlataforma, nomePodCast);
                break;
            case 13:
            	removePlataforma(&plataformas, nomePlataforma);
                break;
            case 14:
                listarPlataformas(plataformas);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opcao inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

	
    return 0;
}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
    