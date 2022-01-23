#include "player.h"
#include "Theitems.h"
#include "bullet.h"
#include "splashkit.h"


player_data new_player() //Factory function
{
    //Our player data
    player_data player;

    //Default sprite
    bitmap player_ship = bitmap_named("DefaultPlayerShip");
    player.player_sprite = create_sprite(player_ship);

    //Crosshair
    bitmap player_crosshair = bitmap_named("Crosshair_1");
    player.crosshair = create_sprite(player_crosshair);

    // Position in the centre of the initial screen
    float sprWidth = sprite_width(player.player_sprite);
    float sprHeight = sprite_height(player.player_sprite);

    float sprOffsetX = sprWidth/2;
    float sprOffsetY = sprHeight/2;

    sprite_set_x(player.player_sprite, 540 - sprOffsetX);
    sprite_set_y(player.player_sprite, 700 - sprOffsetY);

    //Create list of bullet objects
    vector<bullet_data> bullets;
    player.bullet_list = bullets;

    //UI section, Initial max health, and our health frame and bar components are created using the member functions
    player.health = MAX_PLAYER_HEALTH;
    // player.health = 10;
    player.healthFrame = player.calc_healthFrame();
    player.healthBar = player.calc_healthBar(player.healthFrame);
    player.isDead = false;

    return player;
}

void draw_player(const player_data &player)
{
    draw_sprite(player.player_sprite);
    draw_sprite(player.crosshair);
}

void update_player(player_data &player, vector<enemy_data> &enemies)
{
    //For now we'll assume that deltaT is occuring at a fixed rate based on the refresh rate
    float currentReloadTime = player.time_since_shot;
    float deltaT = (float)1/60;
    player.time_since_shot = currentReloadTime + deltaT; 

    //This update function is called each tick, and invokes the functions needed to update the player
    is_shooting(player);
    calculate_position(player);
    calculate_rotation(player);
    calculate_crosshair_position(player);

    //Damage taken/Collisions
    player.check_collisions(enemies);

    //Upate UI
    player.healthBar = player.calc_healthBar(player.healthFrame);
}

void calculate_position(player_data &player)
{
    //This function will perform the calculations to move the player each game tick

    //Where is the player
    double currentPosX = sprite_x(player.player_sprite);
    double currentPosY = sprite_y(player.player_sprite);
    point_2d currentPosition = {currentPosX, currentPosY};

    //Which direction and how fast are they moving?
    vector_2d acceleration = { 0.0, 0.0};

    //Boundaries Window 1080 x 800
    float LeftWindowBoundary = 0;
    float RightWindowBoundary = 1080;
    float UpperWindowBoundary = 0;
    float LowerWindowBoundary = 800;

    //Get the sprite position and sprite offset
    point_2d playerCurrentPosition = center_point(player.player_sprite);

    float sprWidth = sprite_width(player.player_sprite);
    float sprHeight = sprite_height(player.player_sprite);
    float sprOffsetX = sprWidth/2;
    float sprOffsetY = sprHeight/2;

    //Center point of the sprite, and adjust for the sprite offset
    float spriteLeftEdge = playerCurrentPosition.x - sprOffsetX;
    float spriteRightEdge = playerCurrentPosition.x + sprOffsetX;
    float spriteTopEdge = playerCurrentPosition.y -sprOffsetY;
    float spriteBottomEdge = playerCurrentPosition.y + sprOffsetY;

    //before we add our acceleration value - we should check to see if we're at boundary
    if (key_down(A_KEY) && (spriteLeftEdge > LeftWindowBoundary))
    {
        acceleration.x -= PLAYER_ACCELERATION;
    }
    if (key_down(D_KEY) && (spriteRightEdge < RightWindowBoundary))
    {
        acceleration.x += PLAYER_ACCELERATION;
    }
    if (key_down(W_KEY) && (spriteTopEdge > UpperWindowBoundary))
    {
        acceleration.y -= PLAYER_ACCELERATION;
    }
    if (key_down(S_KEY) && (spriteBottomEdge < LowerWindowBoundary))
    {
        acceleration.y += PLAYER_ACCELERATION;
    }

    //Where do they end up?
    point_2d newPosition = {currentPosition.x + acceleration.x, currentPosition.y + acceleration.y};
    sprite_set_position(player.player_sprite, newPosition);
}

