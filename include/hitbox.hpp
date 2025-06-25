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
        /**
         * @brief  Constroi a partir de coordenadas passadas por valor, indo do ponto (x1,y1) ate o ponto (x2,y2) em formato retangular
         */    
        Hitbox(const float& posX,const float& posY,float x1,float y1,float x2,float y2);
        /**
         * @brief  Constroi a partir do objeto renderizavel em que se conecta
         */        
        Hitbox(const float& posX,const float& posY, float width,float height);
        /**
         * @brief  Checa se houve colisao com a hitbox enviada
         */        
        bool has_collision(Hitbox hitbox);
        /**
         * @brief  Muda alinhamento da hitbox em relação ao objeto renderizável
         */        
        void set_offset(float x,float y);
        /**
         * @brief  Desenha o retangulo que representa a hitbox
         */        
        void draw_hitbox();
        /**
         * @brief  Comportamento a cada tick da Hitbox (acompanha o ObjetoRenderizavel associado)
         */        
        void on_tick();
        /**
         * @brief  Retorna posição X da Hitox
         */        
        float get_posX();
        /**
         * @brief  Retorna posição Y da Hitbox
         */       
        float get_posY();

};

#endif