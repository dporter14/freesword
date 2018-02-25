#include <iostream>
#include <math.h>

#include "global.h"

#define PLAYER_MAX_SPEED 5

//init player
void Player::init()
{
	//player shape radius
	pradius = 30;
	status = 0;
	VecMake(0,1,0,dir);
	VecMake(0,0,0,vel);
	VecMake(50,50,0,pos);
	
	pointer[0] = 75;
	pointer[1] = 75;
	
}

//move player according to its velocity
void Character::move()
{
	pos[0] += vel[0]; 
	pos[1] += vel[1];
}

//manually move player
void Character::setPosition(Flt x, Flt y)
{
	pos[0] = x; 
	pos[1] = y;
}

//manually change velocity
void Character::setVelocity(Flt x, Flt y)
{
	vel[0] = x; 
	vel[1] = y;
	
	//make sure velocity is within speed bounds
	vel[0] = CLAMP(vel[0], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED); 
	vel[1] = CLAMP(vel[1], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);
}

//relatively change velocity
void Character::addVelocity(Flt x, Flt y)
{
	vel[0] += x; 
	vel[1] += y;
	
	//make sure velocity is within speed bounds
	vel[0] = CLAMP(vel[0], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED); 
	vel[1] = CLAMP(vel[1], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);
}

void Character::lookAt(Flt x, Flt y)
{
	//Set direction to a unit vector pointing at x and y
	VecMake(x-pos[0], y-pos[1], 0, dir);
	Flt scale = 1/VecLen(dir);
	VecS(scale, dir, dir);
	
}

void Player::lookAt(Flt x, Flt y)
{
	Character::lookAt(x, y);
	
	//Set player pointer to face direction
	VecAddS(pradius+1, dir, pos, pointer);
}

void jacob_func(){
	strcpy(g.title.text,"Jacob test");
	g.title.text_color = 0x00ffffff;
}