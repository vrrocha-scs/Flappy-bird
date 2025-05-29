#ifndef OBSTACULO
#define OBSTACULO

#include <allegro5/allegro.h>

class Obstaculo{
    private:
    // posicaoX != posX: estou usando assim para diferenciar do bitmap
    float _posicaoX, _posicaoY;
    float _velocidadeX;
    float _larguraObs, _alturaObs;
    float distmin;
    ALLEGRO_COLOR cor;

    public:
    Obstaculo();
    Obstaculo(float posicaoX, float posicaoY, float velocidadeX, float largura, float altura);

    float get_posicaoX();
    void set_posicaoX(float);

    float get_posicaoY();
    void set_posicaoY(float);

    float get_velocidadeX();
    void set_velocidadeX(float);

    float get_larguraObs();
    void set_larguraObs(float);

    float get_alturaObs();
    void set_alturaObs(float);

    void mover_obstaculos();
    void desenhar_canos();
    
};

#endif