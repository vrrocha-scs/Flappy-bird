#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

int main()
{
    //inicializando as bibliotecas
    al_init();
    al_init_font_addon();
    al_init_primitives_addon();

    //inicializando a tela e decidindo a sua posição
    ALLEGRO_DISPLAY * display = al_create_display(640,480);
    al_set_window_position(display, 200, 200);

    return 0;
}