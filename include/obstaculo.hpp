#ifndef OBSTACULO
#define OBSTACULO

#include <allegro5/allegro.h>
#include <vector>
#include "../include/objetorenderizavel.hpp"
#include "hitbox.hpp"
#include "personagem.hpp"
#include "randomizador.hpp"


class Obstaculo  : public ObjetoRenderizavel{
    private:
    float _larguraObs, _alturaObs;
    bool passou_personagem;

    public:
    Obstaculo(float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura);
    Obstaculo(float posX, float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura);


    float get_larguraObs();
    void set_larguraObs(float);
    float get_alturaObs();
    void set_alturaObs(float);
    bool get_passou();
    void set_passou(bool);
    /**
     * @brief Move o obstáculo no eixo X
     */
    void mover_obstaculos();
    /**
     * @brief A cada tick, atualiza a posição
     */
    void on_tick() override;
    /**
     * @brief Checka se o passaro passou pelo cano
     */
    void check_passagem(Personagem* personagem,int multiplicador_pontuacao);
};

/**
 * @brief Adiciona um par de canos ao vetor
 */
void adicionando_canos(vector<Obstaculo*>& canos, int altura_buraco, int tamanho_gap, ALLEGRO_BITMAP* sprite_cano_cima,
     ALLEGRO_BITMAP* sprite_cano_baixo, int velocidade);
/**
 * @brief Limpa os canos que estão fora da tela
 */
void limpando_obstaculos(vector<Obstaculo*>& canos);
/**
 * @brief Define até onde chega o sprite/hitbox do cano superior
 */
int definir_tamanho_gap(int multiplicador, ALLEGRO_BITMAP* sprite_personagem);

#endif