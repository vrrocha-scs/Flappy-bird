#include "maingame.hpp"

float MainGame::SCREEN_H = 1000;
float MainGame::SCREEN_W = 1000;

MainGame::MainGame() : MainGame(1.5){}

MainGame::MainGame(float game_speed) {
    init_addons();
    load_assets();
    display = al_create_display(SCREEN_W, SCREEN_H);
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);

    if (!display || !event_queue) {
        std::cerr << "Erro fatal: Falha ao inicializar Display ou Fila de Eventos." << std::endl;
    }
    al_set_window_position(display, 100, 100);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);
    default_color = al_map_rgba_f(0, 0, 0, 0);
    
    interfaces = new Interfaces(display, event_queue, menu_font);
    
    volume_musica = 0.3;
    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);
    music_instance = al_create_sample_instance(music);
    al_attach_sample_instance_to_mixer(music_instance, mixer);
    al_set_sample_instance_playmode(music_instance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(music_instance, volume_musica); // volume ajustável [0,1]


    al_play_sample_instance(music_instance); 

    al_rest(0.3); 
    interfaces->mostrarSplash(splash_img);
    ultimo_spawn_canos = 0;
    ultimo_spawn_coletavel = 0;
    tamanho_gap = multiplicador_espaco_canos*al_get_bitmap_height(character_sprite);
    current_state = GameState::START;
    randomizer = new Randomizador(100, SCREEN_H - (al_get_bitmap_height(ground_sprite) + tamanho_gap + 100));
    
    background.push_back(new ObjetoRenderizavel(1000,0,galaxia_sprite));
    background.push_back(new ObjetoRenderizavel(0,SCREEN_H - 90, ground_sprite,1));
    background.push_back(new ObjetoRenderizavel(0,0,estrelas1_sprite));
    background.push_back(new ObjetoRenderizavel(0,0,estrelas2_sprite));
    character = new Personagem(SCREEN_W / 2 - 250, SCREEN_H / 2, character_sprite,jumping_sprite);


    background[3]->set_velocityX(-game_speed*0.6);
    background[2]->set_velocityX(-game_speed*0.4);
    background[1]->set_velocityX(-game_speed);
    background[0]->set_velocityX(-game_speed*0.2);

}

MainGame::~MainGame(){
    delete character;
    delete randomizer;
    delete interfaces;
    for(auto i : background){
        delete i;
    }

    for (auto c : canos) {
        delete c;
    }
    canos.clear();

    // Destruindo as IMAGENS (Bitmaps)
    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(jumping_sprite);
    al_destroy_bitmap(ground_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);

    // Destruindo as FONTES
    al_destroy_font(menu_font);
    al_destroy_font(score_font);

    // Destruindo os SONS
    al_destroy_sample(som_pulo);
    al_destroy_sample(som_gameover);
    al_destroy_sample(music);
    al_destroy_sample_instance(music_instance);
    al_destroy_mixer(mixer);
    al_destroy_voice(voice);
    al_uninstall_audio();


    // Destruindo os recursos principais do Allegro
    al_destroy_display(display); 
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
}

void MainGame::init_addons(){
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);
}

int MainGame::load_assets(){
    //Imagens
    character_sprite = al_load_bitmap("assets/images/character.png");
    jumping_sprite = al_load_bitmap("assets/images/character_jumping.png");
    ground_sprite = al_load_bitmap("assets/images/chao.png");
    upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");
    estrelas1_sprite = al_load_bitmap("assets/images/estrelas1.png");
    estrelas2_sprite = al_load_bitmap("assets/images/estrelas2.png");
    galaxia_sprite = al_load_bitmap("assets/images/galaxia.png");
    splash_img = al_load_bitmap("assets/images/splash.png");
    green_ball_sprite = al_load_bitmap("assets/images/bolaverde.png");

    if (!character_sprite || !jumping_sprite || !ground_sprite || !upper_pipe_sprite || !lower_pipe_sprite ||
         !estrelas1_sprite || !estrelas2_sprite || !splash_img || !green_ball_sprite) {
        std::cerr << "Erro fatal: Falha ao carregar uma ou mais imagens" << std::endl;
        return -1;
    }
    
    //Fontes

    menu_font = al_load_font("assets/fonts/game_over.ttf", 80, 0);
    score_font = al_load_font("assets/fonts/game_over.ttf", 160, 0);

    if (!menu_font || !score_font) {
        std::cerr << "Erro fatal: Falha ao carregar uma ou mais fontes" << std::endl;
        return -1;
    }
    //Sons
    som_pulo = al_load_sample("assets/sounds/jump_sound.wav");
    som_gameover = al_load_sample("assets/sounds/gameover_sound.wav");
    music = al_load_sample("assets/sounds/background-music.ogg");

    if (!som_pulo || !som_gameover || !music) {
        std::cerr << "Erro fatal: Falha ao carregar um ou mais audios" << std::endl;
        return -1;
    }
    return 0;
}

