#ifndef HITBOX
#define HITBOX

#include <allegro5/allegro_primitives.h>
#include "objetorenderizavel.hpp"

class Hitbox{
    private:
        float x1,y1,x2,y2;
        ObjetoRenderizavel* attached_object;
    public:
        Hitbox(float x1,float y1,float x2,float y2);
        //Constroi a partir do objeto renderizavel em que se conecta
        Hitbox(ObjetoRenderizavel* object);
        //Checa se houve colisao com a hitbox enviada
        bool has_collision(Hitbox hitbox);
        //Desenha o retangulo que representa a hitbox
        void draw_hitbox();
        //Comportamento a cada tick da Hitbox (acompanha o ObjetoRenderizavel associado)
        void on_tick();
};

#endif