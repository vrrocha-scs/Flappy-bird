#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<string>

class Leaderboard{
    public:
    void cadastro_tabela(std::string,int);
    void display_tabela(ALLEGRO_DISPLAY*, ALLEGRO_FONT*, const char*);//a fazer
};
#endif