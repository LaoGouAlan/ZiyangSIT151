#ifndef SPACE_GAME_ITEMS
#define SPACE_GAME_ITEMS

#include "player.h"
#include "splashkit.h"
#include <vector>

using namespace std;

#define ITEMS_VELOCITY_MAX 0.0

struct items_data
{
    sprite items_sprite;
    vector_2d velocity;
    float speed;
    point_2d spawnPoint;

    bool operator == (const items_data& item) const
    {
        return items_sprite == item.items_sprite && speed == item.speed & vectors_equal(velocity, item.velocity);
    }
    bool operator != (const items_data& item) const {return !operator==(item);}

    void calculate_rotation(point_2d &tarLocation);
    void calculate_velocity(point_2d &tarLocation);
    void remove(vector<items_data> &items);
    void check_collisions(vector<items_data> &bullets, vector<player_data> &players);
};

items_data new_items(point_2d &spawnpoint, point_2d &tarLocation);

void draw_items(vector<items_data> &item);
void update_items(vector<items_data> &item);

struct item_spawner_data
{
    point_2d spawnPoints[8];
    float spawnRate;
    float previousSpawnTime;

    bool spawn_item(float &currentTime);
};

item_spawner_data new_items_spawner();


#endif