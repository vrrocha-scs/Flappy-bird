#include "hitbox.hpp"
Hitbox::Hitbox(float x1,float y1,float x2,float y2) : x1(x1),y1(y1),x2(x2),y2(y2){};

Hitbox::Hitbox(ObjetoRenderizavel* object){
    attached_object = object;
    float bitmap_height = al_get_bitmap_height(attached_object->get_bitmap());
    float bitmap_width = al_get_bitmap_width(attached_object->get_bitmap());
    x1 = attached_object->get_posX();// - bitmap_width/2;
    x2 = attached_object->get_posX() + bitmap_width;
    y1 = attached_object->get_posY();// - bitmap_height/2;
    y2 = attached_object->get_posY() + bitmap_height;
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
    float bitmap_height = al_get_bitmap_height(attached_object->get_bitmap());
    float bitmap_width = al_get_bitmap_width(attached_object->get_bitmap());
    x1 = attached_object->get_posX();// - bitmap_width/2;
    x2 = attached_object->get_posX() + bitmap_width;
    y1 = attached_object->get_posY();// - bitmap_height/2;
    y2 = attached_object->get_posY() + bitmap_height;
}

void Hitbox::draw_hitbox(){
    al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgba_f(0,1,0,0.25));
}