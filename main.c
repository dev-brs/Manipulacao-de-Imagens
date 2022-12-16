#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
 
#define IMAGEM2 "immortals.bmp"
#define IMAGEM "lg.bmp"
//#define IMAGEM "rickandmorty.bmp"

#define DESLOCAMENTO 3
#define AUXCIRCULO 2000 //quantidade de pontos que seram salvos

typedef struct cabecalho
{
    unsigned short fType;//tipo do arquivo
    unsigned int fSize;//Tamanho do arquivo (cabeçalho)
    unsigned short mR1;//espaço reservado
    unsigned short mR2;//espaço reservado
    unsigned int byteshift;//especifica o DESLOCAMENTO em bytes do início
} CAB;

typedef struct dados
{
    unsigned int tamanho_cab; // tamanho do cabeçalho
    unsigned int largura;// largura da imagem em pixels
    unsigned int altura;//altura da imagem em pixels
    unsigned short planos;//qntdade de planos
    unsigned short bitcount;//bits/pixel
    unsigned int bCompress;//compressão utilizada
    unsigned long int tamanho_img_byte;//tamanho da imagem em byte
    unsigned int resXPPMeter;//resolução horizontal pixel/metro
    unsigned int resYPPMeter;//resolução vertical pixel/metro
    unsigned int numClrUsed;//numeros de cores na imagem
    unsigned int numClrImpor;//numero de cores importantes
} DADOS;
 
typedef struct circulo
{
    int linha;
    int coluna;
}COORDENADAS;


