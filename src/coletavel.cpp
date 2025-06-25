#include "coletavel.hpp"
#include "objetorenderizavel.hpp"
#include "hitbox.hpp"
#include "gamestate.hpp"
#include "randomizador.hpp"
#include <stdlib.h>

using namespace std;

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const float DISTANCIA_ENTRE_CANOS = 450;

//Probabilidades de vir o PowerUp
int chance_invincible = 10; //Isso significa que existe 10% de vir (invencibilidade)
int chance_plus_score = 40 + chance_invincible; // Isso vai ser o range entre 11 - 50, ou seja, 40% de chance de vir (plus_score)
// O resto, vai ser nulo, ou futuros power_ups

Coletavel::Coletavel(int posX, int posY, ALLEGRO_BITMAP* sprite, int velocidadeX, TiposColetaveis tipo) :
ObjetoRenderizavel(posX+50, posY, sprite, true)
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

void limpando_coletaveis(vector<Coletavel*>& coletaveis)
{
    for (vector<Coletavel*>::iterator it = coletaveis.begin(); it != coletaveis.end();)
    {
        if ((*it)->get_posX() < -100)
        {
            delete *it;
            it = coletaveis.erase(it);
        }else it++;
    }      
}
TiposColetaveis sortear_powerup()
{
    int chance = rand() % 100 + 1;

    if (chance <= chance_invincible)
    {
        return TiposColetaveis::INVINCIBLE;
    }
    else if (chance <= chance_plus_score)
    {
        return TiposColetaveis::PLUS_SCORE;
    }
    else
        return TiposColetaveis::NONE;
}

void construir_coletavel(vector<Coletavel*>& coletaveis, int posX, int posY, int velocidadeX,
    ALLEGRO_BITMAP* sprite_invincible, ALLEGRO_BITMAP* sprite_plus_score)
{
    TiposColetaveis tipo_power_up = sortear_powerup();
    if (tipo_power_up == TiposColetaveis::INVINCIBLE)
    {
        coletaveis.push_back(new Coletavel(posX, posY, sprite_invincible, velocidadeX, TiposColetaveis::INVINCIBLE));
        return;
    }
    else if (tipo_power_up == TiposColetaveis::PLUS_SCORE)
    {
        coletaveis.push_back(new Coletavel(posX, posY, sprite_plus_score, velocidadeX, TiposColetaveis::PLUS_SCORE));
        return;
    }
    else return;
}

//void 