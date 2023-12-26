/*
    Checkpoint do projeto prático de ED produzido por:
    Bernado Henrique Diniz Felix
    José Acerbi Almeida Neto
    Rafael Alves Rezende
    Turma 10A
*/

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <cmath>

using namespace std;

const string RESET_COLOR = "\033[0m";
const string RED_TEXT = "\033[31m";
const string GREEN_TEXT = "\033[32m";
const string YELLOW_TEXT = "\033[33m";

// Facilitar a manipulação do arquivo binário
const string NomeArquivo = "Binario_final.dat";
const int tamanhoStruct = 200;

class Projeto
{
private:
    char measure[10];
    char quantile[10];
    char area[70];
    char sex[15];
    char age[20];
    char geography[50];
    char ethnic[15];
    char value[10];

public:
    fstream mArquivo;
    Projeto *mDados;
    bool continuar = true; // Verifica se o usuário deseja continuar o funcionamento do programa
    // Sobrecarga de operador de = para copiar dados de uma classe para outra
    Projeto &operator=(const Projeto &other)
    {
        if (this != &other) // Evitar autoatribuição
        {
            // Copiar membros individualmente
            strcpy(measure, other.measure);
            strcpy(quantile, other.quantile);
            strcpy(area, other.area);
            strcpy(sex, other.sex);
            strcpy(age, other.age);
            strcpy(geography, other.geography);
            strcpy(ethnic, other.ethnic);
            strcpy(value, other.value);
        }
        return *this;
    }
    void Inserir();     // Inserir elemento em uma dada posição
    void Imprimir();    // Imprimir dados
    void Alterar();     // Trocar elementos de posição
    void Editar();      // Alterar os dados de uma posição
    void Menu();        // Menu do usuário
    void ExportarCSV(); // Gera arquivo CSV
    void AbreArquivo(); // Abre o arquivo
    void ordenaArquivos(int nomeArquivo, int inicio, int fim);
    void mergeSort(Projeto *vetor, int inicio, int fim);
    void intercala(Projeto *vetor, int inicio, int meio, int fim);
    void mesclaArquivos(int numeroArquivos);
    void divideArquivo(int &numeroArquivos);
    void mergeSortExterno();
    void limpaDados();
};

void Projeto::AbreArquivo()
{
    mArquivo.open(NomeArquivo, ios::in | ios::out | ios::binary);
}

