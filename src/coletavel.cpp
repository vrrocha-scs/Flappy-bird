#include "coletavel.hpp"
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"
#include "gamestate.hpp"

using namespace std;

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;

Coletavel::Coletavel(int posY, ALLEGRO_BITMAP* sprite, int velocidadeX) :
ObjetoRenderizavel(SCREEN_W + 50, posY, sprite, true)
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
bool Coletavel::checkCollision(Hitbox other_hitbox){
    if(get_hitbox().has_collision(other_hitbox)){
        return true;
    }
    return false;
}

//void 