void MainGame::checkCollision(){
    //Colisão com chão
    if (character->checkCollision(background[1]->get_hitbox())) {
        al_play_sample(som_gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        current_state = GameState::GAMEOVER;
        cadastro->modificar_dados(character->get_score());
    }

    //Colisão com obstáculos
    for (auto c : canos) {
        if (character->checkCollision(c->get_hitbox())) {
            al_play_sample(som_gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            current_state = GameState::GAMEOVER;
            cadastro->modificar_dados(character->get_score());
            break;
        }
    }

    //Colisão com coletáveis
    for (auto p : coletaveis){
        if(p->checkCollision(character->get_hitbox())){
            p->set_coletado(true);
            character->set_invincible(true);
            //current_state = GameState::INVINCIBLE;
        };
    }
}

void MainGame::render_screen(){
    al_clear_to_color(default_color);
    for (auto i : background) {
        i->render_object();
    }
    if(current_state == GameState::PLAYING){
    character->render_object();
    for (auto c : canos) {
        c->render_object();
    }
    al_draw_textf(score_font, al_map_rgb(255, 255, 255), SCREEN_W/2, 20, ALLEGRO_ALIGN_CENTRE,"%i", character->get_score());
    }
    al_flip_display();
}

void MainGame::vsync(){
    current_time = al_get_time();
    elapsed_time = current_time - previous_time;
    previous_time = current_time;

    if (elapsed_time > 0.25) {
        elapsed_time = SECONDS_PER_UPDATE;
    }

    lag += elapsed_time;
    
}

void MainGame::process_events(){
    ALLEGRO_EVENT ev;
    while (al_get_next_event(event_queue, &ev)) {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            current_state = GameState::EXITING;
        }

        if (current_state == GameState::PLAYING && ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                if (current_time - jump_cooldown > JUMP_COOLDOWN_SECONDS) {
                    character->jump();
                    jump_cooldown = current_time;
                    al_play_sample(som_pulo, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                current_state = GameState::PAUSED;
            }
        }
    }
}

void MainGame::restart_game() {
    character->reset_position(SCREEN_W / 2 - 250, SCREEN_H / 2);
    
    for (auto c : canos) {
        delete c;
    }
    
    for(auto p : coletaveis){
        delete p;
    }
    
    canos.clear();
    coletaveis.clear();
    ultimo_spawn_canos = 0;
    ultimo_spawn_coletavel = 0;
}

void MainGame::on_tick(){
    while (lag >= SECONDS_PER_UPDATE) {
        character->on_tick();
        background[0]->on_tick();
        if(background[0]->get_posX() < -1000){
            background[0]->set_posX(1000);
        }
        for(int i = 1;i < 4;i++) {
            background[i]->on_tick();
            if(background[i]->get_posX() < -1000){
                background[i]->set_posX(0);
            }
        }
        for (auto c : canos) {
            c->on_tick();
            c->check_passagem(character);
        }
        
        // Lógica de Spawn
        ultimo_spawn_canos += SECONDS_PER_UPDATE;
        ultimo_spawn_coletavel += SECONDS_PER_UPDATE;
        if (ultimo_spawn_canos >= intervalo_spawn_canos) {
            float altura_buraco = randomizer->valor_aleatorio();
            adicionar_canos(altura_buraco);
            ultimo_spawn_canos -= intervalo_spawn_canos;
            if (ultimo_spawn_coletavel >= intervalo_spawn_coletavel)
            {
                coletaveis.push_back(new Coletavel(altura_buraco + (tamanho_gap/2), green_ball_sprite, game_speed));
                ultimo_spawn_coletavel -= intervalo_spawn_coletavel; 
            }
        }

        efeito_invencivel_timer -= SECONDS_PER_UPDATE;
        if(character->get_invincible() == true)
        {
            //std::cout << "estou invencivel" << std::endl;
            if(efeito_invencivel_timer <= 0)
            {
                character->set_invincible(false);
                efeito_invencivel_timer = 10;
            }
        }
        lag -= SECONDS_PER_UPDATE;

    }
    if (!canos.empty())
    {
        for (vector<Obstaculo*>::iterator it = canos.begin(); it != canos.end(); it++)
            {
                if ((*it)->remover_obstaculo())
                {
                    canos.erase(it);
                }
            }     
    }
}

void MainGame::adicionar_canos(float altura_buraco)
{   
    canos.push_back(new Obstaculo(altura_buraco - (al_get_bitmap_height(upper_pipe_sprite)), upper_pipe_sprite, game_speed, altura_buraco));
    canos.push_back(new Obstaculo(altura_buraco + tamanho_gap, lower_pipe_sprite, game_speed, (al_get_bitmap_height(lower_pipe_sprite) - (altura_buraco))));  
    return;
}

void MainGame::create_menu(){
    // --- Seção de Lógica de MENUS (Bloqueante) ---
    if (current_state == GameState::START || current_state == GameState::PAUSED || current_state == GameState::GAMEOVER)
    {
        if (current_state == GameState::GAMEOVER)
            {
            interfaces->mostrarGameOver(score_font,character->get_score());
            }
        //Determina o tipo de menu a ser criado com base no estado atual
        MenuType menu_type_to_show = MenuType::START;
        if (current_state == GameState::PAUSED) menu_type_to_show = MenuType::PAUSE;
        if (current_state == GameState::GAMEOVER) menu_type_to_show = MenuType::END;

        // Cria o menu
        Menu active_menu = Menu(event_queue, menu_font, menu_type_to_show);
        process_state_logic(active_menu);
    }   

}

void MainGame::process_state_logic(Menu& active_menu){
    MenuResult result = active_menu.show(background, character, canos, coletaveis);

    // Lógica para START
    if (active_menu.get_menu_type() == MenuType::START) {
        if (result == MenuResult::START_NEW_GAME) {
            try {
                std::string nome_digitado = get_player_name(this->event_queue, this->menu_font, background);
                if (!nome_digitado.empty()) {
                    cadastro = Cadastro::verificar_dados(nome_digitado);
                    if (cadastro != nullptr) {
                        restart_game();
                        current_state = GameState::PLAYING;
                        previous_time = al_get_time();
                        lag = 0.0;
                        ultimo_spawn_canos = 0;
                    }
                }
            } 
            catch (const std::invalid_argument& e) {
                std::string texto = e.what();

                active_menu.draw(background, character, canos, coletaveis);

                al_draw_text(this->menu_font, al_map_rgb(255, 0, 0),
                al_get_display_width(display) / 2,
                al_get_display_height(display) / 2 + 150,
                ALLEGRO_ALIGN_CENTRE, texto.c_str());
                
                al_flip_display();
                al_rest(2.0);
            }
        } 
        else if (result == MenuResult::OPEN_DIFFICULTY_MENU) {
            Menu difficulty_menu(this->event_queue, this->menu_font, MenuType::DIFFICULTY);
            MenuResult difficulty_result = difficulty_menu.show(background, character, canos, coletaveis);

            if (difficulty_result == MenuResult::SET_DIFFICULTY_EASY) {
                set_game_speed(1.5f);
                multiplicador_espaco_canos = 4;
            } else if (difficulty_result == MenuResult::SET_DIFFICULTY_MEDIUM) {
                set_game_speed(2.0f);
                multiplicador_espaco_canos = 3;
            } else if (difficulty_result == MenuResult::SET_DIFFICULTY_HARD) {
                set_game_speed(2.5f);
                multiplicador_espaco_canos = 2;
            }
        } 
        else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    } 


    // Lógica para PAUSE
    else if (active_menu.get_menu_type() == MenuType::PAUSE) {
        if (result == MenuResult::CONTINUE_GAME) {
            current_state = GameState::PLAYING;
            previous_time = al_get_time();
        } else if (result == MenuResult::RESTART_GAME) {
            restart_game();
            current_state = GameState::PLAYING;
        } else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    }
    // Lógica para GAMEOVER
    else if (active_menu.get_menu_type() == MenuType::END) {
        if (result == MenuResult::RESTART_GAME) {
            restart_game();
            current_state = GameState::PLAYING;
        } else if (result == MenuResult::SHOW_LEADERBOARD) {
            Leaderboard leaderboard;
            leaderboard.display_tabela(display, this->menu_font, "Melhores Pontuacoes");
        } else if (result == MenuResult::EXIT_GAME) {
            current_state = GameState::EXITING;
        }
    } 
}



GameState MainGame::get_game_state(){
    return current_state;
}

ALLEGRO_EVENT_QUEUE* MainGame::get_event_queue(){
    return event_queue;
}

Personagem* MainGame::get_character(){
    return character;
}

float MainGame::get_game_speed(){
    return game_speed;
}


void MainGame::set_game_state(GameState next_state){
    current_state = next_state;
}

void MainGame::set_game_speed(float x){
    game_speed = x;
    background[3]->set_velocityX(-game_speed*0.6);
    background[2]->set_velocityX(-game_speed*0.4);
    background[1]->set_velocityX(-game_speed);
    background[0]->set_velocityX(-game_speed*0.2);
}

void MainGame::set_display_color(ALLEGRO_COLOR next_color){
    default_color = next_color;
}

void MainGame::set_game_volume(double x){
    volume_musica = x;
    al_set_sample_instance_gain(music_instance, volume_musica);
}