#ifndef MENU
#define MENU

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<vector>
#include<string>

enum class MenuType {
    START,
    END,
    PAUSE,
    REGISTER
};

enum class MenuResult {
    NO_ACTION,
    START_NEW_GAME,
    RESTART_GAME,
    CADASTRO,
    EXIT_GAME,
    CONTINUE_GAME
};

class Menu {
    private:
    void draw();
    MenuResult handle_input(ALLEGRO_EVENT ev);
    //ALLEGRO_DISPLAY *main_display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_FONT *menu_font;
    MenuType menu_type;
    
    std::vector<std::string> m_options;
    int selected_option;
    bool is_active;

    ALLEGRO_COLOR color_text;
    ALLEGRO_COLOR color_background;
    ALLEGRO_COLOR color_selected;
    ALLEGRO_COLOR color_title;

    public:
    Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type);
    ~Menu();
    MenuResult show();
};
#endif