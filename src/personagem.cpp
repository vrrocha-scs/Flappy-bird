#include "../include/personagem.hpp"
#include <algorithm> 
#include <math.h>

#define M_PI 3.14159265358979323846
#define JUMP_POWER 5
#define GRAVITY 0.25

Personagem::Personagem(float x, float y,ALLEGRO_BITMAP* bitmap) : ObjetoRenderizavel(x,y,bitmap,1),jump_power(JUMP_POWER),gravity(GRAVITY), score(0){};

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
        get_hitbox()->on_tick();
        set_velocityY(get_velocityY()+gravity);
        move_character();
}
void Personagem::jump(){
    set_velocityY(-jump_power);
}
bool Personagem::checkCollision(Hitbox other_hitbox){
    if(get_hitbox()->has_collision(other_hitbox)){
        return true;
    }
    return false;
}
void Personagem::render_object(){
    get_hitbox()->draw_hitbox();

    float rotation = min((velocityY/jump_power)*(M_PI/4)*0.45,M_PI/2);
    
    al_draw_rotated_bitmap(get_bitmap(),get_centerX(),get_centerY(),get_posX()+get_centerX(),get_posY()+get_centerY(),rotation,0);
}