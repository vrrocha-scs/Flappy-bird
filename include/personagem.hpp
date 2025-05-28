#ifndef PERSONAGEM
#define PERSONAGEM
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "objetorenderizavel.hpp"

class Personagem : public ObjetoRenderizavel{
    private:
        float velocityX;
        float velocityY;
    
    public:
        Personagem(float x, float y,string image_path);
        void move_character();
        float get_velocityX();
        float get_velocityY();
        void set_velocityX(float x);
        void set_velocityY(float y);
};


#endif