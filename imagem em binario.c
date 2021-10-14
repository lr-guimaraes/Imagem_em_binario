#include <stdio.h>
#include <stdlib.h>//MANIPULAÇÃO DE STRINGS 
#include <string.h>//MANIPULAÇÃO DE STRINGS
#include <ctype.h>

//PILHA
typedef struct sPixel{
    unsigned char d; //DIFERENTE DO FUNDO 
    unsigned char f; //FUNDO
}sPixel;

typedef struct sAnaliza{
    int I; //LINHA
    int J; //COLUNA
    struct sAnaliza *antes;
}sAnaliza;

typedef struct sPilha{
    int qnt;
    sAnaliza *topo;
}sPilha;
//CABEÇARIO DAS FUNÇOES
sPilha *construtor(int *);
void PUSH(sPilha *,int ,int ,int *);
void POP(sPilha *, sAnaliza *, int *);
void VeriCont(sPixel **,sPilha *,int ,int ,int ,int );

int main(){
    //VARIAVEIS RELACIONADAS AO PRINT DA IMAGEM
    char arc[20], *id[3]; 
    unsigned char r,g,b;
    unsigned char rf, gf, bf;
    int posicao;
    FILE *fp;
    //VARIAVEIS RELACIONADAS AO CONTADOR DE OBJETOS
    sAnaliza analiza;
    sPilha *pilha;
    sPixel **IMG;
    int linhas,colunas,max;//VARIAVEIS RELACIONADAS AO TAMANHO DA IMAGEM
    //VARIAVEIS AUXILIARES
    int i,j, opcao,obj,resp;
    do{
        obj= 0;//A CONTAGEM DE OBEJETOS INICIA EM 0 TODA VEZ QUE O LOOP do->while ACONTECER
        //MENU
        setbuf(stdin,NULL);
        printf("===== BEM VINDO =====\n");
        printf("deseja abrir qual imagem?\n");
        printf("0 - sair\n");
        printf("1 - imagem 01 \n");
        printf("2 - imagem 05\n");
        printf("3 - imagem 06\n");                                                     
        printf("4 - digitar o nome da imagem\n");
        printf("Opcao: ");
        scanf("%d",&opcao);
        //ESCOLHA DA IMAGEM
        switch (opcao){
            case 0://SAIR
                printf("Obrigado por usar o software");
                break;
                return 0;
            case 1:
                fp = fopen("01.ppm", "rb"); 
                while (IMG == '#'){
                    while (IMG = getc(fp) != "\n" );//ATÉ ENCONTRAR O FINAL DA LINHA
                }
                fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SALVANDO O COMPRIMENTO E A AUTURA DA IMAGEM
                break;
            case 2:
                fp = fopen("05.ppm", "rb");
                while (IMG == '#'){
                    while (IMG = getc(fp) != "\n" );//ATÉ ENCONTRAR O FINAL DA LINHA
                }
                fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SALVANDO O COMPRIMENTO E A AUTURA DA IMAGEM
                break;
            case 3:
                fp = fopen("06.ppm", "rb");
                while (IMG == '#'){
                    while (IMG = getc(fp) != "\n" );//ATÉ ENCONTRAR O FINAL DA LINHA
                }
                fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SALVANDO O COMPRIMENTO E A AUTURA DA IMAGEM
                break;
            case 4:
                setbuf(stdin,NULL);
                printf("Digite o nome da imagem com o formato: ");
                gets(arc);
                fp = fopen(arc, "rb");
                  fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SALVANDO O COMPRIMENTO E A AUTURA DA IMAGEM
                if (strcmp(id,"P6") > 0 ||strcmp(id,"P6")<0){//TESTANDO O FORMATO
                    printf("ERRO!! O arquivo nao esta no fomato <.ppm>, converta-o aquivo e tente novamente\n");
                }else{
                    printf("A imagem escolhida foi encontrada com sussesso\n");
                    while (IMG == '#'){
                        while (IMG = getc(fp) != "\n" );//ATÉ ENCONTRAR O FINAL DA LINHA
                    }
                }
                break;
            default:
                printf("Essa imagem nao existe");
                return 0;
                break;
        }
         //ALOCAÇÃO DA INMAAGEM
        pilha = construtor(&resp);//ARRUMAR5
        IMG = (sPixel **)calloc(linhas,sizeof(sPixel*)); // alocando a matriz na memoria
        for (i = 0; i < linhas; i++){
            IMG[i] = (int *)calloc(colunas,sizeof(sPixel*));//alocando colunas com 0
        }
 
        posicao = ftell(fp); // pegar a posicao fisica  sPixel arquivo anter de ler dados da imagem    
        fscanf (fp, "%c%c%c", &rf, &gf, &bf);
        fseek(fp, posicao, SEEK_SET); // voltando ao início da imagem    

    // lendo os pixels da imagem, para cada pixel ler 3 valores1, Red Green Blue (a cor em sPixel)
        for (i=0; i<linhas; i++) {
            for (j=0; j<colunas; j++){
                fscanf (fp, "%c%c%c", &r, &g, &b);
                if ((r!=rf) || (g!=gf)  || (b!=bf)){  // houve mudanca de cor em relacao ao fundo
                    printf("1"); // colocar 1 na matriz alocada
                    IMG[i][j].d = 1;//DEFININCDO 1 COMO DIFERENTE DO FUNDO
                } else { 
                     IMG[i][j].f = 0;//DEFININCDO 0 COMO FUNDO
                    printf("0");
                }
            }
            printf("\n"); 
        }
        //ACHANDO A SEMENTE
       for (i = 0; i < linhas; i++){
            for (j = 0; j < colunas; j++){
                if((IMG[i][j].d == 1) &&(IMG[i][j].f == 0)){
                   VeriCont(IMG,pilha,i,j,linhas,colunas);
                    while ((pilha->qnt) >0){
                        POP(pilha,&analiza,&resp);
                        VeriCont(IMG,pilha,analiza.I,analiza.J,linhas,colunas);             
                    } 
                    obj++;//MAIS 1 OBJETO CONTADO NA IMAGEM  
                }
            }  
        }   
        if(obj > 0){
         printf("A imagem possui %d objetos\n",obj);
        }
        fclose(fp);//ECHER ARQUIVO
        linhas = colunas = 0; //PARA NÃO PRINTAR IMAGEM DE "ZEROS" NO FINAL
    }while (opcao != 0);
    exit(EXIT_SUCCESS); //EQUIVALENTE AO return 0;
}

