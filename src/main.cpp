#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <vector>

#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"
#include "../include/randomizador.hpp"

//tamanho da tela
const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const float FPS = 60;

double ultimo_spawn = 0;

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
    //ALLEGRO_EVENT_QUEUE * spawner_queue = NULL;
    ALLEGRO_TIMER * timer = NULL;
    //ALLEGRO_TIMER * spawner = NULL;
    Randomizador rando(200, 800);

    bool playing = true;

    //spawner_queue = al_create_event_queue();
    event_queue = al_create_event_queue();
    display = al_create_display(SCREEN_W,SCREEN_H);
    al_set_window_position(display,100,100);
    al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
    al_flip_display();
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    al_start_timer(timer);

    //sprites do jogo
    ALLEGRO_BITMAP* character_sprite = al_load_bitmap("assets/images/character_placeholder.png");
    ALLEGRO_BITMAP* upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    ALLEGRO_BITMAP* lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");

    //objetos do jogo
    vector<Obstaculo*> canos;
    Personagem* character = new Personagem(SCREEN_W/2 -250,SCREEN_H/2,character_sprite);
    vector<ObjetoRenderizavel*> objects_to_render;
    objects_to_render.push_back(character);

    //WHILE PRINCIPAL
    while(playing){

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        double tempo_atual = al_get_time();
        //---RENDER---
        if(ev.type == ALLEGRO_EVENT_TIMER){
            
            //Renderiza e chama o comportamento dos objetos a cada segundo
            for(auto i : objects_to_render){
                i->render_object();
                i->on_tick();
            }
            for (auto c : canos)
            {
                c->on_tick();
            }

            if (tempo_atual - ultimo_spawn >= 6)
            {
                ultimo_spawn = tempo_atual;
                int altura_buraco = rando.valor_aleatorio();
                int tamanho_gap = 100;
                canos.push_back(new Obstaculo(SCREEN_W + 50, -1000 + altura_buraco, upper_pipe_sprite, 1.2, 50, altura_buraco));
                canos.push_back(new Obstaculo(SCREEN_W + 50, altura_buraco - tamanho_gap, lower_pipe_sprite, 1.2, 50, (SCREEN_H - (altura_buraco))));
            }
            if(canos.size() >= 10)
            {
                vector<Obstaculo*>::iterator it = canos.begin();
                delete *(it);
                canos.erase(it);
                delete *(it);
                canos.erase(it);
            }

            al_flip_display();
            al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
        }

        //---CONTROLES---
        else if (ev.keyboard.keycode == ALLEGRO_KEY_UP){
            character->jump();
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