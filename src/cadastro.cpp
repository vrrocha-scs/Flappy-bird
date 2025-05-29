#include <allegro5/allegro.h>
#include "cadastro.hpp"
#include<fstream>
#include<iostream>

cadastro::cadastro(std::string _nome,int _vitorias,int _derrotas, int _score):nome_jogador(_nome),vitorias(_vitorias),derrotas(_derrotas),high_score(_score){}

cadastro* cadastro::verificar_dados(std::string possivel_nome){
    std::string nome_dados;int aux_vitorias=0,aux_derrotas=0,aux_score=0;
    std::ifstream file("dados.txt");
    if(file.is_open()){
     while(file>>nome_dados){
      if(possivel_nome==nome_dados){
       file>>aux_vitorias>>aux_derrotas>>aux_score;
       cadastro* novo_player = new cadastro(nome_dados, aux_vitorias, aux_derrotas, aux_score);
       return novo_player;
      }
      cadastro* novo_player = new cadastro(nome_dados, aux_vitorias, aux_derrotas, aux_score);
      return novo_player;
    }
    }
    }

    

