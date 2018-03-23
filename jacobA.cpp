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
    
void Wall::initWall(Flt initx, Flt inity, Flt initWidth, Flt initHeight)
{   
/*  g.n.width = g.xres;
    g.n.height = 5;
    g.n.pos[0]= g.xres/2;
    g.n.pos[1]= 0;

    g.e.width = 5;
    g.e.height = g.yres;
    g.e.pos[0]= g.xres;
    g.e.pos[1]= g.yres/2;

    g.s.width = g.xres;
    g.s.height = 5;
    g.s.pos[0]= g.xres/2;
    g.s.pos[1]= g.yres;

    g.w.width = 5;
    g.w.height = g.yres;
    g.w.x = 0;
    g.w.y = g.yres/2;
*/
    pos[0] = initx;
    pos[1] = inity;
    width = initWidth;
    height = initHeight;
    left = pos[0]-(width/2);
    right = pos[0]+(width/2);
    top = pos[1]+(height/2);
    bot = pos[1]-(height/2);

}

void Wall::draw(){

	//lab7 changes
    static double runt = 0.0;
    static char* info_here = g.info.get_place();
    double startTime, endTime;
    startTime = current_time();

    glColor3f(1.0, 0.0, 0.0);

    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
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

void wallCollision(Wall object, Enemy being, int num)
{
    if (being.pos[0] >= object.left-10 && being.pos[0] <= object.right+10
        && being.pos[1] <= object.top+10 && being.pos[1] >= object.bot-10) {
        if (being.pos[0] < object.left+5)
            g.enemies[num].pos[0] = object.left-10;
        else if (being.pos[0] > object.right-5) 
            g.enemies[num].pos[0] = object.right+10;
        if (being.pos[1] < object.bot+5)
            g.enemies[num].pos[1] = object.bot-10;
        else if (being.pos[1] > object.top-5)
            g.enemies[num].pos[1] = object.top+10;
    }
    
}

void wallCollision(Wall object, Player being)
{
    if (being.pos[0] >= object.left-10 && being.pos[0] <= object.right+10
        && being.pos[1] <= object.top+10 && being.pos[1] >= object.bot-10) {
        if (being.pos[0] < object.left+5)
            g.player.pos[0] = object.left-10;
        else if (being.pos[0] > object.right-5) 
            g.player.pos[0] = object.right+10;
        if (being.pos[1] < object.bot+5)
            g.player.pos[1] = object.bot-10;
        else if (being.pos[1] > object.top-5)
            g.player.pos[1] = object.top+10;
    }
    
}

void Door::swing()
{
    if (isHoriz) {
        if (isOpen) {
            pos[0] = pos[0] - (height/2) + (width/2);
            pos[1] = pos[1] - (height/2) + (width/2);
        } else {
            pos[0] = pos[0] + (width/2) - (height/2);
            pos[1] = pos[1] + (width/2) - (height/2);
        }
    }
    else {
        if (isOpen) {
            pos[0] = pos[0] - (width/2) + (height/2);
            pos[1] = pos[1] - (height/2) + (width/2);
        } else {
            pos[0] = pos[0] - (width/2) + (height/2);
            pos[1] = pos[1] + (width/2) - (height/2);
        }
    }

    Flt tmp = height;
    height = width;
    width = tmp;
    left = pos[0]-(width/2);
    right = pos[0]+(width/2);
    top = pos[1]+(height/2);
    bot = pos[1]-(height/2);
    isOpen = !isOpen;
    
}

void Door::draw()
{
    glColor3f(1.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
    glBegin(GL_POLYGON);
    glVertex2f(-(width/2), height/2);
    glVertex2f(-(width/2), -(height/2));
    glVertex2f(width/2, -(height/2));
    glVertex2f(width/2, height/2);
    glEnd();
    glPopMatrix();
}

void Door::initDoor(Flt initx, Flt inity, Flt initWidth, Flt initHeight, bool horz)
{
    pos[0] = initx;
    pos[1] = inity;
    width = initWidth;
    height = initHeight;
    isHoriz = horz;

    isOpen = false;
    left = pos[0]-(width/2);
    right = pos[0]+(width/2);
    top = pos[1]+(height/2);
    bot = pos[1]-(height/2);
}

void interactDoor()
{
    for (int i=0; i<100; i++) {
        if (g.player.pos[0] <= g.level1.doors[i].right+50 && 
                g.player.pos[0] >= g.level1.doors[i].left-50) {
            if (g.player.pos[1] <= g.level1.doors[i].top+50 &&
                    g.player.pos[1] >= g.level1.doors[i].bot-50) {
                g.level1.doors[i].swing();
            }
        }
    }
}

void doorCollision(Door object, Enemy being, int num)
{
    if (being.pos[0] >= object.left-10 && being.pos[0] <= object.right+10
        && being.pos[1] <= object.top+10 && being.pos[1] >= object.bot-10) {
        if (being.pos[0] < object.left+5)
            g.enemies[num].pos[0] = object.left-10;
        else if (being.pos[0] > object.right-5) 
            g.enemies[num].pos[0] = object.right+10;
        if (being.pos[1] < object.bot+5)
            g.enemies[num].pos[1] = object.bot-10;
        else if (being.pos[1] > object.top-5)
            g.enemies[num].pos[1] = object.top+10;
    }
    
}

void doorCollision(Door object, Player being)
{
    if (being.pos[0] >= object.left-10 && being.pos[0] <= object.right+10
        && being.pos[1] <= object.top+10 && being.pos[1] >= object.bot-10) {
        if (being.pos[0] < object.left+5)
            g.player.pos[0] = object.left-10;
        else if (being.pos[0] > object.right-5) 
            g.player.pos[0] = object.right+10;
        if (being.pos[1] < object.bot+5)
            g.player.pos[1] = object.bot-10;
        else if (being.pos[1] > object.top-5)
            g.player.pos[1] = object.top+10;
    }
    
}
       
void Level::buildLevel1()
{
    walls[0].initWall(0.0, 750.0, 45.0, 300.0);
    doors[0].initDoor(92.5, 615.0, 140.0, 30.0, true);
    doors[1].initDoor(g.xres/2, g.yres/2, 20.0, 200.0, false);
} 