void Projeto::Imprimir()
{
    system("clear || cls");

    cout << "======================== VISUALIZAR ========================" << endl
         << endl;

    int opcao;

    cout << "Opcoes disponiveis: " << endl;
    cout << "[-1] Para voltar ao menu " << endl;
    cout << "[1] Visualizar todos os dados " << endl;
    cout << "[2] Visualizar os dados dentro de um intervalo " << endl;
    cout << YELLOW_TEXT << "Digite a opcao desejada: " << RESET_COLOR;

    cin >> opcao;
    ifstream arquivo;
    arquivo.open("Binario_final.dat", ios::binary);

    arquivo.seekg(0, arquivo.end);
    long int mBytes = arquivo.tellg();
    int mTamanhoTotal = int(mBytes / tamanhoStruct);
    arquivo.seekg(0, arquivo.beg);

    if (opcao == -1)
    {
        return;
    }

    if (opcao == 1)
    { // Listar todos os dados

        Projeto Dados;

        // O arquivo vai ler blocos de dados do tamanho da classe
        // Reinterpreta os dados lidos no bloco como um ponteiro char
        // O ponteiro é armazenado em Dados
        // E o while continuará enquanto isso acontecerá

        while (arquivo.read(reinterpret_cast<char *>(&Dados), tamanhoStruct))
        {
            cout << "Measure: " << Dados.measure << endl;
            cout << "Quantile: " << Dados.quantile << endl;
            cout << "Area: " << Dados.area << endl;
            cout << "Sex: " << Dados.sex << endl;
            cout << "Age: " << Dados.age << endl;
            cout << "Geography: " << Dados.geography << endl;
            cout << "Ethnic: " << Dados.ethnic << endl;
            cout << "Value: " << Dados.value << endl;
            cout << "-----------------------------" << endl;
        }
    }
    else
    { // Imprimir dentro de um intervalo;

        int posicao1, posicao2;
        cout << "Insira a posicao onde deseja iniciar a impressao: " << endl;
        cin >> posicao1;
        cout << "Insira a posicao onde deseja finalizar a impressao: " << endl;
        cin >> posicao2;

        if (posicao1 < 0 || posicao1 >= mTamanhoTotal || posicao2 < 0 || posicao2 >= mTamanhoTotal)
        {
            system("clear || cls");
            cerr << RED_TEXT << "Posicoes invalidas!" << RESET_COLOR << endl;
            sleep(2);
            return;
        }

        arquivo.clear();

        int inicio = min(posicao1, posicao2);
        int fim = max(posicao1, posicao2);

        arquivo.seekg(inicio * tamanhoStruct);

        int numElementosParaImprimir = fim - inicio + 1;
        Projeto dado;
        for (int i = 0; i < numElementosParaImprimir; i++)
        {
            if (arquivo.read(reinterpret_cast<char *>(&dado), tamanhoStruct))
            {
                cout << "Measure: " << dado.measure << endl;
                cout << "Quantile: " << dado.quantile << endl;
                cout << "Area: " << dado.area << endl;
                cout << "Sex: " << dado.sex << endl;
                cout << "Age: " << dado.age << endl;
                cout << "Geography: " << dado.geography << endl;
                cout << "Ethnic: " << dado.ethnic << endl;
                cout << "Value: " << dado.value << endl;
                cout << "---------------------------------------------" << endl;
            }
            else
            {
                system("clear || cls");
                cerr << RED_TEXT << "Erro ao ler o arquivo!" << RESET_COLOR << endl;
                sleep(2);
                return;
            }
        }
    }

    arquivo.close();

    AbreArquivo();

    cout << GREEN_TEXT << "Impresssao realizada com sucesso!" << RESET_COLOR << endl;

    cout << "Digite qualquer tecla para voltar ao menu... " << endl;

    string aux;

    cin >> aux;

    Menu();
}

void Projeto::ExportarCSV()
{

    ofstream saida("ListaCSV.csv");

    saida << "measure,quantile,area,sex,age,geography,ethnic,value" << endl;

    ifstream arquivo;
    arquivo.open("Binario_final.dat", ios::binary);

    arquivo.seekg(0, arquivo.end);
    long int mBytes = arquivo.tellg();
    int mTamanhoTotal = int(mBytes / tamanhoStruct);
    arquivo.seekg(0, arquivo.beg);

    Projeto dado;
    for (int i = 0; i < mTamanhoTotal; i++)
    {

        arquivo.seekg((i)*tamanhoStruct);
        arquivo.read((char *)(&dado), tamanhoStruct);

        saida << dado.measure << ",";
        saida << dado.quantile << ",";
        saida << dado.area << ",";
        saida << dado.sex << ",";
        saida << dado.age << ",";
        saida << dado.geography << ",";
        saida << dado.ethnic << ",";
        saida << dado.value << endl;
    }

    arquivo.close();

    AbreArquivo();

    system("clear || cls");

    cout << GREEN_TEXT << "Conversao concluida com sucesso! " << RESET_COLOR << endl;

    sleep(2);

    Menu();
}

