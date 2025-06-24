#include "menu.hpp"
#include "interfaces.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <iostream>

extern const float FPS;
extern void restart_game(Personagem*& character, std::vector<Obstaculo*>& canos, std::vector<Coletavel*>& coletaveis); // Falando para o menu que existe uma funcao no main para que nao haja erro de compilacao

Menu::Menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT *font, MenuType type): 
    menu_font(font), 
    menu_type(type), 
    selected_option(0),
    is_active(false),
    event_queue(queue),
    select_sound(nullptr)
{

    select_sound = al_load_sample("assets/sounds/select-sfx.ogg");
    confirm_sound = al_load_sample("assets/sounds/confirm-sfx.ogg");
    if (!select_sound || !confirm_sound) {
        std::cerr << "Erro ao carregar som de selecao ou de confirmação" << std::endl;
    }
    if (menu_type == MenuType::LOGIN) {
        m_options.push_back("Cadastro / Login");
        m_options.push_back("Sair");
    }
    else if (menu_type == MenuType::MAIN_MENU) {
        m_options.push_back("Jogar");
        m_options.push_back("Dificuldade");
        m_options.push_back("Estatisticas");
        m_options.push_back("Tutorial");
        m_options.push_back("Sair");
    }
    else if(menu_type == MenuType::DIFFICULTY) {
        m_options.push_back("Selecione a Dificuldade:");
        m_options.push_back("Facil");
        m_options.push_back("Medio");
        m_options.push_back("Dificil");
        selected_option = 1;
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
        int start_index = (menu_type == MenuType::PAUSE || menu_type == MenuType::DIFFICULTY) ? 1 : 0;
        size_t num_options = m_options.size();

        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                if (select_sound) al_play_sample(select_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //Toca o som ao selecionar uma opcao
                selected_option--;
                if (selected_option < start_index) selected_option = num_options - 1;
                break;

            case ALLEGRO_KEY_DOWN:
                if (select_sound) al_play_sample(select_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Toca o som ao selecionar uma opcao
                selected_option++;
                if (selected_option >= num_options) selected_option = start_index;
                break;

            case ALLEGRO_KEY_ENTER:
                if (confirm_sound) al_play_sample(confirm_sound, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); // Toca o som de confirmacao
                al_rest(0.2);
                is_active = false;
            // Fala o que cada opcao retorna de acordo com o que esta selecionado e de acordo com o menu
                if (menu_type == MenuType::LOGIN)
                {
                    return (selected_option == 0) ? MenuResult::PROCESS_LOGIN : MenuResult::EXIT_GAME;
                } else if (menu_type == MenuType::MAIN_MENU) { 
                    switch (selected_option) {
                        case 0: return MenuResult::START_NEW_GAME;
                        case 1: return MenuResult::OPEN_DIFFICULTY_MENU;
                        case 2: return MenuResult::SHOW_STATISTICS;
                        case 3: return MenuResult::TUTORIAL;
                        case 4: return MenuResult::EXIT_GAME;
                        default: return MenuResult::NO_ACTION;
                    }
                } else if (menu_type == MenuType::DIFFICULTY) {
                    switch (selected_option) {
                        case 1: return MenuResult::SET_DIFFICULTY_EASY;
                        case 2: return MenuResult::SET_DIFFICULTY_MEDIUM;
                        case 3: return MenuResult::SET_DIFFICULTY_HARD;
                    }
                } else if (menu_type == MenuType::PAUSE) {
                    switch (selected_option) {
                        case 1: return MenuResult::CONTINUE_GAME;
                        case 2: return MenuResult::RESTART_GAME;
                        case 3: return MenuResult::EXIT_GAME;
                    }
                } else if (menu_type == MenuType::END) {
                    switch (selected_option) {
                        case 0: return MenuResult::RESTART_GAME;
                        case 1: return MenuResult::SHOW_LEADERBOARD;
                        case 2: return MenuResult::EXIT_GAME;
                    }
                }
                break;

            case ALLEGRO_KEY_ESCAPE: // Apertar esc so vai fechar o jogo se estiver no Inicio ou no Final
                is_active = false;
                if (menu_type == MenuType::PAUSE) return MenuResult::CONTINUE_GAME;
                if (menu_type == MenuType::DIFFICULTY) return MenuResult::NO_ACTION;
                return MenuResult::EXIT_GAME;
        }
    } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
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
        float panel_width = 600;
        float panel_height = 200;
        float panel_x = (SCREEN_W - panel_width) / 2.0;
        float panel_y = (SCREEN_H - panel_height) / 2.0;

        al_draw_filled_rectangle(panel_x, panel_y, panel_x + panel_width, panel_y + panel_height, al_map_rgba(0, 0, 0, 150));
        al_draw_rectangle(panel_x, panel_y, panel_x + panel_width, panel_y + panel_height, al_map_rgb(255, 255, 255), 2.0);

        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, panel_y + 40, ALLEGRO_ALIGN_CENTER, "Digite seu nome e pressione enter:");

        if (al_get_time() - cursor_time > 0.5) {
            show_cursor = !show_cursor;
            cursor_time = al_get_time();
        }
        std::string text_to_draw = name + (show_cursor ? "|" : "");

        // Desenha o texto
        const float INPUT_BOX_WIDTH = 500;
        const float start_x = (SCREEN_W - INPUT_BOX_WIDTH) / 2.0;
        const float text_y = panel_y + 100;
        
        al_draw_text(font, al_map_rgb(255, 255, 0), start_x, text_y, ALLEGRO_ALIGN_LEFT, text_to_draw.c_str());

        al_flip_display();

        // Espera e processa eventos
        al_wait_for_event(queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
                if (!name.empty()) editing = false;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                if (!name.empty()) name.pop_back();
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            float current_width = al_get_text_width(font, text_to_draw.c_str());
            if (ev.keyboard.unichar >= 32 && current_width < INPUT_BOX_WIDTH) {
                char utf8[4];
                int len = al_utf8_encode(utf8, ev.keyboard.unichar);  // obtém número de bytes escritos
                name.append(utf8, len);  // adiciona apenas os bytes válidos à string
            }
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return "";
        }
    }
    return name;
}

