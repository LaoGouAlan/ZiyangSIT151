#include "splashkit.h"
#include "Theitems.h"
#include "player.h"

using namespace std;

items_data new_items(point_2d &spawnPoint, point_2d &tarLocation)
{
    items_data items;
    bitmap items_icon = bitmap_named("hpbooster");
    items.items_sprite = create_sprite(items_icon);

    float spriteWidth = sprite_width(items.items_sprite);
    float spriteHeight = sprite_height(items.items_sprite);

    float spriteOffsetX = spriteWidth/2;
    float spriteOffsetY = spriteHeight/2;

    items.spawnPoint = {spawnPoint.x - spriteOffsetX, spawnPoint.y - spriteOffsetY};

    sprite_set_x(items.items_sprite, spawnPoint.x - spriteOffsetX);
    sprite_set_y(items.items_sprite, spawnPoint.y - spriteOffsetY);

    items.calculate_rotation(tarLocation);

    float sprite_size = 0.5f;
    sprite_set_scale(items.items_sprite, sprite_size);

    items.calculate_velocity(tarLocation);

    return items;

}

void draw_items(vector<items_data> &item)
{
    for(items_data items : item )
    {
        draw_sprite(items.items_sprite);
    }
}

void update_items(vector<items_data> &item)
{
    for (items_data items : item)
    {
        //change in a value over time is referred to as a delta
        vector_2d currentPos = vector_to(sprite_position(items.items_sprite));
        vector_2d deltaPos = vector_add(currentPos, items.velocity);
        point_2d deltaPosPoint = {deltaPos.x, deltaPos.y};

                //----

        //Boundaries Window 1080 x 800
        float LeftWindowBoundary = 0;
        float RightWindowBoundary = 1080;
        float UpperWindowBoundary = 0;
        float LowerWindowBoundary = 800;

        //Get the sprite position and sprite offset
        point_2d bulletCurrentPosition = center_point(items.items_sprite);

        float sprWidth = sprite_width(items.items_sprite);
        float sprHeight = sprite_height(items.items_sprite);
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
            sprite_set_position(items.items_sprite, items.spawnPoint);
            return;
        }
        else
        {
            sprite_set_position(items.items_sprite, deltaPosPoint);
        }
        
    }
}

void items_data::calculate_rotation(point_2d &tarLocation)
{
    sprite itemsSpr = this->items_sprite;
    vector_2d currLocation = {center_point(itemsSpr).x, center_point(itemsSpr).y};
    vector_2d tarVector = vector_to(tarLocation);

    float rotationAngle = angle_between(tarVector, currLocation) + 90;
    sprite_set_rotation(this->items_sprite, rotationAngle);
}

void items_data::calculate_velocity(point_2d &tarLocation)
{
    sprite itemsSpr = this->items_sprite;
    point_2d currLocation = center_point(itemsSpr);

    vector_2d distance = vector_point_to_point(currLocation, tarLocation);
    vector_2d velocityUnitVector = unit_vector(distance);
    vector_2d currentVelocity = vector_multiply(velocityUnitVector, ITEMS_VELOCITY_MAX);

    this->velocity = currentVelocity;
}


bool items_data::check_hpAdded(vector<player_data> &players, vector<items_data> items)
{
    vector<items_data> bullets_to_remove;
    vector<items_data> items_to_remove;
    for (player_data player : players)
    {
        if(sprite_collision(this->items_sprite, player.player_sprite))
        {
            //Do collision stuff here
            player.remove(items);
            return true;
        }
    }
    return false;
}

void items_data::remove(vector<items_data> & items)
{
    int index = 0;
    int indexFound;
    for (items_data item : items)
    {
        if (item.operator==(*this)){
            indexFound = index;
            break;
        }
        index++;
    }
    items.erase(items.begin() + indexFound);
}

item_spawner_data new_items_spawner(){

    item_spawner_data items_spawer;
    
    //Fixed spawn points
    items_spawer.spawnPoints[0] = {-200, 600}; //Bottom left
    items_spawer.spawnPoints[1] = {-200, 200}; //upper left
    items_spawer.spawnPoints[2] = {216, -200}; //left most top
    items_spawer.spawnPoints[3] = {432, -200}; //center left top
    items_spawer.spawnPoints[4] = {648, -200}; //center right top
    items_spawer.spawnPoints[5] = {864, -200}; //right most top
    items_spawer.spawnPoints[6] = {1280, 200}; //upper right
    items_spawer.spawnPoints[7] = {1280, 600}; //bottom right
    
    items_spawer.spawnRate = 2.0; //This could be defined globally

    items_spawer.previousSpawnTime = 0;    

    return items_spawer;   
}

bool item_spawner_data::spawn_item(float &currentTime)
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