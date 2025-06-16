#include "obstaculo.hpp"
#include "objetorenderizavel.hpp"
#include "randomizador.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

Obstaculo::Obstaculo(float posX, float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float largura, float altura) : 
ObjetoRenderizavel(posX, posY, bitmap), _velocidadeX(velocidade), _larguraObs(largura), _alturaObs(altura),hitbox(this)
{
    const float distmin = 170;
    cor = al_map_rgb(0, 255, 0);
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

    void Obstaculo::mover_obstaculos()
    {
        this->set_posX(this->get_posX() - this->get_velocidadeX());
        return;
    }
    void Obstaculo::desenhar_canos()
    {
        //Aleatorizando onde o buraco spawna
        float posX2, posY2;
        posX2 = get_posX() + get_larguraObs();
        posY2 = get_posY() + get_alturaObs();

        //al_draw_filled_rectangle(get_posX(), get_posY(), posX2, posY2, cor);
        render_object();
        return;
    }

    void Obstaculo::on_tick()
    {
        hitbox.on_tick();
        this->desenhar_canos();
        this->mover_obstaculos();

    }



