#include "menu.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>

extern void restart_game(Personagem*& character, std::vector<Obstaculo*>& canos); // Falando para o menu que existe uma funcao no main para que nao haja erro de compilacao

Menu::Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type): 
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
        m_options.push_back("Leaderboard");
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

    color_text = al_map_rgb(255, 255, 255); // branco
    color_background = al_map_rgba(0, 0, 0, 180); // preto transparente
    color_selected = al_map_rgb(255, 255, 0);
    color_title = al_map_rgb(255, 0, 0); // vermelho
}

Menu::~Menu() {

}
// Desenhar o menu recebendo o background para que o fundo permaneca aparecendo
void Menu::draw(std::vector<ObjetoRenderizavel*>& background_items) {
    for (auto& item : background_items) {
        item->render_object();
    }
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
// Funcao para lidar com a entrada no menu
MenuResult Menu::handle_input(ALLEGRO_EVENT ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP: // Mexer nas opcoes usando a setinha para cima
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

            case ALLEGRO_KEY_DOWN: // Mexer nas opcoes usando a setinha para baixo
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
            
            case ALLEGRO_KEY_ENTER: // Selecionar a opcao com enter em cada menu
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
                    switch(selected_option) {
                        case 0: return MenuResult::RESTART_GAME;
                        case 1: return MenuResult::SHOW_LEADERBOARD;
                        case 2: return MenuResult::EXIT_GAME;
                        default: return MenuResult::EXIT_GAME;
        }
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

MenuResult Menu::show(std::vector<ObjetoRenderizavel*>& background_items) {
    is_active = true;
    ALLEGRO_EVENT ev;
    MenuResult final_result = (menu_type == MenuType::PAUSE) ? MenuResult::CONTINUE_GAME : MenuResult::EXIT_GAME;
    
    while(is_active) {
        // Desenha a cena 
        draw(background_items);
        al_flip_display();

        // Espera por um evento
        al_wait_for_event(event_queue, &ev);

        // Processa o evento
        MenuResult current_action = handle_input(ev);
        if (current_action != MenuResult::NO_ACTION) {
            final_result = current_action;
        }
    }
    return final_result;
}

std::string get_player_name(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, std::vector<ObjetoRenderizavel*>& background_items){
    std::string name = "";
    bool editing = true;
    double cursor_time = al_get_time();
    bool show_cursor = true;
    ALLEGRO_COLOR overlay_color = al_map_rgba(0, 0, 0, 180);

    // Define uma "caixa de texto" invisível
    const float INPUT_BOX_WIDTH = 500;
    const float start_x = (SCREEN_W - INPUT_BOX_WIDTH) / 2.0;

    while (editing) {
        ALLEGRO_EVENT ev;

        //Desenha a cena
        for (auto& item : background_items) {
            item->render_object();
        }
        al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, overlay_color);
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_CENTER, "Digite seu nome e pressione enter:");

        if (al_get_time() - cursor_time > 0.5) {
            show_cursor = !show_cursor;
            cursor_time = al_get_time();
        }
        std::string text_to_draw = name + (show_cursor ? "|" : "");

        //Desenha o texto
        al_draw_text(font, al_map_rgb(255, 255, 0), start_x, SCREEN_H / 2, ALLEGRO_ALIGN_LEFT, text_to_draw.c_str());

        al_flip_display();

        //Espera e processa eventos
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
                if (!name.empty()) editing = false;
            } else if(ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE){
                if(!name.empty()) name.pop_back();
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            float current_width = al_get_text_width(font, text_to_draw.c_str());
            if (ev.keyboard.unichar >= 32 && current_width < INPUT_BOX_WIDTH) {
                char utf8[4];
                al_utf8_encode(utf8, ev.keyboard.unichar);
                name += utf8;
            }
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return "";
        }
    }
    return name;
}

void Menu::process_state_logic(
    GameState& current_state,
    Personagem*& character,
    std::vector<Obstaculo*>& canos,
    ALLEGRO_DISPLAY* display,
    std::vector<ObjetoRenderizavel*>& background_items,
    double& previous_time,
    double& ultimo_spawn
) {
    MenuResult result = this->show(background_items);
    MenuType type = this->menu_type;
    
    // Lógica para START
    if (type == MenuType::START) {
        if (result == MenuResult::START_NEW_GAME) {
            while(1){
             try{
                std::string nome_digitado = get_player_name(this->event_queue, this->menu_font, background_items);
                if (!nome_digitado.empty()) {
                    Cadastro* jogador_atual = Cadastro::verificar_dados(nome_digitado);
                    if (jogador_atual != nullptr) {
                      current_state = GameState::PLAYING;
                      previous_time = al_get_time();
                      ultimo_spawn = al_get_time();
                      break;
                   }else {
                      current_state = GameState::EXITING;
                      break;
                    }
                }
             }
             catch(std::invalid_argument& e){
               // Limpa a tela e desenha o fundo
                for (auto& item : background_items) {
                    item->render_object();
                }
                ALLEGRO_DISPLAY *display = al_get_current_display();
                al_draw_filled_rectangle(0, 0, al_get_display_width(display), al_get_display_height(display), al_map_rgba(0,0,0,180));
                // Desenha a mensagem de erro no centro da tela
                al_draw_text(this->menu_font, al_map_rgb(255, 0, 0), al_get_display_width(display)/2, al_get_display_height(display)/2, ALLEGRO_ALIGN_CENTER, e.what());
                al_flip_display();
                al_rest(2.0);
           }
            } 
          }
           else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
    // Lógica para PAUSE
    else if (type == MenuType::PAUSE) {
        if (result == MenuResult::CONTINUE_GAME) {
            current_state = GameState::PLAYING;
            previous_time = al_get_time();
        } else if (result == MenuResult::RESTART_GAME) {
            restart_game(character, canos);
            current_state = GameState::PLAYING;
        } else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
    // Lógica para GAMEOVER
    else if (type == MenuType::END) {
        if (result == MenuResult::RESTART_GAME) {
            restart_game(character, canos);
            current_state = GameState::PLAYING;
        } else if (result == MenuResult::SHOW_LEADERBOARD) {
            Leaderboard leaderboard;
            leaderboard.display_tabela(display, this->menu_font, "Melhores Pontuacoes");
        } else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
}