#ifndef CADASTRO_H
#define CADASTRO_H

#include<string>

class Cadastro{
  private:
   std::string nome_jogador;
   int numero_partidas;
   int high_score;
  public:
   Cadastro* verificar_dados(std::string);
   void modificar_dados(int);

   Cadastro(std::string,int,int);
};

#endif