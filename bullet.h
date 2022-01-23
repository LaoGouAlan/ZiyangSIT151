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

    //Equality and inequality overloads so that we can remove items from list....
    bool operator == (const bullet_data& bullet) const
    {
        return bullet_sprite == bullet.bullet_sprite && speed == bullet.speed && vectors_equal(velocity, bullet.velocity);
    }
    bool operator != (const bullet_data& bullet) const { return !operator==(bullet);}

    //member function
    void calculate_velocity();
    void remove(vector<bullet_data> &bullets);
    bool check_collisions(vector<enemy_data> &enemies); //<<
};

//Initialize/construct the bullet data
bullet_data new_bullet(point_2d &spawnPoint, float &rotationAngle);

void update_all_bullets(vector<bullet_data> &bullets, vector<enemy_data> &enemies);
void draw_all_bullets(vector<bullet_data> &bullets);

#endif