int DESENHA(unsigned int largura, int tamanho,int el,unsigned char *pPixels,unsigned char *pPixelsAUX,int x1,int x2,int y1,int y2)
{
    COORDENADAS reta[AUXCIRCULO];

    int i,x,aux;
    float delta_y = y2-y1;
    float delta_x = x2-x1;

    float m = (delta_y/delta_x);
    int y = m*(x - x1) + y1;


    for(unsigned long int i=0; i<tamanho; i++)
    {
        pPixels[i]=0;
    }

    i=0;
    for(x=0;x<largura;x++)
    {
        y = m*(x - x1) + y1;
        reta[i].linha=y;
        reta[i].coluna=x;
        printf("Imagem[%d][%d]\n",reta[i].linha,reta[i].coluna);
        i++;
        pPixels[(y*largura+x)*DESLOCAMENTO]=255;
    }
    i=0;
}
int FIND2(unsigned int largura,unsigned int altura,int tamanho2,int red2,int blue2,int green2,unsigned char *pPixels,int *x2,int *y2)
{
    FILE *coord2;
    COORDENADAS pontos2[AUXCIRCULO];

    int aux,aux1,ponto=0;
    int maior_linha,menor_linha;
    int maior_coluna,menor_coluna;
    int coluna_origem2,linha_origem2;
    int diametro2;
    coord2 =fopen("Coordenadas da segunda imagem.txt","w");
    for(unsigned long int i=0; i<tamanho2; i+=DESLOCAMENTO)
    {
        aux=0;
        if(pPixels[i]==blue2 && pPixels[i+1]==green2 && pPixels[i+2]==red2)
        {
            aux1=i/3;
            while(aux1>largura)
            {
                aux++;
                aux1=aux1-largura;
            }
            pontos2[ponto].linha=aux1;
            pontos2[ponto].coluna=aux;

            fprintf(coord2,"Ponto %d, se localiza na posição [%d][%d]\n",ponto,pontos2[ponto].linha,pontos2[ponto].coluna);
            ponto++;
        }
    }
    for (aux=0;aux<ponto;aux++)
    {
        if(aux==0)
        {
            maior_linha=pontos2[aux].linha;
            menor_linha=pontos2[aux].linha;
        }
        if(pontos2[aux].linha>maior_linha)
        {
            maior_linha=pontos2[aux].linha;
        }
        else
        {
            if(pontos2[aux].linha<menor_linha)
            {
                menor_linha=pontos2[aux].linha;
            }
        }
    }
    for (aux=0;aux<ponto;aux++)
    {
        if(aux==0)
        {
            maior_coluna=pontos2[aux].coluna;
            menor_coluna=pontos2[aux].coluna;
        }
        if(pontos2[aux].coluna>maior_coluna)
        {
            maior_coluna=pontos2[aux].coluna;
        }
        else
        {
            if(pontos2[aux].coluna<menor_coluna)
            {
                menor_coluna=pontos2[aux].coluna;
            }
        }
    }

    fclose(coord2);
    diametro2=maior_coluna-menor_coluna;
    coluna_origem2=menor_coluna+(diametro2/2);
    linha_origem2=menor_linha+(diametro2/2);
    printf("\nAs coordenadas de cada ponto se encontram no arquivo 'Coordenadas da segunda imagem.txt'");

    *x2 = linha_origem2;
    *y2 = coluna_origem2;
}
int FIND1(unsigned int largura,unsigned int altura,int tamanho,int red,int blue,int green,unsigned char *pPixels,int *x1,int *y1)
{
    COORDENADAS pontos[AUXCIRCULO];
    FILE *coord1;

    int aux,aux1,ponto=0;
    int maior_linha,menor_linha;
    int maior_coluna,menor_coluna;
    int coluna_origem,linha_origem;
    int diametro;

    coord1 =fopen("Coordenadas da primeira imagem.txt","w");
    for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
    {
        aux=0;
        if(pPixels[i]==blue && pPixels[i+1]==green && pPixels[i+2]==red)
        {
            aux1=i/3;
            while(aux1>largura)
            {
                aux++;
                aux1=aux1-largura;
            }
            pontos[ponto].linha=aux1;
            pontos[ponto].coluna=aux;
            fprintf(coord1,"Ponto %d, se localiza na posição [%d][%d]\n",ponto,pontos[ponto].linha,pontos[ponto].coluna);
            ponto++;
        }
    }
    for (aux=0;aux<ponto;aux++)
    {
        if(aux==0)
        {
            maior_linha=pontos[aux].linha;
            menor_linha=pontos[aux].linha;
        }
        if(pontos[aux].linha>maior_linha)
        {
            maior_linha=pontos[aux].linha;
        }
        else
        {
            if(pontos[aux].linha<menor_linha)
            {
                menor_linha=pontos[aux].linha;
            }
        }
    }
    for (aux=0;aux<ponto;aux++)
    {
        if(aux==0)
        {
            maior_coluna=pontos[aux].coluna;
            menor_coluna=pontos[aux].coluna;
        }
        if(pontos[aux].coluna>maior_coluna)
        {
            maior_coluna=pontos[aux].coluna;
        }
        else
        {
            if(pontos[aux].coluna<menor_coluna)
            {
                menor_coluna=pontos[aux].coluna;
            }
        }
    }
    fclose(coord1);
    diametro=maior_coluna-menor_coluna;
    coluna_origem=menor_coluna+(diametro/2);
    linha_origem=menor_linha+(diametro/2);

    printf("\nAs coordenadas de cada ponto se encontram no arquivo 'Coordenadas da primera imagem.txt'");

    *x1 = linha_origem;
    *y1 = coluna_origem;
}
void BORDA(unsigned int largura, int tamanho,int el,unsigned char *pPixels,unsigned char *pPixelsAUX)
{
    int aux=0;
    int media=0;
    for(int i=0; i<tamanho;i+=DESLOCAMENTO)
    {
        pPixelsAUX[aux]=(pPixels[i]+pPixels[i+1]+pPixels[i+2])/3;
        aux++;
    }
    aux=0;
    for(int i=0; i<tamanho;i+=DESLOCAMENTO)
    {
        pPixels[i]=pPixelsAUX[aux];
        pPixels[i+1]=pPixelsAUX[aux];
        pPixels[i+2]=pPixelsAUX[aux];
        aux++;
    }

    if (el==1)
    {
        aux=0;
        media=0;
        for(int i=0;i<tamanho;i+=DESLOCAMENTO)
        {
            media=0;
            if(i>0)
            {
                aux++;
            }
                if(i>(largura*DESLOCAMENTO)*2 && i<tamanho - ((largura*(DESLOCAMENTO))*2))
                {
                    media=pPixelsAUX[aux]*(8);
                    media+=pPixelsAUX[aux+1]*(-1);
                    media+=pPixelsAUX[aux-1]*(-1);
                    media+=pPixelsAUX[aux+largura]*(-1);
                    media+=pPixelsAUX[aux+largura+1]*(-1);
                    media+=pPixelsAUX[aux+largura-1]*(-1);
                    media+=pPixelsAUX[aux-largura]*(-1);
                    media+=pPixelsAUX[aux-largura+1]*(-1);
                    media+=pPixelsAUX[aux-largura-1]*(-1);

                    if(media<90)
                    {
                        pPixels[i]=255;
                        pPixels[i+1]=255;
                        pPixels[i+2]=255;
                    }
                    else
                    {
                        pPixels[i]=255;
                        pPixels[i+1]=0;
                        pPixels[i+2]=255;
                    }
                }
                else
                {
                    pPixels[i]=255;
                    pPixels[i+1]=0;
                    pPixels[i+2]=255;
                }
        }
    }
    if(el==2)
    {
        aux=0;
        media=0;
        for(int i=0;i<tamanho;i+=DESLOCAMENTO)
        {
            media=0;
            if(i>0)
            {
                aux++;
            }
                if(i>(largura*DESLOCAMENTO)*2 && i<tamanho - ((largura*(DESLOCAMENTO))*2)){
                    
                    media= pPixelsAUX[aux]*(16);
                    media+= (pPixelsAUX[aux+largura])*(-2);
                    media+= (pPixelsAUX[aux-largura])*(-2);
                    media+= (pPixelsAUX[aux+1])*(-2);
                    media+= (pPixelsAUX[aux-1])*(-2);
                    media+= (pPixelsAUX[aux+(2*largura)])*(-1);
                    media+= (pPixelsAUX[aux-(2*largura)])*(-1);
                    media+= (pPixelsAUX[aux+largura+1])*(-1);
                    media+= (pPixelsAUX[aux+largura-1])*(-1);
                    media+= (pPixelsAUX[aux-largura+1])*(-1);
                    media+= (pPixelsAUX[aux-largura+1])*(-1);
                    media+= (pPixelsAUX[aux-2])*(-1);
                    media+= (pPixelsAUX[aux+2])*(-1);

                    if(media<90)
                    {
                        pPixels[i]=255;
                        pPixels[i+1]=255;
                        pPixels[i+2]=255;
                    }
                    else
                    {
                        pPixels[i]=255;
                        pPixels[i+1]=0;
                        pPixels[i+2]=255;
                    }
                }
                else
                {
                    pPixels[i]=255;
                    pPixels[i+1]=0;
                    pPixels[i+2]=255;
                }
        }
    }
}
void BLUR(unsigned int largura, int tamanho,int el,unsigned char *pPixels,unsigned char *pPixelsAUX)
{
        int aux=0;
        for(unsigned long int i=0; i<tamanho;i+=DESLOCAMENTO)
        {
            pPixelsAUX[aux]=(pPixels[i]+pPixels[i+1]+pPixels[i+2])/3;
            aux++;
        }
        aux=0;
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
            pPixels[i]=pPixelsAUX[aux];
            pPixels[i+1]=pPixelsAUX[aux];
            pPixels[i+2]=pPixelsAUX[aux];
            aux++;
        }
        int media;

        if(el==1)//borda 3x3
        {
            aux=0;
            for(int i=0;i<tamanho;i+=DESLOCAMENTO)
            {
                if(i>0)
                {
                    aux++;
                }
                if(i>largura*DESLOCAMENTO && i<tamanho - (largura*(DESLOCAMENTO)))
                {
                    media=pPixelsAUX[aux];
                    media+=pPixelsAUX[aux+1];
                    media+=pPixelsAUX[aux-1];

                    media+=pPixelsAUX[aux+largura];
                    media+=pPixelsAUX[aux+largura+1];
                    media+=pPixelsAUX[aux+largura-1];

                    media+=pPixelsAUX[aux-largura];
                    media+=pPixelsAUX[aux-largura+1];
                    media+=pPixelsAUX[aux-largura-1];

                    media/=9;

                    pPixels[i]=media;
                    pPixels[i+1]=media;
                    pPixels[i+2]=media;
                }
                else
                {
                    pPixels[i]=0;
                    pPixels[i+1]=0;
                    pPixels[i+2]=0;
                }  
            }
        }
    if(el==2)//borda 5x5
    {
        aux=0;
        for(int i=0;i<tamanho;i+=DESLOCAMENTO)
        {
            if(i>0)
            {
                aux++;
            }
                if(i>largura*DESLOCAMENTO && i<tamanho - (largura*(DESLOCAMENTO)))
                {
                    media=0;
                    for(int aux1=0; aux1<9; aux1++)
                    {
                        media+=pPixelsAUX[aux-4+aux1]; // pixels do meio
                        
                        if(i<tamanho - (largura*(DESLOCAMENTO)))
                            media+= pPixelsAUX[aux-4+aux1+(largura)]; // pixels 1 acima do meio
                    
                        if(i<tamanho - (largura*(DESLOCAMENTO)*2))
                            media+= pPixelsAUX[aux-4+aux1+(largura*2)]; // pixels 2 acima do meio
                        
                        if(i<tamanho - (largura*(DESLOCAMENTO)*3))
                            media+= pPixelsAUX[aux-4+aux1+(largura*3)]; // pixels 3 acima do meio
        
                        if(i<tamanho - (largura*(DESLOCAMENTO)*4))
                            media+= pPixelsAUX[aux-4+aux1+(largura*4)]; // pixels 4 acima do meio

                        if(i>largura*DESLOCAMENTO)
                            media+= pPixelsAUX[aux-4+aux1-(largura)]; // pixels 1 abaixo do meio

                        if(i>largura*DESLOCAMENTO*2)
                            media+= pPixelsAUX[aux-4+aux1-(largura*2)]; // pixels 2 abaixo do meio

                        if(i>largura*DESLOCAMENTO*3)
                            media+= pPixelsAUX[aux-4+aux1-(largura*3)]; // pixels 3 abaixo do meio

                        if(i>largura*DESLOCAMENTO*4)
                            media+= pPixelsAUX[aux-4+aux1-(largura*4)]; // pixels 4 abaixo do meio
                    }

                        media/=81;

                        pPixels[i]=media;
                        pPixels[i+1]=media;
                        pPixels[i+2]=media;
                }
                else
                {
                    pPixels[i]=0;
                    pPixels[i+1]=0;
                    pPixels[i+2]=0;
                }  
        }
    }
}
void DEIXA_COR(int tamanho, unsigned char *pPixels,int el)
{
    if(el==1)
    {
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
        pPixels[i+1]=0;
        pPixels[i+2]=0;
        }
    }
    if(el==2)
    {
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
        pPixels[i]=0;
        pPixels[i+2]=0;
        }
    }
    if(el==3)
    {
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
        pPixels[i]=0;
        pPixels[i+1]=0;
        }
    }
}
void REMOVE_COR(int tamanho, unsigned char *pPixels,int el)
{
    if(el==1)
    {
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
        pPixels[i]=0;
        }
    }
    if(el==2)
    {
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
        pPixels[i+1]=0;
        }
    }
    if(el==3)
    {
        for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
        {
        pPixels[i+2]=0;
        }
    }
}
void BLACK_AND_WHITE(int tamanho,unsigned char *pPixels)
{
    for(unsigned long int i=0; i<tamanho; i+=DESLOCAMENTO)
    {
        int media=(pPixels[i]+pPixels[i+1]+pPixels[i+2])/3;
        pPixels[i]=media;
        pPixels[i+1]=media;
        pPixels[i+2]=media;
    }
}
int UI()
{
    time_t mytime;
    mytime = time(NULL);
    printf(ctime(&mytime));
    int opc;
    printf("Manipulação de arquivo de imagem usando matrizes, ponteiros, structs.\n");
    printf("\nQual tipo de alteração você deseja fazer?\n");
    printf("\n[1] - Tornar a imagem em tons de cinza.");
    printf("\n[2] - Retirar uma cor (R,G,B).");
    printf("\n[3] - Manter uma cor (R,G,B).");
    printf("\n[4] - Deixar a imagem em tons de cinza e borrar.");
    printf("\n[5] - Contornar a imagem.");
    printf("\n[6] - Achar um objeto na imagem.");
    printf("\n[7] - Colocar uma linha entre dois objetos em duas imagens diferentes.");
    printf("\n[0] - Para sair do programa");

    printf("\nSua opcao:");
    scanf("%d",&opc);

    return opc;
}
int main()
{
    setlocale(LC_ALL, "Portuguese");
    int x,x1,x2,y1,y2;
    int a,b,c;
    float m,n;
    unsigned long int tamanho;
    unsigned long int tamanho2;
    int red,blue,green,red2,blue2,green2;
    int aux=0;
    int opc=0;
    int el=0;

    CAB header,header2;
    DADOS dados_imagem,dados_imagem2;

    FILE *imagem;//Ponteiro de arquivo
    FILE *nova_imagem = fopen("nova_imagem.bmp","wb");

    FILE *imagem2;//Ponteiro de arquivo
    FILE *nova_imagem2 = fopen("nova_imagem2.bmp","wb");


    //IMAGEM 1
    imagem=fopen(IMAGEM,"rb");
    
    if(imagem==NULL)
    {
        printf("\n\nErro, A imagem não pode ser aberta, o programa será fechado\n\n");
        return 1;
    }
    fread(&header,14,1,imagem);//le dados do arquivo e armazena em cab
    fread(&dados_imagem,40,1,imagem);//armazena os bytes no endereço da struct

    tamanho = dados_imagem.largura * dados_imagem.altura * 4;//o 4 é por causa da quantidade de Bytes por pixel
    
    unsigned char *pPixels = (unsigned char*) malloc(tamanho);
    unsigned char *pPixelsAUX = (unsigned char*) malloc(tamanho/3);
    fread(pPixels,tamanho,1,imagem);
    fclose(imagem);//fecha o arquivo

    if(nova_imagem == NULL)
    {
        printf("\nNao foi possivel abrir imagem para escrita\n\n");
        return 2;
    }
    
    fwrite(&header,14,1,nova_imagem);//coloca o cabecalho
    fwrite(&dados_imagem,40,1,nova_imagem);//coloca a imagem
    fwrite(pPixels,tamanho,1,nova_imagem);//escreve os dados de pixels no arquivo
    fclose(nova_imagem);
    system("cls");

    fopen ("nova_imagem.bmp","wb");
    if(nova_imagem == NULL)
    {
        printf("\nNao foi possivel abrir imagem para escrita\n\n");
        exit(0);
    }
    fwrite(&header,14,1,nova_imagem);//coloca o cabecalho
    fwrite(&dados_imagem,40,1,nova_imagem);//coloca a imagem

    //IMAGEM 2

    imagem2=fopen(IMAGEM2,"rb");
    
    if(imagem2==NULL)
    {
        printf("\n\nErro, A imagem não pode ser aberta, o programa será fechado\n\n");
        return 1;
    }
    fread(&header2,14,1,imagem2);//le dados do arquivo e armazena em cab
    fread(&dados_imagem2,40,1,imagem2);//armazena os bytes no endereço da struct

    tamanho2 = dados_imagem2.largura * dados_imagem2.altura * 4;//o 4 é por causa da quantidade de Bytes por pixel
    
    unsigned char *pPixels2 = (unsigned char*) malloc(tamanho2);
    unsigned char *pPixelsAUX2 = (unsigned char*) malloc(tamanho2/3);
    fread(pPixels2,tamanho2,1,imagem2);
    fclose(imagem2);//fecha o arquivo

    if(nova_imagem2 == NULL)
    {
        printf("\nNao foi possivel abrir imagem para escrita\n\n");
        return 2;
    }
    
    fwrite(&header2,14,1,nova_imagem2);//coloca o cabecalho
    fwrite(&dados_imagem2,40,1,nova_imagem2);//coloca a imagem
    fwrite(pPixels2,tamanho2,1,nova_imagem2);//escreve os dados de pixels no arquivo
    fclose(nova_imagem2);
    system("cls");
 
    opc=UI();
    //abrindo imagem1
    fopen ("nova_imagem.bmp","wb");
    if(nova_imagem == NULL)
    {
        printf("\nNao foi possivel abrir imagem para escrita\n\n");
        exit(0);
    }
    fwrite(&header,14,1,nova_imagem);//coloca o cabecalho
    fwrite(&dados_imagem,40,1,nova_imagem);//coloca a imagem

    //abrindo imagem2
    fopen ("nova_imagem2.bmp","wb");
    if(nova_imagem2 == NULL)
    {
        printf("\nNao foi possivel abrir imagem para escrita\n\n");
        exit(0);
    }
    fwrite(&header2,14,1,nova_imagem2);//coloca o cabecalho
    fwrite(&dados_imagem2,40,1,nova_imagem2);//coloca a imagem

    switch (opc)
    {
        case 0://inicio do case 0
        exit(0);
        break;//fim do case 0

        case 1://inicio do case 2
            printf("Você escolheu deixar a imagem em tons de cinza");
            BLACK_AND_WHITE(tamanho,pPixels);
        break;//fim do case 2
        
        case 2://inicio do case 3
            printf("Você escolheu retirar uma cor");
            printf("Qual cor você deseja remover");
            printf("\n[1] - BLUE.");
            printf("\n[2] - GREEN.");
            printf("\n[3] - RED (R,G,B).");

            printf("\nSua opcao:");
            scanf("%d",&el);

            REMOVE_COR(tamanho,pPixels,el);
        break;//fim do case 3

        case 3://inicio do case 4
            printf("Você escolheu manter uma cor");
            printf("Qual cor você deseja manter");
            printf("\n[1] - BLUE.");
            printf("\n[2] - GREEN.");
            printf("\n[3] - RED (R,G,B).");

            printf("\nSua opcao:");
            scanf("%d",&el);

            DEIXA_COR(tamanho,pPixels,el);
        break;//fim do case 4

        case 4://inicio do case 5
            printf("O quão borrado você deseja que a imagem fique?");
            printf("OBS: Quanto maior a matriz, mais borrada a imagem ficará");
            printf("\n[1] - Borrar (3x3).");
            printf("\n[2] - Borrar (5x5).");

            printf("\nSua opcao:");
            scanf("%d",&el);

            BLUR(dados_imagem.largura,tamanho,el,pPixels,pPixelsAUX);
        break;//fim do case 5

        case 5://inicio do case 6
            printf("O quão fácil deve ser de detectar a borda?");
            printf("OBS: Quanto maior a matriz, mais fácil a borda será reconhecida");
            printf("\n[1] - Detectar borda (3x3).");
            printf("\n[2] - Detectar borda (5x5).");

            printf("\nSua opcao:");
            scanf("%d",&el);

            BORDA(dados_imagem.largura,tamanho,el,pPixels,pPixelsAUX);
        break;//fim do case 6

        case 6://inicio do case 7
            printf("Você escolheu achar um objeto, insira as respectivas cores do objeto que você deseja achar");
            printf("\nRED: ");
            scanf("%d",&red);
            printf("\nBLUE:");
            scanf("%d",&blue);
            printf("\nGREEN:");
            scanf("%d",&green);

            FIND1(dados_imagem.largura,dados_imagem.altura,tamanho,red,blue,green,pPixels,&x1,&y1);
            printf("%d,%d",x1,y1);
        break;//fim do case 7

        case 7://inicio do case 7
            printf("Você escolheu achar um objeto, insira as respectivas cores do objeto que você deseja achar");
            printf("\nCor do objeto - Primeira Imagem.");
            printf("\nRED: ");
            scanf("%d",&red);
            printf("\nBLUE:");
            scanf("%d",&blue);
            printf("\nGREEN:");
            scanf("%d",&green);

            printf("\nCor do objeto - Segunda Imagem.");
            printf("\nRED: ");
            scanf("%d",&red2);
            printf("\nBLUE:");
            scanf("%d",&blue2);
            printf("\nGREEN:");
            scanf("%d",&green2);

            FIND1(dados_imagem.largura,dados_imagem.altura,tamanho,red,blue,green,pPixels,&x1,&y1);
            FIND2(dados_imagem2.largura,dados_imagem2.altura,tamanho2,red2,blue2,green2,pPixels2,&x2,&y2);

            DESENHA(dados_imagem.largura,tamanho,el,pPixels,pPixelsAUX,x1,x2,y1,y2);

        break;//fim do case 7
    }
    fwrite(pPixels2,tamanho2,1,nova_imagem2);
    fclose(nova_imagem2);
    fwrite(pPixels,tamanho,1,nova_imagem);
    fclose(nova_imagem);
}