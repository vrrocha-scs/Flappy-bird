#ifndef OBJETO_RENDERIZAVEL
#define OBJETO_RENDERIZAVEL

#include <allegro5/allegro.h>
#include <string>
#include "hitbox.hpp"

using namespace std;

class ObjetoRenderizavel{
    private:
        float posX, posY,centerX,centerY;
        ALLEGRO_BITMAP* object_bitmap;
        bool hasHitbox;
        Hitbox hitbox;

    public:
        //Construtor padrão com coordenadas e BITMAP associados
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap);
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap,bool hasHitbox);

        virtual ~ObjetoRenderizavel(){};
        //Retorna posição de X
        float get_posX();
        //Retorna posição de Y
        float get_posY();
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
        //Define um novo bitmap para o Objeto;
        void set_bitmap(ALLEGRO_BITMAP* bitmap);
        //Renderiza a imagsem nas coordenadas (x,y)
        virtual void render_object();
        //Comportamento a cada tick padrão
        virtual void on_tick(){};
};

#endif