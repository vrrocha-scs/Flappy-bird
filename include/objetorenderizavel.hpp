#ifndef OBJETO_RENDERIZAVEL
#define OBJETO_RENDERIZAVEL

#include <allegro5/allegro.h>
#include <string>

using namespace std;

class ObjetoRenderizavel{
    private:
        ALLEGRO_BITMAP* object_bitmap;
        float posX, posY,centerX,centerY;
    public:
        ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap);
        virtual ~ObjetoRenderizavel(){};
        float get_posX();
        float get_posY();
        float get_centerX();
        float get_centerY();
        ALLEGRO_BITMAP* get_bitmap();
        void set_posX(float x);
        void set_posY(float y);
        virtual void render_object();
        virtual void on_tick(){};
};

#endif