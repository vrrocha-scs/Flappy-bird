#ifndef PERSONAGEM
#define PERSONAGEM
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"

class Personagem : public ObjetoRenderizavel{
    private:
        float velocityX;
        float velocityY;
        float jump_power;
        float gravity;
        int score;
        Hitbox hitbox;
    public:
        Personagem(float x, float y,string image_path);
        void move_character();
        float get_velocityX();
        float get_velocityY();
        int get_score();
        void gain_score(int x);
        void set_velocityX(float x);
        void set_velocityY(float y);
        void on_tick() override;
        void jump();
        void render_object() override;
};


#endif