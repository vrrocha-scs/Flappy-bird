#ifndef MENU
#define MENU

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include<vector>
#include<string>
#include"../include/coletavel.hpp"
#include"../include/personagem.hpp"
#include"../include/obstaculo.hpp"
#include"../include/gamestate.hpp"
#include"../include/cadastro.hpp"
#include"../include/leaderboard.hpp"

// Clase para dizer quais os tipos de menus existentes
enum class MenuType {
    LOGIN,
    MAIN_MENU,
    START,
    DIFFICULTY,
    END,
    PAUSE,
    REGISTER
};
// Classe para listar os resultados ao selecionar uma opcao no menu
enum class MenuResult {
    NO_ACTION,
    START_NEW_GAME,
    RESTART_GAME,
    CADASTRO,
    EXIT_GAME,
    CONTINUE_GAME,
    SHOW_LEADERBOARD,
    SHOW_STATISTICS,
    OPEN_DIFFICULTY_MENU,
    SET_DIFFICULTY_EASY,
    SET_DIFFICULTY_MEDIUM,
    SET_DIFFICULTY_HARD
};
// Pega o nome do jogador da entrada da caixa de texto e envia para a funcao cadastro para saber se a pessoa ja existe ou nao
std::string get_player_name(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, std::vector<ObjetoRenderizavel*>& background_items);

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;

class Menu {
    private:
    // Desenha o menu
    void draw(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis);
    // Lida com a entrada de teclas
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
    //Construtor do Menu usando a queue principal
    Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type);
    ~Menu();
    // Funcao para mostrar o menu e esperar um evento
    MenuResult show(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis);
    // Abre o Menu de acordo com o estado e da return de acordo com a selecao da opcao
    void process_state_logic(
        GameState& current_state,
        Cadastro*& jogador_atual,
        Personagem*& character,
        std::vector<Obstaculo*>& canos,
        ALLEGRO_DISPLAY* display,
        std::vector<ObjetoRenderizavel*>& background_items,
        std::vector<Coletavel*>& coletaveis,
        double& previous_time,
        double& ultimo_spawn,
        double& lag,
        float& velocidade_canos,
        int& multiplicador_espaco_canos,
        float& intervalo_spawn_canos
    );

    
};
#endif