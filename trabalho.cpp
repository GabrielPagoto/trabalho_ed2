#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <iostream>

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

struct greater_than {
    bool operator()(No* const& a, No* const& b) const {
        return a->cont > b->cont;
    }
};

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

// busca uma Palavra. Seu retorno eh um endereço ou NULL
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

void populaTabela(char* arq){
    int i = 0;
    FILE *fp;
    char str[50];
    char c;
    int p = 31, p_pow = 1;
	unsigned int hash = 0;

	fp = fopen(arq, "rt");
	if(arq == NULL){
		printf("Arquivo nao encontrado.");
		return;
	}

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
    fclose(fp);
    limparHash();

}

void palavrasMaiorFreq (char* arq[]) {
    int first = 1;
    std::vector<No*> elements;
    std::vector<No*> s;
    populaTabela(arq[3]);
    int n = strtol(arq[2], NULL, 10);
    for(int k = 0; k < M; k++){
        while(tabela[k]->inicio != NULL) {
            s.push_back(tabela[k]->inicio);
            tabela[k]->inicio = tabela[k]->inicio->proximo;
        }
        for (int i = 0; i < s.size(); i++)
        {
            // add the first 5 elements to the vector
            if (i < n && first == 1)
            {
                elements.push_back(s.at(i));
                if ( elements.size() == n ){
                    // make the max-heap of the 5 elements
                    std::make_heap(elements.begin(), elements.end(), greater_than());
                    first = 0;
                }
                continue;
            }

            // now check if the next element is smaller than the top of the heap
            if (elements.front()->cont <= s.at(i)->cont)
            {
                // remove the front of the heap by placing it at the end of the vector
                std::pop_heap(elements.begin(), elements.end(), greater_than());

                // get rid of that item now
                elements.pop_back();

                // add the new item
                elements.push_back(s.at(i));

                // heapify
                std::push_heap(elements.begin(), elements.end(), greater_than());

            }
        }
        s.clear();
    }
        // sort the heap
        std::sort_heap(elements.begin(), elements.end(), greater_than());

        for(int i = 0; i < elements.size(); i++){
            printf("%s %d\n", elements.at(i)->texto, elements.at(i)->cont);
        }


}

void buscaFreqPalavra(char* str, char* arq){
    populaTabela(arq);
	int indice = funcaoHashString(str);
	buscarPalavra(str, tabela[indice]);
}

/*int main() {
    int escolha = 0, n = 0;
    char c, pal[50], arq[50];

    inicializar();

    printf("Escolha a operacao:\n\n1-Numero de ocorrencia de N palavras\n2-Frequencia de uma palavra\n3-TBD\n");
    scanf("%d", &escolha);

	switch (escolha) {
    case 1:
        printf("Informe o numero de palavras desejado:\n");
        scanf("%d", &n);
        printf("Informe o arquivo:\n");
        scanf("%s", &arq);
        palavrasMaiorFreq(n, arq);
    break;

    case 2:
        printf("Informe a palavra que deseja saber a frequencia:\n");
        scanf("%s", &pal);
        printf("Informe o arquivo:\n");
        scanf("%s", &arq);
        for(int i = 0; pal[i]; i++){
            pal[i] = tolower(pal[i]);
        }
        buscaFreqPalavra(pal, arq);
    break;

    case 3:

    break;

    default:
    break;
	}

    return 0;
}*/


int main(int argc, char* argv[]) {
	int escolha = 0, n = 0;
    char c, pal[50], arq[50];

    inicializar();
	
    if (argc <= 3) {
        printf("Trabalho ED2 - Indexador \n");
        printf("Alunos: Gabriel Felipe Pagoto, Jackson Longo dos Santos e Jose Adilson de Paula Cardoso \n");
        printf("Opcoes:\n");
        printf("--freq N ARQUIVO \nExibe o numero de ocorrencia das N palavras que mais aparecem em ARQUIVO, em ordem decrescente de ocorrencia.\n\n");
        printf("--freq-word PALAVRA ARQUIVO \nExibe o numero de ocorrencias de PALAVRA em ARQUIVO. \n\n");
        printf("--search TERMO ARQUIVO [ARQUIVO ...]\nExibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por TERMO. A listagem eh apresentada em ordem descrescente de relevancia. TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre aspas.\n");
    } else if (strcmp(argv[1], "--freq") == 0) {
       // opcaoFreq(argv);
       palavrasMaiorFreq(argv);
    } /*else if (strcmp(argv[1], "--freq-word") == 0) {
    	break;
       // opcaoFreqWord(argv[3], argv[2]);
    } else if (strcmp(argv[1], "--search") == 0) {
       // opcaoSearch(argc, argv);
    }*/ else {
        printf("Os parametros apresentados estao errados. Verifique a opcao desejada e tente novamente\n");
    }
}
