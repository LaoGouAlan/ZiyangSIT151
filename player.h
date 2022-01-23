#ifndef SPACE_GAME_PLAYER
#define SPACE_GAME_PLAYER

#include "splashkit.h"
#include "bullet.h"
#include <vector>

using namespace std;

#define PLAYER_ACCELERATION 5.0
#define RELOAD_SPEED 0.25
#define MAX_PLAYER_HEALTH 100

/**
 * The player data keeps track of all of the information related to the player.
 * To begin with our player will only need a sprite, but we can add more information later on.
 */
struct player_data
{
    sprite      player_sprite;
    sprite      crosshair;
    vector<bullet_data> bullet_list;
    float time_since_shot;
    

    //UI stuff
    float health;
    rectangle healthFrame;
    rectangle healthBar;
    bool isDead;

    rectangle calc_healthFrame();
    rectangle calc_healthBar(rectangle &healthFrame);
    void check_collisions(vector<enemy_data> &enemies);
    void check_active_health(vector<items_data> &items);
    void remove(vector<items_data> &items);
};

/**
 * Creates a new player in the centre of the screen with the default ship.
 * 
 * @returns         The new player data
 */
player_data new_player();

/**
 * Draws the player to the screen. 
 * 
 * @param player    The player to draw to the screen
 */
void draw_player(const player_data &player);

/**
 * call all player functions, including movement, shooting, collisions, take damage, and so on
 * 
 * @param player    The player being updated
 */
void update_player(player_data &player, vector<enemy_data> &enemies);

/**
 * Read user input and update the player based on this interaction.
 * 
 * @param player    The player being updated
 */
void calculate_position(player_data &player);

/**
 * Read user input and update the player based on this interaction.
 * 
 * @param player    The player being updated
 */
void calculate_rotation(player_data &player);

void calculate_crosshair_position(player_data &player);

void is_shooting(player_data &player);

void draw_UI(const player_data &player);

#endif 