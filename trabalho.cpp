#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stddef.h>

// tamanho da tabela
#define M 3989

// tipo Palavra
/*typedef struct {
    int cont;
    char texto[50];
} Palavra;*/

// tipo nó usado na lista encadeada
typedef struct no {
    //Palavra palavra;
    struct no *proximo;
    int cont;
    char texto[50];
} No;

// tipo lista com um ponteiro para o primeiro nó
typedef struct {
    No *inicio;
    int tam;
} Lista;

typedef struct arvoreRB {
 int valor;
 int cor;
 struct arvoreRB *esq;
 struct arvoreRB *dir;
} ArvoreRB;

// nossa tabela (vetor de ponteiros para listas)
Lista *tabela[M];

//--------------------------------- fim definições variáveis --------------------

//--------------------------------- funções meus tipos --------------------------

// imprime uma Palavra

//-------------------------------- início funções lista -------------------------
// cria uma lista vazia e retorna seu endereço na memória
Lista* criarLista() {
    Lista *l = (Lista*)malloc(sizeof(Lista));
    l->inicio = NULL;
    l->tam = 0;
    return l;
}

/*
    inserir no início da lista
    PARÂMETROS
    p - nova Palavra a ser inserida
    *lista - endereço de uma lista encadeada.
*/
void inserirInicio(char* p, Lista *lista) {

    No *no = (No*)malloc(sizeof(No));
    strcpy(no->texto,p);
	no->cont = 1;    
    no->proximo = lista->inicio;
    lista->inicio = no;
    lista->tam++;
}

// busca um elemento na lista
void buscarNo(char* str, Lista *lista) {
	No* ant;
    No* plv = NULL;
    No* aux = lista->inicio;
    while(aux != NULL) {
        if(strcmp(aux->texto, str) == 0){
            if(plv == NULL){
        	//aux->palavra.cont++;
            plv = aux;
            ant = aux;
            aux = aux->proximo;
            }else{
            	plv->cont++;
		        ant->proximo = aux->proximo;
		        free(aux);
		        aux = ant->proximo;
		        lista->tam--;
            }
		}
        else{
            ant = aux;
        	aux = aux->proximo;
		}
    }
}

void buscarPalavra(char* str, Lista *lista) {
    No* aux = lista->inicio;
    while(aux != NULL) {
        if(strcmp(aux->texto, str) == 0){
            printf("\nFrequencia da palavra %s eh %d.", str, aux->cont);
            return;
		}
        else{            
        	aux = aux->proximo;
		}
    }
    printf("\nPalavra %s nao encontrada.", str);
    return;
}


//---------------------------------- fim funções lista -------------------------

//--------------------------- início funções tabela hash -----------------------
// inicializa a tabela com uma lista vazia em cada posição do vetor
void inicializar(){
    int i;
    for(i = 0; i < M; i++)
        tabela[i] = criarLista();
}

/*
    Função hash a partir de texto
*/
int funcaoHashString(char* str){

    int i, p = 31, p_pow = 1, tamS = strlen(str);
    unsigned int hash = 0;

    for(i = 0; i < tamS; i++){
        hash = (hash + (str[i] - 'a' + 1) * p_pow) % M;
        p_pow = (p_pow * p) % M;
    }
    return hash;
}

// cria uma Palavra e a insere na tabela
void inserTabela(char* str, int hash){
    //Palavra pal = criarPalavra();
    if(strlen(str) <= 2){
    	return;
	}

	//int indice = funcaoHashString(str);
    inserirInicio(str, tabela[hash]);
}

// busca uma Palavra. Seu retorno é um endereço ou NULL
/*Palavra* buscarPalavraTabela(int mat){
	Palavra* palavra;
    //int indice = funcaoEspalhamento(mat);
    int indice = funcaoHashString(palavra->texto);
    No *no = buscarNo(mat, tabela[indice]->inicio);
    if(no)
        return &no->palavra;
    else
        return NULL;
}*/
void limparPalavras(char* str) {
    int indice = funcaoHashString(str);
    buscarNo(str,tabela[indice]);
}

void imprimirPalavra(No* p) {
    printf("\tNome: %s Contador: %d\n", p->texto, p->cont);
}
void percorrerLista(No *inicio) {
    while(inicio != NULL) {
        limparPalavras(inicio->texto);
        inicio = inicio->proximo;
    }
}
void imprimirLista(No *inicio) {
    while(inicio != NULL) {
        imprimirPalavra(inicio);
        inicio = inicio->proximo;
    }
}

// imprimir tabela
void limparHash(){
    int i;

    for(i = 0; i < M; i++){
       percorrerLista(tabela[i]->inicio);
       }
}

void imprimirTabela(){
    int i;
    printf("\n---------------------TABELA-------------------------\n");
    for(i = 0; i < M; i++){
        percorrerLista(tabela[i]->inicio);
        printf("%d Lista tamanho: %d\n", i, tabela[i]->tam);
        //imprimirLista(tabela[i]->inicio);
    }
    printf("---------------------FIM TABELA-----------------------\n");
}



void buscaFreqPalavra(char* str){
	int indice = funcaoHashString(str);
	buscarPalavra(str, tabela[indice]);
}











int main() {
    int i = 0;
    FILE *fp;
    char str[50];
    char c;
    
    int p = 31, p_pow = 1;
	unsigned int hash = 0;

    if(fopen("512mb.txt", "rt") == NULL){
    	printf("Arquivo nao encontrado.");
    	exit(0);
	}
	//fclose(fp);
	fp = fopen("512mb.txt", "rt");

    inicializar();


    
    while((c = fgetc(fp))!=EOF) {
    	
		if (isalpha(c)) { // verifica se o caractere eh uma letra
            c = tolower(c); //transforma caractere em letra minusculo
    		str[i] = c;
    		

	        hash = (hash + (c - 'a' + 1) * p_pow) % M;
	        p_pow = (p_pow * p) % M;	        
	        i++;
        }
        else{
        	inserTabela(str, hash);
        	i=0;
        	memset(str,0,strlen(str));
        	hash = 0;
        	p_pow = 1;
		}
	}	

    //inserTabela(str);
    fclose(fp);
    limparHash();
    //imprimirTabela();
    
    buscaFreqPalavra("ascii");

    return 0;
}