void Projeto::Editar()
{
    int indice;

    system("clear || cls");

    cout << "======================== EDITAR ========================" << endl
         << endl;
    cout << "Digite [-1] para voltar ao menu. " << endl;
    cout << YELLOW_TEXT << "Digite o indice do elemento que deseja editar: " << RESET_COLOR << endl;

    cin >> indice;

    ifstream arquivo;
    arquivo.open("Binario_final.dat", ios::binary);

    arquivo.seekg(0, arquivo.end);
    long int mBytes = arquivo.tellg();
    int mTamanhoTotal = int(mBytes / tamanhoStruct);
    arquivo.seekg(0, arquivo.beg);

    if (indice == -1)
    {
        return;
    }

    // Verifica se o indice esta dentro dos limites do arquivo
    if (indice < 0 || indice >= mTamanhoTotal)
    {
        system("clear || cls");
        cerr << RED_TEXT << "Indice invalido! A edicao nao e possivel." << RESET_COLOR << endl;
        sleep(2);
        return;
    }

    Projeto novosDados;
    cin.ignore(); // Limpa o caractere de nova linha

    system("clear || cls");

    cout << "Digite os novos dados para o elemento:" << endl;

    cout << "Measure: ";
    cin.getline(novosDados.measure, sizeof(novosDados.measure));
    novosDados.measure[sizeof(novosDados.measure) - 1] = '\0'; // Adicionar o caractere nulo

    cout << "Quantile: ";
    cin.getline(novosDados.quantile, sizeof(novosDados.quantile));
    novosDados.quantile[sizeof(novosDados.quantile) - 1] = '\0';

    cout << "Area: ";
    cin.getline(novosDados.area, sizeof(novosDados.area));
    novosDados.area[sizeof(novosDados.area) - 1] = '\0';

    cout << "Sex: ";
    cin.getline(novosDados.sex, sizeof(novosDados.sex));
    novosDados.sex[sizeof(novosDados.sex) - 1] = '\0';

    cout << "Age: ";
    cin.getline(novosDados.age, sizeof(novosDados.age));
    novosDados.age[sizeof(novosDados.age) - 1] = '\0';

    cout << "Geography: ";
    cin.getline(novosDados.geography, sizeof(novosDados.geography));
    novosDados.geography[sizeof(novosDados.geography) - 1] = '\0';

    cout << "Ethnic: ";
    cin.getline(novosDados.ethnic, sizeof(novosDados.ethnic));
    novosDados.ethnic[sizeof(novosDados.ethnic) - 1] = '\0';

    cout << "Value: ";
    cin.getline(novosDados.value, sizeof(novosDados.value));
    novosDados.value[sizeof(novosDados.value) - 1] = '\0';

    // Posiciona o ponteiro do arquivo no registro a ser editado
    mArquivo.seekp(indice * tamanhoStruct, ios::beg);

    // Escreve os novos dados
    mArquivo.write(reinterpret_cast<const char *>(&novosDados), tamanhoStruct);

    mArquivo.close();

    AbreArquivo();

    system("clear || cls");

    cout << GREEN_TEXT << "Elemento no indice " << indice << " foi editado com sucesso!" << RESET_COLOR << endl;

    sleep(3);

    Menu();
}

void Projeto::Inserir()
{

    system("clear || cls");

    cout << "======================== INSERIR ========================" << endl
         << endl;
    cout << "Digite [-1] caso queira retornar ao menu" << endl;
    cout << YELLOW_TEXT << "Insira a posicao onde deseja inserir os dados: " << RESET_COLOR;

    int posicao;
    cin >> posicao;

    ifstream arquivo;
    arquivo.open("Binario_final.dat", ios::binary);

    arquivo.seekg(0, arquivo.end);
    long int mBytes = arquivo.tellg();
    int mTamanhoTotal = int(mBytes / tamanhoStruct);
    arquivo.seekg(0, arquivo.beg);

    if (posicao == -1)
    {
        return;
    }

    // Verifica se a posição é válida e solicite entrada válida
    if (posicao < 0 || posicao >= mTamanhoTotal)
    {
        cerr << RED_TEXT << "Posicao invalida, tente novamente" << RESET_COLOR;
        Inserir();
    }

    Projeto novoRegistro;

    cout << "Digite os dados para o novo elemento:" << endl;

    cout << "Measure: ";
    cin.ignore();
    cin.getline(novoRegistro.measure, sizeof(novoRegistro.measure));

    cout << "Quantile: ";
    cin.getline(novoRegistro.quantile, sizeof(novoRegistro.quantile));

    cout << "Area: ";
    cin.getline(novoRegistro.area, sizeof(novoRegistro.area));

    cout << "Sex: ";
    cin.getline(novoRegistro.sex, sizeof(novoRegistro.sex));

    cout << "Age: ";
    cin.getline(novoRegistro.age, sizeof(novoRegistro.age));

    cout << "Geography: ";
    cin.getline(novoRegistro.geography, sizeof(novoRegistro.geography));

    cout << "Ethnic: ";
    cin.getline(novoRegistro.ethnic, sizeof(novoRegistro.ethnic));

    cout << "Value: ";
    cin.getline(novoRegistro.value, sizeof(novoRegistro.value));

    // Move os registros existentes para abrir espaço para o novo registro
    for (int i = mTamanhoTotal; i > posicao; i--)
    {
        mArquivo.seekg((i - 1) * tamanhoStruct, ios::beg);
        mArquivo.read(reinterpret_cast<char *>(&mDados), tamanhoStruct);
        mArquivo.seekp(i * tamanhoStruct, ios::beg);
        mArquivo.write(reinterpret_cast<char *>(&mDados), tamanhoStruct);
    }

    // Volta ao início do arquivo para escrever o novo registro
    mArquivo.seekp(posicao * tamanhoStruct, ios::beg);

    // Escreve os dados do novo elemento na posição desejada
    mArquivo.write(reinterpret_cast<char *>(&novoRegistro), tamanhoStruct);

    mTamanhoTotal++;

    mArquivo.flush();

    mArquivo.close();

    AbreArquivo();

    system("clear || cls");

    cout << GREEN_TEXT << "Elemento inserido com sucesso!" << RESET_COLOR << endl;

    sleep(2);

    Menu();
}

