#ifndef OBJETO_RENDERIZAVEL
#define OBJETO_RENDERIZAVEL

#include <allegro5/allegro.h>
#include <string>

using namespace std;

class ObjetoRenderizavel{
    private:
        ALLEGRO_BITMAP* object_bitmap;
        float posX, posY;
    public:
        ObjetoRenderizavel(float x,float y,string image_path); 
        void render_object();
};

#endif