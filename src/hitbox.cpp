#include "hitbox.hpp"


Hitbox::Hitbox(float x1,float y1,float x2,float y2,float posX,float posY) : x1(x1), y1(y1), x2(x2), y2(y2),posX(posX),posY(posY){};
Hitbox::Hitbox(ALLEGRO_BITMAP* bitmap,float posX,float posY){
    this->posX = posX;
    this->posY = posY;
    float bitmap_height = al_get_bitmap_height(bitmap);
    float bitmap_width = al_get_bitmap_width(bitmap);
    x1 = posX - bitmap_width/2;
    x2 = posX + bitmap_width/2;
    y1 = posY - bitmap_height/2;
    y2 = posY + bitmap_height/2;
}

bool Hitbox::has_collision(Hitbox hitbox){
    float a1 = hitbox.x1;
    float a2 = hitbox.x2;
    float b1 = hitbox.y1;
    float b2 = hitbox.y2;
    return ((x2 > a1 && y2 > b1 && x2 < a2 && y2 < b2) ||
            (x1 > a1 && y1 > b1 && x1 < a2 && y1 < b2) ||
            (x1 > a1 && y2 > b1 && x1 < a2 && y2 < b2) ||
            (x2 > a1 && y1 > b1 && x2 < a2 && y1 < b2)); 
};

void Hitbox::move_hitbox(float new_posX,float new_posY){
    float dposX = new_posX - posX;
    float dposY = new_posY - posY;
    x1 += dposX;
    x2 += dposX;
    y1 += dposY;
    y2 += dposY;
}

void Hitbox::draw_hitbox(){
    al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgba_f(1,0,0,0.5));
}