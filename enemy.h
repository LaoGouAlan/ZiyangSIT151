#ifndef SPACE_GAME_ENEMY
#define SPACE_GAME_ENEMY

#include "splashkit.h"
#include <vector>

using namespace std;

#define ENEMY_VELOCITY_MAX 0.0

struct enemy_data
{
    sprite enemy_sprite;
    float health;
    vector_2d velocity;
    point_2d spawnPoint;

    bool operator == (const enemy_data& enemy) const
    {
        return enemy_sprite == enemy.enemy_sprite && health == enemy.health & vectors_equal(velocity, enemy.velocity);
    }
    bool operator != (const enemy_data& enemy) const { return !operator==(enemy);}

    void calculate_rotation(point_2d &tarLocation);
    void calculate_velocity(point_2d &tarLocation);
    void remove(vector<enemy_data> &enemies);
};

enemy_data new_enemy(point_2d &spawnpoint, point_2d &tarLocation);

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