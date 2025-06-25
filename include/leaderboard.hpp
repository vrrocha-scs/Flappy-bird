#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<string>

class Leaderboard{
    public:
    /**
    * @brief  recebe os dados da partida e verifica se ela entrara na leaderboard
    */ 
    void cadastro_tabela(std::string,int);
    /**
    * @brief  desenha o Leaderboard
    */ 
    void display_tabela(ALLEGRO_DISPLAY*, ALLEGRO_FONT*, const char*);//a fazer
};
#endif