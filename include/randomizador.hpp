#ifndef RANDOMIZADOR
#define RANDOMIZADOR

#include <random>
#include <iostream>

using namespace std;

class Randomizador
{
    private:
    int min;
    int max;
    random_device rd;
    mt19937 gerador;


    public:
    Randomizador(int min, int max);
    //Gera um inteiro aleatório, com base na distribuição uniforme no intervalo (min, max);
    int valor_aleatorio();
    //Define o valor de mínimo
    void set_min(int);
    //Retorna o valor de mínimo
    float get_min();
    //Define o valor de máximo
    void set_max(int);
    //Retorna o valor de máximo
    float get_max();

};

/*Define um inteiro aleatório, recebendo uma distribuição já definida, com máximos e mínimos.
Usado para calcular alturas de spawn*/
int definir_altura(Randomizador* rando);

#endif