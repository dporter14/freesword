#include <iostream>
#include <math.h>
#include <ctime>
#include "global.h"

double timeDiff(struct timespec*, struct timespec*);

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
	status = 0;
	VecMake(0,1,0,dir);
	VecMake(0,0,0,vel);
	VecMake(50,50,0,pos);
	VecMake(0.5, 0.0, 0.5, color);
	VecMake(35,0,0,rhand_pos);
	VecMake(0,1,0,rhand_dir);
	
	
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

double Wall::draw(){
   
	//lab7 changes
    static double runt = 0.0;
    struct timespec startTime, endTime;
    clock_gettime(CLOCK_REALTIME, &startTime);
   
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
    
    clock_gettime(CLOCK_REALTIME, &endTime);
    runt += timeDiff(&startTime, &endTime);

    return(runt);
}

void Door::swing()
{
    if (isOpen) {
        x = x - (height/2) + (width/2);
        y = y - (height/2) + (width/2);
    } else {
        x = x + (width/2) - (height/2);
        y = y + (width/2) - (height/2);
    }
    
    Flt tmp = height;
    height = width;
    width = tmp;
    left = x-(width/2);
    right = x+(width/2);
    top = y+(height/2);
    bot = y-(height/2);
    isOpen = !isOpen;
    
}

void Door::draw()
{
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
}

void Door::initDoor()
{
    x = g.xres/2;
    y = g.yres/2;
    width = 500;
    height = 50;
    isOpen = false;
    left = x-(width/2);
    right = x+(width/2);
    top = y+(height/2);
    bot = y-(height/2);
}

void interactDoor()
{
    for (int i=0; i<4; i++) {
        if (g.player.pos[0] <= g.doors[i].right+50 && 
                g.player.pos[0] >= g.doors[i].left-50) {
            if (g.player.pos[1] <= g.doors[i].top+50 &&
                    g.player.pos[1] >= g.doors[i].bot-50) {
                g.doors[i].swing();
            }
        }
    }
}

void collide(Door object)
{
    if (g.player.pos[0] >= object.left-10 && g.player.pos[0] <= object.right+10
        && g.player.pos[1] <= object.top+10 && g.player.pos[1] >= object.bot-10) {
        if (g.player.pos[0] < object.left+5)
            g.player.pos[0] = object.left-10;
        else if (g.player.pos[0] > object.right-5) 
            g.player.pos[0] = object.right+10;
        if (g.player.pos[1] < object.bot+5)
            g.player.pos[1] = object.bot-10;
        else if (g.player.pos[1] > object.top-5)
            g.player.pos[1] = object.top+10;
    }
    
}
        
