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

void Interfaces::mostrarSplash(ALLEGRO_BITMAP* splash) {
    if (!splash) return;

    al_draw_bitmap(splash, 0, 0, 0);
    al_flip_display();

    ALLEGRO_EVENT ev;
    while (true) {
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN ||
            ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ||
            ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }
}

void Interfaces::mostrarGameOver(int pontuacao) {
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);

    std::string texto = "Game Over! Pontuacao: " + std::to_string(pontuacao);

    al_draw_text(menu_font, al_map_rgb(255, 0, 0), largura / 2, altura / 2 - 40, ALLEGRO_ALIGN_CENTER, texto.c_str());

    al_flip_display();
    al_rest(2.5);  // Mostra por 2.5 segundos antes de continuar
}