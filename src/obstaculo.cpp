#include "obstaculo.hpp"
#include "objetorenderizavel.hpp"
#include "randomizador.hpp"
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const int SCREEN_W = 1000;
const int SCREEN_H = 1000;

Obstaculo::Obstaculo(float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura) : 
ObjetoRenderizavel(SCREEN_W + 50, posY, bitmap,1), _larguraObs(al_get_bitmap_width(bitmap)), _alturaObs(altura)
{
    passou_personagem = false;
    set_velocityX(velocidade);
}
//Construtor de canos
Obstaculo::Obstaculo(float posX, float posY, ALLEGRO_BITMAP* bitmap, float velocidade, float altura) : 
ObjetoRenderizavel(posX, posY, bitmap,1), _larguraObs(al_get_bitmap_width(bitmap)), _alturaObs(altura)
{
    set_velocityX(velocidade);
    passou_personagem = false;

}
//Construtor de outros obstáculos


    float Obstaculo::get_larguraObs()
    {
        return this->_larguraObs;
    }
    void Obstaculo::set_larguraObs(float a)
    {
        this->_larguraObs = a;
        return;
    }

    float Obstaculo::get_alturaObs()
    {
        return this->_alturaObs;
    }
    void Obstaculo::set_alturaObs(float a)
    {
        this->_alturaObs = a;
        return;
    }
    bool Obstaculo::get_passou()
    {
        return passou_personagem;
    }
    void Obstaculo::set_passou(bool aux)
    {
        passou_personagem = aux;
        return;
    }

    void Obstaculo::mover_obstaculos()
    {
        this->set_posX(this->get_posX() - this->get_velocityX());
        return;
    }
    void Obstaculo::on_tick()
    {
        this->mover_obstaculos();
        get_hitbox().on_tick();
    }
    void Obstaculo::check_passagem(Personagem* personagem, int multiplicador_pontuacao)
    {
        if ((personagem->get_posX() > ((this->get_hitbox()).get_posX() + this->get_larguraObs()) && !this->get_passou()))
        {
            this->set_passou(true);
            personagem->gain_score(multiplicador_pontuacao);
        }
        return;
    }
    void limpando_obstaculos(vector<Obstaculo*>& canos)
    {
        for (vector<Obstaculo*>::iterator it = canos.begin(); it != canos.end();)
            {
                if ((*it)->get_posX() < -100)
                {
                    delete *it;
                    it = canos.erase(it);
                } else it++;
            }      
    }
    void adicionando_canos(vector<Obstaculo*>& canos, int altura_buraco, int tamanho_gap,
         ALLEGRO_BITMAP* upper_pipe_sprite, ALLEGRO_BITMAP* lower_pipe_sprite, int velocidade_canos)
    {
        canos.push_back(new Obstaculo(altura_buraco - (al_get_bitmap_height(upper_pipe_sprite)), upper_pipe_sprite, velocidade_canos, altura_buraco));
        canos.push_back(new Obstaculo(altura_buraco + tamanho_gap, lower_pipe_sprite, velocidade_canos, (al_get_bitmap_height(lower_pipe_sprite) - (altura_buraco))));  
        return;
    }
    int definir_tamanho_gap(int multiplicador, ALLEGRO_BITMAP* sprite_personagem)
    {
        return multiplicador * al_get_bitmap_height(sprite_personagem);
    }

