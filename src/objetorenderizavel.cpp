#include "../include/objetorenderizavel.hpp"
#include <allegro5/allegro_image.h>

ObjetoRenderizavel::ObjetoRenderizavel(float x,float y,string image_path){
    posX = x;
    posY = y;
    object_bitmap = al_load_bitmap(image_path.c_str());
    centerX = (al_get_bitmap_width(object_bitmap))/2;
    centerY = (al_get_bitmap_height(object_bitmap))/2;
}

ObjetoRenderizavel::~ObjetoRenderizavel(){
    al_destroy_bitmap(object_bitmap);
}

void ObjetoRenderizavel::render_object(){
    al_draw_bitmap(object_bitmap,posX,posY,0);
}

float ObjetoRenderizavel::get_posX(){
    return posX;
}

float ObjetoRenderizavel::get_posY(){
    return posY;
}
float ObjetoRenderizavel::get_centerX(){
    return centerX;
}
float ObjetoRenderizavel::get_centerY(){
    return centerY;
}
ALLEGRO_BITMAP* ObjetoRenderizavel::get_bitmap(){
    return object_bitmap;
}

void ObjetoRenderizavel::set_posX(float x){
    posX = x;
}

void ObjetoRenderizavel::set_posY(float y){
    posY = y;
}



