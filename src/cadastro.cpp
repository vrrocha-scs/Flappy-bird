#include "cadastro.hpp"
#include "leaderboard.hpp"
#include "utils.hpp"
#include<fstream>
#include<iostream>
#include<limits>
#include<vector>
#include<sstream>


Cadastro::Cadastro(std::string _nome, int _score, int _numero_partidas):nome_jogador(_nome), high_score(_score), numero_partidas(_numero_partidas) {}
//faz a verificação dos dados do player, retornando um ponteiro para um tipo Cadastro se ja houver um jogador cadastrado ou cadastrando e retornando um ponteiro se 
//nao houver cadastro
Cadastro* Cadastro::verificar_dados(std::string possivel_nome) {
    //chamo a funcao para limpar os caracteres indefinidos da entrada
    std::string nome_limpo_input = trim(possivel_nome);
    //verifica se entra em alguma excessão que resultaria em erro
    if(possivel_nome.find(',') != std::string::npos) {
    throw std::invalid_argument("Não utilize ',' no nome");
    }
    //caso o nome seja vazio retorna nullptr
    if (nome_limpo_input.empty()) {
        return nullptr;
    }
    //verifica se o player existe e retorna um ponteiro com o cadastro
    std::ifstream leitura_arq("assets/images/dados.csv");
    if (leitura_arq.is_open()) {
      std::string linha;
      while (getline(leitura_arq, linha)) {
        std::stringstream ss(linha);
        std::string nome_arquivo, score_str, partidas_str;
        if (std::getline(ss, nome_arquivo, ',') && std::getline(ss, score_str, ',') && std::getline(ss, partidas_str)) {
          //limpo o nome do arquivo para comparacao, para ter certeza que nao haja caracteres invisisveis
          std::string nome_limpo_arquivo = trim(nome_arquivo);
          if (nome_limpo_input == nome_limpo_arquivo) {
            int score = std::stoi(score_str);
            int partidas = std::stoi(partidas_str);
            leitura_arq.close();
            return new Cadastro(nome_limpo_input, score, partidas);
          }
        }
      }
      leitura_arq.close();

      //cadastra o player como novo e retorna um ponteiro para seu cadastro
      std::ofstream escrita_arq("assets/images/dados.csv", std::ios::app);
      if (escrita_arq.is_open()) {
        escrita_arq << nome_limpo_input << ",0,0" << std::endl;
        escrita_arq.close();
      } else {
        std::cout << "erro ao abrir arquivo para escrita";
        return nullptr;
        }
        return new Cadastro(nome_limpo_input, 0, 0);
    }
    else {
        //se o arquivo nao existir cria um novo e cadastra o primeiro jogador
        std::ofstream novo_arquivo("assets/images/dados.csv");
        if (novo_arquivo.is_open()) {
            novo_arquivo << nome_limpo_input << ",0,0" << std::endl;
            novo_arquivo.close();
            return new Cadastro(nome_limpo_input, 0, 0);
        }
        std::cout << "erro ao criar/abrir arquivo" << std::endl;
        return nullptr;
    }
}


//recebe os dados da partida e modifica no arquivo csv
void Cadastro::modificar_dados(int score_partida){
  if(high_score < score_partida){
    high_score = score_partida;
  }
  numero_partidas++;

  std::ifstream arq_armazenagem("assets/images/dados.csv");
  std::vector<Cadastro> cadastros_jogadores;
  std::string linha;

  if(arq_armazenagem.is_open()){
    while(getline(arq_armazenagem,linha)){
      std::stringstream ss(linha);
      std::string nome, score_str,partidas_str;
      if(std::getline(ss, nome, ',') && std::getline(ss, score_str, ',') && std::getline(ss, partidas_str)){
        int score = std::stoi(score_str);
        int partidas = std::stoi(partidas_str);
            
        std::string nome_limpo = trim(nome);

        if(nome_limpo == nome_jogador){
          cadastros_jogadores.push_back(Cadastro(nome_limpo, high_score, numero_partidas));
        } else {
          cadastros_jogadores.push_back(Cadastro(nome_limpo, score, partidas));
          }
      }
    }
    arq_armazenagem.close();
  }
  else{
    std::cerr<<"erro ao abrir arquivo de armazenagem"<<std::endl;
    return;
  }

  std::ofstream arq_escrita("assets/images/dados.csv");
  if(arq_escrita.is_open()){
    for(const auto& j:cadastros_jogadores){
        arq_escrita << j.nome_jogador << "," << j.high_score << "," << j.numero_partidas << std::endl;
    }
    arq_escrita.close();
  }
  else{
    std::cerr<<"erro ao abrir arquivo de escrita na funcao modificar dados"<<std::endl;
  }

  Leaderboard leaderboard;
  leaderboard.cadastro_tabela(nome_jogador,high_score);
}