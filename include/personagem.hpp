#ifndef PERSONAGEM
#define PERSONAGEM
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "objetorenderizavel.hpp"

class Personagem : public ObjetoRenderizavel{
    public:
        Personagem(float x, float y,string image_path);
        void move_character(float x,float y); 
};


#endif