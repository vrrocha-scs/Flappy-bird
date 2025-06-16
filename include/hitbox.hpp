#ifndef HITBOX
#define HITBOX

#include <allegro5/allegro_primitives.h>
#include "objetorenderizavel.hpp"

class Hitbox{
    private:
        float x1,y1,x2,y2;
        ObjetoRenderizavel* attached_object;
    public:
        //Constroi a partir do objeto renderizavel em que se conecta
        Hitbox(ObjetoRenderizavel* object);
        //Checa se houve colisao com a hitbox enviada
        bool has_collision(Hitbox hitbox);
        //Desenha o retangulo que representa a hitbox
        void draw_hitbox();
        //Move a hitbox para a coordenada nova explicitada. ATENÇÃO: NÃO ESQUECER DE SEMPRE MOVIMENTAR A HITBOX JUNTO COM O OBJETO
        void move_hitbox(float new_posX,float new_posY);
        void on_tick();
};

#endif