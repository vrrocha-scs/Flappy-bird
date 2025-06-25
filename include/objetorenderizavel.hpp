#ifndef OBJETO_RENDERIZAVEL
#define OBJETO_RENDERIZAVEL

#include <allegro5/allegro.h>
#include <string>
#include "hitbox.hpp"

using namespace std;

class ObjetoRenderizavel{
    private:
        float posX, posY,centerX,centerY,velocityX,velocityY;
        ALLEGRO_BITMAP* object_bitmap;
        bool hasHitbox;
        Hitbox hitbox;

    public:

        /**
         * @brief  Construtor padrão com coordenadas e BITMAP associados, sem hitbox
         */    
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap);
        /**
         * @brief  Construtor padrão com coordenadas e BITMAP associados, com hitbox construida sobre BITMAP
         */
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap,bool hasHitbox);
        /**
         * @brief  Construtor padrão com coordenadas e BITMAP associados, com hitbox de dimensões especificadas
         */
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap,bool hasHitbox,float width,float height);
        /**
         * @brief  Destrutor padrão do Objeto Renderizável
         */
        virtual ~ObjetoRenderizavel(){};
        /**
         * @brief  Retorna posição de X
         */        
        float get_posX();
        /**
         * @brief  Retorna posição de Y
         */        
        float get_posY();
        /**
         * @brief  Retorna velocidade no eixo X
         */        
        float get_velocityX();
        /**
         * @brief  Retorna velocidade no eixo Y
         */        
        float get_velocityY();
        /**
         * @brief  Retorna posição de X do centro da imagem, relativo ao bitmap
         */        
        float get_centerX();
        /**
         * @brief  Retorna posição de Y do centro da imagem, relativo ao bitmap
         */        
        float get_centerY();
        /**
         * @brief  Retorna a hitbox associada ao Objeto
         */        
        Hitbox& get_hitbox();
        /**
         * @brief  Retorna o bitmap associado ao Objeto
         */        
        ALLEGRO_BITMAP* get_bitmap();
        /**
         * @brief  Define a posição X
         */        
        void set_posX(float x);
        /**
         * @brief  Define a posição Y
         */       
        void set_posY(float y);
        /**
         * @brief  Define velocidade no eixo X
         */        
        void set_velocityX(float x);
        /**
         * @brief  Define velocidade no eixo Y
         */        
        void set_velocityY(float y);
        /**
         * @brief  Define um novo bitmap para o Objeto;
         */        
        void set_bitmap(ALLEGRO_BITMAP* bitmap);
        /**
         * @brief  Renderiza a imagsem nas coordenadas (x,y)
         */        
        virtual void render_object();
        /**
         * @brief  Comportamento a cada tick padrão
         */        
        virtual void on_tick();
        /**
         * @brief  Move o Objeto de acordo com a velocidade
         */     
        virtual void move_object();
};

#endif