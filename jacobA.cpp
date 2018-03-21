#include <iostream>
#include <math.h>
#include <ctime>
#include "global.h"

void jacob_func(){
	strcpy(g.title.text,"Jacob test");
	g.title.text_color = 0x00ffffff;
}

#define PLAYER_MAX_SPEED 10

//init player
void Player::init()
{
	//player shape radius
	pradius = 30;
	state = 0;
	VecMake(0,1,0,dir);
	VecMake(0,0,0,vel);
	VecMake(50,50,0,pos);
	VecMake(0.5, 0.0, 0.5, color);
	VecMake(35,0,0,rhand_pos);
	VecMake(0,1,0,rhand_dir);
	
	VecCopy(pos, hitbox.pos);
	hitbox.width = hitbox.height = pradius/1.41;

}

//move player according to its velocity
void Character::move()
{
    pos[0] += vel[0];
	pos[1] += vel[1];
	VecCopy(pos, hitbox.pos);
	
}

//manually move player
void Character::setPos(Flt x, Flt y)
{
	pos[0] = x;
	pos[1] = y;
	VecCopy(pos, hitbox.pos);
	
}

//manually change velocity
void Character::setVel(Flt x, Flt y)
{
	vel[0] = x;
	vel[1] = y;

	//make sure velocity is within speed bounds
	vel[0] = CLAMP(vel[0], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);
	vel[1] = CLAMP(vel[1], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);
}

//relatively change velocity
void Character::addVel(Flt x, Flt y)
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
	if (ABS(dir[0])>ABS(dir[1])){
		dir[0]=SGN(dir[0]);
		dir[1]=0;
	} else {
		dir[1]=SGN(dir[1]);
		dir[0]=0;
	}
}

void initWalls()
{
    g.n.width = g.xres;
    g.n.height = 5;
    g.n.x = g.xres/2;
    g.n.y = 0;

    g.e.width = 5;
    g.e.height = g.yres;
    g.e.x = g.xres;
    g.e.y = g.yres/2;

    g.s.width = g.xres;
    g.s.height = 5;
    g.s.x = g.xres/2;
    g.s.y = g.yres;

    g.w.width = 5;
    g.w.height = g.yres;
    g.w.x = 0;
    g.w.y = g.yres/2;
}

void Wall::draw(){

	//lab7 changes
    static double runt = 0.0;
    static char* info_here = g.info.get_place();
    double startTime, endTime;
    startTime = current_time();

    glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-(width/2), height/2);
    glVertex2f(-(width/2), -(height/2));
    glVertex2f(width/2, -(height/2));
    glVertex2f(width/2, height/2);
    glEnd();
    glPopMatrix();

    endTime = current_time();
    runt += endTime - startTime;
	sprintf(info_here, "Wall Draw function: %f", runt);
	
}
