#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <allegro5/allegro.h>

#include "personagem.hpp"
#include "objetorenderizavel.hpp"

TEST_CASE("Testando a fisica e as acoes do Personagem") {

    // =============== SETUP ====================
    REQUIRE(al_init());

    ALLEGRO_BITMAP* bitmap_fantasma = al_create_bitmap(10, 10);

    REQUIRE(bitmap_fantasma != nullptr);

    Personagem personagem_teste(100, 200, bitmap_fantasma);

    SUBCASE("Testando a acao de pular") {
        personagem_teste.jump(); // jump twice

        // O valor deve corresponder ao #define JUMP_POWER no personagem.cpp
        CHECK(personagem_teste.get_velocityY() == -5.0f);
    }

    // =================== TESTES ==================
    SUBCASE("Testando a aplicacao da gravidade") {
        personagem_teste.set_velocityY(0);
        
        personagem_teste.on_tick(); // gravity once

        // O valor deve corresponder ao #define GRAVITY no personagem.cpp
        CHECK(personagem_teste.get_velocityY() == doctest::Approx(0.25f));

        personagem_teste.on_tick(); // gravity twice
        CHECK(personagem_teste.get_velocityY() == doctest::Approx(0.50f));
    }

    al_destroy_bitmap(bitmap_fantasma);
    al_uninstall_system();
}

#include "obstaculo.hpp"
#include "randomizador.hpp"
#include "hitbox.hpp"

TEST_CASE("Testando a movimentacao dos canos"){

    // =============== SETUP ====================
    REQUIRE(al_init());

    ALLEGRO_BITMAP* bitmap_fantasma = al_create_bitmap(10, 200);

    REQUIRE(bitmap_fantasma != nullptr);

    Randomizador rando(200, 800);

    int altura_buraco = rando.valor_aleatorio();
    int tamanho_gap = 100;
    const int SCREEN_W = 1000;
    const int SCREEN_H = 1000;

    Obstaculo obstaculo_teste(SCREEN_W + 50, -1000 + altura_buraco, bitmap_fantasma, 1.2, 50, altura_buraco);

   // =================== TESTES ==================
    SUBCASE("Construtor atribui corretamente todas as propriedades") {
        float velocidade = 1.5f;
        float largura = 50.0f;
        float altura = 300.0f;
        Obstaculo obstaculo(100, 200, bitmap_fantasma, velocidade, largura, altura);

        CHECK(obstaculo.get_posX() == 100);
        CHECK(obstaculo.get_posY() == 200);
        CHECK(obstaculo.get_velocidadeX() == doctest::Approx(velocidade));
        CHECK(obstaculo.get_larguraObs() == doctest::Approx(largura));
        CHECK(obstaculo.get_alturaObs() == doctest::Approx(altura));
    }

    SUBCASE("Setters modificam o estado corretamente") {
        Obstaculo obstaculo(0, 0, bitmap_fantasma, 0, 0, 0);

        obstaculo.set_velocidadeX(6.9f);
        CHECK(obstaculo.get_velocidadeX() == doctest::Approx(6.9f));

        obstaculo.set_larguraObs(123.0f);
        CHECK(obstaculo.get_larguraObs() == doctest::Approx(123.0f));
    }

    SUBCASE("Movimento com casos de velocidade especiais") {

        SUBCASE("Velocidade zero nao altera a posicao") {
            Obstaculo obstaculo_parado(500, 200, bitmap_fantasma, 0.0f, 50, 300);
            float pos_inicial = obstaculo_parado.get_posX();

            obstaculo_parado.on_tick(); // Executa o movimento

            CHECK(obstaculo_parado.get_posX() == doctest::Approx(pos_inicial)); // Posição não deve mudar
        }

         SUBCASE("Velocidade negativa move o obstaculo para a direita") {
            Obstaculo obstaculo_reverso(500, 200, bitmap_fantasma, -2.0f, 50, 300);
            float pos_inicial = obstaculo_reverso.get_posX();
            
            obstaculo_reverso.on_tick(); // Executa o movimento
            
            // posX -= -2.0  =>  posX += 2.0
            CHECK(obstaculo_reverso.get_posX() == doctest::Approx(pos_inicial + 2.0f));
        }
    }

    SUBCASE("Testando velocidade da movimentacao") {
        
        float posicao_inicial_x = obstaculo_teste.get_posX();
        
        float velocidade_esperada = 1.2f;

        obstaculo_teste.on_tick();

        float posicao_depois_de_1_tick = obstaculo_teste.get_posX();
        
        CHECK(posicao_depois_de_1_tick == doctest::Approx(posicao_inicial_x - velocidade_esperada));

        obstaculo_teste.on_tick();
        float posicao_depois_de_2_ticks = obstaculo_teste.get_posX();
        CHECK(posicao_depois_de_2_ticks == doctest::Approx(posicao_inicial_x - (2 * velocidade_esperada)));
    }

    al_destroy_bitmap(bitmap_fantasma);
    al_uninstall_system();

}


TEST_CASE("Testando a classe base ObjetoRenderizavel") {
    // =================== SETUP ===================
    REQUIRE(al_init());
    ALLEGRO_BITMAP* bitmap_fantasma = al_create_bitmap(30, 40);
    REQUIRE(bitmap_fantasma != nullptr);


    // =================== TESTES ==================

    SUBCASE("Construtor padrao (sem hitbox explicita)") {
        ObjetoRenderizavel objeto_teste(100, 200, bitmap_fantasma);

        // Verifica se as posições foram atribuídas corretamente
        CHECK(objeto_teste.get_posX() == 100.0f);
        CHECK(objeto_teste.get_posY() == 200.0f);

        // Verifica se o centro foi calculado corretamente (metade das dimensões do bitmap)
        CHECK(objeto_teste.get_centerX() == doctest::Approx(15.0f)); // 30 / 2
        CHECK(objeto_teste.get_centerY() == doctest::Approx(20.0f)); // 40 / 2
    }
/*
    SUBCASE("Construtor com hitbox ativada") {
        ObjetoRenderizavel objeto_com_hitbox(50, 80, bitmap_fantasma, true);

        Hitbox* hitbox = objeto_com_hitbox.get_hitbox();
        REQUIRE(hitbox != nullptr);

        // Verifica se a hitbox foi inicializada com a posição e as dimensões do objeto
        CHECK(hitbox->get_x() == doctest::Approx(50.0f));
        CHECK(hitbox->get_y() == doctest::Approx(80.0f));
        CHECK(hitbox->get_largura() == doctest::Approx(30.0f)); // Largura do bitmap
        CHECK(hitbox->get_altura() == doctest::Approx(40.0f));  // Altura do bitmap
    }

    SUBCASE("Metodos Setters de posicao") {
        ObjetoRenderizavel objeto_teste(0, 0, bitmap_fantasma);

        // Atribuímos novas posições
        objeto_teste.set_posX(999.0f);
        objeto_teste.set_posY(-50.0f);

        // Verificamos se as posições foram atualizadas
        CHECK(objeto_teste.get_posX() == doctest::Approx(999.0f));
        CHECK(objeto_teste.get_posY() == doctest::Approx(-50.0f));
    }*/

    al_destroy_bitmap(bitmap_fantasma);
    al_uninstall_system();
}