#include "maingame.hpp"

float MainGame::SCREEN_H = 1000;
float MainGame::SCREEN_W = 1000;

MainGame::MainGame() : MainGame(1.5){}

MainGame::MainGame(float game_speed){
    init_addons();
    load_assets();

    display = al_create_display(SCREEN_W, SCREEN_H);
    event_queue = al_create_event_queue();
    if (!display || !event_queue) {
        std::cerr << "Erro fatal: Falha ao inicializar Display ou Fila de Eventos." << std::endl;
    }
    al_set_window_position(display, 100, 100);

    default_color = al_map_rgba_f(0, 0, 1, 0);
    
    ultimo_spawn = 0;
    tamanho_gap = 3*al_get_bitmap_height(character_sprite);

    current_state = GameState::START;
    randomizer = new Randomizador(100, SCREEN_H - (al_get_bitmap_height(ground_sprite) + tamanho_gap + 100));

    character = new Personagem(SCREEN_W / 2 - 250, SCREEN_H / 2, character_sprite,jumping_sprite);
    background.push_back(new ObjetoRenderizavel(0,SCREEN_H - 90, ground_sprite,1));
    background.push_back(new ObjetoRenderizavel(0,650,hills_sprite));
    background.push_back(new ObjetoRenderizavel(0,0,mountains_sprite));
    background[2]->set_velocityX(-game_speed*0.4);
    background[1]->set_velocityX(-game_speed*0.8);
    background[0]->set_velocityX(-game_speed);

}

MainGame::~MainGame(){
    delete character;
    delete randomizer;
    for(auto i : background){
        delete i;
    }

    for (auto c : canos) {
        delete c;
    }
    canos.clear();

    al_destroy_bitmap(character_sprite);
    al_destroy_bitmap(upper_pipe_sprite);
    al_destroy_bitmap(lower_pipe_sprite);
    al_destroy_font(menu_font);
    al_destroy_font(score_font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
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

void MainGame::load_assets(){
    character_sprite = al_load_bitmap("assets/images/character.png");
    jumping_sprite = al_load_bitmap("assets/images/character_jumping.png");
    ground_sprite = al_load_bitmap("assets/images/chao.png");
    upper_pipe_sprite = al_load_bitmap("assets/images/canocima.png");
    lower_pipe_sprite = al_load_bitmap("assets/images/canobaixo.png");
    menu_font = al_load_font("assets/fonts/game_over.ttf", 80, 0);
    score_font = al_load_font("assets/fonts/game_over.ttf", 160, 0);
    som_game_over = al_load_sample("assets/sounds/gameover_sound.wav");
    if (!character_sprite || !jumping_sprite || !upper_pipe_sprite || !lower_pipe_sprite || !menu_font || !som_game_over) {
        std::cerr << "Erro fatal: Falha ao carregar um ou mais assets." << std::endl;
    }
}

void MainGame::checkCollision(){
        if (character->checkCollision(background[0]->get_hitbox())) {
            al_play_sample(som_game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            current_state = GameState::GAMEOVER;
            cadastro->modificar_dados(character->get_score());
        }

        //Colisão com obstáculos
        for (auto c : canos) {
            if (character->checkCollision(c->get_hitbox())) {
                al_play_sample(som_game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                current_state = GameState::GAMEOVER;
                cadastro->modificar_dados(character->get_score());
                break;
            }
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
        c->desenhar_canos();
    }
    al_draw_textf(score_font, al_map_rgb(255, 255, 255), SCREEN_W/2, 20, ALLEGRO_ALIGN_CENTRE,"%i", character->get_score());
    }
}

void MainGame::restart_game() {
    character->reset_position(SCREEN_W / 2 - 250, SCREEN_H / 2);
    
    for (auto c : canos) {
        delete c;
    }
    
    canos.clear();
    ultimo_spawn = al_get_time();
}

void MainGame::create_menu(double& previous_time){

        // Determina o tipo de menu a ser criado com base no estado atual
        MenuType menu_type_to_show = MenuType::START;
            if (current_state == GameState::PAUSED) menu_type_to_show = MenuType::PAUSE;
            if (current_state == GameState::GAMEOVER) menu_type_to_show = MenuType::END;
        Menu active_menu(event_queue, menu_font, menu_type_to_show);
        active_menu.process_state_logic(
            current_state,
            cadastro,
            character,
            canos,
            display,
            background,
            previous_time,
            ultimo_spawn
            );          
}

void MainGame::on_tick(float current_time){
        character->on_tick();
        for (auto i : background) {
            i->on_tick();
            if(i->get_posX() < -2000){
                i->set_posX(0);
            }
        }
        for (auto c : canos) {
            c->on_tick();
            c->check_passagem(character);
        }
        
        // Lógica de Spawn
        if (current_time - ultimo_spawn >= PIPE_SPAWN_INTERVAL) {
            ultimo_spawn = current_time;
            adicionar_canos();
        }
        if (!canos.empty())
        {
            limpando_obstaculos(canos);
        }
}

void MainGame::adicionar_canos()
{   
    float altura_buraco = randomizer->valor_aleatorio();
    canos.push_back(new Obstaculo(altura_buraco - (al_get_bitmap_height(upper_pipe_sprite)), upper_pipe_sprite, game_speed, altura_buraco));
    canos.push_back(new Obstaculo(altura_buraco + tamanho_gap, lower_pipe_sprite, game_speed, (al_get_bitmap_height(lower_pipe_sprite) - (altura_buraco))));  
    return;
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
}

void MainGame::set_display_color(ALLEGRO_COLOR next_color){
    default_color = next_color;
}