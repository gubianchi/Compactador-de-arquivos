#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM 256

typedef struct no{
    unsigned char caracter;
    unsigned int frequencia;
    struct no *direita;
    struct no *esquerda;
    struct no *proximo;
} No;

typedef struct {
    No *inicio;
    int tam;
} Lista;


void iniciaizar_tabela_com_zero(unsigned int tab[]){
    int i;
    for (i = 0; i < 250; i++) tab[i] = 0;
}


void preenche_tabela_sequencia(unsigned int tab[], unsigned char *texto){
    int i =0;
    while(texto[i] != '\0'){
        tab[texto[i]]++;
        i++;
    }
}

void imprime_tabela_sequencia(unsigned int tab[]){
    int i;
    printf("\tTABELA DE FREQUENCIA\n");
    for (i =0; i< TAM; i++){
        if(tab[i] > 0)
            printf("\t%d = %d = %c\n,", i, tab[i], i);
    }
}


void cria_lista (Lista *lista ){
    lista -> inicio = NULL;
    lista -> tam =0;
}


void inserir_ordenado (Lista *lista, No *no){
    No * aux;
    if (lista->inicio == NULL){
        lista->inicio = no;
    }
    else if (no->frequencia < lista->inicio->frequencia){
        no -> proximo = lista -> inicio;
        lista -> inicio = no;
    }
    else{
        aux = lista -> inicio;
        while (aux  -> proximo && aux -> proximo ->frequencia <= no -> frequencia )
            aux = aux -> proximo;
        no -> proximo = aux-> proximo ;
        aux -> proximo = no ;
    }
    lista -> tam++;
}


void preencher_lista(unsigned int tab[],Lista *lista){
    int i;
    for (i = 0; i < TAM; i++) {
        if (tab [i] > 0 ){
            No *novo = malloc (sizeof (No));
            if (novo) {
                novo -> caracter = i;
                novo -> frequencia = tab[i];
                novo -> direita = NULL;
                novo -> esquerda = NULL;
                novo -> proximo = NULL;

                inserir_ordenado(lista, novo);
            }
            else{
                printf("\t Erro ao alocar memoria em preencher_lista\n ");
                break;
            }
        }
    }
}


void imprimir_lista(Lista *lista){
    No *aux = lista -> inicio;

    printf("\n\tLista ordenada: Tamanho: %d\n",lista-> tam);
    while(aux != NULL){
        printf("\tCaracter: %c Frequencia: %d\n", aux->caracter, aux->frequencia);
        aux = aux->proximo;
    }
}


No* remove_no_inicio (Lista * lista){
    No *aux = NULL;

    if (lista -> inicio){
        aux = lista -> inicio;
        lista -> inicio = aux -> proximo;
        aux -> proximo = NULL;
        lista-> tam--;
    }
    return aux;
}


No*montar_arvore(Lista *lista){
    No *primeiro, *segundo, *novo;
    while ( lista-> tam > 1)
    {
        primeiro = remove_no_inicio(lista);
        segundo = remove_no_inicio (lista);
        novo = malloc (sizeof(No));

        if(novo){
            novo -> caracter = '+';
            novo -> frequencia = primeiro -> frequencia + segundo -> frequencia;
            novo -> esquerda = primeiro;
            novo -> direita = segundo;
            novo -> proximo = NULL;

            inserir_ordenado(lista, novo);
        }

        else{
            printf("\n\tErro na alocação de memoria em montar_arvore\n");
            break;
        }
    }
    return lista -> inicio;

}


void imprimir_arvore(No *raiz, int tam){
    if (raiz-> esquerda == NULL && raiz -> direita == NULL)
        printf("\tFolha: %c\tAltura: %d\n", raiz ->caracter, tam);

    else{
        imprimir_arvore(raiz-> esquerda, tam + 1);
        imprimir_arvore(raiz-> direita,tam + 1);
    }
}


int altura_arvore(No *raiz){
    int esquerda,direita;

    if (raiz == NULL)
        return -1;
    else{
        esquerda = altura_arvore(raiz -> esquerda) +1;
        direita = altura_arvore (raiz-> direita) +1;

        if (esquerda > direita)
            return esquerda;
        else
            return direita;
    }
}


char** aloca_dicionario (int colunas){
    char**dicionario;
    int i;

    dicionario = malloc(sizeof(char*) *TAM);

    for(i = 0;i< TAM; i++)
        dicionario[i]=calloc(colunas, sizeof(char));

    return dicionario;
}


void gerar_dicionario(char **dicionario, No *raiz, char *caminho, int colunas){
    char esq [colunas], dir[colunas];

    if ( raiz->esquerda == NULL && raiz->direita == NULL)
        strcpy(dicionario[raiz->caracter], caminho);
    else{
        strcpy(esq,caminho);
        strcpy(dir,caminho);

        strcat(esq, "0");
        strcat(dir, "1");

        gerar_dicionario(dicionario, raiz->esquerda, esq, colunas);
        gerar_dicionario(dicionario, raiz->direita, dir, colunas);
    }
}


void imprime_dicionario(char **dicionario){
    int i;
    printf("\tDicionario:\n");
    for(i=0; i < TAM; i++){
        if(strlen(dicionario[i])>0)
            printf("\t%3d: %s\n", i, dicionario[i]);
    }
}


