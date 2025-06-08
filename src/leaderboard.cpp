#include "leaderboard.hpp"
#include <fstream>
#include <list>
#include <iostream>

#define TAMANHO 5

 void Leaderboard::cadastro_tabela(std::string nome_jogador,int score_partida){
    std::list<Leaderboard> tabelados;
    std::string aux_nome;
    int aux_pontucao_tabelados;
    bool encontrou=false;

    std::fstream arq_tabela_leit("tabela.txt");
    if(arq_tabela_leit.is_open()){
        while (arq_tabela_leit>>aux_nome>>aux_pontucao_tabelados)
        {
            tabelados.emplace_back(aux_nome,aux_pontucao_tabelados);
        }
        for (auto it=tabelados.begin();it!=tabelados.end();it++){
          if(nome_jogador==it->nome_tabelados){
            if(score_partida>it->pontuacao_tabelados){
                it->pontuacao_tabelados=score_partida;
                break;
            }
            encontrou=true;
            break;
          }   
        }
        if (!encontrou){
          for (auto it=tabelados.begin();it!=tabelados.end();it++){
            if(score_partida>it->pontuacao_tabelados){
               tabelados.insert(it, Leaderboard(nome_jogador,score_partida));
               if(tabelados.size()>TAMANHO){
               tabelados.pop_back();
               }
               break;
            }
          }   
        }   
    }
    else{
        std::cout<<"erro ao abrir tabela para leitura";
    }
    arq_tabela_leit.close();
   
    std::ofstream arq_tabela_esc("tabela.txt");
    if(arq_tabela_esc.is_open()){
      for (const auto& it : tabelados){
        arq_tabela_esc<<it.nome_tabelados<<" "<<it.pontuacao_tabelados<<std::endl;
      }
    }
    else{
        std::cout<<"erro ao abrir tabela para escrita";
    }
    arq_tabela_esc.close();
 }