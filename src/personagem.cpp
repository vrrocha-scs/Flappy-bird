#include "../include/personagem.hpp"
#include <algorithm> 
#include <math.h>

#define M_PI 3.14159265358979323846
#define JUMP_POWER 5
#define GRAVITY 0.25

Personagem::Personagem(float posX, float posY,ALLEGRO_BITMAP* bitmap1,ALLEGRO_BITMAP* bitmap2) : ObjetoRenderizavel(posX,posY,bitmap1,1),jump_power(JUMP_POWER),gravity(GRAVITY),idle_sprite(bitmap1),jumping_sprite(bitmap2){
    velocityX = 0;
    velocityY = 0;
    score = 0;
};

void Personagem::move_character(){

    this->set_posX(this->get_posX()+velocityX);
    this->set_posY(this->get_posY()+velocityY);

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
        if(get_velocityY() < 0){
            set_bitmap(jumping_sprite);
        }
        else{
            set_bitmap(idle_sprite);
        }
        this->set_velocityY(this->get_velocityY()+gravity);
        this->move_character();
        this->get_hitbox().on_tick();

}
void Personagem::jump(){
    this->set_velocityY(-jump_power);
}
bool Personagem::checkCollision(Hitbox other_hitbox){
    if(get_hitbox().has_collision(other_hitbox)){
        return true;
    }
    return false;
}
void Personagem::render_object(){
    get_hitbox().draw_hitbox();

    float rotation = min((velocityY/jump_power)*(M_PI/4)*0.45,M_PI/2);
    
    al_draw_rotated_bitmap(get_bitmap(),get_centerX(),get_centerY(),get_posX()+get_centerX(),get_posY()+get_centerY(),rotation,0);
}
void Personagem::reset_position(float start_x, float start_y){
    set_posX(start_x);
    set_posY(start_y);

    set_velocityY(0);
    set_velocityX(0);

    this->score = 0;
}