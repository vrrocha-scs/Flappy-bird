#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/coletavel.hpp"
#include "../include/personagem.hpp"
#include "../include/obstaculo.hpp"
#include "../include/randomizador.hpp"
#include "../include/menu.hpp"
#include "../include/cadastro.hpp"
#include "../include/leaderboard.hpp"
#include "../include/gamestate.hpp"
#include "../include/utils.hpp"
#include "../include/interfaces.hpp"


TEST_CASE("Testando funcionalidades da classe Personagem") {
    // =================== SETUP ===================
    REQUIRE(al_init());

    ALLEGRO_BITMAP* bitmap_idle = al_create_bitmap(64, 50);
    ALLEGRO_BITMAP* bitmap_jump = al_create_bitmap(64, 50);

    REQUIRE(bitmap_idle != nullptr);
    REQUIRE(bitmap_jump != nullptr);

    Personagem personagem(100, 200, bitmap_idle, bitmap_jump);

    SUBCASE("Testando salto") {
        personagem.jump();
        CHECK(personagem.get_velocityY() == doctest::Approx(-5.0f)); // JUMP_POWER
    }

     // =================== TESTES ==================
    SUBCASE("Testando gravidade acumulada com on_tick") {
        personagem.set_velocityY(0);

        personagem.on_tick(); //once
        CHECK(personagem.get_velocityY() == doctest::Approx(0.25f)); // GRAVITY

        personagem.on_tick();// twice
        CHECK(personagem.get_velocityY() == doctest::Approx(0.5f));
    }

    SUBCASE("Testando alteracao de sprite conforme movimento vertical") {
        personagem.set_velocityY(-3.0f); // Para cima
        personagem.on_tick();
        CHECK(personagem.get_bitmap() == bitmap_jump);

        personagem.set_velocityY(3.0f); // Para baixo
        personagem.on_tick();
        CHECK(personagem.get_bitmap() == bitmap_idle);
    }

    SUBCASE("Testando ganho de pontuacao") {
        int total_pontos = 10 + 4;
        personagem.gain_score(10);
        personagem.gain_score(4);
        CHECK(personagem.get_score() == total_pontos/2); // score / 2
    }

    SUBCASE("Testando reset da posicao e score") {
        personagem.set_posX(500);
        personagem.set_posY(300);
        personagem.set_velocityX(10);
        personagem.set_velocityY(-5);
        personagem.gain_score(20);

        personagem.reset_position(100, 200);

        CHECK(personagem.get_posX() == doctest::Approx(100));
        CHECK(personagem.get_posY() == doctest::Approx(200));
        CHECK(personagem.get_velocityX() == doctest::Approx(0));
        CHECK(personagem.get_velocityY() == doctest::Approx(0));
        CHECK(personagem.get_score() == 0);
    }
    
    /*
    SUBCASE("Testando colisao com hitbox") {
    TODO
    }*/


    al_destroy_bitmap(bitmap_idle);
    al_destroy_bitmap(bitmap_jump);
    al_uninstall_system();
}

TEST_CASE("Testando a classe ObjetoRenderizavel") {
    // =================== SETUP ===================
    REQUIRE(al_init());
    REQUIRE(al_init_image_addon());

    ALLEGRO_BITMAP* bmp = al_create_bitmap(100, 50);
    REQUIRE(bmp != nullptr);

    // =================== TESTESS ===================
    SUBCASE("Construtor básico (sem hitbox)") {
        ObjetoRenderizavel obj(10.0f, 20.0f, bmp);
        CHECK(obj.get_posX() == doctest::Approx(10.0f));
        CHECK(obj.get_posY() == doctest::Approx(20.0f));
        CHECK(obj.get_centerX() == doctest::Approx(50.0f));
        CHECK(obj.get_centerY() == doctest::Approx(25.0f));
    }

    SUBCASE("Construtor com hitbox automática baseada no bitmap") {
        ObjetoRenderizavel obj2(15, 25, bmp, true);
        CHECK(obj2.get_posX() == doctest::Approx(15));
        CHECK(obj2.get_posY() == doctest::Approx(25));
        CHECK(obj2.get_centerX() == doctest::Approx(al_get_bitmap_width(bmp) / 2.0f));
        CHECK(obj2.get_centerY() == doctest::Approx(al_get_bitmap_height(bmp) / 2.0f));

        Hitbox& hb = obj2.get_hitbox();
        CHECK(hb.get_posX() == doctest::Approx(15));
        CHECK(hb.get_posY() == doctest::Approx(25));
    }

    SUBCASE("Construtor com hitbox de largura e altura customizadas") {
        ObjetoRenderizavel obj3(5, 10, bmp, true, 50, 60);
        CHECK(obj3.get_posX() == doctest::Approx(5));
        CHECK(obj3.get_posY() == doctest::Approx(10));
        CHECK(obj3.get_centerX() == doctest::Approx(al_get_bitmap_width(bmp) / 2.0f));
        CHECK(obj3.get_centerY() == doctest::Approx(al_get_bitmap_height(bmp) / 2.0f));

        Hitbox& hb = obj3.get_hitbox();
        CHECK(hb.get_posX() == doctest::Approx(5));
        CHECK(hb.get_posY() == doctest::Approx(10));
    }

    /*
    SUBCASE("Testando colisao com hitbox") {
    TODO
    }*/

    SUBCASE("Setters e getters de posição e velocidade") {
        ObjetoRenderizavel obj(0, 0, bmp);
        obj.set_posX(123.4f);
        obj.set_posY(567.8f);
        obj.set_velocityX(2.5f);
        obj.set_velocityY(-1.5f);

        CHECK(obj.get_posX() == doctest::Approx(123.4f));
        CHECK(obj.get_posY() == doctest::Approx(567.8f));
        CHECK(obj.get_velocityX() == doctest::Approx(2.5f));
        CHECK(obj.get_velocityY() == doctest::Approx(-1.5f));
    }

    SUBCASE("Testando movimento com on_tick() e move_object()") {
        ObjetoRenderizavel obj(50.0f, 50.0f, bmp);
        obj.set_velocityX(5.0f);
        obj.set_velocityY(3.0f);

        obj.on_tick();
        CHECK(obj.get_posX() == doctest::Approx(55.0f));
        CHECK(obj.get_posY() == doctest::Approx(47.0f)); // posY -= velocityY

        obj.move_object();
        CHECK(obj.get_posX() == doctest::Approx(60.0f));
        CHECK(obj.get_posY() == doctest::Approx(44.0f));
    }

    al_destroy_bitmap(bmp);
    al_shutdown_image_addon();
    al_uninstall_system();
}
