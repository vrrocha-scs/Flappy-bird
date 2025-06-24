#include "interfaces.hpp"
#include "menu.hpp"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>

Interfaces::Interfaces(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font) {
    this->display = display;
    this->queue = queue;
    this->menu_font = font;
}

void Interfaces::mostrarSplash(ALLEGRO_BITMAP* splash_img) {
    // Primeira "chamada forçada" de render
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(splash_img, 0, 0, 0);
    al_flip_display();
    al_rest(0.05);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(splash_img, 0, 0, 0);
    al_flip_display();

    al_rest(1);

    ALLEGRO_EVENT ev;
    while (true) {
        al_wait_for_event(this->queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
    }
}

void Interfaces::mostrarGameOver(ALLEGRO_FONT* font, int pontuacao) {
    std::string texto = "GAME OVER!";
    std::string score_text = "Pontuação: " + std::to_string(pontuacao);
    std::string dica = "Pressione qualquer tecla para continuar";

    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Fonte maior para o "GAME OVER"
    ALLEGRO_FONT* font_maior = al_load_font("assets/fonts/game_over.ttf", 150, 0);
    if (font_maior) {
        al_draw_text(font_maior, al_map_rgb(255, 50, 50), SCREEN_W / 2, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_CENTER, texto.c_str());
        al_destroy_font(font_maior);
    }

    // Fonte menor para a pontuação
    ALLEGRO_FONT* font_menor = al_load_font("assets/fonts/game_over.ttf", 70, 0);
    if (font_menor) {
        al_draw_text(font_menor, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 4, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, score_text.c_str());
        al_destroy_font(font_menor);
    }

    // Fonte pequena para a dica
    ALLEGRO_FONT* dica_font = al_load_font("assets/fonts/game_over.ttf", 60, 0);
    if (dica_font) {
        al_draw_text(dica_font, al_map_rgb(180, 180, 180), SCREEN_W / 2, SCREEN_H / 2 + 70, ALLEGRO_ALIGN_CENTER, dica.c_str());
        al_destroy_font(dica_font);
    }

    al_flip_display();

    al_rest(1);

    ALLEGRO_EVENT evento;
    while (true) {
        al_wait_for_event(this->queue, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN || evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
    }
}
