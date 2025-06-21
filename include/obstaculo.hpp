#ifndef OBSTACULO
#define OBSTACULO

#include <allegro5/allegro.h>
#include <vector>
#include "../include/objetorenderizavel.hpp"
#include "hitbox.hpp"
#include "randomizador.hpp"

class Obstaculo  : public ObjetoRenderizavel{
    private:
    float _larguraObs, _alturaObs;

    public:
    Obstaculo(float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura);
    //Construtor de canos
    Obstaculo(float posX, float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura);
    //Construtor de outros obstáculos


    float get_larguraObs();
    void set_larguraObs(float);
    float get_alturaObs();
    void set_alturaObs(float);
    void mover_obstaculos();
    //Move o obstáculo no eixo X
    void desenhar_canos();
    //Renderiza os canos e(ou) desenha os hitbox
    void on_tick() override;
    //Atualiza a posição
    bool remover_obstaculo();
    //Remove obstáculos
};


void adicionando_canos(vector<Obstaculo*>& canos, int altura_buraco, int tamanho_gap, ALLEGRO_BITMAP* sprite_cano_cima, ALLEGRO_BITMAP* sprite_cano_baixo, int velocidade);
//Adiciona um par de canos ao vetor
void limpando_obstaculos(vector<Obstaculo*>& canos);
//Limpa os canos que estão fora da tela
int definir_altura_superior(Randomizador* rando);
//Define até onde chega o sprite/hitbox do cano superior
int definir_tamanho_gap(int multiplicador, ALLEGRO_BITMAP* sprite_personagem);
//Define o tamanho do espaço entre os canos

#endif