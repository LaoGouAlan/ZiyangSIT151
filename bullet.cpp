#include "bullet.h"
#include "enemy.h"
#include "splashkit.h"

using namespace std;

bullet_data new_bullet(point_2d &spawnPoint, float &rotationAngle)
{
    bullet_data bullet;

    bitmap bulletSpr = bitmap_named("Bullet");
    bullet.bullet_sprite = create_sprite(bulletSpr);

    float spritewidth = sprite_width(bullet.bullet_sprite);
    float spriteheight = sprite_height(bullet.bullet_sprite);

    float spriteOffsetX = spritewidth/2;
    float spriteOffsetY = spriteheight/2;

    sprite_set_x(bullet.bullet_sprite, spawnPoint.x - spriteOffsetX);
    sprite_set_y(bullet.bullet_sprite, spawnPoint.y - spriteOffsetY);

    sprite_set_rotation(bullet.bullet_sprite, rotationAngle);

    bullet.speed = 5.0f;
    bullet.calculate_velocity();

    return bullet;
}

void bullet_data::calculate_velocity()
{
    sprite bulletSpr = this->bullet_sprite;
    vector_2d currentVelocity = vector_from_angle(sprite_rotation(bulletSpr) - 90, this->speed);
    this->velocity = currentVelocity;
}

void update_all_bullets(vector<bullet_data> &bullets, vector<enemy_data> &enemies)
{
    for(bullet_data bullet : bullets)
    {
        vector_2d currentPos = vector_to(sprite_position(bullet.bullet_sprite));
        vector_2d deltaPos = vector_add(currentPos, bullet.velocity);
        point_2d deltaPosPoint = {deltaPos.x, deltaPos.y};

        float LeftWindowBoundary = 0;
        float RightWindowBoundary = 1080;
        float UpperWindowBoundary = 0;
        float LowerWindowBoundary = 800;

        point_2d bulletCurrentPosition = center_point(bullet.bullet_sprite);

        float sprWidth = sprite_width(bullet.bullet_sprite);
        float sprHeight = sprite_height(bullet.bullet_sprite);
        float sprOffsetX = sprWidth/2;
        float sprOffsetY = sprHeight/2;

        float buffZone = 50;

        float spriteLeftEdge = bulletCurrentPosition.x + sprOffsetX + buffZone;
        float spriteRightEdge = bulletCurrentPosition.x - sprOffsetX - buffZone;
        float spriteTopEdge = bulletCurrentPosition.y + sprOffsetY + buffZone;
        float spriteBottomEdge = bulletCurrentPosition.y - sprOffsetY - buffZone;

        if((spriteLeftEdge < LeftWindowBoundary) || (spriteRightEdge > RightWindowBoundary) || (spriteTopEdge < UpperWindowBoundary) || (spriteBottomEdge > LowerWindowBoundary))
        {
            bullet.remove(bullets);
            return;
        }

        sprite_set_position(bullet.bullet_sprite, deltaPosPoint);
        bullet.check_collisions(bullets, enemies);
    }
}

void draw_all_bullets(vector<bullet_data> &bullets)
{
    for (bullet_data bullet : bullets)
    {
        draw_sprite(bullet.bullet_sprite);
    }
}

void bullet_data::check_collisions(vector<bullet_data> &bullets, vector<enemy_data> &enemies)
{
    enemy_data *tarEnemy;
    bool found = false;
    
    for (enemy_data enemy : enemies)
    {
        if(sprite_collision(this->bullet_sprite, enemy.enemy_sprite))
        {
            // enemy.remove(enemies);
            found = true;
            tarEnemy = &enemy;
        }
    }
    
    if(found)
    {
        tarEnemy->remove(enemies);
        this->remove(bullets);
    }
}

void bullet_data::remove(vector<bullet_data> &bullets)
{
    int index = 0;
    write_line("index val: " + to_string(index));
    for (bullet_data bullet : bullets)
    {
        if (bullet.operator==(*this))
        {
            write_line("index val: " + to_string(index));
            write_line("bullet vector length: " + to_string(bullets.size()));
            bullets.erase(bullets.begin() + index);
            write_line("erased bullet: " + to_string(index));
            write_line("bullet vector length: " + to_string(bullets.size()));
        }
        index++;
    }
    delete this;
}