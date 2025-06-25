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

// Define os diferentes tipos de menus que existem no jogo.
enum class MenuType {
    LOGIN,
    MAIN_MENU,
    START,
    DIFFICULTY,
    END,
    PAUSE,
    REGISTER
};
// Define os possíveis resultados de uma interação com o menu, representando a escolha do jogador.
enum class MenuResult {
    NO_ACTION,
    START_NEW_GAME,
    PROCESS_LOGIN,
    RESTART_GAME,
    CADASTRO,
    EXIT_GAME,
    CONTINUE_GAME,
    TUTORIAL,
    SHOW_LEADERBOARD,
    SHOW_STATISTICS,
    OPEN_DIFFICULTY_MENU,
    SET_DIFFICULTY_EASY,
    SET_DIFFICULTY_MEDIUM,
    SET_DIFFICULTY_HARD,
    RETURN_TO_MAIN_MENU
};
// Exibe uma tela bloqueante para que o jogador digite seu nome.
std::string get_player_name(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, std::vector<ObjetoRenderizavel*>& background_items);

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;

// Gerencia a exibição e interação com as diferentes telas de menu do jogo.
class Menu {
    private:
    // Desenha o quadro atual do menu na tela.
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
    // Construtor do Menu usando a queue principal
    Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type);
    ~Menu();
    // Mostra o menu na tela e entra em um loop bloqueante até que o jogador faça uma escolha.
    MenuResult show(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis);
    // Orquestra a lógica de um estado de menu, desde mostrar até processar o resultado.
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
        float& intervalo_spawn_canos,
        int& multiplicador_pontuacao
    );

    
};
#endif