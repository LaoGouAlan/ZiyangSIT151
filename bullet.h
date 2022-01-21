#ifndef SPACE_GAME_BULLET
#define SPACE_GAME_BULLET

#include "enemy.h"
#include "splashkit.h"
#include <vector>

using namespace std;

struct bullet_data
{
    sprite bullet_sprite;
    vector_2d velocity;
    float speed;

    bool operator == (const bullet_data& bullet) const
    {
        return bullet_sprite == bullet.bullet_sprite && speed == bullet.speed & vectors_equal(velocity, bullet.velocity);
    }
    bool operator != (const bullet_data& bullet) const {return !operator==(bullet);}

    void calculate_velocity();
    void remove(vector<bullet_data> &bullets);
    void check_collisions(vector<bullet_data> &bullets, vector<enemy_data> &enemies);
};

bullet_data new_bullet(point_2d &spawnPoint, float &rotationAngle);

void update_all_bullets(vector<bullet_data> &bullets, vector<enemy_data> &enemies);
void draw_all_bullets(vector<bullet_data> &bullets);

#endif 