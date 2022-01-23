#include "bullet.h"
#include "enemy.h"
#include "splashkit.h"

using namespace std;

bullet_data new_bullet(point_2d &spawnPoint, float &rotationAngle)
{
    //Build the data
    bullet_data bullet;

    //Create and assign sprite
    bitmap bulletSpr = bitmap_named("Bullet");
    bullet.bullet_sprite = create_sprite(bulletSpr);

    //Use the sprite width/height to create offsets to place sprite in world
    float spriteWidth = sprite_width(bullet.bullet_sprite);
    float spriteHeight = sprite_height(bullet.bullet_sprite);

    float spriteOffsetX = spriteWidth/2;
    float spriteOffsetY = spriteHeight/2;

    sprite_set_x(bullet.bullet_sprite, spawnPoint.x - spriteOffsetX);
    sprite_set_y(bullet.bullet_sprite, spawnPoint.y - spriteOffsetY);

    //Set Rotation based on input param
    sprite_set_rotation(bullet.bullet_sprite, rotationAngle);

    //Set initial speed value, calculate velocity
    bullet.speed = 5.0f;
    bullet.calculate_velocity();

    //Return the initilized bullet object
    return bullet;
}

void bullet_data::calculate_velocity()
{
    sprite bulletSpr = this->bullet_sprite; //because this is a member function, we can use the this keyword to access the object properties/attributes
    vector_2d currentVelocity = vector_from_angle(sprite_rotation(bulletSpr) - 90, this->speed);
    this->velocity = currentVelocity;
}


void update_all_bullets(vector<bullet_data> &bullets, vector<enemy_data> &enemies)
{
    for (bullet_data bullet : bullets)
    {
        vector_2d currentPos = vector_to(sprite_position(bullet.bullet_sprite));
        vector_2d deltaPos = vector_add(currentPos, bullet.velocity);
        point_2d deltaPosPoint = {deltaPos.x, deltaPos.y};

        //Check if the new position deltaPosPoint is outside of our window boundaries
        //Both the x and y Value do not need be checked and found true, if one is true that we can remove the bullet

        //----

        //Boundaries Window 1080 x 800
        float LeftWindowBoundary = 0;
        float RightWindowBoundary = 1080;
        float UpperWindowBoundary = 0;
        float LowerWindowBoundary = 800;

        //Get the sprite position and sprite offset
        point_2d bulletCurrentPosition = center_point(bullet.bullet_sprite);

        float sprWidth = sprite_width(bullet.bullet_sprite);
        float sprHeight = sprite_height(bullet.bullet_sprite);
        float sprOffsetX = sprWidth/2;
        float sprOffsetY = sprHeight/2;

        float buffZone = 50;

        //Center point of the sprite, and adjust for the sprite offset
        float spriteLeftEdge = bulletCurrentPosition.x + sprOffsetX + buffZone;
        float spriteRightEdge = bulletCurrentPosition.x - sprOffsetX - buffZone;
        float spriteTopEdge = bulletCurrentPosition.y + sprOffsetY + buffZone;
        float spriteBottomEdge = bulletCurrentPosition.y - sprOffsetY - buffZone;

        //-----

        //Checking to see if we've gone outside of the boundaries
        if((spriteLeftEdge < LeftWindowBoundary) || (spriteRightEdge > RightWindowBoundary) || (spriteTopEdge < UpperWindowBoundary) || (spriteBottomEdge > LowerWindowBoundary))
        {
            bullet.remove(bullets);
            return;
        }


        sprite_set_position(bullet.bullet_sprite, deltaPosPoint);
        if(bullet.check_collisions(enemies))
        {
            write_line("remove bullet");
            bullet.remove(bullets);
        }
    }
}

void draw_all_bullets(vector<bullet_data> &bullets)
{
    for (bullet_data bullet : bullets)
    {
        draw_sprite(bullet.bullet_sprite);
    }
}

bool bullet_data::check_collisions(vector<enemy_data> &enemies)
{
    vector<bullet_data> bullets_to_remove;
    vector<bullet_data> enemies_to_remove;
    for (enemy_data enemy : enemies)
    {
        if(sprite_collision(this->bullet_sprite, enemy.enemy_sprite))
        {
            //Do collision stuff here
            enemy.remove(enemies);
            return true;
        }
    }
    return false;
}

void bullet_data::remove(vector<bullet_data> &bullets)
{
    int index = 0;
    int indexFound = 0;
    // write_line("index val: " + to_string(index));
    for (bullet_data bullet : bullets)
    {
        if(bullet.operator==(*this))
        {
            write_line("index val: " + to_string(index));
            write_line("bullet vector length: " + to_string(bullets.size()));
            indexFound = index;
            write_line("erased bullet: " + to_string(index));
            write_line("bullet vector length: " + to_string(bullets.size()));
        }
        index++;
    }
    bullets.erase(bullets.begin() + indexFound);
}

