#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include<string>

class Leaderboard{
    private:
     std::string nome_tabelados;
     int pontuacao_tabelados;
    public:
    Leaderboard(const std::string& nome,int pontuacao) : nome_tabelados(nome),pontuacao_tabelados(pontuacao){}

    void cadastro_tabela(std::string,int);
    void display_tabela();//a fazer
};
#endif