#include <stdio.h>
#include <stdlib.h>//manipulation of STRINGS 
#include <string.h>//Mmanipulation of STRINGS
#include <ctype.h>

//PILHA
typedef struct sPixel{
    unsigned char d; //different from the background
    unsigned char f; //background
}sPixel;

typedef struct sAnaliza{
    int I; //Line
    int J; //column
    struct sAnaliza *antes;
}sAnaliza;

typedef struct sPilha{
    int qnt;
    sAnaliza *topo;
}sPilha;
//HEADING OF FUNCTIONS
sPilha *construtor(int *);
void PUSH(sPilha *,int ,int ,int *);
void POP(sPilha *, sAnaliza *, int *);
void VeriCont(sPixel **,sPilha *,int ,int ,int ,int );

int main(){
    //VARIABLES RELATED TO IMAGE PRINT
    char arc[20], *id[3]; 
    unsigned char r,g,b;
    unsigned char rf, gf, bf;
    int posicao;
    FILE *fp;
    //VARIABLES RELATED TO THE OBJECT COUNTER
    sAnaliza analiza;
    sPilha *pilha;
    sPixel **IMG;
    int linhas,colunas,max;//VARIABLES RELATED TO IMAGE SIZE
    int i,j, opcao,obj,resp; //AUXILIARY VARIABLES
    do{
        obj= 0;//OBJECT COUNT STARTS AT 0 EVERY TIME DO->while LOOP HAPPENS
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
        //CHOOSE THE IMAGE
        switch (opcao){
            case 0://EXIT
                printf("Obrigado por usar o software");
                break;
                return 0;
            case 1:
                fp = fopen("01.ppm", "rb"); 
                while (IMG == '#'){
                    while (IMG = getc(fp) != "\n" );//UNTIL YOU FIND THE END OF THE LINE
                }
                fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SAVING THE LENGTH AND AUTURE OF THE IMAGE
                break;
            case 2:
                fp = fopen("05.ppm", "rb");
                while (IMG == '#'){
                    while (IMG = getc(fp) != "\n" );//UNTIL YOU FIND THE END OF THE COLUMN
                }
                fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SAVING THE LENGTH AND AUTURE OF THE IMAGE
                break;
            case 3:
                fp = fopen("06.ppm", "rb");
                while (IMG == '#'){
                    while (IMG = getc(fp) != "\n" );//UNTIL YOU FIND THE END OF THE LINE
                }
                fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SAVING THE LENGTH AND AUTURE OF THE IMAGE
                break;
            case 4:
                setbuf(stdin,NULL);
                printf("Digite o nome da imagem com o formato: ");
                gets(arc);
                fp = fopen(arc, "rb");
                  fscanf(fp,"%s %d %d %d %c \n",&id, &linhas,&colunas,&max, &rf);//SAVING THE LENGTH AND AUTURE OF THE IMAGE
                if (strcmp(id,"P6") > 0 ||strcmp(id,"P6")<0){//TESTING THE FORMAT
                    printf("ERRO!! O arquivo nao esta  sPixel fomato <.ppm>, converta-o aquivo e tente novamente\n");
                }else{
                    printf("A imagem escolhida foi encontrada com sussesso\n");
                    while (IMG == '#'){
                        while (IMG = getc(fp) != "\n" );//UNTIL YOU FIND THE END OF THE LINE
                    }
                }
                break;
            default:
                printf("Essa imagem nao existe");
                return 0;
                break;
        }
         //IMAGE ALLOCATION
        pilha = construtor(&resp);//ARRUMAR5
        IMG = (sPixel **)calloc(linhas,sizeof(sPixel*)); // allocating the array in memory
        for (i = 0; i < linhas; i++){
            IMG[i] = (int *)calloc(colunas,sizeof(sPixel*));//allocating columns with 0
        }
 
        posicao = ftell(fp); //get the physical position sPixel file before reading image data    
        fscanf (fp, "%c%c%c", &rf, &gf, &bf);
        fseek(fp, posicao, SEEK_SET); //going back to the beginning of the image   

    //reading image pixels, for each pixel read 3 values 1, Red Green Blue (the color in sPixel)
        for (i=0; i<linhas; i++) {
            for (j=0; j<colunas; j++){
                fscanf (fp, "%c%c%c", &r, &g, &b);
                if ((r!=rf) || (g!=gf)  || (b!=bf)){  // there was a change in color in relation to the background
                    printf("1"); //put 1 in the allocated array
                    IMG[i][j].d = 1;//DEFINING 1 AS DIFFERENT FROM THE FUND
                } else { 
                     IMG[i][j].f = 0;//DEFINING 0 AS THE BACKGROUND
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
                    obj++;//MORE 1 OBJECT COUNTED IN THE IMAGE
                }
            }  
        }   
        if(obj > 0){
         printf("A imagem possui %d objetos\n",obj);
        }
        fclose(fp);//CLOSE THE FILE
        linhas = colunas = 0; //NOT TO PRINT "ZEROS" IMAGE AT THE END
    }while (opcao != 0);
    exit(EXIT_SUCCESS); 
}

sPilha *construtor(int *resp){
    sPilha *p;
    p = (sPilha*)malloc(sizeof(sPilha));
    if(p==NULL){
        *resp = -1;// BATTERY LOCATION ERROR
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
    else if (pilha->topo == NULL){//IF THE BATTERY IS EMPTY IT PLACES THE FIRST PIXEL ON THE TOP
        analiza->antes = NULL; 
        pilha->topo = analiza;
    }
     
    else{//OTHERWISE IT PUTS THE NEW PIXEL AT THE TOP
        analiza->antes = pilha->topo;
        pilha->topo = analiza;
    }
    pilha->qnt++;//CELL ELEMENTS
    *result = 0;
}
//read part of the stack
void POP(sPilha *pilha, sAnaliza *analiza, int *tamanho){
    sAnaliza *aux;
    if (pilha->topo == NULL){
        printf("A pilha esta vazia");//TEST
        *tamanho = -1;//the pile is empty
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
