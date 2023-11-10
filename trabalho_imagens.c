#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define NOME_DA_IMAGEM "C:\\USUARIO\\ROBERTO\\AREA_DE_TRABALHO\\nome_da_imagem.bmp"

#pragma pack(1) // Desativa o alinhamento de bytes

struct CAB
{
    uint16_t fType;    // Tipo do arquivo
    uint32_t fSize;    // Tamanho do arquivo (cabeçalho)
    uint16_t mR1;      // Espaço reservado
    uint16_t mR2;      // Espaço reservado
    uint32_t byteshift; // Especifica o DESLOCAMENTO em bytes do início
};

struct DADOS
{
    uint32_t tamanho_cab;      // Tamanho do cabeçalho
    int32_t largura;           // Largura da imagem em pixels
    int32_t altura;            // Altura da imagem em pixels
    uint16_t planos;           // Quantidade de planos
    uint16_t bitcount;         // Bits por pixel
    uint32_t bCompress;        // Compressão utilizada
    uint32_t tamanho_img_byte; // Tamanho da imagem em bytes
    int32_t resXPPMeter;       // Resolução horizontal pixel/metro
    int32_t resYPPMeter;       // Resolução vertical pixel/metro
    uint32_t numClrUsed;       // Número de cores na imagem
    uint32_t numClrImpor;      // Número de cores importantes
};

#pragma pack() // Restaura o alinhamento de bytes padrão

void imprimirDados(const char *s, const struct CAB *cabecalho, const struct DADOS *dados)
{
    printf("Dados do Cabecalho:\n");
    printf("Tipo do arquivo: %04X\n", cabecalho->fType);
    printf("\nDados da Imagem:\n");
    printf("Nome da imagem: %s\n", s);
    printf("Tamanho da imagem (bytes): %u\n", dados->tamanho_img_byte);
    printf("Largura da imagem: %d\n", dados->largura);
    printf("Altura da imagem: %d\n", dados->altura);
    printf("Profundidade: %u\n", dados->bitcount);
}

void tons_de_cinza(FILE *imagem, FILE *nova_imagem, const struct DADOS *dados_imagem)
{
    int linha_bytes = dados_imagem->largura * 3; // Cada pixel possui 3 bytes (RGB)
    unsigned char *linha = (unsigned char *)malloc(linha_bytes);

    for (int i = 0; i < dados_imagem->altura; i++)
    {
        fread(linha, 1, linha_bytes, imagem); // Lê uma linha de pixels da imagem original
        for (int j = 0; j < linha_bytes; j += 3) // Percorre os pixels na linha
        {
            unsigned char blue = linha[j];
            unsigned char green = linha[j + 1];
            unsigned char red = linha[j + 2];

            // Calcula o valor de cinza ponderado para substituir as componentes RGB
            unsigned char gray = (unsigned char)(red * 0.3 + green * 0.59 + blue * 0.11);

            // Substitui as componentes RGB pelo valor de cinza
            linha[j] = linha[j + 1] = linha[j + 2] = gray;
        }
        fwrite(linha, 1, linha_bytes, nova_imagem); // Escreve a linha processada na nova imagem
    }

    free(linha);
}

void trocar_cores(FILE *imagem, FILE *nova_imagem, const struct DADOS *dados_imagem)
{
    int linha_bytes = dados_imagem->largura * 3; // Cada pixel possui 3 bytes (RGB)
    unsigned char *linha = (unsigned char *)malloc(linha_bytes);

    for (int i = 0; i < dados_imagem->altura; i++)
    {
        fread(linha, 1, linha_bytes, imagem); // Lê uma linha de pixels da imagem original
        for (int j = 0; j < linha_bytes; j += 3) // Percorre os pixels na linha
        {
            unsigned char blue = linha[j];
            unsigned char green = linha[j + 1];
            unsigned char red = linha[j + 2];

            // Substitui as componentes RGB
            linha[j] = red;
            linha[j + 1] = blue;
            linha[j + 2] = green;
        }
        fwrite(linha, 1, linha_bytes, nova_imagem); // Escreve a linha processada na nova imagem
    }

    free(linha);
}
int main()
{
    struct CAB header; // Criando uma estrutura CAB para armazenar os valores do cabeçalho
    struct DADOS dados_imagem; // Criando uma estrutura DADOS para armazenar os valores da imagem

    int opc;

    printf("1 - Trocar cores; 2 - Tons de cinza\n");
    printf("Qual opcao voce deseja executar? ");
    scanf("%d", &opc);
    
    FILE *imagem = fopen(NOME_DA_IMAGEM, "rb"); // Abrindo a imagem original para leitura
    FILE *nova_imagem = fopen("nova_imagem.bmp", "wb"); // Criando a imagem nova para escrita

    if (imagem == NULL) 
    {
        fprintf(stderr, "Erro: A imagem de origem nao pode ser aberta.\n");
        return 1;
    }

    if (nova_imagem == NULL)
    {
        fprintf(stderr, "Erro: Nao foi possivel criar a nova imagem.\n");
        return 2;
    }

    fread(&header, sizeof(struct CAB), 1, imagem); // Preenchendo a estrutura com os valores de cabeçalho
    fread(&dados_imagem, sizeof(struct DADOS), 1, imagem); // Preenchendo a estrutura com os valores da imagem

    fwrite(&header, sizeof(struct CAB), 1, nova_imagem); // Copiando os valores de cabeçalho para a imagem nova
    fwrite(&dados_imagem, sizeof(struct DADOS), 1, nova_imagem); // Copiando os valores da imagem para a imagem nova
    
    switch (opc)
    {
        case 1:
            trocar_cores(imagem, nova_imagem, &dados_imagem); // Chamando a função para trocar cores
            break;

        case 2:
            tons_de_cinza(imagem, nova_imagem, &dados_imagem); // Chamando a função tons de cinza
            break;

        default: // Caso o usuário insira uma entrada inválida
            printf("OPCAO INVALIDA\n");
            return 4;
    }

    imprimirDados(NOME_DA_IMAGEM, &header, &dados_imagem); // Função para imprimir os dados das imagens

    // Fechando ambas as imagens
    fclose(imagem);
    fclose(nova_imagem);

    printf("Operacao concluida.\n");

    return 0;
}
