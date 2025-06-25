#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <string>

/**
 * Classe que mostra as telas do jogo (splash, game over, tutorial).
 */
class Interfaces {
public:
    /**
     * Construtor que recebe a fila de eventos e a fonte.
     */
    Interfaces(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font);

    /**
     * Construtor que recebe a tela, a fila de eventos e a fonte.
     */
    Interfaces(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font);

    /**
     * Mostra a tela de abertura com uma imagem.
     */
    void mostrarSplash(ALLEGRO_BITMAP* imagem);

    /**
     * Mostra a tela de game over com a pontuação.
     */
    void mostrarGameOver(ALLEGRO_FONT* font, int pontuacao);

    /**
     * Mostra o tutorial com imagens trocando ao apertar tecla.
     */
    void mostrarTutorial(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue);

private:
    ALLEGRO_DISPLAY* display;      ///< Tela principal do jogo
    ALLEGRO_EVENT_QUEUE* queue;    ///< Fila de eventos
    ALLEGRO_FONT* menu_font;       ///< Fonte usada nos menus
};

#endif