void calculate_rotation(player_data &player)
{
    //This function performs the rotation calculations - which can be quite tricky
    vector_2d playerPos = {center_point(player.player_sprite).x, center_point(player.player_sprite).y};
    vector_2d mPos = mouse_position_vector();

    float rotationAngle = angle_between(playerPos, mPos) + 90;
    sprite_set_rotation(player.player_sprite, rotationAngle);
}

void calculate_crosshair_position(player_data &player)
{
    // Position of crosshair and offset
    float sprWidth = sprite_width(player.crosshair);
    float sprHeight = sprite_height(player.crosshair);
    float sprOffsetX = sprWidth/2;
    float sprOffsetY = sprHeight/2;

    float newXPos = mouse_position().x - sprOffsetX;
    float newYPos = mouse_position().y - sprOffsetY;

    point_2d newPos = {newXPos, newYPos};

    sprite_set_position(player.crosshair, newPos);
}

void is_shooting(player_data &player)
{
    sound_effect laser = sound_effect_named("SFX1");

    point_2d spawnPos = center_point(player.player_sprite); //We'll need to adjust this later to account for the rotation and length of the player ship
    float rotAngle = sprite_rotation(player.player_sprite);
        
    if (mouse_down(LEFT_BUTTON) && (player.time_since_shot >= RELOAD_SPEED))
    {
        play_sound_effect(laser);

        bullet_data bullet = new_bullet(spawnPos, rotAngle);
        player.bullet_list.push_back(bullet);
        player.time_since_shot = 0.0f;
    }
}

rectangle player_data::calc_healthFrame()
{
    //Need to use our window width for the frame size
    float windowWidth = 1080;
    point_2d healthFrameTopLeft = {40, 740};
    point_2d healthFrameBottomRight = {windowWidth-healthFrameTopLeft.x, 20 + healthFrameTopLeft.y};
    rectangle healthFrame = rectangle_from(healthFrameTopLeft, healthFrameBottomRight);
    return healthFrame;
}

rectangle player_data::calc_healthBar(rectangle &healthFrame)
{
    //Inside rectangle is created using an inset of the original frame
    float barInset = 2.0f;
    rectangle HealthBarBase = inset_rectangle(healthFrame, barInset);

    //The health percent variable represents how much health we still have out of 100
    //Health % = current health / max health
    //length of the bar should the health % multiplied by the total max length of the bar
    float healthPercent = HealthBarBase.width * (this->health/MAX_PLAYER_HEALTH);
    rectangle HealthBar =  rectangle_from(HealthBarBase.x, HealthBarBase.y, healthPercent, HealthBarBase.height);
    return HealthBar;
}

void draw_UI(const player_data &player)
{
    //Colours
    color frameColour = color_white_smoke();
    color barColour = color_cyan();

    // //Draw Healthbar
    draw_rectangle(frameColour, player.healthFrame);
    fill_rectangle(frameColour, player.healthFrame);
    draw_rectangle(barColour, player.healthBar);
    fill_rectangle(barColour, player.healthBar);
}

void player_data::check_collisions(vector<enemy_data> &enemies)
{
    for (enemy_data enemy : enemies)
    {
        if(sprite_collision(this->player_sprite, enemy.enemy_sprite))
        {
            //Collision has occured
            enemy.remove(enemies);

            //Take damage
            this->health = this->health - 10;
            if(this->health <= 0)
            {
                //Game over here
                this->isDead = true;
            }
        }
    }
}

void player_data::check_active_health(vector<items_data> &items)
{
    for (items_data item : items )
    {
        if(sprite_collision(this->player_sprite, item.items_sprite))
        {
            //Collision has occured
            while (this->health >= 100);
            {
            item.remove(items);
            }

            //add HP
            this->health = this->health + 20;
        }
    }
}