// A variavel "aux" do tipo Registro ira receber os valores do elemento na posicao "ElementoUM"
// O elemento da posicao "ElementoUM" recebera os valores do elemento na posicao "ElementoDOIS"
// O elemento da posicao "EleentoDOIS" recebera os valores de "aux"
void Projeto::Alterar()
{
    system("clear || cls");

    cout << "======================== ALTERAR ========================" << endl
         << endl;
    cout << "Digite [-1] caso queira retornar ao menu" << endl;
    cout << YELLOW_TEXT << "Digite o indice dos dois elementos a serem trocados: " << RESET_COLOR << endl;

    int ElementoUM, ElementoDOIS;
    Projeto aux, temp;

    cin >> ElementoUM;

    ifstream arquivo;
    arquivo.open("Binario_final.dat", ios::binary);

    arquivo.seekg(0, arquivo.end);
    long int mBytes = arquivo.tellg();
    int mTamanhoTotal = int(mBytes / tamanhoStruct);
    arquivo.seekg(0, arquivo.beg);

    if (ElementoUM == -1)
    {
        return;
    }
    else
    {
        cin >> ElementoDOIS;
    }

    if ((ElementoUM < 0) or (ElementoUM > mTamanhoTotal) or (ElementoDOIS < 0) or (ElementoDOIS > mTamanhoTotal))
    {
        system("clear || cls");
        cout << RED_TEXT << "Um dos indices inseridos e invalido por acessar uma posicao inexistente " << RESET_COLOR << endl;
        sleep(2);
        Alterar();
    }

    // Armazeno os dados do elemento na posicao ElementoUM em aux
    mArquivo.seekg((ElementoUM)*tamanhoStruct);
    mArquivo.read((char *)(&mDados), tamanhoStruct);

    strcpy(aux.measure, mDados->measure);
    strcpy(aux.quantile, mDados->quantile);
    strcpy(aux.area, mDados->area);
    strcpy(aux.sex, mDados->sex);
    strcpy(aux.age, mDados->age);
    strcpy(aux.geography, mDados->geography);
    strcpy(aux.ethnic, mDados->ethnic);
    strcpy(aux.value, mDados->value);

    // Armazeno os dados do elemento na posicao ElementoDOIS em temp
    mArquivo.seekg((ElementoDOIS)*tamanhoStruct);
    mArquivo.read((char *)(&mDados), tamanhoStruct);

    strcpy(temp.measure, mDados->measure);
    strcpy(temp.quantile, mDados->quantile);
    strcpy(temp.area, mDados->area);
    strcpy(temp.sex, mDados->sex);
    strcpy(temp.age, mDados->age);
    strcpy(temp.geography, mDados->geography);
    strcpy(temp.ethnic, mDados->ethnic);
    strcpy(temp.value, mDados->value);

    // Escrevo os dados de temp (dados do ElementoDOIS) na posicao ElementoUM
    mArquivo.seekp(ElementoUM * tamanhoStruct, ios::beg);
    mArquivo.write(reinterpret_cast<char *>(&temp), tamanhoStruct);

    // Escrevo os dados de aux (dados do ElementoUM) na posicao ElementoDOIS
    mArquivo.seekp(ElementoDOIS * tamanhoStruct, ios::beg);
    mArquivo.write(reinterpret_cast<char *>(&aux), tamanhoStruct);

    mArquivo.close();

    AbreArquivo();

    system("clear || cls");

    cout << GREEN_TEXT << "Operacao concluida com sucesso!" << RESET_COLOR;

    sleep(2);

    Menu();
}

