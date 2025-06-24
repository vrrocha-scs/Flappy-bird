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
#include "../include/maingame.hpp"


int main() {
    MainGame main_game;

    // Loop Principal do Jogo
    while (main_game.get_game_state() != GameState::EXITING) {
        // --- Seção de Controle de Tempo ---//
        main_game.vsync();

        // --- Seção de Processamento de Input Não Bloqueante ---
        main_game.process_events();

        // --- Seção de Lógica do Jogo ---
        main_game.checkCollision();
        main_game.on_tick();
  
        main_game.create_menu();

        // --- Seção de Renderização ---
        main_game.render_screen();
    }

    return 0;
}
