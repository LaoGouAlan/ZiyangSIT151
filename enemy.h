#ifndef SPACE_GAME_ENEMY
#define SPACE_GAME_ENEMY

#include "splashkit.h"
#include <vector> //This is so we can use dynamic data structures

using namespace std;

#define ENEMY_VELOCITY_MAX 8.0

//Struct to contain the object and the object variables etc
//member functions are defined in the struct, or by invoking the class name on the function declaration (StructClass::Function format)
struct enemy_data
{
    sprite enemy_sprite;
    float health;
    vector_2d velocity;
    point_2d spawnPoint;
 
    //Equality and inequality overloads so that we can remove items from list....
    bool operator == (const enemy_data& enemy) const
    {
        return enemy_sprite == enemy.enemy_sprite && health == enemy.health && vectors_equal(velocity, enemy.velocity);
    }
    bool operator != (const enemy_data& enemy) const { return !operator==(enemy);}

    //rotation and velocity functions for movement and orientation
    void calculate_rotation(point_2d &tarLocation);
    void calculate_velocity(point_2d &tarLocation);
    void remove(vector<enemy_data> &enemies);
};

//constructor
enemy_data new_enemy(point_2d &spawnpoint, point_2d &tarLocation);

//draw and update functions
void draw_enemy(vector<enemy_data> &enemies);
void update_enemy(vector<enemy_data> &enemies);

struct spawner_data
{
    point_2d spawnPoints[8];
    float spawnRate;
    float previousSpawnTime;

    bool spawn_enemy(float &currentTime);
};

spawner_data new_spawner();

#endif