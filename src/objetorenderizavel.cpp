#include "../include/objetorenderizavel.hpp"
#include <allegro5/allegro_image.h>

ObjetoRenderizavel::ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap) : posX(x), posY(y), object_bitmap(bitmap),hasHitbox(0),hitbox(-1,-1,-1,-1,-1,-1){
    centerX = (al_get_bitmap_width(object_bitmap))/2;
    centerY = (al_get_bitmap_height(object_bitmap))/2;
}
ObjetoRenderizavel::ObjetoRenderizavel(float x,float y,ALLEGRO_BITMAP* bitmap,bool hasHitbox) : posX(x), posY(y), object_bitmap(bitmap),hasHitbox(1),hitbox(posX,posY,al_get_bitmap_width(object_bitmap),al_get_bitmap_height(object_bitmap))
{
    centerX = (al_get_bitmap_width(object_bitmap))/2;
    centerY = (al_get_bitmap_height(object_bitmap))/2;
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

Hitbox& ObjetoRenderizavel::get_hitbox()
{
    return hitbox;
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



