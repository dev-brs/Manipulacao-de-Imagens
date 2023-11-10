#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

#define LARGURA_IMAGEM 84
#define ALTURA_IMAGEM 48
#define DESLOCAMENTO 3

struct CAB
{
    unsigned char fType[2];    // tipo do arquivo
    unsigned int fSize;      // Tamanho do arquivo (cabeçalho)
    unsigned short mR1;      // espaço reservado
    unsigned short mR2;      // espaço reservado
    unsigned int byteshift;  // especifica o DESLOCAMENTO em bytes do início
};

struct DADOS
{
    unsigned int tamanho_cab; // tamanho do cabeçalho
    unsigned int largura;     // largura da imagem em pixels
    unsigned int altura;      // altura da imagem em pixels
    unsigned short planos;    // quantidade de planos
    unsigned short bitcount;  // bits/pixel
    unsigned int bCompress;   // compressão utilizada
    unsigned long int tamanho_img_byte; // tamanho da imagem em byte
    unsigned int resXPPMeter; // resolução horizontal pixel/metro
    unsigned int resYPPMeter; // resolução vertical pixel/metro
    unsigned int numClrUsed;  // número de cores na imagem
    unsigned int numClrImpor; // número de cores importantes
};

void imprimirDados(string s,const CAB& cabecalho, const DADOS& dados)
{
   
    cout << "Dados do Cabecalho:" << endl;
    cout << "Tipo do arquivo: " << cabecalho.fType[0] << cabecalho.fType[1] << endl;
    cout << "\nDados da Imagem:" << endl;
    cout << "Nome da imagem: " << s << endl;
    cout << "Tamanho da imagem (bytes): " << dados.tamanho_img_byte << endl;
    cout << "Largura da imagem: " << dados.largura << endl;
    cout << "Altura da imagem: " << dados.altura << endl;
    cout << "Profundidade: " << dados.bitcount << endl;
}

void recortarImagem(const char* nomeArquivo,unsigned char valor_limiar, unsigned char matrizSaida[][LARGURA_IMAGEM], int x, int y)
{
    ifstream imagem(nomeArquivo, ios::binary);

    if (!imagem.is_open())
    {
        cout << "Erro: A imagem de origem não pode ser aberta." << endl;
        return;
    }

    CAB header;
    DADOS dados_imagem;

    imagem.read(reinterpret_cast<char*>(&header), 14);
    imagem.read(reinterpret_cast<char*>(&dados_imagem), 40);

    int linha_bytes = dados_imagem.largura * 3;
    vector<char> linha(linha_bytes);

    imagem.seekg(header.byteshift + (y * linha_bytes) + (x * 3), ios::beg);

    for (int i = 0; i < ALTURA_IMAGEM; i++)
    {
        imagem.read(&linha[0], linha_bytes);
        for (int j = 0; j < LARGURA_IMAGEM; j++)
        {
            int index = j * 3;
            unsigned char blue = linha[index];
            unsigned char green = linha[index + 1];
            unsigned char red = linha[index + 2];

            unsigned char aux (red * 0.3 + green * 0.59 + blue * 0.11);

            if(aux>=valor_limiar)
                matrizSaida[i][j] = 1;
            else
                matrizSaida[i][j] = 0;
        }
    }

    imagem.close();
}

void salvarMatrizEmArquivo(const unsigned char matriz[][LARGURA_IMAGEM], const char* nomeArquivo)
{
    ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "Erro: Não foi possível criar o arquivo de saída." << endl;
        return;
    }

    arquivo << "unsigned char const imagem[" << ALTURA_IMAGEM * LARGURA_IMAGEM << "] = {" << endl;

    for (int i = 0; i < ALTURA_IMAGEM; i++)
    {
        for (int j = 0; j < LARGURA_IMAGEM; j++)
        {
            arquivo << static_cast<int>(matriz[i][j]);
            if (i == ALTURA_IMAGEM - 1 && j == LARGURA_IMAGEM - 1)
                arquivo << "};";
            else
                arquivo << ",";
        }
        arquivo << endl;
    }

    arquivo.close();
}

int main()
{
    int limiar = 0;
    int x = 0;
    int y = 0;
    string nome_da_imagem;
    unsigned char matrizRecortada[ALTURA_IMAGEM][LARGURA_IMAGEM];

    CAB header;
    DADOS dados_imagem;

    cout << "Insira o nome da imagem: " << endl;
    cin >> nome_da_imagem;

    cout << "Insira o valor de limiar: " << endl;
    cin >> limiar;

    cout << "Insira o valor x do recorte: " << endl;
    cin >> x;

    cout << "Insira o valor y do recorte: " << endl;
    cin >> y;

    ifstream imagem(nome_da_imagem, ios::binary);
    ofstream nova_imagem("nova_imagem.bmp", ios::binary);

    if (!imagem.is_open())
    {
        cerr << "Erro: A imagem de origem não pode ser aberta." << endl;
        return 1;
    }

    if (!nova_imagem.is_open())
    {
        cerr << "Erro: Não foi possível criar a nova imagem." << endl;
        return 2;
    }

    imagem.read(reinterpret_cast<char*>(&header), 14);
    imagem.read(reinterpret_cast<char*>(&dados_imagem), 40);

    nova_imagem.write(reinterpret_cast<char*>(&header), 14);
    nova_imagem.write(reinterpret_cast<char*>(&dados_imagem), 40);

    int linha_bytes = dados_imagem.largura * 3;
    vector<char> linha(linha_bytes);

    for (unsigned int i = 0; i < dados_imagem.altura; i++)
    {
        imagem.read(&linha[0], linha_bytes);
        for (int j = 0; j < linha_bytes; j += 3)
        {
            unsigned char blue = linha[j];
            unsigned char green = linha[j + 1];
            unsigned char red = linha[j + 2];

            unsigned char gray = (red * 0.3 + green*0.59 + blue*0.11);

            linha[j] = linha[j+1] =  linha[j+2] = gray;
        }
        nova_imagem.write(&linha[0], linha_bytes);
    }

    imagem.close();
    nova_imagem.close();

    cout << "operacao concluida'." << endl;

    imprimirDados(nome_da_imagem,header, dados_imagem);
    recortarImagem("nova_imagem.bmp",limiar,matrizRecortada, x, y);
    salvarMatrizEmArquivo(matrizRecortada, "imagem_recortada.txt");

    return 0;
}
