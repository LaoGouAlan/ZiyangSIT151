#include "splashkit.h"
#include "player.h"
#include "environment.h"
#include "enemy.h"
#include "Theitems.h"

#include <cmath>
#include <vector> //We're going to build a vector of active enemies... more on this later

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

    //Major data structures containing objects should typically be above the individual elements in those data structures
    //For example our vector of enemies will be here rather than further down
    vector<enemy_data> activeEnemies; 
    vector<items_data> activeItems;

    player_data player;
    player = new_player();
    

    environment_data Background;
    Background = new_env("DefaultBackground", 0, 0);

    //Create our enemy object, create a spawn location
    point_2d spawnPoint = {1200, -100};
    point_2d tarLocation = center_point(player.player_sprite);
    enemy_data enemy = new_enemy(spawnPoint, tarLocation);
    activeEnemies.push_back(enemy); //Adding our enemy to our enemy data structure...
    items_data items = new_items(spawnPoint, tarLocation);
    activeItems.push_back(items);

    spawner_data spawner;
    spawner = new_spawner();

    item_spawner_data  items_spawer;
    items_spawer = new_items_spawner();


    //Music
    music bgMusic = music_named("Music");
    set_music_volume(0.3f);
    play_music(bgMusic);

    //Mouse
    hide_mouse();

    //Counter used to manage time based events
    float timeCounter = 0;

    //Game loop - each refresh is called a Game Tick
    while ( not quit_requested() )
    {
        //before we process events
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

        //Spawner code
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

    free_music(bgMusic);

    return 0;
}