//////////////////////// ORDENAÇÃO EXTERNA ////////////////////////

// Divisão em arquivos menores
void Projeto::divideArquivo(int &numeroArquivos)
{
    // esse metodo vai dividir o arquivo BinarioOriginal.dat em varios arquivos menores
    cout << "Dividindo o arquivo..." << endl;

    ifstream arquivo;
    arquivo.open(NomeArquivo, ios::binary);
    if (arquivo.is_open())
    {
        // calcula o numero de arquivos menores
        arquivo.seekg(0, ios::end);
        int numeroDados = arquivo.tellg() / tamanhoStruct;
        arquivo.seekg(tamanhoStruct, ios::beg);
        numeroArquivos = (numeroDados / 50000) + 1;

        // cria os arquivos
        for (int i = 0; i < numeroArquivos; i++)
        {
            ofstream arquivo;
            arquivo.open("Binario" + to_string(i) + ".dat", ios::binary);
            arquivo.close();
        }
        // le os dados do arquivo original e escreve nos arquivos menores
        for (int i = 0; i < numeroArquivos; i++)
        {
            int j = 0;
            while (arquivo.good() && j < 50000)
            {
                Projeto dado;
                arquivo.read((char *)&dado, tamanhoStruct);
                ofstream arquivo;
                arquivo.open("Binario" + to_string(i) + ".dat", ios::binary | ios::app);
                if (arquivo.is_open())
                {
                    arquivo.write((char *)&dado, tamanhoStruct);
                    arquivo.close();
                }
                else
                {
                    cout << "Erro ao abrir o arquivo auxiliar: " << i << endl;
                }
                j++;
            }
        }
        arquivo.close();
    }
    else
    {
        cout << "Erro ao abrir o arquivo na divisao" << endl;
    }
    cout << "Arquivo dividido em " << numeroArquivos << " arquivos!" << endl;
}

// Intercala arquivos
void Projeto::intercala(Projeto *vetor, int inicio, int meio, int fim)
{
    // esse metodo vai intercalar os dados de dois vetores, usando area como chave de ordenacao principal e values como secundaria
    int i = inicio, j = meio + 1;
    int tamanho = fim - inicio + 1;
    Projeto *vetorAuxiliar = new Projeto[tamanho];
    string area1, area2;
    for (int k = 0; k < tamanho; k++)
    {
        if ((i <= meio) && (j <= fim))
        {
            area1 = vetor[i].area;
            area2 = vetor[j].area;
            // compara as areas dos dados
            if (area1 < area2)
            {
                // se a area do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                vetorAuxiliar[k] = vetor[i];
                i++;
            }
            else if (area1 > area2)
            {
                // se a area do vetor da direita for menor, pega o proximo do vetor da direita
                vetorAuxiliar[k] = vetor[j];
                j++;
            }
            else
            {
                // se as areas forem iguais, compara os values
                if (vetor[i].value < vetor[j].value)
                {
                    // se o value do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                    vetorAuxiliar[k] = vetor[i];
                    i++;
                }
                else
                {
                    // se o value do vetor da direita for menor, pega o proximo do vetor da direita
                    vetorAuxiliar[k] = vetor[j];
                    j++;
                }
            }
        }
        else if (i > meio)
        {
            // se o vetor da esquerda acabou, pega o proximo do vetor da direita
            vetorAuxiliar[k] = vetor[j];
            j++;
        }
        else
        {
            // se o vetor da direita acabou, pega o proximo do vetor da esquerda
            vetorAuxiliar[k] = vetor[i];
            i++;
        }
    }
    for (int k = 0; k < tamanho; k++)
    {
        // escreve os dados ordenados no vetor original
        vetor[inicio + k] = vetorAuxiliar[k];
    }
    delete[] vetorAuxiliar;
}

