#include "cadastro.hpp"
#include<fstream>
#include<iostream>
#include<limits>
#include<vector>

Cadastro::Cadastro(std::string _nome,int _score,int _derrotas, int _vitorias):nome_jogador(_nome),vitoria(_vitorias),derrota(_derrotas),high_score(_score){}

Cadastro* Cadastro::verificar_dados(std::string possivel_nome){
    std::string nome_dados;
    int aux_vitorias=0,aux_derrotas=0,aux_score=0;
    std::ifstream leitura_arq("dados.txt");
    if(leitura_arq.is_open()){
     while(leitura_arq>>nome_dados){
      if(possivel_nome==nome_dados){
       leitura_arq>>aux_score>>aux_derrotas>>aux_vitorias;
       Cadastro* novo_player = new Cadastro(nome_dados, aux_score, aux_derrotas, aux_vitorias);
       return novo_player;
      }
      else 
      leitura_arq.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    Cadastro* novo_player = new Cadastro(possivel_nome, aux_score, aux_derrotas, aux_vitorias);
    leitura_arq.close();

    std::ofstream escrita_arq("dados.txt", std::ios::app);
    if(escrita_arq.is_open()){
    escrita_arq<<possivel_nome<<" 0 0 0"/*devo mudar para outro arquivo o high score?*/<<std::endl;
    escrita_arq.close();
    }
    else{
      std::cout<<"erro ao abrir arquivo para escrita";
      return nullptr;
    }
    return novo_player;
    }
    else{
      std::cout<<"erro ao abrir arquivo"<<std::endl;
    }
}

void Cadastro::modificar_dados(int score_partida,bool resultado){
  if(high_score<score_partida){
    high_score=score_partida;
  }
  if(resultado){
    vitoria++;
  }
  else{
  derrota++;
  }

  std::ifstream arq_armazenagem("dados.txt");
  std::vector<Cadastro>cadastros_jogadores;
  std::string nome;
  int aux_vit,aux_der,aux_score;
  bool encontrado=false;
  if(arq_armazenagem.is_open()){
  while(arq_armazenagem>>nome>>aux_score>>aux_der>>aux_vit){
    if(nome!=nome_jogador){
      cadastros_jogadores.push_back(Cadastro(nome,aux_score,aux_der,aux_vit));
    }
    else
    cadastros_jogadores.push_back(Cadastro(nome_jogador,high_score,derrota,vitoria));
    encontrado=true;
  }
  arq_armazenagem.close();
  }
  else{
    std::cout<<"erro ao abrir arquivo de armazenagem"<<std::endl;
    return;
  }

  std::ofstream arq_escrita("dados.txt");
  if(arq_escrita.is_open()){
  for(const auto& j:cadastros_jogadores){
    arq_escrita<<j.nome_jogador<<" "<<j.high_score<<" "<<j.derrota<<" "<<j.vitoria<<std::endl;
  }
  arq_escrita.close();
  }
  else{
    std::cout<<"erro ao abrir arquivo de escrita na funcao modificar dados"<<std::endl;
  }

}
