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
#define M 99991


// tipo Palavra
/*typedef struct {
    int cont;
    char texto[50];
} Palavra;*/

// tipo nó usado na lista encadeada
typedef struct no
{
    struct no *proximo;
    int cont;
    char texto[100];
} No;

typedef struct relevancia
{
    struct relevancia *proximo;
    float valor;
    char nomeArquivo[100];
} Relevancia;

// tipo lista com um ponteiro para o primeiro nó
typedef struct
{
    No *inicio;
    int tam;
} Lista;

typedef struct
{
    Relevancia *inicio;    
} ListaRelevancia;

struct greater_than
{
    bool operator()(No *const &a, No *const &b) const
    {
        return a->cont > b->cont;
    }
};

// nossa tabela (vetor de ponteiros para listas)
Lista *tabela[M];
ListaRelevancia *listaRelevancia;

//--------------------------------- fim definições variáveis --------------------

//--------------------------------- funções meus tipos --------------------------

// imprime uma Palavra

//-------------------------------- início funções lista -------------------------
// cria uma lista vazia e retorna seu endereço na memória
Lista *criarLista()
{
    Lista *l = (Lista *)malloc(sizeof(Lista) * 3);
    l->inicio = NULL;
    l->tam = 0;
    return l;
}

ListaRelevancia *criarListaRelevancia()
{
    ListaRelevancia *lr = (ListaRelevancia *)malloc(sizeof(ListaRelevancia));
    lr->inicio = NULL;
    return lr;
}

void insereOrdenado(Relevancia *rel){
    Relevancia *relaux = (Relevancia*)malloc(sizeof(Relevancia));

    // a lista est� vazia?
    if(listaRelevancia == NULL){
        listaRelevancia = criarListaRelevancia();
		listaRelevancia->inicio = rel;        
    } 
	// � o menor?    
	if(rel->valor < (listaRelevancia)->inicio->valor){
        rel->proximo = listaRelevancia->inicio;
        listaRelevancia->inicio = rel;
    }
    else{
        relaux = listaRelevancia->inicio;
        while(relaux->proximo && rel->valor > relaux->proximo->valor)
            relaux = relaux->proximo;
        rel->proximo = relaux->proximo;
        relaux->proximo = rel;
    }

}

/*
    inserir no início da lista
    PARÂMETROS
    p - nova Palavra a ser inserida
    *lista - endereço de uma lista encadeada.
*/
void inserirInicio(char *p, Lista *lista)
{

    No *no = (No *)malloc(sizeof(No) * 3);
    strcpy(no->texto, p);
    no->cont = 1;
    no->proximo = lista->inicio;
    lista->inicio = no;
    lista->tam++;
}

// busca um elemento na lista
void buscarNo(char *str, Lista *lista)
{
    No *ant;
    No *plv = NULL;
    No *aux = lista->inicio;
    while (aux != NULL)
    {
        if (strcmp(aux->texto, str) == 0)
        {
            if (plv == NULL)
            {
                // aux->palavra.cont++;
                plv = aux;
                ant = aux;
                aux = aux->proximo;
            }
            else
            {
                plv->cont++;
                ant->proximo = aux->proximo;
                free(aux);
                aux = ant->proximo;
                lista->tam--;
            }
        }
        else
        {
            ant = aux;
            aux = aux->proximo;
        }
    }
}

int incrementaContador(char *str, Lista *lista)
{
    No *aux = lista->inicio;
    while (aux != NULL)
    {
        if (strcmp(aux->texto, str) == 0)
        {
            aux->cont++;
            return 1;
        }
        else
        {
            aux = aux->proximo;
        }
    }
    return 0;
}

void buscarPalavra(char *str, Lista *lista)
{
    No *aux = lista->inicio;
    while (aux != NULL)
    {
        if (strcmp(aux->texto, str) == 0)
        {
            printf("\nFrequencia da palavra %s eh %d.\n", str, aux->cont);
            return;
        }
        else
        {
            aux = aux->proximo;
        }
    }
    printf("\nPalavra %s nao encontrada.\n", str);
    return;
}

//---------------------------------- fim funções lista -------------------------

//--------------------------- início funções tabela hash -----------------------
// inicializa a tabela com uma lista vazia em cada posição do vetor
void inicializar()
{
    int i;
    for (i = 0; i < M; i++)
        tabela[i] = criarLista();
}

/*
    Função hash a partir de texto
*/
int funcaoHashString(char *str)
{

    int i, p = 31, p_pow = 1, tamS = strlen(str);
    unsigned int hash = 0;

    for (i = 0; i < tamS; i++)
    {
        hash = (hash + (str[i] - 'a' + 1) * p_pow) % M;
        p_pow = (p_pow * p) % M;
    }
    return hash;
}

