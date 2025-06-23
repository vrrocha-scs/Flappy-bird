#include "leaderboard.hpp"
#include <allegro5/allegro_primitives.h>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "utils.hpp"

#define TAMANHO 5

struct EntradaTabela
{
  std::string nome;
  int pontuacao;
  EntradaTabela(const std::string& n,int p):nome(n),pontuacao(p){}
  EntradaTabela() : nome("--"), pontuacao(0) {} // adicionei um construtor padrao para implementar a logica de limpar o nome
};


bool comparaEntradas(const EntradaTabela& a, const EntradaTabela& b) { //logica para organizar a tabela
    return a.pontuacao > b.pontuacao;
}

//recebe os dados da partida e verifica se ela entrara na leaderboard
void Leaderboard::cadastro_tabela(std::string nome_jogador, int score_partida) {
    std::list<EntradaTabela> tabelados;
    std::ifstream arq_tabela_leit("assets/datas/tabela.csv");
    std::string linha;

    if (arq_tabela_leit.is_open()) {
        while (std::getline(arq_tabela_leit, linha)) {
            std::stringstream ss(linha);
            std::string nome, pontuacao_str;
            if (std::getline(ss, nome, ',') && std::getline(ss, pontuacao_str, ',')) {
                int pontuacao = std::stoi(pontuacao_str);
                std::string nome_limpo = trim(nome); // chamo a funcao trim que limpa o nome de qualquer tipo de entradas que nao seja o proprio nome
                if (nome_limpo != "--") { // não adiciona as entradas vazias
                    tabelados.emplace_back(nome_limpo, pontuacao);
                }
            }
        }
    } else {
        std::cerr << "erro ao abrir tabela para leitura" << std::endl;
    }
    arq_tabela_leit.close();
    
    // verifica se o player ja esta na tabela, caso exista modifico e saio do for
    bool jogador_existia = false;
    for (auto& entrada : tabelados) {
        if (entrada.nome == nome_jogador) {
            jogador_existia = true;
            if (score_partida > entrada.pontuacao) {
                entrada.pontuacao = score_partida;
            }
            break;
        }
    }
    // se nao existe eu insiro ele
    if (!jogador_existia) {
        tabelados.emplace_back(nome_jogador, score_partida);
    }

    // ordena a lista da maior pontuação para a menor
    tabelados.sort(comparaEntradas);

    // confiro se nao ultrapassou o tamanho maximo ao alocar um novo highscore
    if (tabelados.size() > TAMANHO) {
        tabelados.resize(TAMANHO);
    }
    
    // registra a tabela(somente se ouver alguma modificação nos dados)
    std::ofstream arq_tabela_esc("assets/datas/tabela.csv");
    if (arq_tabela_esc.is_open()) {
        for (const auto& it : tabelados) {
            arq_tabela_esc << it.nome << "," << it.pontuacao << std::endl;
        }
        int linhas_escritas = tabelados.size();
        // completa-se a tabela se houver menos players do que o tamanho da tabela
        while (linhas_escritas < TAMANHO) {
            arq_tabela_esc << "--,0" << std::endl;
            linhas_escritas++;
        }
        arq_tabela_esc.close();
    } else {
        std::cerr << "erro ao abrir tabela para escrita" << std::endl;
    }
}


  void Leaderboard::display_tabela(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font, const char* titulo) {
    std::ifstream arq_tabela("assets/datas/tabela.csv");
    if (!arq_tabela.is_open()) {
        std::cerr << "Erro ao abrir tabela.csv" << std::endl;
        return;
    }
    std::vector<std::string> linhas;
    std::string linha;
    while (std::getline(arq_tabela, linha)) {
        linhas.push_back(linha);
    }
    arq_tabela.close();

    int largura = 600;
    int altura = 60 + 60 * linhas.size();
    int x = (al_get_display_width(display) - largura) / 2;
    int y = (al_get_display_height(display) - altura) / 2;

    //realiza o display do retangulo e do titulo
    al_draw_filled_rectangle(x, y, x+largura, y+altura, al_map_rgba(0, 0, 0, 255));
    al_draw_rectangle(x, y, x+largura, y+altura, al_map_rgb(255,255,255), 2);
    al_draw_text(font, al_map_rgb(255,255,0), x+largura/2, y+10, ALLEGRO_ALIGN_CENTRE, titulo);

    //display da tabela
    int y_offset = y + 50;
    for (size_t i = 0; i < linhas.size(); ++i) {
        std::string nome, pontuacao;
        std::stringstream ss(linhas[i]);
        std::getline(ss, nome, ',');
        std::getline(ss, pontuacao, ',');
        nome = trim(nome);
        pontuacao = trim(pontuacao);
        std::string texto = std::to_string(i+1) + ". " + nome + " - " + pontuacao;
        al_draw_text(font, al_map_rgb(255,255,255), x+largura/2, y_offset + i*40, ALLEGRO_ALIGN_CENTRE, texto.c_str());
    }

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


