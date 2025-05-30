#ifndef CADASTRO_H
#define CADASTRO_H

#include<string>

class cadastro{
  private:
   std::string nome_jogador;
   int vitoria;
   int derrota;
   int high_score;
  public:
   cadastro* verificar_dados(std::string);
   void modificar_dados(int,bool);

   cadastro(std::string,int,int,int);
};

#endif