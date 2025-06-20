#include "../include/randomizador.hpp"
#include <random>
#include <iostream>

using namespace std;

Randomizador::Randomizador(int _min, int _max) : min(_min), max(_max) 
{
};

int Randomizador::valor_aleatorio()
{
    mt19937 gerador(randy());
    uniform_int_distribution<> distrib(get_min(), get_max());
    return distrib(gerador);
}
void Randomizador::set_min(int a)
{
    this->min = a;
    return;
}
float Randomizador::get_min()
{
    return this->min;
}
void Randomizador::set_max(int a)
{   
    this->max = a;
    return;
}
float Randomizador::get_max()
{
    return this->max;
}

