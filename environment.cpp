#include "splashkit.h"
#include "environment.h"

environment_data new_env(string spriteName, float posX, float posY)
{
    environment_data env;
    bitmap bitmap = bitmap_named(spriteName);

    env.environment_sprite = create_sprite(bitmap);
    sprite_set_x(env.environment_sprite, posX);
    sprite_set_y(env.environment_sprite, posY);

    return env;
}

void draw_env(const environment_data &env)
{
    draw_sprite(env.environment_sprite);
}

void update_env(const environment_data &env, float newPosX, float newPosY)
{
    sprite_set_x(env.environment_sprite , newPosX);
    sprite_set_y(env.environment_sprite , newPosY);
}