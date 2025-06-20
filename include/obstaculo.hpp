#ifndef OBSTACULO
#define OBSTACULO

#include <allegro5/allegro.h>
#include "../include/objetorenderizavel.hpp"
#include "hitbox.hpp"

class Obstaculo  : public ObjetoRenderizavel{
    private:
    float _velocidadeX;
    float _larguraObs, _alturaObs;

    public:
    Obstaculo(float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura);
    //Construtor de canos
    Obstaculo(float posX, float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura);
    //Construtor de outros obstáculos


    float get_velocidadeX();
    void set_velocidadeX(float);
    float get_larguraObs();
    void set_larguraObs(float);
    float get_alturaObs();
    void set_alturaObs(float);
    void mover_obstaculos();
    //Move o obstáculo no eixo X
    void desenhar_canos();
    //Renderiza os canos e(ou) desenha os hitbox
    void on_tick() override;
    //Atualiza a posição
};

#endif