// MergeSort
void Projeto::mergeSort(Projeto *vetor, int inicio, int fim)
{
    // esse metodo vai ordenar os dados de um vetor
    if (inicio < fim)
    {
        int meio = (inicio + fim) / 2;
        mergeSort(vetor, inicio, meio);
        mergeSort(vetor, meio + 1, fim);
        intercala(vetor, inicio, meio, fim);
    }
}

// Limpar dado
void Projeto::limpaDados()
{
    for (int i = 0; i < 10; i++)
    {
        measure[i] = '\0';
    }
    for (int i = 0; i < 20; i++)
    {
        age[i] = '\0';
    }
    for (int i = 0; i < 15; i++)
    {
        ethnic[i] = '\0';
    }
    for (int i = 0; i < 50; i++)
    {
        geography[i] = '\0';
    }
    for (int i = 0; i < 10; i++)
    {
        quantile[i] = '\0';
    }
    for (int i = 0; i < 70; i++)
    {
        area[i] = '\0';
    }
    for (int i = 0; i < 15; i++)
    {
        sex[i] = '\0';
    }
    for (int i = 0; i < 10; i++)
    {
        value[i] = '\0';
    }
}

// Ordena arquivos
void Projeto::ordenaArquivos(int numeroArquivos, int inicio, int fim)
{
    // esse metodo vai ordenar os arquivos auxiliares
    Projeto *vetor = new Projeto[50000];
    for (int i = 0; i < numeroArquivos; i++)
    {
        for (int j = 0; j < 50000; j++)
        {
            vetor[j].limpaDados();
        }
        ifstream arquivo;
        arquivo.open("Binario" + to_string(i) + ".dat", ios::binary);
        if (arquivo.is_open())
        {
            for (int j = 0; j < 50000; j++)
            {
                arquivo.read((char *)&vetor[j], tamanhoStruct);
            }
            // ordenar o vetor
            mergeSort(vetor, inicio, fim);
            arquivo.close();
            // abrir o arquivo para escrever
            ofstream arquivo;
            arquivo.open("Binario" + to_string(i) + ".dat", ios::binary);
            if (arquivo.is_open())
            {
                // escrever o vetor ordenado no arquivo
                arquivo.seekp(0, ios::beg);
                for (int j = 0; j < 50000; j++)
                {
                    if (vetor[j].area[0] != '\0')
                    {
                        arquivo.write((char *)&vetor[j], tamanhoStruct);
                    }
                }
                arquivo.close();
                cout << "Arquivo " << i << " ordenado!" << endl;
            }
            else
            {
                cout << "Erro ao abrir o arquivo auxiliar: " << i << "para escrever na ordenacao" << endl;
            }
        }
        else
        {
            cout << "Erro ao abrir o arquivo auxiliar: " << i << "para ler na ordenacao" << endl;
        }
    }
}

void Projeto::mesclaArquivos(int numeroArquivos)
{
    // esse metodo vai mesclar os arquivos auxiliares em um arquivo final
    int indices[numeroArquivos];
    for (int i = 0; i < numeroArquivos; i++)
    {
        indices[i] = 0;
    }
    // abrir os arquivos auxiliares
    ifstream arquivos[numeroArquivos];
    for (int i = 0; i < numeroArquivos; i++)
    {
        arquivos[i].open("Binario" + to_string(i) + ".dat", ios::binary);
        if (!arquivos[i].is_open())
        {
            cout << "Erro ao abrir o arquivo auxiliar: " << i << "na mescla" << endl;
        }
    }
    // abrir o arquivo final
    ofstream arquivoFinal;
    arquivoFinal.open("Binario_final.dat", ios::binary);
    if (!arquivoFinal.is_open())
    {
        cout << "Erro ao abrir o arquivo final na mescla" << endl;
        return;
    }
    // mesclar os arquivos auxiliares
    bool arquivosAbertos = true;
    string area1, area2;
    Projeto menor, aux;
    int indiceMenor = -1;
    int tamanho;
    while (arquivosAbertos)
    {
        arquivosAbertos = false;
        indiceMenor = -1;
        for (int i = 0; i < numeroArquivos; i++)
        {
            arquivos[i].seekg(0, ios::end);
            tamanho = arquivos[i].tellg() / tamanhoStruct;
            arquivos[i].seekg(0, ios::beg);
            if (indices[i] < tamanho)
            {
                arquivosAbertos = true;
                arquivos[i].seekg(indices[i] * tamanhoStruct, ios::beg);
                arquivos[i].read((char *)&aux, tamanhoStruct);
                if (indiceMenor == -1)
                {
                    menor = aux;
                    indiceMenor = i;
                }
                else
                {
                    area1 = aux.area;
                    area2 = menor.area;
                    // compara as areas dos dados
                    if (area1 < area2)
                    {
                        // se a area do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                        menor = aux;
                        indiceMenor = i;
                    }
                    else if (area1 > area2)
                    {
                        // se a area do vetor da direita for menor, pega o proximo do vetor da direita
                    }
                    else
                    {
                        // se as areas forem iguais, compara os values
                        if (aux.value < menor.value)
                        {
                            // se o value do vetor da esquerda for menor, pega o proximo do vetor da esquerda
                            menor = aux;
                            indiceMenor = i;
                        }
                        else
                        {
                            // se o value do vetor da direita for menor, pega o proximo do vetor da direita
                        }
                    }
                }
            }
        }
        if (indiceMenor != -1)
        {
            arquivoFinal.write((char *)&menor, tamanhoStruct);
            indices[indiceMenor]++;
        }
    }
    // fechar os arquivos
    for (int i = 0; i < numeroArquivos; i++)
    {
        arquivos[i].close();
    }
    arquivoFinal.close();
    cout << "Arquivos mesclados, arquivo final gerado com sucesso!" << endl;
}

