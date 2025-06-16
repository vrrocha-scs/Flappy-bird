#include "hitbox.hpp"


Hitbox::Hitbox(ObjetoRenderizavel* object){
    attached_object = object;
    float bitmap_height = al_get_bitmap_height(attached_object->get_bitmap());
    float bitmap_width = al_get_bitmap_width(attached_object->get_bitmap());
    x1 = attached_object->get_posX() - bitmap_width/2;
    x2 = attached_object->get_posX() + bitmap_width/2;
    y1 = attached_object->get_posY() - bitmap_height/2;
    y2 = attached_object->get_posY() + bitmap_height/2;
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
    x1 = attached_object->get_posX() - bitmap_width/2;
    x2 = attached_object->get_posX() + bitmap_width/2;
    y1 = attached_object->get_posY() - bitmap_height/2;
    y2 = attached_object->get_posY() + bitmap_height/2;
}

void Hitbox::draw_hitbox(){
    al_draw_filled_rectangle(x1,y1,x2,y2,al_map_rgba_f(1,0,0,0.25));
}