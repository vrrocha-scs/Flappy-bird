#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "../include/personagem.hpp"

int main(){

    ALLEGRO_DISPLAY* display = NULL;
    
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();

    display = al_create_display(600,400);

}