const float DISTANCIA_ENTRE_CANOS = 450.0f;
const float FPS = 60;

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
    double& lag,
    float& velocidade_canos,
    int& multiplicador_espaco_canos,
    float& intervalo_spawn_canos
) {
    MenuResult result = this->show(background_items, character, canos, coletaveis);

    // Lógica para START
    if (this->menu_type == MenuType::LOGIN) {
        if (result == MenuResult::PROCESS_LOGIN) {
            try {
                std::string nome_digitado = get_player_name(this->event_queue, this->menu_font, background_items);
                if (!nome_digitado.empty()) {
                    jogador_atual = Cadastro::verificar_dados(nome_digitado);
                    if (jogador_atual != nullptr) {
                        current_state = GameState::MAIN_MENU;
                    }
                }
            } 
            catch (const std::invalid_argument& e) {
                std::string texto = e.what();

                this->draw(background_items, character, canos, coletaveis);

                al_draw_text(this->menu_font, al_map_rgb(255, 0, 0),
                al_get_display_width(display) / 2,
                al_get_display_height(display) / 2 + 150,
                ALLEGRO_ALIGN_CENTRE, texto.c_str());
                
                al_flip_display();
                al_rest(2.0);
            }
        }
        else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    } 
        else if (this->menu_type == MenuType::MAIN_MENU) {
            if (result == MenuResult::START_NEW_GAME) { 
            restart_game(character, canos, coletaveis);
            current_state = GameState::PLAYING;
            previous_time = al_get_time();
            lag = 0.0;
            ultimo_spawn_canos = 0;
            
        } else if (result == MenuResult::OPEN_DIFFICULTY_MENU) {
            Menu difficulty_menu(this->event_queue, this->menu_font, MenuType::DIFFICULTY);
            MenuResult difficulty_result = difficulty_menu.show(background_items, character, canos, coletaveis);

            if (difficulty_result == MenuResult::SET_DIFFICULTY_EASY) {
                velocidade_canos = 1.0f;
                multiplicador_espaco_canos = 4;
            } else if (difficulty_result == MenuResult::SET_DIFFICULTY_MEDIUM) {
                velocidade_canos = 2.0f;
                multiplicador_espaco_canos = 3;
            } else if (difficulty_result == MenuResult::SET_DIFFICULTY_HARD) {
                velocidade_canos = 3.0f;
                multiplicador_espaco_canos = 2;
            }
            if (difficulty_result != MenuResult::NO_ACTION) {
                intervalo_spawn_canos = DISTANCIA_ENTRE_CANOS / (velocidade_canos * FPS);
        }
        }
        else if (result == MenuResult::SHOW_STATISTICS) {
            if(jogador_atual) {
                jogador_atual->display_estatisticas(display, this->menu_font, "Suas Estatisticas");
            }
        }
        else if (result == MenuResult::TUTORIAL) {
            Interfaces interfaces(this->event_queue, this->menu_font);
            interfaces.mostrarTutorial(display, this->event_queue);
        } 
        else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    } 


    // Lógica para PAUSE
    else if (this->menu_type == MenuType::PAUSE) {
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
    else if (this->menu_type == MenuType::END) {
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
