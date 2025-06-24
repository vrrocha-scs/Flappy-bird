#ifndef MAIN_GAME
#define MAIN_GAME

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>

#include "personagem.hpp"
#include "obstaculo.hpp"
#include "gamestate.hpp"
#include "cadastro.hpp"
#include "menu.hpp"
#include "coletavel.hpp"
#include "interfaces.hpp"

class MainGame{
    private:
        ALLEGRO_DISPLAY* display;
        ALLEGRO_EVENT_QUEUE* event_queue;
        ALLEGRO_TIMER* timer;

        ALLEGRO_BITMAP *character_sprite, *jumping_sprite, *ground_sprite, *upper_pipe_sprite, *lower_pipe_sprite,*galaxia_sprite,*estrelas1_sprite,*estrelas2_sprite,*splash_img,*green_ball_sprite;
        ALLEGRO_FONT *menu_font, *score_font;
        ALLEGRO_SAMPLE *som_gameover,*som_pulo,*music;
        ALLEGRO_COLOR default_color;
        ALLEGRO_VOICE* voice;
        ALLEGRO_MIXER* mixer;
        ALLEGRO_SAMPLE_INSTANCE* music_instance;

        Personagem* character;
        Cadastro* cadastro;
        Randomizador* randomizer;
        Interfaces* interfaces;

        vector<Obstaculo*> canos;
        vector<Coletavel*> coletaveis;
        vector<ObjetoRenderizavel*> background;

        GameState current_state;

        static float SCREEN_W,SCREEN_H;
        double game_speed,multiplicador_espaco_canos, ultimo_spawn_canos,ultimo_spawn_coletavel, tamanho_gap,volume_musica;
          
        double current_time,elapsed_time,previous_time,lag,jump_cooldown,efeito_invencivel_timer;

        const float FPS = 60;
        const float SECONDS_PER_UPDATE = 1.0f / FPS;
        const float JUMP_COOLDOWN_SECONDS = 0.25f;
        const float PIPE_SPAWN_INTERVAL = 6.0f;
        const float intervalo_spawn_canos = 5.0;
        const float intervalo_spawn_coletavel = 10;
        const int MAX_PIPES = 10;

    public:
    MainGame();
    MainGame(float game_speed);
    ~MainGame();
    void init_addons();
    int load_assets();
    
    void vsync();
    void process_events();
    void process_state_logic(Menu& active_menu);
    void render_screen();
    void create_menu();
    void restart_game();

    float get_game_speed();
    GameState get_game_state();
    ALLEGRO_EVENT_QUEUE* get_event_queue();
    Personagem* get_character();
    
    void set_game_state(GameState next_state);
    void set_game_speed(float x);
    void set_display_color(ALLEGRO_COLOR next_color);
    void set_game_volume(double x);

    void adicionar_canos(float altura_buraco);
    void checkCollision();
    void on_tick();
};

#endif