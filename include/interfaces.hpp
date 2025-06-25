#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <string>

/**
 * @brief Classe responsável por mostrar telas do jogo,
 * como splash screen, game over e tutorial.
 */
class Interfaces {
public:
    /**
     * @brief Construtor que recebe apenas a fila de eventos e a fonte.
     * @param queue Ponteiro para a fila de eventos do Allegro.
     * @param font Ponteiro para a fonte usada nos menus.
     */
    Interfaces(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font);

    /**
     * @brief Construtor que recebe a tela, fila de eventos e a fonte.
     * @param display Ponteiro para a janela principal do jogo.
     * @param queue Ponteiro para a fila de eventos do Allegro.
     * @param font Ponteiro para a fonte usada nos menus.
     */
    Interfaces(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font);

    /**
     * @brief Mostra a tela de abertura com uma imagem (splash screen).
     * @param imagem Ponteiro para a imagem que será exibida.
     */
    void mostrarSplash(ALLEGRO_BITMAP* imagem);

    /**
     * @brief Mostra a tela de Game Over com a pontuação do jogador.
     * @param font Fonte usada para exibir o texto.
     * @param pontuacao Pontuação final do jogador.
     */
    void mostrarGameOver(ALLEGRO_FONT* font, int pontuacao);

    /**
     * @brief Mostra o tutorial com imagens, avançando ao pressionar uma tecla.
     * @param display Ponteiro para a janela principal do jogo.
     * @param queue Ponteiro para a fila de eventos.
     */
    void mostrarTutorial(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* queue);

private:
    ALLEGRO_DISPLAY* display;      ///< Ponteiro para a janela principal do jogo.
    ALLEGRO_EVENT_QUEUE* queue;    ///< Ponteiro para a fila de eventos.
    ALLEGRO_FONT* menu_font;       ///< Ponteiro para a fonte usada nas interfaces.
};

#endif
