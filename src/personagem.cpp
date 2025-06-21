#include "../include/personagem.hpp"
#include <algorithm> 
#include <math.h>

#define M_PI 3.14159265358979323846
#define JUMP_POWER 5
#define GRAVITY 0.25

Personagem::Personagem(float posX, float posY,ALLEGRO_BITMAP* bitmap1,ALLEGRO_BITMAP* bitmap2) : ObjetoRenderizavel(posX,posY,bitmap1,1,64,50),jump_power(JUMP_POWER),gravity(GRAVITY),idle_sprite(bitmap1),jumping_sprite(bitmap2){
    score = 0;
    get_hitbox().set_offset(0,14);
};

void Personagem::move_character(){
    float dy = max((this->get_posY()+get_velocityY()),(float)0);
    this->set_posY(dy);

}

void Personagem::gain_score(int x){
    score += x;
}

int Personagem::get_score(){
    return score;
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
    float rotation = min((get_velocityY()/jump_power)*(M_PI/4)*0.45,M_PI/2);
    
    al_draw_scaled_rotated_bitmap(get_bitmap(),get_centerX(),get_centerY(),get_posX()+get_centerX(),get_posY()+get_centerY(),1.5,1.5,rotation,0);
}
void Personagem::reset_position(float start_x, float start_y){
    set_posX(start_x);
    set_posY(start_y);
    this->get_hitbox().on_tick();

    set_velocityY(0);
    set_velocityX(0);

    this->score = 0;
}