int calcula_tamanho_string(char **dicionario, unsigned char *texto){
    int i = 0, tam = 0;
    while (texto[i]!= '\0'){
        tam = tam + strlen(dicionario [texto[i]]);
        i++;
    }

    return tam + 1;
}


char* codificar (char **dicionario, unsigned char *texto){

    int i = 0, tam = calcula_tamanho_string(dicionario,texto);

    char *codigo = calloc (tam, sizeof(char));

    while (texto[i] != '\0')
    {
        strcat(codigo, dicionario[texto[i]]);
        i++;
    }
    return codigo;
}


char* decodificar (unsigned char texto[], No *raiz){

    int i = 0;
    No*aux = raiz;
    char temp[2];
    char *decodificado = calloc(strlen(texto),sizeof(char));

    while (texto[i] != '\0')
    {
        if(texto[i] == '0')
            aux = aux->esquerda;
        else
            aux = aux->direita;

        if(aux-> esquerda == NULL && aux -> direita == NULL){
            temp [0] = aux ->caracter;
            temp [1] = '\0';
            strcat(decodificado,temp);
            aux = raiz ;
        }

        i++;

    }
    return decodificado;
}


int descobrir_tamanho(){
    FILE *arq = fopen("teste.txt", "r");
    int tam = 0;

    if (arq){
        while(fgetc(arq) != -1){
            tam++;
        }
    }
    else
        printf("Erro ao abrir arquivo em descobrir_tamanho\n");
    return tam;
}


void ler_texto(unsigned char *texto){
    FILE *arq = fopen("teste.txt", "r");
    char letra;
    int i = 0;

    if (arq){
        while(!feof(arq)){
            letra = fgetc(arq);
            if (letra != -1){
                texto[i] = letra;
                i++;
            }
        }
    }
    else
        printf("Erro ao abrir arquivo em ler_texto\n");
    return ;
}


void compactar (unsigned char str[]){
    FILE*arquivo = fopen("compactado.wg", "wb");
    int i = 0, j = 7;
    unsigned char mascara, byte = 0;

    if (arquivo){
        while(str[i] != '\0'){
            mascara = 1;
            if(str[i] =='1'){
                mascara = mascara <<j;
                byte = byte | mascara;
            }
            j--;

            if(j<0){    //tem um byte formado
                fwrite(&byte, sizeof(unsigned char), 1, arquivo);
                byte = 0;
                j = 7;
            }

            i++;
        }
        printf("TEXTO COMPACTADO -> 'compactado.wg'");
        if (j != 7)
            fwrite(&byte, sizeof(unsigned char), 1, arquivo);

        fclose(arquivo);

    }

    else
        printf("\n Erro ao abrir/criar arquivo em compactar \n");
}


unsigned int eh_bit_um(unsigned char byte, int i){
    unsigned char mascara = (1 << i);
    return byte & mascara;
}


void descompactar (No *raiz){
    FILE *arquivo = fopen("compactado.wg", "rb");
    unsigned char byte;
    int i;
    No *aux = raiz;

    if (arquivo) {
        while(fread(&byte, sizeof(unsigned char), 1, arquivo)){
            for(i = 7; i>= 0; i--) {
                if(eh_bit_um(byte, i))
                    aux = aux->direita;
                else
                    aux = aux->esquerda;

                if (aux->esquerda == NULL && aux->direita == NULL){ //Está na folha da árvore {
                    printf("%c", aux->caracter);
                    aux = raiz;
                }
            }
        }
        fclose(arquivo);
    }
    else
        printf("\nErro ao abrir arquivo em descompactar\n");
}

int main(){

    unsigned char *texto;
    unsigned int tabela_frequencia[TAM];
    Lista *lista;
    No *arvore;
    int colunas, tam, escolha;
    char **dicionario;
    char * codificado, *decodificado;

    setlocale(LC_ALL, "Portuguese");

    tam = descobrir_tamanho();
    printf("\nTamanho do texto: %d\n", tam);

    texto = calloc(tam + 2, sizeof(unsigned char));
    ler_texto(texto);
    printf("\nTEXTO: %s\n", texto);

    //PARTE 1
    iniciaizar_tabela_com_zero(tabela_frequencia);
    preenche_tabela_sequencia(tabela_frequencia, texto);
    imprime_tabela_sequencia(tabela_frequencia);

    //PARTE 2
    cria_lista(&lista);
    preencher_lista (tabela_frequencia, &lista);
    imprimir_lista(&lista);

    //PARTE 3
    arvore = montar_arvore(&lista);
    printf("\n\tArvore de huffam\n");
    imprimir_arvore(arvore, 0);

    //PARTE 4
    colunas = altura_arvore(arvore)+ 1;

    printf("Alocando dicionário");
    dicionario = aloca_dicionario(colunas);

    printf("Gerando dicionario");

    gerar_dicionario(dicionario, arvore,"", colunas);

    printf("Printando dicionário");
    imprime_dicionario(dicionario);

    //PARTE 5
    codificado = codificar(dicionario,texto);
    printf("\n\tTexto codificado: %s\n", codificado);


    //decodificado = decodificar(codificado,arvore);
    //printf("\n\tTexto decodificado: %s\n",decodificado);

    printf("1 - Compactar\n2 - Descompactar\n--> ");
    scanf("%d", &escolha);

    if (escolha == 1){
        compactar(codificado);
    }
    else{
        descompactar(arvore);
    }

    free(texto);
    free(codificado);
    free(decodificado);

    return 0;
}