// cria uma Palavra e a insere na tabela
void inserTabela(char *str, int hash)
{
    // Palavra pal = criarPalavra();
    if (strlen(str) <= 2)
    {
        return;
    }

    if (tabela[hash] == NULL)
    {
        tabela[hash] = criarLista();
    }

    // int indice = funcaoHashString(str);
    // inserirInicio(str, tabela[hash]);
    if (incrementaContador(str, tabela[hash]) == 0)
    {
        inserirInicio(str, tabela[hash]);
    }
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
void limparPalavras(char *str)
{
    int indice = funcaoHashString(str);
    buscarNo(str, tabela[indice]);
}

void imprimirPalavra(No *p)
{
    printf("\tNome: %s Contador: %d\n", p->texto, p->cont);
}
void percorrerLista(No *inicio)
{
    while (inicio != NULL)
    {
        limparPalavras(inicio->texto);
        inicio = inicio->proximo;
    }
}
void imprimirLista(No *inicio)
{
    while (inicio != NULL)
    {
        imprimirPalavra(inicio);
        inicio = inicio->proximo;
    }
}

// imprimir tabela
void limparHash()
{
    int i;

    for (i = 0; i < M; i++)
    {
        percorrerLista(tabela[i]->inicio);
    }
}

void imprimirTabela()
{
    int i;
    printf("\n---------------------TABELA-------------------------\n");
    for (i = 0; i < M; i++)
    {
        percorrerLista(tabela[i]->inicio);
        printf("%d Lista tamanho: %d\n", i, tabela[i]->tam);
        // imprimirLista(tabela[i]->inicio);
    }
    printf("---------------------FIM TABELA-----------------------\n");
}

void populaTabela(char *arq)
{
    int i = 0;
    FILE *fp;
    char str[100];
    char c;
    int p = 31, p_pow = 1;
    unsigned int hash = 0;

    fp = fopen(arq, "rt");
    if (arq == NULL)
    {
        printf("Arquivo nao encontrado.");
        return;
    }

    while ((c = fgetc(fp)) != EOF)
    {

        if (isalpha(c))
        {                   // verifica se o caractere eh uma letra
            c = tolower(c); // transforma caractere em letra minusculo
            str[i] = c;

            hash = (hash + (c - 'a' + 1) * p_pow) % M;
            p_pow = (p_pow * p) % M;
            i++;
        }
        else
        {
            inserTabela(str, hash);
            i = 0;
            memset(str, 0, strlen(str));
            hash = 0;
            p_pow = 1;
        }
    }
    fclose(fp);
    // limparHash();
}

void palavrasMaiorFreq(char *arq[])
{
    int first = 1;
    std::vector<No *> elements;
    std::vector<No *> s;
    populaTabela(arq[3]);
    int n = strtol(arq[2], NULL, 10);
    for (int k = 0; k < M; k++)
    {
        if (tabela[k] == NULL)
        {
            continue;
        }
        else
        {
            while (tabela[k]->inicio != NULL)
            {

                s.push_back(tabela[k]->inicio);
                tabela[k]->inicio = tabela[k]->inicio->proximo;
            }
        }
        for (int i = 0; i < s.size(); i++)
        {
            // add the first 5 elements to the vector
            if (i < n && first == 1)
            {
                elements.push_back(s.at(i));
                if (elements.size() == n)
                {
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

    for (int i = 0; i < elements.size(); i++)
    {
        printf("%s %d\n", elements.at(i)->texto, elements.at(i)->cont);
    }
}

void buscaFreqPalavra(char *str, char *arq)
{
    populaTabela(arq);
    int indice = funcaoHashString(str);
    buscarPalavra(str, tabela[indice]);
}

void inserirVetor(std::vector<No *> termos, char * str){
	int i;
	
	for (i = 0; i < termos.size(); i++)
    {
        if(strcmp(str, termos.at(i)->texto) == 0){
        	termos.at(i)->cont++;
        	return;
		}
    }
}

float * calculoTF(char * nomeArquivo, std::vector<No *> termos, int contador){
	int i;
	float TF[termos.size()];
	
	for (i = 0; i < termos.size(); i++)
    {
    	TF[i] = (termos.at(i)->cont) / contador; 
    }
    
    return TF;
}


void buscaRelevanciaTermoArquivo(char * termo, char * arq){
	std::vector<No *> termos;
	std::vector<No *> arquivos;	
	int i, cont = 0, contador = 0, j;
	char str[100];	
    FILE *fp;
    char c;
	
	for(i=0; termo[i]; i++){
		if (isalpha(termo[i]))
        {                   // verifica se o caractere eh uma letra            
            str[cont] = tolower(termo[i]);  // transforma caractere em letra minusculo            
            cont++;
        }
        else
        {
			No *no = (No *)malloc(sizeof(No));
		    strcpy(no->texto, str);
		    no->cont = 0;
		    no->proximo = NULL;
		    termos.push_back(no);
		    cont=0;
            memset(str, 0, strlen(str));            
        }
	}
	No *no = (No *)malloc(sizeof(No));
    strcpy(no->texto, str);
    no->cont = 0;
    no->proximo = NULL;
    termos.push_back(no);
    cont=0;
    memset(str, 0, strlen(str)); 
	
	for(i=0; arq[i]; i++){
		if (isalnum(arq[i]) || arq[i] == '.')
        {                   // verifica se o caractere eh uma letra            
            str[cont] = tolower(arq[i]);  // transforma caractere em letra minusculo
            cont++;
        }
        else
        {
            No *no = (No *)malloc(sizeof(No));
		    strcpy(no->texto, str);
		    no->cont = 0;
		    no->proximo = NULL;
		    arquivos.push_back(no);
		    cont=0;
            memset(str, 0, strlen(str));
        }
	}
	No *no2 = (No *)malloc(sizeof(No));
    strcpy(no2->texto, str);
    no2->cont = 0;
    no2->proximo = NULL;
    arquivos.push_back(no2);
    cont=0;
    memset(str, 0, strlen(str));
	
	/*for (i = 0; i < termos.size(); i++)
    {
        printf("%s %d\n", termos.at(i)->texto, termos.at(i)->cont);
    }
    
    for (i = 0; i < arquivos.size(); i++)
    {
        printf("%s \n", arquivos.at(i)->texto);
    }*/
    
    for(i=0; i<arquivos.size(); i++){
    	j=0;
	
	    fp = fopen(arquivos.at(i)->texto, "rt");
	    if (arquivos.at(i)->texto == NULL)
	    {
	        printf("Arquivo nao encontrado.");
	        return;
	    }
	
	    while ((c = fgetc(fp)) != EOF)
	    {
	
	        if (isalpha(c))
	        {                   // verifica se o caractere eh uma letra
	            c = tolower(c); // transforma caractere em letra minusculo
	            str[j] = c;
	            j++;
	        }
	        else
	        {
				inserirVetor(termos, str);
	            j = 0;
	            memset(str, 0, strlen(str));
	            contador++;
	        }
	    }
	    calculoTF(arquivos.at(i)->texto, termos, contador);
	    fclose(fp);
	    contador = 0;
	    memset(str, 0, strlen(str));
	}
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

int main(int argc, char *argv[])
{
    int escolha = 0, n = 0;
    char c, pal[100], arq[100];
    char strTermos[100];
    char strArquivos[100];

    // inicializar();

    if (argc <= 3)
    {
        printf("Trabalho ED2 - Indexador \n");
        printf("Alunos: Gabriel Felipe Pagoto, Jackson Longo dos Santos e Jose Adilson de Paula Cardoso \n");
        printf("Opcoes:\n");
        printf("--freq N ARQUIVO \nExibe o numero de ocorrencia das N palavras que mais aparecem em ARQUIVO, em ordem decrescente de ocorrencia.\n\n");
        printf("--freq-word PALAVRA ARQUIVO \nExibe o numero de ocorrencias de PALAVRA em ARQUIVO. \n\n");
        printf("--search TERMO ARQUIVO [ARQUIVO ...]\nExibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por TERMO. A listagem eh apresentada em ordem descrescente de relevancia. TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre aspas.\n");
    }
    else if (strcmp(argv[1], "--freq") == 0)
    {
        // opcaoFreq(argv);
        palavrasMaiorFreq(argv);
    }
    else if (strcmp(argv[1], "--freq-word") == 0)
    {
        // break;
        // opcaoFreqWord(argv[3], argv[2]);
        buscaFreqPalavra(argv[2], argv[3]);
    } else if (strcmp(argv[1], "--search") == 0) {    
    	strcpy(strTermos, argv[2]);
    	strcpy(strArquivos, argv[3]);
    	buscaRelevanciaTermoArquivo(strTermos, strArquivos);
        //opcaoSearch(argc, argv);
    }
    else
    {
        printf("Os parametros apresentados estao errados. Verifique a opcao desejada e tente novamente\n");
    }
}
