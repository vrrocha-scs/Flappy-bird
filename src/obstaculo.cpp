#include "obstaculo.hpp"
#include "objetorenderizavel.hpp"
#include <allegro5/allegro.h>

Obstaculo::Obstaculo() : Obstaculo::Obstaculo(0, 0, 0, 0, 0) {};
Obstaculo::Obstaculo(float posicaoX, float posicaoY, float velocidade, float largura, float altura)
{
    _posicaoX = posicaoX;
    _posicaoY = posicaoY;
    _velocidadeX = velocidade;
    _larguraObs = largura;
    _alturaObs = altura;
}

    float Obstaculo::get_posicaoX()
    {
        return this->_posicaoX;
    }
    void Obstaculo::set_posicaoX(float a)
    {
        this->_posicaoX = a;
        return;
    }

    float Obstaculo::get_posicaoY()
    {
        return this->_posicaoY;
    }
    void Obstaculo::set_posicaoY(float a)
    {
        this->_posicaoY = a;
        return; 
    }

    float Obstaculo::get_velocidadeX()
    {
        return this->_velocidadeX;
    }
    void Obstaculo::set_velocidadeX(float a)
    {
        this->_velocidadeX = a;
        return;
    }

    float Obstaculo::get_larguraObs()
    {
        return this->_larguraObs;
    }
    void Obstaculo::set_larguraObs(float a)
    {
        this->_larguraObs = a;
        return;
    }

    float Obstaculo::get_alturaObs()
    {
        return this->_alturaObs;
    }
    void Obstaculo::set_alturaObs(float a)
    {
        this->_alturaObs = a;
        return;
    }



