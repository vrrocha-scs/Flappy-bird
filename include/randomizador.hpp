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
    random_device randy;

    public:
    Randomizador(int min, int max);
    int valor_aleatorio();
    void set_min(int);
    float get_min();
    void set_max(int);
    float get_max();

};

#endif