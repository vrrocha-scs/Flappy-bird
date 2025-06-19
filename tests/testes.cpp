#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <allegro5/allegro.h>

#include "personagem.hpp"
#include "objetorenderizavel.hpp"

TEST_CASE("Testando a fisica e as acoes do Personagem") {

    REQUIRE(al_init());

    ALLEGRO_BITMAP* bitmap_fantasma = al_create_bitmap(10, 10);

    REQUIRE(bitmap_fantasma != nullptr);

    Personagem personagem_teste(100, 200, bitmap_fantasma);

    SUBCASE("Testando a acao de pular") {
        personagem_teste.jump(); // jump twice

        // O valor deve corresponder ao #define JUMP_POWER no personagem.cpp
        CHECK(personagem_teste.get_velocityY() == -5.0f);
    }

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