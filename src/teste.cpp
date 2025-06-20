#include "cadastro.hpp"
#include "leaderboard.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main(){
    Cadastro* player = Cadastro::verificar_dados("vinicius");
    if (!player) {
        std::cerr << "Erro: player não encontrado!\n";
        return -1;
    }
    player->modificar_dados(211);

    Leaderboard leaderboard;
    if (!al_init()) {
        std::cerr << "Erro ao inicializar Allegro!\n";
        return -1;
    }
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* display = al_create_display(700, 800);
    if (!display) {
        std::cerr << "Erro ao criar display!\n";
        return -1;
    }

    //ALLEGRO_FONT* font = al_load_ttf_font("fonts/game_over.ttf", 32, 0);
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        std::cerr << "Failed to load font.\n";
        al_destroy_display(display);
        return -1;
    }
    leaderboard.display_tabela(display, font, "Melhores Colocados");

    al_destroy_font(font);
    al_destroy_display(display);
    return 0;
}