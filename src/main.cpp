#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "../include/personagem.hpp"
//tamanho da tela
const int SCREEN_W = 800;
const int SCREEN_H = 600;

int main()
{
    //inicializando as bibliotecas
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();

    //inicializando as structs padrão
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_COLOR corteste = al_map_rgb(0, 255, 0);
    bool playing = true;

    //tela
    display = al_create_display(640,480);

    //objetos do jogo
    Personagem character(200,300,"assets/images/character_placeholder.png");
    
    //WHILE PRINCIPAL
    /*To do: Adicionar queue de eventos com:
        -Evento do timer
        -Evento de clicar
    */    
    while(playing){
        //Evento de renderização (precisa ser executado todo frame)
        al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
        character.set_velocityY(character.get_velocityY()+0.1);
        character.move_character();
        character.render_object();
        al_flip_display();
    }

    //fecha a janela
    al_destroy_display(display);
    return 0;
}