// Merge sort
void Projeto::mergeSortExterno()
{
    // esse metodo vai chamar os metodos necessarios para ordenar o arquivo
    int numeroArquivos;
    divideArquivo(numeroArquivos);
    if (numeroArquivos > 1)
    {
        ordenaArquivos(numeroArquivos, 0, 49999);
        mesclaArquivos(numeroArquivos);
        for (int i = 0; i < numeroArquivos; i++)
        {
            remove(("Binario" + to_string(i) + ".dat").c_str());
        }
    }
    else
    {
        cout << "Arquivo pequeno demais" << endl;
    }

    cout << GREEN_TEXT << "Operacao concluida com sucesso!" << RESET_COLOR;

    sleep(2);

    Menu();
}

// O numero digitado pelo usuario é armazenado em 'comando'
// Se o digito nao for valido, é imprimida uma msg de erro e o menu roda novamente
// O programa roda enquanto o usuario nao digita 0;
// a bool 'continuar' (declarada na classe), é true se comando !=0 e false se comando==0
// quando for false, o programa encerra
void Projeto::Menu()
{
    int comando;
    bool valido;
    while (continuar)
    {
        do
        {
            valido = true;
            system("clear || cls");

            cout << "======================== MENU ========================" << endl
                 << endl;

            cout << "Opcoes disponiveis: " << endl;
            cout << "[0] Sair do programa" << endl;
            cout << "[1] Inserir elemento em uma dada posicao" << endl;
            cout << "[2] Visualizar elementos" << endl;
            cout << "[3] Editar um elemento" << endl;
            cout << "[4] Trocar dois elementos de posicao" << endl;
            cout << "[5] Exportar dados para arquivo CSV" << endl;
            cout << "[6] Ordenar arquivo binário (Processo pode levar até 5 minutos)" << endl;
            cout << YELLOW_TEXT << "Digite uma opcao: " << RESET_COLOR << endl;

            if (!(cin >> comando) or (comando < 0) or (comando > 6))
            {
                system("clear || cls");
                cerr << RED_TEXT << "Opcao invalida, tente novamente" << RESET_COLOR << endl;
                sleep(3);
                Menu();
            }

        } while (!valido);

        if (comando == 0)
        {
            continuar = false;
        }

        switch (comando)
        {
        case 1:
            Inserir();
            break;
        case 2:
            Imprimir();
            break;
        case 3:
            Editar();
            break;
        case 4:
            Alterar();
            break;
        case 5:
            ExportarCSV();
            break;
        case 6:
            mergeSortExterno();
            break;
        default:
            continuar = false;
        }
    }
}

int main()
{

    Projeto MeuProjeto;
    // cout << "Ordenando arquivo binário " << endl;
    // MeuProjeto.mergeSortExterno();
    MeuProjeto.Menu();

    return 0;
}