#include "../include/personagem.hpp"
#include "../include/objetorenderizavel.hpp"

Personagem::Personagem(float x, float y,string image_path) : ObjetoRenderizavel(x,y,image_path) {};

void Personagem::move_character(){
    set_posX(get_posX()+velocityX);
    set_posY(get_posY()+velocityY);
}

void Personagem::set_velocityX(float x){
    velocityX = x;
}

void Personagem::set_velocityY(float y){
    velocityY = y;
}

float Personagem::get_velocityX(){
    return velocityX;
}

float Personagem::get_velocityY(){
    return velocityY;
}

