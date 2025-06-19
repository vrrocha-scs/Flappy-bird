#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <vector>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"
#include "../include/randomizador.hpp"
#include "../include/menu.hpp"

//Classe de estados do jogo
enum class GameState {
    PLAYING,
    PAUSED,
    GAMEOVER,
    EXITING
};


//tamanho da tela
const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const float FPS = 60;
const float SECONDS_PER_UPDATE = 1.0f / FPS;
double ultimo_spawn = 0;

int main()
{
    //inicializando as bibliotecas
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();


    //inicializando as structs padrão
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    //ALLEGRO_EVENT_QUEUE * spawner_queue = NULL;
    ALLEGRO_TIMER * timer = NULL;
    //ALLEGRO_TIMER * spawner = NULL;
    Randomizador rando(200, 800);

    ALLEGRO_FONT* menu_font = al_load_font("assets/fonts/game_over.ttf", 48, 0);
    if (!menu_font) {
        std::cerr << "Erro ao carregar fonte do menu!" << std::endl;
        return -1;
    }

    //spawner_queue = al_create_event_queue();
    event_queue = al_create_event_queue();
    display = al_create_display(SCREEN_W,SCREEN_H);
    al_set_window_position(display,100,100);
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    al_start_timer(timer);

    //sprites do jogo
    ALLEGRO_BITMAP* character_sprite = al_load_bitmap("assets/images/character_placeholder.png");
    ALLEGRO_BITMAP* upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    ALLEGRO_BITMAP* lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");

    //objetos do jogo
    std::vector<Obstaculo*> canos;
    Personagem* character = new Personagem(SCREEN_W/2 -250,SCREEN_H/2,character_sprite);
    ObjetoRenderizavel Chao(SCREEN_H-100,0,al_create_bitmap(SCREEN_W,100));
    
    //controle de tempo
    double previous_time = al_get_time();
    double lag = 0;
    double jump_cooldown = 0;
    GameState current_state = GameState::PLAYING;

    //WHILE PRINCIPAL
    while(current_state != GameState::EXITING){
        
        double current_time = al_get_time();
        double elapsed = current_time-previous_time;
        previous_time = current_time;
        
        if(elapsed > 0.25){
            elapsed = SECONDS_PER_UPDATE;
        }
        lag += elapsed;
        ALLEGRO_EVENT ev;
        
        //---INPUT NAO BLOCANTE---
        while(al_get_next_event(event_queue,&ev)){
            if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                current_state = GameState::EXITING;
            }

            if (current_state == GameState::PLAYING)
            {
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                    if (ev.keyboard.keycode == ALLEGRO_KEY_UP){
                        if(current_time-jump_cooldown > 0.25){
                            character->jump();
                            jump_cooldown = current_time;
                        }
                    }
                    else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        current_state = GameState::PAUSED;
                    }
                }
            }
        }

        if(current_state == GameState::PLAYING){
            //---LOGICA DE COLISÂO---
            if(character->checkCollision(*Chao.get_hitbox())){
                //IMPLEMENTAR TELA DE GAME OVER AQUI
                current_state = GameState::GAMEOVER;
            }
            for (auto c : canos)
             { 
                 if(character->checkCollision(*c->get_hitbox())){
                     current_state = GameState::GAMEOVER;
                     break;
                 }
            }
            
            //---LOGICA DE TEMPO FIXO---
            while(lag >= SECONDS_PER_UPDATE){
                
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
                    std::vector<Obstaculo*>::iterator it = canos.begin();
                    delete *(it);
                    canos.erase(it);
                    delete *(it);
                    canos.erase(it);
                }
                lag -= SECONDS_PER_UPDATE;
            }
        }
        
        if (current_state == GameState::PAUSED){
            Menu pause_menu(display, menu_font, MenuType::PAUSE);
            MenuResult result = pause_menu.show();
            
            if (result == MenuResult::CONTINUE_GAME){
                current_state = GameState::PLAYING;
                previous_time = al_get_time();
            }
            //else if (result == MenuResult::RESTART_GAME){
            // 
            //}
            else if (result == MenuResult::EXIT_GAME){
                current_state = GameState::EXITING;
            }
        }
        else if (current_state == GameState::GAMEOVER){
            Menu end_menu(display, menu_font, MenuType::END);
            MenuResult result = end_menu.show();

            //if (result == MenuResult::RESTART_GAME){
            // 
            //}
            //else if (result == MenuResult::EXIT_GAME){
            //  current_state = GameState::EXITING;
            //}
        }

        //--- RENDERIZAÇÃO ---
        al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));

        character->render_object();
        Chao.get_hitbox()->draw_hitbox();
        for (auto c : canos)
        { 
            c->desenhar_canos();
        }

        al_flip_display();
    }

    //dealocando memória
    delete character;
    for (auto c : canos) {
        delete c;
    }
    canos.clear();


    //dealocando imagens
    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);
    al_destroy_font(menu_font);

    //fecha a janela
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return 0;
}