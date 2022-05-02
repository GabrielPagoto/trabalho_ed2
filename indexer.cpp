#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stddef.h>
#include <vector>
#include <algorithm>
#include <math.h>

// tamanho da tabela
#define M 99991

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

// tipo lista com um ponteiro para o primeiro nÃ³
typedef struct
{
    No *inicio;
    int tam;
} Lista;

struct greater_than
{
    bool operator()(No *const &a, No *const &b) const
    {
        return a->cont > b->cont;
    }
};

struct more_relevant
{
    bool operator()(Relevancia *const &a, Relevancia  *const &b) const
    {
        return a->valor > b->valor;
    }
};

// nossa tabela (vetor de ponteiros para listas)
Lista *tabela[M];

//--------------------------------- fim definiÃ§Ãµes variÃ¡veis --------------------

//--------------------------------- funÃ§Ãµes meus tipos --------------------------

// imprime uma Palavra

//-------------------------------- inÃ­cio funÃ§Ãµes lista -------------------------
// cria uma lista vazia e retorna seu endereÃ§o na memÃ³ria
Lista *criarLista()
{
    Lista *l = (Lista *)malloc(sizeof(Lista) * 3);
    l->inicio = NULL;
    l->tam = 0;
    return l;
}

/*
    inserir no inÃ­cio da lista
    PARÃ‚METROS
    p - nova Palavra a ser inserida
    *lista - endereÃ§o de uma lista encadeada.
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

/*
    Funcao hash a partir de texto
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

// busca uma Palavra. Seu retorno eh um endereÃ§o ou NULL
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
            if (i < n && first == 1) // adiciona os primeiros n elementos ao heap
            {
                elements.push_back(s.at(i));
                if (elements.size() == n)
                {
                    std::make_heap(elements.begin(), elements.end(), greater_than()); // cria o min_heap de n elementos
                    first = 0;
                }
                continue;
            }

            if (elements.front()->cont <= s.at(i)->cont) //checa se o proxímo elemento na lista eh maior
            {
                std::pop_heap(elements.begin(), elements.end(), greater_than()); // coloca o primeiro elemento no final

                elements.pop_back(); // remove esse elemento

                elements.push_back(s.at(i));// adiciona o novo elemento

                std::push_heap(elements.begin(), elements.end(), greater_than()); //faz o heap
            }
        }
        s.clear(); // limpa o vetor para receber a proxima lista da tabela
    }
    std::sort_heap(elements.begin(), elements.end(), greater_than());// ordena o heap

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

std::vector<Relevancia *> calculoTF(char* nome, std::vector<Relevancia *> TF, std::vector<No *> termos, int contador){
	int i;
	float aux;

	for (i = 0; i < termos.size(); i++)
    {
        Relevancia* rel = (Relevancia*)malloc(sizeof(Relevancia));
        aux = (float)(termos.at(i)->cont / (float)contador);
        strcpy(rel->nomeArquivo, nome);
        rel->valor = aux;
        rel->proximo = NULL;
    	TF.push_back(rel);
    }
    return TF;
}

float calculoIDF(std::vector<No*> arquivos, int arqspresente) {
    float aux;
    if(arqspresente!=0){
        aux = (float)(arquivos.size() / (float)arqspresente);
    } else {
        return 0;
    }
    if(aux == 1) {
        return 0.01;
    }
    return log10(aux);

}


void buscaRelevanciaTermoArquivo(char * termo, char * arq){
	std::vector<No *> termos;
	std::vector<No *> arquivos;
	std::vector<Relevancia *> TF, TFIDF;
	std::vector<int> arqspresente;
	std::vector<float> IDF;
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
		    arqspresente.push_back(0);
		    cont=0;
            memset(str, 0, strlen(str));
        }
	}
	No *no = (No *)malloc(sizeof(No));
    strcpy(no->texto, str);
    no->cont = 0;
    no->proximo = NULL;
    arqspresente.push_back(0);
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
	    TF = calculoTF(arquivos.at(i)->texto, TF, termos, contador);
	    fclose(fp);
	    contador = 0;
	    memset(str, 0, strlen(str));
	}

	    for(int h = 0; h<arquivos.size();  h++){
            for(int k = 0; k < termos.size(); k++){
                if(TF.at(contador)->valor > 0.0){
                    arqspresente.at(k)++;
                }
                contador++;
            }
	    }

	    for(int i = 0; i < termos.size(); i++) {
            IDF.push_back(calculoIDF(arquivos, arqspresente.at(i)));
	    }
	    contador = 0;
	    float aux = 0, media;
	    for(int i = 0; i < arquivos.size(); i++){
            for(int j = 0; j < termos.size(); j++){
                aux += TF.at(contador)->valor * IDF.at(j);
                contador++;
            }
            media = aux / termos.size();
            Relevancia* rel = (Relevancia*)malloc(sizeof(Relevancia));
            strcpy(rel->nomeArquivo, arquivos.at(i)->texto);
            rel->valor = media;
            rel->proximo = NULL;
            TFIDF.push_back(rel);
	    }

	    std::sort_heap(TFIDF.begin(), TFIDF.end(), more_relevant());
	    for(int i = 0; i<TFIDF.size(); i++){
            printf("Arquivo: %s Relevancia: %f\n", TFIDF.at(i)->nomeArquivo, TFIDF.at(i)->valor);
	    }

}

int main(int argc, char *argv[])
{
    int escolha = 0, n = 0;
    char c, pal[100], arq[100];
    char strTermos[100];
    char strArquivos[100];

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
        palavrasMaiorFreq(argv);
    }
    else if (strcmp(argv[1], "--freq-word") == 0)
    {
        buscaFreqPalavra(argv[2], argv[3]);
    } else if (strcmp(argv[1], "--search") == 0) {
    	strcpy(strTermos, argv[2]);
    	strcpy(strArquivos, argv[3]);
    	buscaRelevanciaTermoArquivo(strTermos, strArquivos);
    }
    else
    {
        printf("Os parametros apresentados estao errados. Verifique a opcao desejada e tente novamente\n");
    }
}
