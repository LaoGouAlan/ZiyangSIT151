#include "splashkit.h"
#include "player.h"
#include "environment.h"
#include "enemy.h"
#include "items.h"

#include <cmath>
#include <vector>

using namespace std;

/**
 * Load the game images, sounds, etc.
 */
void load_resources()
{
    load_resource_bundle("game_bundle", "SpaceGame.txt");
}

/**
 * Entry point.
 * 
 * Manages the initialisation of data, the event loop, and quitting.
 */
int main()
{
    float windowWidth = 1080;
    float windowHeight = 800;

     bool GAMEOVER = false;

    open_window("Space Game", int(windowWidth), int(windowHeight));
    load_resources();

    vector<enemy_data> activeEnemies;
    vector<items_data> activeItems;

    player_data player;
    player = new_player();
    
    environment_data Background;
    Background = new_env("DefaultBackground", 0, 0);

    point_2d spawnpoint = {540, 100};
    point_2d tarLocation = center_point(player.player_sprite);
    enemy_data enemy = new_enemy(spawnpoint, tarLocation);
    activeEnemies.push_back(enemy);

    spawner_data spawner;
    spawner = new_spawner();

    items_data items = new_items(spawnpoint, tarLocation);
    activeItems.push_back(items);

    item_spawner_data  items_spawer;
    items_spawer = new_items_spawner();

    //float angle = 0.0f;
   // bullet_data bullet = new_bullet(spawnpoint, angle);

    hide_mouse();

    float timeCounter = 0;

    while ( not quit_requested() )
    {
        float deltaT = (float)1/60;
        timeCounter = timeCounter + deltaT; 

        // Check inputs
        process_events();

        // Game updates and calculations
        update_player(player, activeEnemies);
        if (player.isDead == true)
        {
            GAMEOVER = true;
        }

        update_items(activeItems);

        update_enemy(activeEnemies);
        update_all_bullets(player.bullet_list, activeEnemies);

        // Clear the screen, and redraw the sprites
        clear_screen(COLOR_BLACK);       
        draw_env(Background);
        draw_enemy(activeEnemies);
        draw_items(activeItems);
        draw_player(player);
        draw_all_bullets(player.bullet_list);

        if(spawner.spawn_enemy(timeCounter) && !GAMEOVER)
        {
            int randomPos = rand() % 7;
            write_line("random Pos calc: " + std::to_string(randomPos));
            tarLocation = center_point(player.player_sprite);
            enemy_data enemyObj = new_enemy(spawner.spawnPoints[randomPos], tarLocation);
            activeEnemies.push_back(enemyObj);
        }

        draw_UI(player);

        //Update UI text information
        draw_text("Player Position: " + point_to_string(center_point(player.player_sprite)), COLOR_WHITE, 0, 10, option_to_screen());

        // int num = player.bullet_list.size();
        // draw_text("Number of bullets: " + to_string(num), COLOR_WHITE, 0, 30, option_to_screen());
        
        // draw_text("Reload time: " + to_string(player.time_since_shot), COLOR_WHITE, 0, 50, option_to_screen());

        draw_text("Time: " + to_string(timeCounter), COLOR_WHITE, 0, 30, option_to_screen());

        if(GAMEOVER){
            draw_text("GAME OVER", COLOR_RED, "NORMAL_FONT",  1000, 500, 400.0, option_to_screen());
        }

        refresh_screen(60);
    }

    return 0;
}