#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"

//tamanho da tela
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const float FPS = 60;

int main()
{
    //inicializando as bibliotecas
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();

    //inicializando as structs padrão
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    ALLEGRO_TIMER * timer = NULL;

    bool playing = true;


    event_queue = al_create_event_queue();
    display = al_create_display(640,480);
    al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
    al_flip_display();
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    al_start_timer(timer);

    //objetos do jogo
    Personagem character(300,300,"assets/images/character_placeholder.png");
    
    //WHILE PRINCIPAL
    /*To do: Adicionar queue de eventos com:
        -Evento do timer
        -Evento de clicar
    */    
    while(playing){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        if(ev.type == ALLEGRO_EVENT_TIMER){
            //Evento de renderização (precisa ser executado todo frame)
            al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
            character.set_velocityY(character.get_velocityY()+0.05);
            character.move_character();
            character.render_object();
            al_flip_display();
        }
        else if (ev.type == ALLEGRO_KEY_UP){
            character.set_velocityY(-5);
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            playing = false;
        }
    }

    //fecha a janela
    al_destroy_display(display);
    return 0;
}