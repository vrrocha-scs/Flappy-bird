#ifndef PERSONAGEM
#define PERSONAGEM
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include "objetorenderizavel.hpp"

class Personagem : public ObjetoRenderizavel{
    private:
        float jump_power;
        float gravity;
        int score;
        ALLEGRO_BITMAP* idle_sprite;
        ALLEGRO_BITMAP* jumping_sprite;
        bool invincible;
    public:
        /**
         * @brief  Construtor padrão
         */
        Personagem(float posX, float posY,ALLEGRO_BITMAP* bitmap1,ALLEGRO_BITMAP* bitmap2);
        /**
         * @brief  Move o personagem de acordo com a velocidadeY
         */
        void move_character();
        /**
         * @brief  Retorna pontuação do personagem
         */ 
        int get_score();
        /**
         * @brief  Adiciona à pontuação o valor x
         */ 
        void gain_score(int x);
        /**
         * @brief  Comportamento por tick do personagem
         */ 
        void on_tick() override;
        /**
         * @brief  Comando do pulo
         */       
        void jump();
        /**
         * @brief  Checa se houve colisão com hitbox passada
         */       
        bool checkCollision(Hitbox other_hitbox);
        /**
         * @brief  Renderizador especial do personagem
         */        
        void render_object() override;
        /**
         * @brief  reset do personagem
         */        
        void reset_position(float start_x, float start_y);
        /**
         * @brief  Retorna se o personavel está invencível
         */        
        bool get_invincible();
        /**
         * @brief  Determina se o personagem está ou não invencível
         */
        void set_invincible(bool aux);
};


#endif