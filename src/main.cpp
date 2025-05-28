#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <vector>

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
    display = al_create_display(SCREEN_W,SCREEN_H);
    al_set_window_position(display,200,200);
    al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
    al_flip_display();
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    al_start_timer(timer);

    //objetos do jogo
    Personagem character(SCREEN_W/2, SCREEN_H/2,"assets/images/character_placeholder.png");
    Obstaculo Cano1(600, 0, 1.2, 30, 150);
    Obstaculo Cano2(600, 320, 1.2, 30, SCREEN_H - 320);

    
    Personagem* character = new Personagem(SCREEN_W/2 + 20,SCREEN_H/2,"assets/images/character_placeholder.png");
    vector<ObjetoRenderizavel*> objects_to_render;
    objects_to_render.push_back(character);

    //WHILE PRINCIPAL
    while(playing){

        al_flip_display();
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        //---RENDER---
        if(ev.type == ALLEGRO_EVENT_TIMER){
            
            for(auto i : objects_to_render){
                i->render_object();
            }
            //Descreve comportamento do personagem a cada segundo
            character->onTick();
            al_flip_display();
            
            character.set_velocityY(character.get_velocityY()+0.05);
            character.move_character();
            character.render_object();  
            Cano1.desenhar_canos();
            Cano1.mover_obstaculos();
            Cano2.desenhar_canos();
            Cano2.mover_obstaculos();
            al_flip_display();
            al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
        }
        //---CONTROLES---
        else if (ev.keyboard.keycode == ALLEGRO_KEY_UP){
            character->set_velocityY(-3.5);
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            playing = false;
        }
    }

    //fecha a janela
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}