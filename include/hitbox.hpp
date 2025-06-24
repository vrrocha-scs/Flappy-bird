#ifndef HITBOX
#define HITBOX

#include <allegro5/allegro_primitives.h>

class Hitbox{
    private:
        const float& posX;
        const float& posY;
        float x1,y1,x2,y2;
        float width,height;
        float offsetX,offsetY;

    public:
        Hitbox(const float& posX,const float& posY,float x1,float y1,float x2,float y2);
        //Constroi a partir do objeto renderizavel em que se conecta
        Hitbox(const float& posX,const float& posY, float width,float height);
        //Checa se houve colisao com a hitbox enviada
        bool has_collision(Hitbox hitbox);
        //Muda alinhamento da hitbox em relação ao objeto renderizável
        void set_offset(float x,float y);
        //Desenha o retangulo que representa a hitbox
        void draw_hitbox();
        //Comportamento a cada tick da Hitbox (acompanha o ObjetoRenderizavel associado)
        void on_tick();
        float get_posX();
        float get_posY();

};

#endif