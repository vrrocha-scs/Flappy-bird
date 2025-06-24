#ifndef CADASTRO_H
#define CADASTRO_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include<string>

class Cadastro{
  private:
   std::string nome_jogador;
   int high_score;
   int numero_partidas;
   int score_geral;
  public:
   static Cadastro* verificar_dados(std::string);
   void modificar_dados(int);
   void display_estatisticas(ALLEGRO_DISPLAY*, ALLEGRO_FONT*, const char*);

   Cadastro(std::string,int,int,int);
};

#endif