#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"
#include "../include/randomizador.hpp"
#include "../include/menu.hpp"
#include "../include/cadastro.hpp"
#include "../include/leaderboard.hpp"
#include "../include/gamestate.hpp"
#include "../include/utils.hpp"

// Constantes da tela e tempo
const float FPS = 60;
const float SECONDS_PER_UPDATE = 1.0f / FPS;
double ultimo_spawn = 0;


// Função de reinício do jogo
void restart_game(Personagem*& character, std::vector<Obstaculo*>& canos) {
    character->reset_position(SCREEN_W / 2 - 250, SCREEN_H / 2);
    
    for (auto c : canos) {
        delete c;
    }
    
    canos.clear();
    ultimo_spawn = al_get_time();
}

int main() {
     //================================================================================
    // Bloco de Inicialização do Allegro
    //================================================================================
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);


    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!display || !event_queue) {
        std::cerr << "Erro fatal: Falha ao inicializar Display ou Fila de Eventos." << std::endl;
        return -1;
    }
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    al_set_window_position(display, 100, 100);

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    //================================================================================
    // Bloco de Carregamento de Assets
    //================================================================================
    ALLEGRO_BITMAP* character_sprite = al_load_bitmap("assets/images/character.png");
    ALLEGRO_BITMAP* jumping_sprite = al_load_bitmap("assets/images/character_jumping.png");
    ALLEGRO_BITMAP* ground_sprite = al_load_bitmap("assets/images/chao.png");
    ALLEGRO_BITMAP* upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    ALLEGRO_BITMAP* lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");
    ALLEGRO_FONT* menu_font = al_load_font("assets/fonts/game_over.ttf", 80, 0);
    ALLEGRO_FONT* score_font = al_load_font("assets/fonts/game_over.ttf", 160, 0);
    ALLEGRO_SAMPLE* som_pulo = al_load_sample("assets/sounds/jump_sound_3.wav");
    ALLEGRO_SAMPLE* som_gameover = al_load_sample("assets/sounds/gameover_sound.wav");

    ALLEGRO_BITMAP* mountains_background = al_load_bitmap("assets/images/montanhas.png");
    ALLEGRO_BITMAP* hills_background = al_load_bitmap("assets/images/morros.png");
    if (!character_sprite || !jumping_sprite || !upper_pipe_sprite || !lower_pipe_sprite || !menu_font || !som_pulo) {
        std::cerr << "Erro fatal: Falha ao carregar um ou mais assets." << std::endl;
        return -1;
    }

    //================================================================================
    // Bloco de Variáveis e Objetos do Jogo
    //================================================================================
    int multiplicador_espaco_canos = 3.0;
    int velocidade_canos = 1.5;
    int tamanho_gap = definir_tamanho_gap(multiplicador_espaco_canos, character_sprite);
    GameState current_state = GameState::START;
    Cadastro* jogador_atual = nullptr;
    int score_da_partida = 0;
    int minimo = 100;
    int maximo = SCREEN_H - (al_get_bitmap_height(ground_sprite) + tamanho_gap + minimo);
    Randomizador* rando = new Randomizador(minimo, maximo);

    std::vector<Obstaculo*> canos;
    Personagem* character = new Personagem(SCREEN_W / 2 - 250, SCREEN_H / 2, character_sprite,jumping_sprite);
    ObjetoRenderizavel* Chao = new ObjetoRenderizavel(0,SCREEN_H - 90, ground_sprite,1);
    ObjetoRenderizavel* Morros = new ObjetoRenderizavel(0,650,hills_background);
    ObjetoRenderizavel* Montanhas = new ObjetoRenderizavel(0,0,mountains_background);
    Montanhas->set_velocityX(-velocidade_canos*0.4);
    Morros->set_velocityX(-velocidade_canos*0.8);
    Chao->set_velocityX(-velocidade_canos);
    vector<ObjetoRenderizavel*> background_items = {Montanhas,Morros,Chao};
    // Controle de tempo
    double previous_time = al_get_time();
    double lag = 0;
    double jump_cooldown = 0;

    // Constantes de gameplay para fácil ajuste
    const float JUMP_COOLDOWN_SECONDS = 0.25f;
    const float PIPE_SPAWN_INTERVAL = 6.0f;
    const int MAX_PIPES = 10;

    //================================================================================
    // Loop Principal do Jogo
    //================================================================================
    while (current_state != GameState::EXITING) {

        // --- Seção de Controle de Tempo ---//
        double current_time = al_get_time();
        double elapsed = current_time - previous_time;
        previous_time = current_time;

        if (elapsed > 0.25) {
            elapsed = SECONDS_PER_UPDATE;
        }

        lag += elapsed;
        
        ALLEGRO_EVENT ev;
        // --- Seção de Processamento de Input Não Bloqueante ---
        while (al_get_next_event(event_queue, &ev)) {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                current_state = GameState::EXITING;
            }

            if (current_state == GameState::PLAYING && ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                    if (current_time - jump_cooldown > JUMP_COOLDOWN_SECONDS) {
                        character->jump();
                        jump_cooldown = current_time;
                        al_play_sample(som_pulo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    }
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    current_state = GameState::PAUSED;
                }
            }
        }

        // --- Seção de Lógica do Jogo ---
        if (current_state == GameState::PLAYING) {
            // Colisão com chão
            if (character->checkCollision(Chao->get_hitbox())) {
                al_play_sample(som_gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                current_state = GameState::GAMEOVER;
                jogador_atual->modificar_dados(score_da_partida);
            }

            //Colisão com obstáculos
            for (auto c : canos) {
                if (character->checkCollision(c->get_hitbox())) {
                    al_play_sample(som_gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    current_state = GameState::GAMEOVER;
                    jogador_atual->modificar_dados(score_da_partida);

                    break;
                }
            }
            

            // Lógica de atualização baseada em tempo fixo
            while (lag >= SECONDS_PER_UPDATE) {
                character->on_tick();
                for (auto i : background_items) {
                    i->on_tick();
                    if(i->get_posX() < -2000){
                        i->set_posX(0);
                    }
                }
                for (auto c : canos) {
                    c->on_tick();
                    c->check_passagem(character);
                }
                
                // Lógica de Spawn
                if (current_time - ultimo_spawn >= PIPE_SPAWN_INTERVAL) {
                    ultimo_spawn = current_time;
                    int altura_buraco = definir_altura_superior(rando);
                    adicionando_canos(canos, altura_buraco, tamanho_gap, upper_pipe_sprite, lower_pipe_sprite, 1.5);
                }
            
                lag -= SECONDS_PER_UPDATE;
            }
            if (!canos.empty())
            {
                limpando_obstaculos(canos);
            }
            

        }

        // // --- Seção de Lógica de MENUS (Bloqueante) ---
        if (current_state == GameState::START || current_state == GameState::PAUSED || current_state == GameState::GAMEOVER) {
        // Determina o tipo de menu a ser criado com base no estado atual
            MenuType menu_type_to_show = MenuType::START;
                if (current_state == GameState::PAUSED) menu_type_to_show = MenuType::PAUSE;
                if (current_state == GameState::GAMEOVER) menu_type_to_show = MenuType::END;

            // Cria o menu
            Menu active_menu(event_queue, menu_font, menu_type_to_show);
    

        active_menu.process_state_logic(
            current_state,
            jogador_atual,
            character,
            canos,
            display,
            background_items,
            previous_time,
            ultimo_spawn
            );
}
        // --- Seção de Renderização ---
        

        al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
        for (auto i : background_items) {
            i->render_object();
        }
        if(current_state == GameState::PLAYING){
        character->render_object();
        for (auto c : canos) {
            c->desenhar_canos();
        }
        al_draw_textf(score_font, al_map_rgb(255, 255, 255), SCREEN_W/2, 20, ALLEGRO_ALIGN_CENTRE,"%i", character->get_score());
        }
        al_flip_display();
    }

    //================================================================================
    // Bloco de Limpeza de Memória
    //================================================================================
    delete character;
    delete rando;
    delete Chao;

    for (auto c : canos) {
        delete c;
    }
    canos.clear();

    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);
    al_destroy_font(menu_font);
    al_destroy_font(score_font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return 0;
}
