#include "cadastro.hpp"
#include<fstream>
#include<iostream>
#include<limits>
#include<vector>
#include<sstream>

Cadastro::Cadastro(std::string _nome,int _score,int _numero_partidas):nome_jogador(_nome),high_score(_score),numero_partidas(_numero_partidas){}

Cadastro* Cadastro::verificar_dados(std::string possivel_nome){
   std::ifstream leitura_arq("dados.csv");
    if(leitura_arq.is_open()){
      std::string linha;
     while(getline(leitura_arq,linha)){
      std::stringstream ss(linha);
      std::string nome, score_str, partidas_str;
      if(std::getline(ss, nome, ',') && std::getline(ss, score_str, ',') && std::getline(ss, partidas_str)){
      int score=std::stoi(score_str);
      int partidas=std::stoi(partidas_str);
      if(possivel_nome==nome){
        return new Cadastro(nome,score,partidas);
      }
    }
    }
    leitura_arq.close();

    std::ofstream escrita_arq("dados.csv", std::ios::app);
    if(escrita_arq.is_open()){
    escrita_arq<<possivel_nome<<",0,0"<<std::endl;
    escrita_arq.close();
    }
    else{
      std::cout<<"erro ao abrir arquivo para escrita";
      return nullptr;
    }
    return new Cadastro(possivel_nome, 0, 0);
    }
    else{
      std::cout<<"erro ao abrir arquivo"<<std::endl;
      return nullptr;
    }
}


void Cadastro::modificar_dados(int score_partida){
  if(high_score<score_partida){
    high_score=score_partida;
  }
  numero_partidas++;

  std::ifstream arq_armazenagem("dados.csv");
  std::vector<Cadastro>cadastros_jogadores;
  std::string linha;
  bool encontrado=false;

  if(arq_armazenagem.is_open()){
  while(getline(arq_armazenagem,linha)){
    std::stringstream ss(linha);
    std::string nome, score_str,partidas_str;
    if(std::getline(ss, nome, ',') && std::getline(ss, score_str, ',') && std::getline(ss, partidas_str)){
      int score=std::stoi(score_str);
      int partidas=std::stoi(partidas_str);
      if(nome==nome_jogador){
      cadastros_jogadores.push_back(Cadastro(nome,high_score,numero_partidas));
      encontrado=true;
      }
    else{
    cadastros_jogadores.push_back(Cadastro(nome_jogador,high_score,numero_partidas));
    }
  }
}
  arq_armazenagem.close();
  }
  else{
    std::cerr<<"erro ao abrir arquivo de armazenagem"<<std::endl;
    return;
  }

  std::ofstream arq_escrita("dados.csv");
  if(arq_escrita.is_open()){
  for(const auto& j:cadastros_jogadores){
    arq_escrita<<j.nome_jogador<<","<<j.high_score<<","<<j.numero_partidas<<std::endl;
  }
  arq_escrita.close();
  }
  else{
    std::cerr<<"erro ao abrir arquivo de escrita na funcao modificar dados"<<std::endl;
  }

}
