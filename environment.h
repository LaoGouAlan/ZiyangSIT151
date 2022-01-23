#ifndef SPACE_GAME_ENVIRONMENT
#define SPACE_GAME_ENVIRONMENT

#include "splashkit.h"

using namespace std;

struct environment_data
{
    sprite environment_sprite;
    //point_2d position;
};

environment_data new_env(string spriteName, float posX, float posY);

void draw_env(const environment_data &env);

void update_position(const environment_data &env);

#endif  