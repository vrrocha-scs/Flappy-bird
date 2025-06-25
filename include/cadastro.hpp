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
  /**
  * @brief  faz a verificação dos dados do player, retornando um ponteiro para um tipo Cadastro se ja houver um jogador cadastrado ou cadastrando e retornando um ponteiro se nao houver cadastro
  */ 
   static Cadastro* verificar_dados(std::string);
  /**
  * @brief  recebe os dados da partida e modifica no arquivo csv
  */ 
   void modificar_dados(int);
  /**
  * @brief  desenha o Leaderboard
  */ 
    void display_estatisticas(ALLEGRO_DISPLAY*, ALLEGRO_FONT*, const char*);

   Cadastro(std::string,int,int,int);
};

#endif