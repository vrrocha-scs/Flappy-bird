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

#include "../include/coletavel.hpp"
#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"
#include "../include/randomizador.hpp"
#include "../include/menu.hpp"
#include "../include/cadastro.hpp"
#include "../include/leaderboard.hpp"
#include "../include/gamestate.hpp"
#include "../include/utils.hpp"
#include "../include/interfaces.hpp"


// Constantes da tela e tempo
const float FPS = 60;
const float SECONDS_PER_UPDATE = 1.0f / FPS;
double ultimo_spawn_canos = 0;
double ultimo_spawn_coletavel = 0;
double inicio_efeito_invencivel = 10;
const float DISTANCIA_ENTRE_CANOS = 450.0f;
int multiplicador_pontuacao=1;

// Função de reinício do jogo
void restart_game(Personagem*& character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis){
    character->reset_position(SCREEN_W / 2 - 250, SCREEN_H / 2);
    
    for (auto c : canos) {
        delete c;
    }

    for(auto p : coletaveis){
        delete p;
    }
    
    canos.clear();
    coletaveis.clear();
    ultimo_spawn_canos = 0;
    ultimo_spawn_coletavel = 0;
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

    //IMAGENS
    ALLEGRO_BITMAP* character_sprite = al_load_bitmap("assets/images/character.png");
    ALLEGRO_BITMAP* jumping_sprite = al_load_bitmap("assets/images/character_jumping.png");
    ALLEGRO_BITMAP* ground_sprite = al_load_bitmap("assets/images/chao.png");
    ALLEGRO_BITMAP* upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    ALLEGRO_BITMAP* lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");
    ALLEGRO_BITMAP* mountains_background = al_load_bitmap("assets/images/montanhas.png");
    ALLEGRO_BITMAP* hills_background = al_load_bitmap("assets/images/morros.png");
    ALLEGRO_BITMAP* icon = al_load_bitmap("assets/images/character_jumping.png");
    ALLEGRO_BITMAP* splash_img = al_load_bitmap("assets/images/splash.png");
    ALLEGRO_BITMAP* green_ball_sprite = al_load_bitmap("assets/images/bolaverde.png");

    if (!character_sprite || !jumping_sprite || !ground_sprite || !upper_pipe_sprite || !lower_pipe_sprite || !mountains_background||
         !hills_background || !icon  || !splash_img || !green_ball_sprite) {
        std::cerr << "Erro fatal: Falha ao carregar uma ou mais imagens" << std::endl;
        return -1;
    }

    al_set_display_icon(display, icon);
    

    //FONTES
    ALLEGRO_FONT* menu_font = al_load_font("assets/fonts/game_over.ttf", 80, 0);
    ALLEGRO_FONT* score_font = al_load_font("assets/fonts/game_over.ttf", 160, 0);

    if (!menu_font || !score_font) {
        std::cerr << "Erro fatal: Falha ao carregar uma ou mais fontes" << std::endl;
        return -1;
    }


    //SONS
    ALLEGRO_SAMPLE* som_pulo = al_load_sample("assets/sounds/jump-sfx.wav");
    ALLEGRO_SAMPLE* som_gameover = al_load_sample("assets/sounds/gameover-sfx.wav");
    ALLEGRO_SAMPLE* music = al_load_sample("assets/sounds/background-music.ogg");

    if (!som_pulo || !som_gameover || !music) {
        std::cerr << "Erro fatal: Falha ao carregar um ou mais audios" << std::endl;
        return -1;
    }




    //BACKGROUND SOUND
    ALLEGRO_VOICE* voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    ALLEGRO_MIXER* mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);
    ALLEGRO_SAMPLE_INSTANCE* music_instance = al_create_sample_instance(music);
    al_attach_sample_instance_to_mixer(music_instance, mixer);
    al_set_sample_instance_playmode(music_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(music_instance, 0.3); // volume ajustável [0,1]
    al_play_sample_instance(music_instance); 

    //SPLASH
    al_rest(0.3); 
    Interfaces interfaces(display, event_queue, menu_font);
    interfaces.mostrarSplash(splash_img);

    //================================================================================
    // Bloco de Variáveis e Objetos do Jogo
    //================================================================================
    int multiplicador_espaco_canos = 3.0;
    float velocidade_canos = 1.5;
    int tamanho_gap = definir_tamanho_gap(multiplicador_espaco_canos, character_sprite);
    GameState current_state = GameState::LOGIN;
    Cadastro* jogador_atual = nullptr;
    int score_da_partida = 0;
    int minimo = 100;
    int maximo = SCREEN_H - (al_get_bitmap_height(ground_sprite) + tamanho_gap + minimo);
    Randomizador* rando = new Randomizador(minimo, maximo);

    std::vector<Obstaculo*> canos;
    std::vector<Coletavel*> coletaveis;
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
    float intervalo_spawn_canos = DISTANCIA_ENTRE_CANOS / (velocidade_canos * FPS);
    const float intervalo_spawn_coletavel = 5;

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
                        al_play_sample(som_pulo, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
                al_play_sample(som_gameover, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                current_state = GameState::GAMEOVER;
                score_da_partida = character->get_score();
                jogador_atual->modificar_dados(score_da_partida);
            }

            //Colisão com obstáculos
            for (auto c : canos) {
                if ((character->checkCollision(c->get_hitbox())) && (character->get_invincible() == false)) {
                    al_play_sample(som_gameover, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    current_state = GameState::GAMEOVER;
                    score_da_partida = character->get_score();
                    jogador_atual->modificar_dados(score_da_partida);
                    break;
                }
            }

            //Colisão com coletáveis
            for (auto p : coletaveis){
                if(p->checkCollision(character->get_hitbox())){
                    if (p->get_tipo() == TiposColetaveis::INVINCIBLE && p->get_coletado() == false)
                    {
                        p->set_coletado(true);
                        character->set_invincible(true);
                    }
                    else if (p->get_tipo() == TiposColetaveis::PLUS_SCORE && p->get_coletado() == false)
                    {
                        p->set_coletado(true);
                        character->gain_score(2 * multiplicador_pontuacao);
                    }
                    //current_state = GameState::INVINCIBLE;
                };
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
                    c->check_passagem(character, multiplicador_pontuacao);
                }
                for (auto p : coletaveis)
                {
                    p->on_tick();
                }
                
                // Lógica de Spawn
                ultimo_spawn_canos += SECONDS_PER_UPDATE;
                ultimo_spawn_coletavel += SECONDS_PER_UPDATE;
                if (ultimo_spawn_canos >= intervalo_spawn_canos) {
                    //ultimo_spawn = current_time;
                    int altura_buraco = definir_altura(rando);
                    adicionando_canos(canos, altura_buraco, tamanho_gap, upper_pipe_sprite, lower_pipe_sprite, velocidade_canos);
                    ultimo_spawn_canos -= intervalo_spawn_canos;
                    if (ultimo_spawn_coletavel >= intervalo_spawn_coletavel)
                    {
                        int altura_coletavel = definir_altura(rando);
                        coletaveis.push_back(new Coletavel(0, altura_coletavel, green_ball_sprite, velocidade_canos, TiposColetaveis::PLUS_SCORE, ultimo_spawn_canos * (FPS * velocidade_canos)));
                        ultimo_spawn_coletavel -= intervalo_spawn_coletavel; 
                    }
                }

                
                

                inicio_efeito_invencivel -= SECONDS_PER_UPDATE;
                if(character->get_invincible() == true)
                {
                    //std::cout << "estou invencivel" << std::endl;
                    if(inicio_efeito_invencivel <= 0)
                    {
                        character->set_invincible(false);
                        inicio_efeito_invencivel = 10;
                    }
                }
            
                lag -= SECONDS_PER_UPDATE;
            }
            if (!canos.empty())
            {
                limpando_obstaculos(canos);
            }
            if (!coletaveis.empty())
            {
                limpando_coletaveis(coletaveis);
            }
            

        }


        // // --- Seção de Lógica de MENUS (Bloqueante) ---
        if (current_state == GameState::LOGIN || current_state == GameState::MAIN_MENU || current_state == GameState::PAUSED || current_state == GameState::GAMEOVER) {
        // Exibe tela de GAMEOVERS
            if (current_state == GameState::GAMEOVER) {
                interfaces.mostrarGameOver(score_font, score_da_partida);
             }
        // Determina o tipo de menu a ser criado com base no estado atual
        MenuType menu_type_to_show;
        if (current_state == GameState::LOGIN) {
            menu_type_to_show = MenuType::LOGIN;
        } else if (current_state == GameState::MAIN_MENU) {
            menu_type_to_show = MenuType::MAIN_MENU;
    } else if (current_state == GameState::PAUSED) {
            menu_type_to_show = MenuType::PAUSE;
    } else { // GAMEOVER
            menu_type_to_show = MenuType::END;
    }
            // Cria o menu
            Menu active_menu(event_queue, menu_font, menu_type_to_show);
    

        active_menu.process_state_logic(
            current_state,
            jogador_atual,
            character,
            canos,
            display,
            background_items,
            coletaveis,
            previous_time,
            ultimo_spawn_canos,
            lag,
            velocidade_canos,
            multiplicador_espaco_canos,
            intervalo_spawn_canos,
            multiplicador_pontuacao
            );
}
        // --- Seção de Renderização ---
        

        al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
        for (auto i : background_items) {
            i->render_object();
        }
        if(current_state == GameState::PLAYING){
        
        for (auto c : canos) {
            c->render_object();
        }
        for (auto p : coletaveis)
        {
            if(p->get_coletado() == false)
            {
                p->render_object();
            }
        }
        character->render_object();
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

    // Destruindo as IMAGENS (Bitmaps)
    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(jumping_sprite);
    al_destroy_bitmap(ground_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);
    al_destroy_bitmap(icon);

    // Destruindo as FONTES
    al_destroy_font(menu_font);
    al_destroy_font(score_font);

    // Destruindo os SONS
    al_destroy_sample(som_pulo);
    al_destroy_sample(som_gameover);
    al_destroy_sample(music);
    al_destroy_sample_instance(music_instance);
    al_destroy_mixer(mixer);
    al_destroy_voice(voice);
    al_uninstall_audio();


    // Destruindo os recursos principais do Allegro
    al_destroy_display(display); 
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);


    return 0;
}
