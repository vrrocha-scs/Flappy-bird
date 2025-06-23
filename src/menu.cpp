#include "menu.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <iostream>


extern void restart_game(Personagem*& character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis); // Falando para o menu que existe uma funcao no main para que nao haja erro de compilacao

Menu::Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type): 
    menu_font(font), 
    menu_type(type), 
    selected_option(0),
    is_active(false),
    event_queue(queue),
    select_sound(nullptr)
{

    select_sound = al_load_sample("assets/sounds/select_sound.ogg");
    confirm_sound = al_load_sample("assets/sounds/confirm_sound.ogg");
    if (!select_sound || !confirm_sound) {
        std::cerr << "Erro ao carregar som de selecao ou de confirmação" << std::endl;
    }

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
    if (select_sound) {
        al_destroy_sample(select_sound);
    }
    if (confirm_sound) {
        al_destroy_sample(confirm_sound);
    }
}

// desenhar o menu
void Menu::draw(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis) {
    al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
    for (auto i : background_items) {
        i->render_object();
    }
    for (auto c : canos) {
        c->render_object();
    }
    for (auto p : coletaveis) {
        if (!p->get_coletado()) {
            p->render_object();
        }
    }
    character->render_object();

    float panel_width = 500;
    float panel_height = 80 + (m_options.size() * 50);
    float panel_x = (SCREEN_W - panel_width) / 2;
    float panel_y = (SCREEN_H - panel_height) / 2;

    al_draw_filled_rectangle(panel_x, panel_y, panel_x + panel_width, panel_y + panel_height, al_map_rgba(0, 0, 0, 150));
    
    al_draw_rectangle(panel_x, panel_y, panel_x + panel_width, panel_y + panel_height, al_map_rgb(255, 255, 255), 2.0);

    for (size_t i = 0; i < m_options.size(); ++i) {
        ALLEGRO_COLOR color = (i == selected_option) ? color_selected : color_text;
        
        if (i == 0 && menu_type == MenuType::PAUSE) {
            color = color_title;
        }

        float text_y = panel_y + 40 + (i * 50);
        al_draw_text(menu_font, color, panel_x + panel_width / 2, text_y, ALLEGRO_ALIGN_CENTRE, m_options[i].c_str());
    }
}

// Função para lidar com a entrada no menu
MenuResult Menu::handle_input(ALLEGRO_EVENT ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP: // Mexer nas opções usando a seta para cima
                selected_option--;
                // Toca o som de seleção ao pressionar para cima
                if (select_sound)
                    al_play_sample(select_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

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

            case ALLEGRO_KEY_DOWN: // Mexer nas opções usando a seta para baixo
                selected_option++;
                // Toca o som de seleção ao pressionar para baixo
                if (select_sound)
                    al_play_sample(select_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

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
            
            case ALLEGRO_KEY_ENTER: // Selecionar a opção com enter em cada menu
                // Toca o som de confirmação ao pressionar ENTER
                if (confirm_sound)
                    al_play_sample(confirm_sound, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    al_rest(0.2); // Espera 100ms só para testar se o som toca antes de sair
                   
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

MenuResult Menu::show(std::vector<ObjetoRenderizavel*>& background_items, Personagem* character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis) {
    is_active = true;
    ALLEGRO_EVENT ev;
    MenuResult final_result = (menu_type == MenuType::PAUSE) ? MenuResult::CONTINUE_GAME : MenuResult::EXIT_GAME;
    
    while(is_active) {
        // Desenha a cena 
        draw(background_items, character, canos, coletaveis);
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
        al_clear_to_color(al_map_rgba_f(0, 0, 1, 0));
        ALLEGRO_EVENT ev;

        // Desenha a cena
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

        // Desenha o texto
        al_draw_text(font, al_map_rgb(255, 255, 0), start_x, SCREEN_H / 2, ALLEGRO_ALIGN_LEFT, text_to_draw.c_str());

        al_flip_display();

        // Espera e processa eventos
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
    Cadastro*& jogador_atual,
    Personagem*& character,
    std::vector<Obstaculo*>& canos,
    ALLEGRO_DISPLAY* display,
    std::vector<ObjetoRenderizavel*>& background_items,
    std::vector<Coletavel*>& coletaveis,
    double& previous_time,
    double& ultimo_spawn_canos,
    double& lag
) {
    MenuResult result = this->show(background_items, character, canos, coletaveis);
    MenuType type = this->menu_type;
    
    // Lógica para START
    if (type == MenuType::START) {
    try {
        if (result == MenuResult::START_NEW_GAME) {
            std::string nome_digitado = get_player_name(this->event_queue, this->menu_font, background_items);

            if (!nome_digitado.empty()) {
                jogador_atual = Cadastro::verificar_dados(nome_digitado);

                if (jogador_atual != nullptr) {
                    current_state = GameState::PLAYING;
                    previous_time = al_get_time();
                    ultimo_spawn_canos = 0;
                    lag = 0.0;
                }
            }
        }
        else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
    catch (const std::invalid_argument& e) {
        std::string texto = e.what();
        al_draw_text(this->menu_font, al_map_rgb(255, 0, 0),
        al_get_display_width(display) / 2,
        al_get_display_height(display) / 2 + 60,
        ALLEGRO_ALIGN_CENTRE, texto.c_str());
        al_flip_display();
        al_rest(1.5);
    }
}
    // Lógica para PAUSE
    else if (type == MenuType::PAUSE) {
        if (result == MenuResult::CONTINUE_GAME) {
            current_state = GameState::PLAYING;
            previous_time = al_get_time();
        } else if (result == MenuResult::RESTART_GAME) {
            restart_game(character, canos, coletaveis);
            current_state = GameState::PLAYING;
        } else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
    // Lógica para GAMEOVER
    else if (type == MenuType::END) {
        if (result == MenuResult::RESTART_GAME) {
            restart_game(character, canos, coletaveis);
            current_state = GameState::PLAYING;
        } else if (result == MenuResult::SHOW_LEADERBOARD) {
            Leaderboard leaderboard;
            leaderboard.display_tabela(display, this->menu_font, "Melhores Pontuacoes");
        } else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
}
