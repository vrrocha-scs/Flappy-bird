#ifndef CADASTRO_H
#define CADASTRO_H

#include<string>

class Cadastro{
  private:
   std::string nome_jogador;
   int high_score;
   int numero_partidas;
  public:
   static Cadastro* verificar_dados(std::string);
   void modificar_dados(int);

   Cadastro(std::string,int,int);
};

#endif