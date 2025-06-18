#include "leaderboard.hpp"
#include <fstream>
#include <list>
#include <iostream>
#include <sstream>

#define TAMANHO 5

 void Leaderboard::cadastro_tabela(std::string nome_jogador,int score_partida){
    std::list<Leaderboard> tabelados;
    std::ifstream arq_tabela_leit("tabela.csv");
    std::string linha;
    bool encontrou=false;
    bool modificado=false;
    if(arq_tabela_leit.is_open()){
        while(std::getline(arq_tabela_leit,linha)){
            std::stringstream ss(linha);
            std::string nome,pontuacao_str;
            if(std::getline(ss,nome,',') && std::getline(ss,pontuacao_str,',') ){
             int pontuacao=std::stoi(pontuacao_str); 
             tabelados.emplace_back(nome, pontuacao);
            }
        }
        for (auto it=tabelados.begin();it!=tabelados.end();it++){
          if(nome_jogador==it->nome_tabelados){
            if(score_partida>it->pontuacao_tabelados){
                it->pontuacao_tabelados=score_partida;
                modificado=true;
            }
            encontrou=true;
            break;
          }   
        }
        if (!encontrou){
          for (auto it=tabelados.begin();it!=tabelados.end();it++){
            if(score_partida>it->pontuacao_tabelados){
               tabelados.insert(it, Leaderboard(nome_jogador,score_partida));
               modificado=true;
               if(tabelados.size()>TAMANHO){
               tabelados.pop_back();
               }
               break;
            }
          }  
          if(!modificado&& tabelados.size()<TAMANHO){
            tabelados.push_back(Leaderboard(nome_jogador,score_partida));
            modificado=true;
          } 
        }   
    }
    else{
        std::cerr<<"erro ao abrir tabela para leitura"<<std::endl;
    }
    arq_tabela_leit.close();

   if(modificado){
     std::ofstream arq_tabela_esc("tabela.csv");
     if(arq_tabela_esc.is_open()){
       int numero_linhas=0;
       for (const auto& it : tabelados){
         arq_tabela_esc<<it.nome_tabelados<<","<<it.pontuacao_tabelados<<std::endl;
         numero_linhas++;
       }
       while(numero_linhas<TAMANHO){
        arq_tabela_esc<<"--,00"<<std::endl;
        numero_linhas++;
       }
       arq_tabela_esc.close();
     }
     else{
        std::cerr<<"erro ao abrir tabela para escrita"<<std::endl;
     }
   }
 }