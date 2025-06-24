#ifndef COLETAVEL
#define COLETAVEL
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"
#include <vector>

enum class TiposColetaveis {
    INVINCIBLE,
    PLUS_SCORE
};


using namespace std;

class Coletavel : public ObjetoRenderizavel{
    private:
    bool coletado;
    TiposColetaveis tipo;

    public:
    Coletavel(int posX, int posY, ALLEGRO_BITMAP* sprite, int velocidadeX, TiposColetaveis tipo, float referencia);
    void set_coletado(bool aux);
    bool get_coletado();
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

void limpando_coletaveis(vector<Coletavel*>& coletaveis);

#endif