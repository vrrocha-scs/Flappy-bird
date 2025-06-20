#include "menu.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

Menu::Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type):
    //main_display(display), 
    menu_font(font), 
    menu_type(type), 
    selected_option(0),
    is_active(false),
    event_queue(queue)
{
    if (menu_type == MenuType::START) {
        m_options.push_back("Cadastro");
        m_options.push_back("Sair");
    }
    else if(menu_type == MenuType::END) {
        m_options.push_back("Jogar Novamente");
        // Colocar o numero de vitorias/derrotas do jogador e informar se bateu o highscore ou nao
        m_options.push_back("Sair");
    }
    else if(menu_type == MenuType::PAUSE) {
        m_options.push_back("Jogo Pausado");
        m_options.push_back("Continuar");  
        m_options.push_back("Reiniciar");     
        m_options.push_back("Sair");           
        selected_option = 1; // Começa a seleção em "Continuar"
}
    else if(menu_type == MenuType::REGISTER) {
        m_options.push_back("Escreva seu nick");
    }

    //event_queue = al_create_event_queue();
    //if (!event_queue) {
    //    std::cerr << "ERRO: Falha ao criar a fila de eventos" << std::endl;
    //}
    //if (event_queue) {
    //    al_register_event_source(event_queue, al_get_keyboard_event_source());
    //    al_register_event_source(event_queue, al_get_display_event_source(main_display));
    //}

    color_text = al_map_rgb(255, 255, 255); // branco
    color_background = al_map_rgba(0, 0, 0, 180); // preto transparente
    color_selected = al_map_rgb(255, 255, 0);
    color_title = al_map_rgb(255, 0, 0); // vermelho
}

Menu::~Menu() {
    //if(event_queue) {
    //    al_destroy_event_queue(event_queue);
    //}
}

void Menu::draw() {
    ALLEGRO_DISPLAY *display = al_get_current_display();
    al_draw_filled_rectangle(0, 0, 
                            al_get_display_width(display),
                            al_get_display_height(display),
                            color_background);
    float screen_width = al_get_display_width(display);
    float font_height = al_get_font_line_height(menu_font);
    float start_y = (al_get_display_height(display) - (m_options.size() * font_height * 1.5)) / 2.0;

    for (size_t i = 0; i < m_options.size(); ++i) {
        ALLEGRO_COLOR current_color;

        if (menu_type == MenuType::PAUSE && i == 0) {
            current_color = color_title;
        } else {
            current_color = (i == selected_option) ? color_selected : color_text;
        }

        const char* option_text = m_options[i].c_str();
        
        float text_width = al_get_text_width(menu_font, option_text);
        float pos_x = (screen_width - text_width) / 2.0;

        float pos_y = start_y + (i * font_height * 1.5);

        al_draw_text(menu_font, current_color, pos_x, pos_y, ALLEGRO_ALIGN_LEFT, option_text);
    }
}

MenuResult Menu::handle_input(ALLEGRO_EVENT ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                selected_option--;
                
                
                if (menu_type == MenuType::PAUSE) {
                    
                    if (selected_option <= 0) { 
                        selected_option = m_options.size() - 1; 
                    }
                } else {
                    
                    if (selected_option < 0) {
                        selected_option = m_options.size() - 1; 
                    }
                }
                break;

            case ALLEGRO_KEY_DOWN:
                selected_option++;

                if (menu_type == MenuType::PAUSE) {
                    if (selected_option >= m_options.size()) {
                        selected_option = 1;
                    }
                } else {
                    if (selected_option >= m_options.size()) {
                        selected_option = 0;
                    }
                }
                break;
            
            case ALLEGRO_KEY_ENTER:
                is_active = false;
                if(menu_type == MenuType::START) {
                    return (selected_option == 0) ? MenuResult::START_NEW_GAME : MenuResult::EXIT_GAME;
                }
                if(menu_type == MenuType::PAUSE){
                     switch(selected_option) {
                        case 1: return MenuResult::CONTINUE_GAME;
                        case 2: return MenuResult::RESTART_GAME;
                        case 3: return MenuResult::EXIT_GAME;
                        default: return MenuResult::CONTINUE_GAME;
                    }
                }
                else if(menu_type == MenuType::END) {
                    return(selected_option == 0) ? MenuResult::RESTART_GAME : MenuResult::EXIT_GAME;
                }
                break;
            
            case ALLEGRO_KEY_ESCAPE:
                is_active = false;
                if (menu_type == MenuType::PAUSE) {
                    return MenuResult::CONTINUE_GAME;
                }
                return MenuResult::EXIT_GAME;
        }
    }
    else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        is_active = false;
        return MenuResult::EXIT_GAME;
    }

    return MenuResult::NO_ACTION;
}

MenuResult Menu::show() {
    is_active = true;
    ALLEGRO_EVENT ev;
    MenuResult final_result = (menu_type == MenuType::PAUSE) ? MenuResult::CONTINUE_GAME : MenuResult::EXIT_GAME;
    while(is_active) {
        al_wait_for_event(event_queue, &ev);

        MenuResult current_action = handle_input(ev);
        if (current_action != MenuResult::NO_ACTION) {
            final_result = current_action;
        }
        
        if (is_active) {
            draw();
            al_flip_display();
        }
    }
    return final_result;
}
