#ifndef CADASTRO_H
#define CADASTRO_H

#include <allegro5/allegro.h>
#include<string>

class Cadastro{
  private:
   std::string nome_jogador;
   int vitorias;
   int derrotas;
   int high_score;
  public:
   Cadastro* verificar_dados(std::string);
   void modificar_dados(){};

   Cadastro(std::string,int,int,int);
};

#endif