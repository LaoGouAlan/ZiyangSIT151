#include "enemy.h"
#include "splashkit.h"

enemy_data new_enemy(point_2d &spawnPoint, point_2d &tarLocation)
{
    //Create enemy object, create sprite bitmap, set enemy sprite to new bitmap
    enemy_data enemy;
    bitmap enemy_ship = bitmap_named("BasicEnemy_3");
    enemy.enemy_sprite = create_sprite(enemy_ship);

    //Use the sprite width/height to create offsets to place sprite in world
    float spriteWidth = sprite_width(enemy.enemy_sprite);
    float spriteHeight = sprite_height(enemy.enemy_sprite);

    float spriteOffsetX = spriteWidth/2;
    float spriteOffsetY = spriteHeight/2;

    //We need to store the spawnpoint so we can respawn enemies
    enemy.spawnPoint = {spawnPoint.x - spriteOffsetX, spawnPoint.y - spriteOffsetY};

    sprite_set_x(enemy.enemy_sprite, enemy.spawnPoint.x);
    sprite_set_y(enemy.enemy_sprite, enemy.spawnPoint.y);

    //Calculate rotation
    enemy.calculate_rotation(tarLocation);

    //Set the scale to make the sprite more appropriate in size
    float sprite_size = 0.5f; 
    sprite_set_scale(enemy.enemy_sprite, sprite_size);

    //Call the member function on the enemy class to move the enemy
    enemy.calculate_velocity(tarLocation);

    //set attribute values - this is where we could manually set the max velocity
    enemy.health = 100;

    return enemy;
}

void draw_enemy(vector<enemy_data> &enemies)
{
    for (enemy_data enemy : enemies)
    {
        draw_sprite(enemy.enemy_sprite);
    }
}

void update_enemy(vector<enemy_data> &enemies)
{
    for (enemy_data enemy : enemies)
    {
        //change in a value over time is referred to as a delta
        vector_2d currentPos = vector_to(sprite_position(enemy.enemy_sprite));
        vector_2d deltaPos = vector_add(currentPos, enemy.velocity);
        point_2d deltaPosPoint = {deltaPos.x, deltaPos.y};

                //----

        //Boundaries Window 1080 x 800
        float LeftWindowBoundary = 0;
        float RightWindowBoundary = 1080;
        float UpperWindowBoundary = 0;
        float LowerWindowBoundary = 800;

        //Get the sprite position and sprite offset
        point_2d bulletCurrentPosition = center_point(enemy.enemy_sprite);

        float sprWidth = sprite_width(enemy.enemy_sprite);
        float sprHeight = sprite_height(enemy.enemy_sprite);
        float sprOffsetX = sprWidth/2;
        float sprOffsetY = sprHeight/2;

        //Respawn buffer zone
        float buffZone = 400;

        //Center point of the sprite, and adjust for the sprite offset
        float spriteLeftEdge = bulletCurrentPosition.x + sprOffsetX + buffZone;
        float spriteRightEdge = bulletCurrentPosition.x - sprOffsetX - buffZone;
        float spriteTopEdge = bulletCurrentPosition.y + sprOffsetY + buffZone;
        float spriteBottomEdge = bulletCurrentPosition.y - sprOffsetY - buffZone;

        //-----

        //Checking to see if we've gone outside of the boundaries
        if((spriteLeftEdge < LeftWindowBoundary) || (spriteRightEdge > RightWindowBoundary) || (spriteTopEdge < UpperWindowBoundary) || (spriteBottomEdge > LowerWindowBoundary))
        {
            //Respawn the enemy
            sprite_set_position(enemy.enemy_sprite, enemy.spawnPoint);
            return;
        }
        else
        {
            sprite_set_position(enemy.enemy_sprite, deltaPosPoint);
        }
        
    }
}

void enemy_data::calculate_rotation(point_2d &tarLocation)
{
    sprite enemySpr = this->enemy_sprite;
    vector_2d currLocation = {center_point(enemySpr).x, center_point(enemySpr).y};
    vector_2d tarVector = vector_to(tarLocation);

    float rotationAngle = angle_between(tarVector, currLocation) + 90;
    sprite_set_rotation(this->enemy_sprite, rotationAngle);
}

void enemy_data::calculate_velocity(point_2d &tarLocation)
{
    sprite enemySpr = this->enemy_sprite;
    point_2d currLocation = center_point(enemySpr);

    vector_2d distance = vector_point_to_point(currLocation, tarLocation);
    vector_2d velocityUnitVector = unit_vector(distance);
    vector_2d currentVelocity = vector_multiply(velocityUnitVector, ENEMY_VELOCITY_MAX);

    this->velocity = currentVelocity;
}

void enemy_data::remove(vector<enemy_data> & enemies)
{
    int index = 0;
    int indexFound;
    for (enemy_data enemy: enemies)
    {
        if (enemy.operator==(*this)){
            indexFound = index;
            break;
        }
        index++;
    }
    enemies.erase(enemies.begin() + indexFound);
}

spawner_data new_spawner(){

    spawner_data spawner;
    
    //Fixed spawn points
    spawner.spawnPoints[0] = {-200, 600}; //Bottom left
    spawner.spawnPoints[1] = {-200, 200}; //upper left
    spawner.spawnPoints[2] = {216, -200}; //left most top
    spawner.spawnPoints[3] = {432, -200}; //center left top
    spawner.spawnPoints[4] = {648, -200}; //center right top
    spawner.spawnPoints[5] = {864, -200}; //right most top
    spawner.spawnPoints[6] = {1280, 200}; //upper right
    spawner.spawnPoints[7] = {1280, 600}; //bottom right
    
    spawner.spawnRate = 2.0; //This could be defined globally

    spawner.previousSpawnTime = 0;    

    return spawner;
}

bool spawner_data::spawn_enemy(float &currentTime)
{
    //previously previous spawn time and the spawn rate was higher than the current time 
    if (currentTime >= (this->previousSpawnTime + this->spawnRate)) //This has been updated/Fixed, Current time has to be more than the previous time + spawn rate
    {
        this->previousSpawnTime = currentTime;
        return true;
    }
    else
    {
        // write_line("Curr Time: " + std::to_string(currentTime) + ", Spawn Progress: " + std::to_string(this->previousSpawnTime+this->spawnRate));
        return false;
    }
}