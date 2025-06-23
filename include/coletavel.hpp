#ifndef COLETAVEL
#define COLETAVEL

#include "objetorenderizavel.hpp"
#include "hitbox.hpp"

using namespace std;

class Coletavel : public ObjetoRenderizavel{
    private:
    bool coletado;

    public:
    Coletavel(int posX, int posY, ALLEGRO_BITMAP* sprite, int velocidadeX);
    void set_coletado(bool aux);
    bool get_coletado();
    //Move o coletável
    void mover_coletavel();
    //Atualiza a posição
    void on_tick() override;

};


#endif