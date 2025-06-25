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
        //Construtor padrão com coordenadas e BITMAP associados, sem hitbox
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap);
        //Construtor padrão com coordenadas e BITMAP associados, com hitbox construida sobre BITMAP
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap,bool hasHitbox);
        //Construtor padrão com coordenadas e BITMAP associados, com hitbox de dimensões especificadas
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap,bool hasHitbox,float width,float height);

        virtual ~ObjetoRenderizavel(){};
        //Retorna posição de X
        float get_posX();
        //Retorna posição de Y
        float get_posY();
        //Retorna velocidade no eixo X
        float get_velocityX();
        //Retorna velocidade no eixo Y
        float get_velocityY();
        //Retorna posição de X do centro da imagem, relativo ao bitmap
        float get_centerX();
        //Retorna posição de Y do centro da imagem, relativo ao bitmap
        float get_centerY();
        //Retorna a hitbox associada ao Objeto
        Hitbox& get_hitbox();
        //Retorna o bitmap associado ao Objeto
        ALLEGRO_BITMAP* get_bitmap();
        //Define a posição X
        void set_posX(float x);
        //Define a posição Y
        void set_posY(float y);
        //Define velocidade no eixo X
        void set_velocityX(float x);
        //Define velocidade no eixo Y
        void set_velocityY(float y);
        //Define um novo bitmap para o Objeto;
        void set_bitmap(ALLEGRO_BITMAP* bitmap);
        //Renderiza a imagsem nas coordenadas (x,y)
        virtual void render_object();
        //Comportamento a cada tick padrão
        virtual void on_tick();
        //Move o Objeto de acordo com a velocidade
        virtual void move_object();
};

#endif