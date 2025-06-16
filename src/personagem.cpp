#include "../include/personagem.hpp"
#include <algorithm> 
#include <math.h>

#define M_PI 3.14159265358979323846

Personagem::Personagem(float x, float y,string image_path) : ObjetoRenderizavel(x,y,image_path),jump_power(3.5),gravity(0.15), score(0), hitbox(this){};

void Personagem::move_character(){

    set_posX(get_posX()+velocityX);
    set_posY(get_posY()+velocityY);

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
        hitbox.on_tick();
        hitbox.draw_hitbox();
        set_velocityY(get_velocityY()+gravity);
        move_character();
}
void Personagem::jump(){
    set_velocityY(-jump_power);
}

void Personagem::render_object(){
    float rotation = min((velocityY/jump_power)*(M_PI/4)*0.45,M_PI/2);
    
    al_draw_rotated_bitmap(get_bitmap(),get_centerX(),get_centerY(),get_posX(),get_posY(),rotation,0);
}