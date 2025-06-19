#include "hitbox.hpp"
#include <iostream>

Hitbox::Hitbox(const float& posX,const float& posY, float x1,float y1,float x2,float y2) : posX(posX),posY(posY),x1(x1),y1(y1),x2(x2),y2(y2){};

Hitbox::Hitbox(const float& posX,const float& posY,float width,float height) : posX(posX), posY(posY),width(width),height(height){
    x1 = posX;
    x2 = posX + width;
    y1 = posY;
    y2 = posY + height;
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

void Hitbox::on_tick(){
    x1 = posX;
    x2 = posX + width;
    y1 = posY;
    y2 = posY + height;
}

void Hitbox::draw_hitbox(){
    al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgba_f(0,1,0,0.25));
}