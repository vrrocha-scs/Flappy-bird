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
const float SECONDS_PER_UPDATE = 1/FPS;
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

    bool running = true;

    //spawner_queue = al_create_event_queue();
    event_queue = al_create_event_queue();
    display = al_create_display(SCREEN_W,SCREEN_H);
    al_set_window_position(display,100,100);
    al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
    al_flip_display();
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    al_start_timer(timer);

    //sprites do jogo
    ALLEGRO_BITMAP* character_sprite = al_load_bitmap("assets/images/character_placeholder.png");
    ALLEGRO_BITMAP* upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    ALLEGRO_BITMAP* lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");

    //objetos do jogo
    vector<Obstaculo*> canos;
    Personagem* character = new Personagem(SCREEN_W/2 -250,SCREEN_H/2,character_sprite);
    Hitbox Chao(0,SCREEN_H-100,SCREEN_W,SCREEN_H);
    //controle de tempo
    double previous_time = al_get_time();
    double lag = 0;
    bool playing = true;
    //WHILE PRINCIPAL
    while(running){
        
        double current_time = al_get_time();
        double elapsed = current_time-previous_time;
        previous_time = current_time;
        double jump_cooldown;
        if(elapsed > 0.25){
            elapsed = SECONDS_PER_UPDATE;
        }
        lag += elapsed;
        ALLEGRO_EVENT ev;
        
        //---INPUT NAO BLOCANTE---
        while(al_get_next_event(event_queue,&ev)){
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP && playing){
                if(current_time-jump_cooldown > 0.25){
                    character->jump();
                    jump_cooldown = current_time;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                running = false;
            }
        }

        //---LOGICA DE COLISÂO---
        if(character->checkCollision(Chao)){
            //IMPLEMENTAR TELA DE GAME OVER AQUI
            playing = false;
        }
        // for (auto c : canos)
        //     {   
        //         if(character->checkCollision(c->get_hitbox())){
        //             playing = false;
        //         }
        //     }
        //---LOGICA DE TEMPO FIXO---
        while((lag >= SECONDS_PER_UPDATE) && playing ){
            
            //Chama o comportamento dos objetos a cada segundo
            character->on_tick();
            for (auto c : canos)
            {   
                c->on_tick();
            }


            if (current_time - ultimo_spawn >= 6)
            {
                ultimo_spawn = current_time;
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
            lag -= SECONDS_PER_UPDATE;
        }

        al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));

        character->render_object();
        Chao.draw_hitbox();
        for (auto c : canos)
        {   
            c->desenhar_canos();
        }

        al_flip_display();
    }

    //dealocando memória
    delete character;

    //dealocando imagens
    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);


    //fecha a janela
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}