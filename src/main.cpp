#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"
#include "../include/randomizador.hpp"
#include "../include/menu.hpp"

// Enumerador de estados do jogo
enum class GameState {
    PLAYING,
    PAUSED,
    GAMEOVER,
    EXITING
};

// Constantes da tela e tempo
const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const float FPS = 60;
const float SECONDS_PER_UPDATE = 1.0f / FPS;

double ultimo_spawn = 0;

//alterações na dificuldade
int multiplicador_espaco_canos = 2.5;
int velocidade_canos = 1.5;

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
    ALLEGRO_BITMAP* character_sprite = al_load_bitmap("assets/images/character_placeholder.png");
    ALLEGRO_BITMAP* jumping_sprite = al_load_bitmap("assets/images/jumping_placeholder.png");
    ALLEGRO_BITMAP* upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    ALLEGRO_BITMAP* lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");
    ALLEGRO_FONT* menu_font = al_load_font("assets/fonts/game_over.ttf", 48, 0);

    if (!character_sprite || !upper_pipe_sprite || !lower_pipe_sprite || !menu_font) {
        std::cerr << "Erro fatal: Falha ao carregar um ou mais assets." << std::endl;
        return -1;
    }

    //================================================================================
    // Bloco de Variáveis e Objetos do Jogo
    //================================================================================
    GameState current_state = GameState::PLAYING;
    Randomizador* rando = new Randomizador(200, 800);

    std::vector<Obstaculo*> canos;
    Personagem* character = new Personagem(SCREEN_W / 2 - 250, SCREEN_H / 2, character_sprite,jumping_sprite);
    ObjetoRenderizavel Chao(0,SCREEN_H - 100, al_create_bitmap(SCREEN_W, 100));

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
                    }
                } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    current_state = GameState::PAUSED;
                }
            }
        }

        // --- Seção de Lógica do Jogo ---
        if (current_state == GameState::PLAYING) {
            // Colisão com chão
            if (character->checkCollision(Chao.get_hitbox())) {
                current_state = GameState::GAMEOVER;
            }

            //Colisão com obstáculos
            for (auto c : canos) {
                if (character->checkCollision(c->get_hitbox())) {
                    current_state = GameState::GAMEOVER;
                    break;
                }
            }
            

            // Lógica de atualização baseada em tempo fixo
            while (lag >= SECONDS_PER_UPDATE) {
                character->on_tick();
                Chao.on_tick();
                for (auto c : canos) {
                    c->on_tick();
                }
                
                // Lógica de Spawn
                if (current_time - ultimo_spawn >= PIPE_SPAWN_INTERVAL) {
                    ultimo_spawn = current_time;
                    int altura_buraco = definir_altura_superior(rando);
                    int tamanho_gap = definir_tamanho_gap(multiplicador_espaco_canos, character_sprite);
                    adicionando_canos(canos, altura_buraco, tamanho_gap, upper_pipe_sprite, lower_pipe_sprite, 1.5);
                }
            
                lag -= SECONDS_PER_UPDATE;
            }

            limpando_obstaculos(canos);

        }

        // // --- Seção de Lógica de MENUS (Bloqueante) ---
        if (current_state == GameState::PAUSED) {
            Menu pause_menu(event_queue, menu_font, MenuType::PAUSE);
            MenuResult result = pause_menu.show();

            if (result == MenuResult::CONTINUE_GAME) {
                current_state = GameState::PLAYING;
                previous_time = al_get_time();
            } else if (result == MenuResult::RESTART_GAME) {
                restart_game(character, canos);
                current_state = GameState::PLAYING;
            } else if (result == MenuResult::EXIT_GAME) {
                current_state = GameState::EXITING;
            }
        } else if (current_state == GameState::GAMEOVER) {
            Menu end_menu(event_queue, menu_font, MenuType::END);
            MenuResult result = end_menu.show();

            if (result == MenuResult::RESTART_GAME) {
                restart_game(character, canos);
                current_state = GameState::PLAYING;
            } else if (result == MenuResult::EXIT_GAME) {
                current_state = GameState::EXITING;
            }
        }

        // --- Seção de Renderização ---
        al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));

        character->render_object();
        Chao.get_hitbox().draw_hitbox();

        for (auto c : canos) {
            c->desenhar_canos();
        }

        al_flip_display();
    }

    //================================================================================
    // Bloco de Limpeza de Memória
    //================================================================================
    delete character;
    delete rando;

    for (auto c : canos) {
        delete c;
    }
    canos.clear();

    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);
    al_destroy_font(menu_font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return 0;
}
