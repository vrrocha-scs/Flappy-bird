#include "../include/personagem.hpp"

Personagem::Personagem(float x, float y,string image_path) : ObjetoRenderizavel(x,y,image_path), score(0), hitbox(0,0,0,0,get_posX(),get_posY()){};

void Personagem::move_character(){
    set_posX(get_posX()+velocityX);
    set_posY(get_posY()+velocityY);
    //hitbox.move_hitbox(get_posX(),get_posY());
}

void Personagem::gain_score(int x){
    score += x;
}

void Personagem::set_velocityX(float x){
    velocityX = x;
}

void Personagem::set_velocityY(float y){
    velocityY = y;
}

int Personagem::get_score(){
    return score;
}

float Personagem::get_velocityX(){
    return velocityX;
}

float Personagem::get_velocityY(){
    return velocityY;
}

void Personagem::on_tick(){
        set_velocityY(get_velocityY()+0.15);
        move_character();
        hitbox.draw_hitbox();
}

//TODO: Personagem rotaciona em funcao de sua velocidade vertical
// void Personagem::render_object(){
//     al_draw_rotated_bitmap(get_bitmap(),get_posX(),get_posY(),get_posX(),get_posY(),velocityY*(0.1),0);
// }