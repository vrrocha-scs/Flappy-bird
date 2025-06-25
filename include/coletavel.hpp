#ifndef COLETAVEL
#define COLETAVEL
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"
#include <vector>

enum class TiposColetaveis {
    INVINCIBLE,
    PLUS_SCORE,
    NONE
};


using namespace std;

class Coletavel : public ObjetoRenderizavel{
    private:
    bool coletado;
    TiposColetaveis tipo;

    public:
    Coletavel(int posX, int posY, ALLEGRO_BITMAP* sprite, int velocidadeX, TiposColetaveis tipo);
    void set_coletado(bool aux);
    bool get_coletado();
    //Retorna qual o tipo de coletável
    TiposColetaveis get_tipo();
    //Move o coletável
    void mover_coletavel();
    //Atualiza a posição
    void on_tick() override;
    //Verifica colisão
    bool checkCollision(Hitbox other_hitbox);
    //Verifica se o coletável está apto a ser deletado
    bool remover_coletavel();
};
//Limpa o vetor que contém os coletáveis, seja porque saíram da tela ou já foram coletados
void limpando_coletaveis(vector<Coletavel*>& coletaveis);
//Decide qual será o coletável a ser spawnado, com a chance de ser Nenhum
TiposColetaveis sortear_powerup();
//Realiza a construção do coletável, dada as opções que temos
void construir_coletavel(vector<Coletavel*>& coletaveis, int posX, int posY, int velocidadeX,
    ALLEGRO_BITMAP* sprite_invincible, ALLEGRO_BITMAP* sprite_plus_score);

#endif