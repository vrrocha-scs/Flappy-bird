#ifndef OBSTACULO
#define OBSTACULO

#include <allegro5/allegro.h>
#include "../include/objetorenderizavel.hpp"

class Obstaculo  : public ObjetoRenderizavel{
    private:
    // posicaoX != posX: estou usando assim para diferenciar do bitmap
    float _velocidadeX;
    float _larguraObs, _alturaObs;
    float distmin;
    ALLEGRO_COLOR cor;
    

    public:
    Obstaculo();
    Obstaculo(float posX, float posY, string image_path, float velocidadeX, float largura, float altura);

    float get_velocidadeX();
    void set_velocidadeX(float);

    float get_larguraObs();
    void set_larguraObs(float);

    float get_alturaObs();
    void set_alturaObs(float);

    void mover_obstaculos();
    void desenhar_canos();
    void onTick();
};

#endif