sPilha *construtor(int *resp){
    sPilha *p;
    p = (sPilha*)malloc(sizeof(sPilha));
    if(p==NULL){
        *resp = -1;//ERRO NA LOCAÇÃO DA PILHA
    }else{
        p->topo = NULL;
        p->qnt = 0;
        *resp = 1;
    }  
     return p;
}

void PUSH(sPilha *pilha,int I,int J,int *result){
    sAnaliza *analiza = (sAnaliza*)malloc(sizeof(sAnaliza));
    analiza->I = I;
    analiza->J = J;
    if(analiza == NULL){  
         *result = -1;
    }
    else if (pilha->topo == NULL){//SE A PILHA ESTA VAZIA ELE COLOCA O PRIMEITO PIXEL NO TOPO 
        analiza->antes = NULL; 
        pilha->topo = analiza;
    }
     
    else{//SENA NAO ELE COLOCA NO NOVO PIXEL NO TOPp
        analiza->antes = pilha->topo;
        pilha->topo = analiza;
    }
    pilha->qnt++;//ELEMENTOS DA PILHA
    *result = 0;
}
// Ler uma oarte da pilha
void POP(sPilha *pilha, sAnaliza *analiza, int *tamanho){
    sAnaliza *aux;
    if (pilha->topo == NULL){
        printf("A pilha esta vazia");//TESTE
        *tamanho = -1;//a pilha esta vazia
    }else{
        aux = pilha->topo;
        analiza->I = aux->I;
        analiza->J = aux->J;
        pilha->topo = aux->antes;
        pilha->qnt--;
        free(aux);
    }
}

void VeriCont(sPixel **IMG,sPilha *pilha,int i ,int j,int linhas ,int colunas){
    int P;
    IMG[i][j].f = 2;
    if (j>0){
        if((IMG[i][j-1].d == 1) && (IMG[i][j-1].f== 0)){
            PUSH(pilha,i,(j-1),&P);
            IMG[i][j-1].f = 2;
        }
    }
   if (i> 0){
        if((IMG[i-1][j].d == 1) && (IMG[i-1][j].f== 0)){
            PUSH(pilha,(i-1),j,&P);
            IMG[i-1][j].f = 2;
        }
    }  
   if(i < linhas-1){
       if((IMG[i+1][j].d == 1) && (IMG[i+1][j].f== 0)){
            PUSH(pilha,(i+1),j,&P);
            IMG[i+1][j].f = 2;
        }
   }   
    if(j < colunas-1){
       if((IMG[i][j+1].d == 1) && (IMG[i][j+1].f== 0)){
            PUSH(pilha,i,(j+1),&P);
            IMG[i][j+1].f = 2;
        }  
    }
} 
