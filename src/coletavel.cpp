#include "coletavel.hpp"
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"
#include "gamestate.hpp"
#include "randomizador.hpp"

using namespace std;

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const float DISTANCIA_ENTRE_CANOS = 450;

Coletavel::Coletavel(int posX, int posY, ALLEGRO_BITMAP* sprite, int velocidadeX, TiposColetaveis tipo, float referencia) :
ObjetoRenderizavel(SCREEN_W + 50 + referencia/2, posY, sprite, true)
{
    this->tipo = tipo;
    this->set_velocityX(velocidadeX);
    coletado = false;
}

void Coletavel::set_coletado(bool aux)
{
    this->coletado = aux;
}
bool Coletavel::get_coletado()
{
    return this->coletado;
}

TiposColetaveis Coletavel::get_tipo()
{
    return this->tipo;
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

bool Coletavel::remover_coletavel()
{
    if (this->get_posX() < -al_get_bitmap_width(this->get_bitmap()))
    {
        delete this;
        return true;
    }
    else    
        return false;
}
void limpando_coletaveis(vector<Coletavel*>& coletaveis)
{
    for (vector<Coletavel*>::iterator it = coletaveis.begin(); it != coletaveis.end(); it++)
    {
        if ((*it)->remover_coletavel())
        {
            coletaveis.erase(it);
        }
    }      
}

//void 