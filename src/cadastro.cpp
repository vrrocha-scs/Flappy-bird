#include "cadastro.hpp"
#include "leaderboard.hpp"
#include "utils.hpp"
#include<fstream>
#include<iostream>
#include<limits>
#include<vector>
#include<sstream>
#include <iomanip>
#include <allegro5/allegro_primitives.h>


Cadastro::Cadastro(std::string _nome, int _score, int _numero_partidas,int score_total):nome_jogador(_nome), high_score(_score), numero_partidas(_numero_partidas),score_geral(score_total) {}
//faz a verificação dos dados do player, retornando um ponteiro para um tipo Cadastro se ja houver um jogador cadastrado ou cadastrando e retornando um ponteiro se 
//nao houver cadastro
Cadastro* Cadastro::verificar_dados(std::string possivel_nome) {
    //chamo a funcao para limpar os caracteres indefinidos da entrada
    std::string nome_limpo_input = trim(possivel_nome);
    //verifica se entra em alguma excessão que resultaria em erro
    if(possivel_nome.find(',') != std::string::npos) {
    throw std::invalid_argument("digite o nome sem ' , '");
    }
    //caso o nome seja vazio retorna nullptr
    if (nome_limpo_input.empty()) {
        return nullptr;
    }
    //verifica se o player existe e retorna um ponteiro com o cadastro
    std::ifstream leitura_arq("assets/datas/dados.csv");
    if (leitura_arq.is_open()) {
      std::string linha;
      while (getline(leitura_arq, linha)) {
        std::stringstream ss(linha);
        std::string nome_arquivo, score_str, partidas_str, score_geral_str;
        if (std::getline(ss, nome_arquivo, ',') && std::getline(ss, score_str, ',') && std::getline(ss, partidas_str,',') && std::getline(ss,score_geral_str)) {
          //limpo o nome do arquivo para comparacao, para ter certeza que nao haja caracteres invisisveis
          std::string nome_limpo_arquivo = trim(nome_arquivo);
          if (nome_limpo_input == nome_limpo_arquivo) {
            int score = std::stoi(score_str);
            int partidas = std::stoi(partidas_str);
            int score_geral=std::stoi(score_geral_str);
            leitura_arq.close();
            return new Cadastro(nome_limpo_input, score, partidas,score_geral);
          }
        }
      }
      leitura_arq.close();

      //cadastra o player como novo e retorna um ponteiro para seu cadastro
      std::ofstream escrita_arq("assets/datas/dados.csv", std::ios::app);
      if (escrita_arq.is_open()) {
        escrita_arq << nome_limpo_input << ",0,0,0" << std::endl;
        escrita_arq.close();
      } else {
        std::cout << "erro ao abrir arquivo para escrita";
        return nullptr;
        }
        return new Cadastro(nome_limpo_input, 0, 0, 0);
    }
    else {
        //se o arquivo nao existir cria um novo e cadastra o primeiro jogador
        std::ofstream novo_arquivo("assets/datas/dados.csv");
        if (novo_arquivo.is_open()) {
            novo_arquivo << nome_limpo_input << ",0,0,0" << std::endl;
            novo_arquivo.close();
            return new Cadastro(nome_limpo_input, 0, 0, 0);
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
  score_geral+=score_partida;

  std::ifstream arq_armazenagem("assets/datas/dados.csv");
  std::vector<Cadastro> cadastros_jogadores;
  std::string linha;

  if(arq_armazenagem.is_open()){
    while(getline(arq_armazenagem,linha)){
      std::stringstream ss(linha);
      std::string nome, score_str,partidas_str,score_geral_str;
      if(std::getline(ss, nome, ',') && std::getline(ss, score_str, ',') && std::getline(ss, partidas_str,',') && std::getline(ss,score_geral_str)){
        int score = std::stoi(score_str);
        int partidas = std::stoi(partidas_str);
        int score_total= std::stoi(score_geral_str);
            
        std::string nome_limpo = trim(nome);

        if(nome_limpo == nome_jogador){
          cadastros_jogadores.push_back(Cadastro(nome_limpo, high_score, numero_partidas,score_geral));
        } else {
          cadastros_jogadores.push_back(Cadastro(nome_limpo, score, partidas,score_total));
          }
      }
    }
    arq_armazenagem.close();
  }
  else{
    std::cerr<<"erro ao abrir arquivo de armazenagem"<<std::endl;
    return;
  }

  std::ofstream arq_escrita("assets/datas/dados.csv");
  if(arq_escrita.is_open()){
    for(const auto& j:cadastros_jogadores){
        arq_escrita << j.nome_jogador << "," << j.high_score << "," << j.numero_partidas<< "," <<j.score_geral<< std::endl;
    }
    arq_escrita.close();
  }
  else{
    std::cerr<<"erro ao abrir arquivo de escrita na funcao modificar dados"<<std::endl;
  }

  //chama a função leaderboard para cadastro na tabela
  Leaderboard leaderboard;
  leaderboard.cadastro_tabela(nome_jogador,high_score);
}


void Cadastro::display_estatisticas(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, const char* titulo){

    int largura = 600;
    int altura = 60 + 60 *5;
    int x = (al_get_display_width(display) - largura) / 2;
    int y = (al_get_display_height(display) - altura) / 2;

    //realiza o display do retangulo e do titulo
    al_draw_filled_rectangle(x, y, x+largura, y+altura, al_map_rgba(0, 0, 0, 255));
    al_draw_rectangle(x, y, x+largura, y+altura, al_map_rgb(255,255,255), 2);
    al_draw_text(font, al_map_rgb(255,255,0), x+largura/2, y+10, ALLEGRO_ALIGN_CENTRE, titulo);

    //display das estatisticas
    int y_offset = y + 50;
    double pontuacao_media=(numero_partidas>0) ? (double)score_geral/numero_partidas : 0.0;
    
        std::string texto = "nome: "+ nome_jogador;
        al_draw_text(font, al_map_rgb(255,255,255), x+largura/2, y_offset + 0*40, ALLEGRO_ALIGN_CENTRE, texto.c_str());

        texto = "numero de partidas: " + std::to_string(numero_partidas);
        al_draw_text(font, al_map_rgb(255,255,255), x+largura/2, y_offset + 1*40, ALLEGRO_ALIGN_CENTRE, texto.c_str());

        texto = "high score: " + std::to_string(high_score);
        al_draw_text(font, al_map_rgb(255,255,255), x+largura/2, y_offset + 2*40, ALLEGRO_ALIGN_CENTRE, texto.c_str());

        std::stringstream ss_pontuacao_media;
        ss_pontuacao_media<< std::fixed<<std::setprecision(2);
        ss_pontuacao_media << "score medio: " << pontuacao_media;
        texto = ss_pontuacao_media.str();
        al_draw_text(font, al_map_rgb(255,255,255), x+largura/2, y_offset + 3*40, ALLEGRO_ALIGN_CENTRE, texto.c_str());
    

    al_draw_text(font, al_map_rgb(180,180,180), x+largura/2, y+altura-60, ALLEGRO_ALIGN_CENTRE, "Pressione qualquer tecla para sair");

    al_flip_display();

    // Espera o player apertar uma tecla ou fechar a janela
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool sair = false;
    while (!sair) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        }
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            sair = true;
        }
    }
    al_destroy_event_queue(queue);
}