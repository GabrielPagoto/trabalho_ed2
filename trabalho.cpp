#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stddef.h>

// tamanho da tabela
#define M 997

// tipo Palavra
typedef struct {
    int cont;
    char texto[50];
} Palavra;

// tipo nó usado na lista encadeada
typedef struct no {
    Palavra palavra;
    struct no *proximo;
} No;

// tipo lista com um ponteiro para o primeiro nó
typedef struct {
    No *inicio;
    int tam;
} Lista;

// nossa tabela (vetor de ponteiros para listas)
Lista *tabela[M];

//--------------------------------- fim definições variáveis --------------------

//--------------------------------- funções meus tipos --------------------------

// cria e retorna um tipo Palavra
Palavra criarPalavra() {
    Palavra p;

	printf("Digite o nome da Palavra: ");
    scanf("%*c");
    fgets(p.texto, 50, stdin);
    return p;
}

// imprime uma Palavra
void imprimirPalavra(Palavra p) {
    printf("\tNome: %s Contador: %d\n", p.texto, p.cont);
}

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
void inserirInicio(Palavra p, Lista *lista) {

    No *no = (No*)malloc(sizeof(No));
    no->palavra = p;
    no->proximo = lista->inicio;
    lista->inicio = no;
    lista->tam++;
}

// busca um elemento na lista
int buscarNo(char str[], Lista *lista) {
	No* aux = lista->inicio;

    while(aux != NULL) {
        if(strcmp(aux->palavra.texto, str) == 0){
        	aux->palavra.cont++;
            return 1;
		}
        else{
        	aux = aux->proximo;
		}
    }
    return 0;// palavra não encontrada
}

void imprimirLista(No *inicio) {
    while(inicio != NULL) {
        imprimirPalavra(inicio->palavra);
        inicio = inicio->proximo;
    }
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
void inserTabela(char* str){
    //Palavra pal = criarPalavra();
    if(strlen(str) <= 2){
    	return;
	}
    Palavra pal;
    strcpy(pal.texto,str);
	int indice = funcaoHashString(pal.texto);

//	if(buscarNo(str,tabela[indice]) == 1){
//		return;
//	}
	pal.cont = 1;
    inserirInicio(pal, tabela[indice]);
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

// imprimir tabela
void imprimirTabela(){
    int i;
    printf("\n---------------------TABELA-------------------------\n");
    for(i = 0; i < M; i++){
        printf("%d Lista tamanho: %d\n", i, tabela[i]->tam);
       // imprimirLista(tabela[i]->inicio);
    }
    printf("---------------------FIM TABELA-----------------------\n");
}

int main() {
    int op, mat, i = 0;
    Palavra *p;
    FILE *fp;
    char str[50];
    char c;

    if(fopen("64mb.txt", "rt") == NULL){
    	printf("Arquivo nao encontrado.");
    	exit(0);
	}
	//fclose(fp);
	fp = fopen("64mb.txt", "rt");

    inicializar();

    /*do {
        printf("\n0 - Sair\n1 - Inserir\n2 - Buscar\n3 - Imprimir tabela\n");
        scanf("%d", &op);
        switch(op) {
        case 0:
            printf("saindo...\n");
            break;
        case 1:
            //inserTabela();
            break;
        case 2:
            printf("Qual a matricula a ser buscada? ");
            scanf("%d", &mat);
            p = buscarPalavraTabela(mat);
            if(p) {
                printf("Palavra encontrada: Matricula: %d\tNome: %s", p->texto);
            } else
                printf("Palavra nao contrada!\n");
            break;
        case 3:
            imprimirTabela();
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while(op != 0);*/

    fscanf(fp, "%c", &c); //le um caractere do arquivo
    do { //le todo o arquivo


        if (isalpha(c)) { // verifica se o caractere eh uma letra
            c = tolower(c); //transforma caractere em letra minusculo
    		str[i] = c;
    		i++;

        }
        else{
        	inserTabela(str);
        	i=0;
        	memset(str,0,strlen(str));
		}

		fscanf(fp, "%c", &c); //le um caractere do arquivo
    } while (!feof(fp)) ;

    inserTabela(str);
    fclose(fp);
    imprimirTabela();

    return 0;
}
