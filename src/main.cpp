#include "..\include\obstaculo.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

//tamanho da tela
const int SCREEN_W = 800;
const int SCREEN_H = 600;

int main()
{
    //inicializando as bibliotecas
    al_init();
    al_init_primitives_addon();

    //inicializando as structs padrão
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_COLOR corteste = al_map_rgb(0, 255, 0);

    //tela
    display = al_create_display(640,480);
    al_set_window_position(display, 200, 200);

    //desenha retangulo
    Obstaculo Cano(0, 0, 5, 250, 500);
    Cano.desenhar_canos();
    al_flip_display();

    //espera por 5 segundos
    al_rest(5.0);

    //fecha a janela
    al_destroy_display(display);
    return 0;
}