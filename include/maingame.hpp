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

class MainGame{
    private:
        ALLEGRO_DISPLAY* display;
        ALLEGRO_EVENT_QUEUE* event_queue;

        ALLEGRO_BITMAP *character_sprite, *jumping_sprite, *ground_sprite, *upper_pipe_sprite, *lower_pipe_sprite,*hills_sprite,*mountains_sprite;
        ALLEGRO_FONT *menu_font, *score_font;
        ALLEGRO_SAMPLE *som_game_over;
        ALLEGRO_COLOR default_color;

        Personagem* character;
        Cadastro* cadastro;
        Randomizador* randomizer;
        vector<Obstaculo*> canos;
        vector<ObjetoRenderizavel*> background;
        GameState current_state;

        static float SCREEN_W,SCREEN_H;
        double game_speed, ultimo_spawn, tamanho_gap;

        const float JUMP_COOLDOWN_SECONDS = 0.25f;
        const float PIPE_SPAWN_INTERVAL = 6.0f;
        const int MAX_PIPES = 10;
    public:
    MainGame();
    MainGame(float game_speed);
    ~MainGame();
    void init_addons();
    void load_assets();
    void render_screen();
    void create_menu(double& previous_time);

    float get_game_speed();
    GameState get_game_state();
    ALLEGRO_EVENT_QUEUE* get_event_queue();
    Personagem* get_character();
    void set_game_state(GameState next_state);
    void set_game_speed(float x);
    void set_display_color(ALLEGRO_COLOR next_color);

    void adicionar_canos();
    void checkCollision();
    void on_tick(float current_time);
};

#endif