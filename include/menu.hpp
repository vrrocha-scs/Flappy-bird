#ifndef MENU
#define MENU

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include<vector>
#include<string>
#include"../include/personagem.hpp"
#include"../include/obstaculo.hpp"
#include"../include/gamestate.hpp"
#include"../include/cadastro.hpp"
#include"../include/leaderboard.hpp"


enum class MenuType {
    START,
    END,
    PAUSE,
    REGISTER
};

enum class MenuResult {
    NO_ACTION,
    START_NEW_GAME,
    RESTART_GAME,
    CADASTRO,
    EXIT_GAME,
    CONTINUE_GAME,
    SHOW_LEADERBOARD
};
std::string get_player_name(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, std::vector<ObjetoRenderizavel*>& background_items);

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;

class Menu {
    private:
    void draw(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos);
    MenuResult handle_input(ALLEGRO_EVENT ev);
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_FONT *menu_font;
    ALLEGRO_SAMPLE* select_sound;
    ALLEGRO_SAMPLE* confirm_sound;
    MenuType menu_type;
    
    std::vector<std::string> m_options;
    int selected_option;
    bool is_active;

    ALLEGRO_COLOR color_text;
    ALLEGRO_COLOR color_background;
    ALLEGRO_COLOR color_selected;
    ALLEGRO_COLOR color_title;

    public:
    Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type);
    ~Menu();
    MenuResult show(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos);
    void process_state_logic(
        GameState& current_state,
        Cadastro*& jogador_atual,
        Personagem*& character,
        std::vector<Obstaculo*>& canos,
        ALLEGRO_DISPLAY* display,
        std::vector<ObjetoRenderizavel*>& background_items,
        double& previous_time,
        double& ultimo_spawn
    );

    
};
#endif