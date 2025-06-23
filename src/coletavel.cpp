#include "coletavel.hpp"
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"
#include "gamestate.hpp"

using namespace std;

Coletavel::Coletavel(int posX, int posY, ALLEGRO_BITMAP* sprite, int velocidadeX) :
ObjetoRenderizavel(posX, posY, sprite,1)
{
    this->set_velocityX(velocidadeX);
    coletado = false;
}

void Coletavel::set_coletado(bool aux)
{
    coletado = aux;
}
bool Coletavel::get_coletado()
{
    return this->coletado;
}

void Coletavel::mover_coletavel()
{
    this->set_posX(this->get_posX() - this->get_velocityX());
    return;
}
void Coletavel::on_tick()
{
    this->mover_coletavel();
    get_hitbox().on_tick();
}

//void 