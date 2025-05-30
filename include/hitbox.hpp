#ifndef HITBOX
#define HITBOX

#include <allegro5/allegro_primitives.h>

class Hitbox{
    private:
        float x1,y1,x2,y2,posX,posY;
    public:
        //Constroi a partir de um retangulo
        Hitbox(float x1,float y1,float x2,float y2,float posX,float posY);
        //Constroi a partir das dimensoes de um bitmap
        Hitbox(ALLEGRO_BITMAP *bitmap,float posX,float posY);
        //Checa se houve colisao com a hitbox enviada
        bool has_collision(Hitbox hitbox);
        //Desenha o retangulo que representa a hitbox
        void draw_hitbox();
        //Move a hitbox para a coordenada nova explicitada. ATENÇÃO: NÃO ESQUECER DE SEMPRE MOVIMENTAR A HITBOX JUNTO COM O OBJETO
        void move_hitbox(float new_posX,float new_posY);
};

#endif