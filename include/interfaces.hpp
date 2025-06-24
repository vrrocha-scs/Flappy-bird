#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <string>

class Interfaces {
public:
    Interfaces(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue);
    Interfaces(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font);

    void mostrarSplash(ALLEGRO_BITMAP* imagem);
   void mostrarGameOver(ALLEGRO_FONT* font, int pontuacao);

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_FONT* menu_font;
};


#endif
