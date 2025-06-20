#ifndef PERSONAGEM
#define PERSONAGEM
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "objetorenderizavel.hpp"

class Personagem : public ObjetoRenderizavel{
    private:
        float velocityX;
        float velocityY;
        float jump_power;
        float gravity;
        int score;
    public:
        //Construtor padrão
        Personagem(float posX, float posY,ALLEGRO_BITMAP* bitmap);
        //Move o personagem de acordo com a velocidadeY
        void move_character();
        //Retorna velocidade horizontal
        float get_velocityX();
        //Retorna velocidade vertical
        float get_velocityY();
        //Retorna pontuação do personagem
        int get_score();
        //Adiciona à pontuação
        void gain_score(int x);
        //Define velocidade horizontal
        void set_velocityX(float x);
        //Define velocidade vertical
        void set_velocityY(float y);
        //Comportamento por tick do personagem
        void on_tick() override;
        //Comando do pulo
        void jump();
        //Checador de colisão
        bool checkCollision(Hitbox other_hitbox);
        //Renderizador especial do personagem
        void render_object() override;
        //reset do personagem
        void reset_position(float start_x, float start_y);
};


#endif