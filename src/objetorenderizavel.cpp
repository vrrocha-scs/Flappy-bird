#include "../include/objetorenderizavel.hpp"
#include <allegro5/allegro_image.h>

ObjetoRenderizavel::ObjetoRenderizavel(float x,float y,string image_path){
    posX = x;
    posY = y;
    object_bitmap = al_load_bitmap(image_path.c_str());
}

void ObjetoRenderizavel::render_object(){
    al_draw_bitmap(object_bitmap,posX,posY,0);
}
