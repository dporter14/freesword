#include <iostream>
#include <math.h>
#include <ctime>
#include "global.h"

void jacob_func() 
{
    strcpy(g.title.text,"Jacob test");
    g.title.text_color = 0x00ffffff;
}

//init player
void Player::init()
{
    //player shape radius
    pradius = 30;
    state = 0;
    max_speed = 8;
    //VecMake(0,1,0,dir);
    rot = 0;
    VecMake(0,0,0,vel);
    VecMake(50,50,0,pos);
    VecMake(0.5, 0.0, 0.5, color);
    VecMake(35, 0, 0,rhand_pos);
    //VecMake(0,1,0,rhand_dir);
    rhand_rot = 0;

    VecCopy(pos, hitbox.pos);
    hitbox.scale[0] = hitbox.scale[1] = pradius/1.41;
    hitbox.dynamic=1;

    VecCopy(pos, hitbox.pos);
    hitbox.scale[0] = hitbox.scale[1] = pradius/1.41;

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

//relatively move player
void Character::addPos(Flt x, Flt y)
{
    pos[0] += x;
    pos[1] += y;
    VecCopy(pos, hitbox.pos);

}

//manually change velocity
void Player::setVel(Flt x, Flt y)
{
    vel[0] = x;
    vel[1] = y;

    //make sure velocity is within speed bounds
    vel[0] = CLAMP(vel[0], -this->max_speed, this->max_speed);
    vel[1] = CLAMP(vel[1], -this->max_speed, this->max_speed);
}

//relatively change velocity
void Player::addVel(Flt x, Flt y)
{
    vel[0] += x;
    vel[1] += y;

    //make sure velocity is within speed bounds
    vel[0] = CLAMP(vel[0], -this->max_speed, this->max_speed);
    vel[1] = CLAMP(vel[1], -this->max_speed, this->max_speed);
}

void Enemy::setVel(Flt x, Flt y)
{
    vel[0] = x;
    vel[1] = y;

    //make sure velocity is within speed bounds
    vel[0] = CLAMP(vel[0], -this->max_speed, this->max_speed);
    vel[1] = CLAMP(vel[1], -this->max_speed, this->max_speed);
}

//relatively change velocity
void Enemy::addVel(Flt x, Flt y)
{
    vel[0] += x;
    vel[1] += y;

    //make sure velocity is within speed bounds
    vel[0] = CLAMP(vel[0], -this->max_speed, this->max_speed);
    vel[1] = CLAMP(vel[1], -this->max_speed, this->max_speed);
}

void Character::lookAt(Flt x, Flt y)
{
    //Set direction to a unit vector pointing at x and y

    Vec dir;
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
    Vec base, upz, cross;
    VecMake(0, 1, 0, base);
    VecMake(0, 0, 1, upz);
    float angl = acos(VecDot(dir, base));	
    VecCross(base, dir, cross);
    if (VecDot(upz, cross)<0)
        angl = -angl;
    rot = angl*180/PI;

}

void Wall::initWall(Flt initx, Flt inity, Flt width, Flt height)
{   

    pos[0] = initx;
    pos[1] = inity;
    scale[0] = width;
    scale[1] = height;
    left = pos[0]-(scale[0]);
    right = pos[0]+(scale[0]);
    top = pos[1]+(scale[1]);
    bot = pos[1]-(scale[1]);

    VecMake(1.0, 0.0, 0.0, color);
    hitbox.type = H_HURTBOX;
    VecCopy(pos, hitbox.pos);
    VecCopy(scale, hitbox.scale);
}

void Wall::draw(){

    //lab7 changes
    static double runt = 0.0;
    static char* info_here = g.info.get_place();
    double startTime, endTime;
    startTime = current_time();

    glColor3f(color[0],color[1],color[2]);

    glPushMatrix();
    glTranslatef(pos[0], pos[1], 0);
    glBegin(GL_POLYGON);
    glVertex2f(-(scale[0]), scale[1]);
    glVertex2f(-(scale[0]), -(scale[1]));
    glVertex2f(scale[0], -(scale[1]));
    glVertex2f(scale[0], scale[1]);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(hitbox.pos[0], hitbox.pos[1], 0);
    if (g.state[S_DEBUG]) {
        glColor3f(0,0,1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(hitbox.scale[0], hitbox.scale[1]);
        glVertex2f(-hitbox.scale[0], hitbox.scale[1]);
        glVertex2f(-hitbox.scale[0], -hitbox.scale[1]);
        glVertex2f(hitbox.scale[0], -hitbox.scale[1]);
        glEnd();

    }
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
            if (openedFrom == false) {
                pos[0] = pos[0] - (scale[1]) + (scale[0]);
                pos[1] = pos[1] + (scale[1]) - (scale[0]);
            } else {
                pos[0] = pos[0] + (scale[0]) - (scale[1]);
                pos[1] = pos[1] + (scale[0]) - (scale[1]);
            }
        } else {
            if (g.player.pos[1] > pos[1]) {
                pos[0] = pos[0] - (scale[1]) + (scale[0]);
                pos[1] = pos[1] + (scale[1]) - (scale[0]);
                openedFrom = false;
            } else {
                pos[0] = pos[0] + (scale[0]) - (scale[1]);
                pos[1] = pos[1] + (scale[0]) - (scale[1]);
                openedFrom = true;
            }
        }
    } else {
        if (isOpen) {
            if (openedFrom == false) {
                pos[0] = pos[0] - (scale[0]) + (scale[1]);
                pos[1] = pos[1] - (scale[0]) + (scale[1]);
            } else {
                pos[0] = pos[0] + (scale[0]) - (scale[1]);
                pos[1] = pos[1] - (scale[0]) + (scale[1]);
            }
        } else {
            if (g.player.pos[0] < pos[0]) {
                pos[0] = pos[0] - (scale[0]) + (scale[1]);
                pos[1] = pos[1] - (scale[0]) + (scale[1]);
                openedFrom = false;
            } else {
                pos[0] = pos[0] + (scale[0]) - (scale[1]);
                pos[1] = pos[1] - (scale[0]) + (scale[1]);
                openedFrom = true;
            }
        }
    }

    Flt tmp = scale[1];
    scale[1] = scale[0];
    scale[0] = tmp;
    left = pos[0]-(scale[0]);
    right = pos[0]+(scale[0]);
    top = pos[1]+(scale[1]);
    bot = pos[1]-(scale[1]);
    isOpen = !isOpen;

    VecCopy(pos, hitbox.pos);
    VecCopy(scale, hitbox.scale);
    Vec temp;
    VecMake(20, 20, 0, temp);
    VecAdd(temp, hitbox.scale, temp);
    VecCopy(pos, trigger.pos);
    VecCopy(temp, trigger.scale);

}

void Door::draw()
{
    Wall::draw();

    glPushMatrix();
    glTranslatef(trigger.pos[0], trigger.pos[1], 0);
    if (g.state[S_DEBUG]) {
        glColor3f(0,1,1);
        glBegin(GL_LINE_LOOP);
        glVertex2f(trigger.scale[0], trigger.scale[1]);
        glVertex2f(-trigger.scale[0], trigger.scale[1]);
        glVertex2f(-trigger.scale[0], -trigger.scale[1]);
        glVertex2f(trigger.scale[0], -trigger.scale[1]);
        glEnd();
    }
    glPopMatrix();
}

void Door::initDoor(Flt initx, Flt inity, Flt width, Flt height, bool horz)
{
    Wall::initWall(initx, inity, width, height);
    isHoriz = horz;
    isOpen = false;

    VecMake(0.0, 0.0, 1.0, color);

    Vec temp;
    VecMake(20, 20, 0, temp);
    VecAdd(temp, scale, temp);

    trigger.type = H_TRIGGER;
    VecCopy(pos, trigger.pos);
    VecCopy(temp, trigger.scale); 
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

void doorCollision(Door object, Enemy& being)
{
    if (being.pos[0] >= object.left-10 && being.pos[0] <= object.right+10
            && being.pos[1] <= object.top+10 && being.pos[1] >= object.bot-10) {
        if (being.pos[0] < object.left+5)
            being.pos[0] = object.left-10;
        else if (being.pos[0] > object.right-5) 
            being.pos[0] = object.right+10;
        if (being.pos[1] < object.bot+5)
            being.pos[1] = object.bot-10;
        else if (being.pos[1] > object.top-5)
            being.pos[1] = object.top+10;
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
    walls[0].initWall(15.0, 750.0, 15.0, 150.0);
    doors[0].initDoor(104.0, 607.5, 75.0, 7.5, true);
}

void createWall(int mousex, int mousey) 
{
    if (g.number[N_WALLS] < 100) {
        g.number[N_WALLS]++;
        g.level1.walls[g.number[N_WALLS]].initWall(mousex, mousey, 12.5, 12.5);
    }

    Flt x = g.level1.walls[g.number[N_WALLS]].pos[0];
    Flt y = g.level1.walls[g.number[N_WALLS]].pos[1];

    printf("x = %f\n", x);
    printf("y = %f\n", y);

    for (Flt i=0; i<49; i++) {
        if (x<=(i*25) && x>=((i-1)*25)) {
            x = ((i*25)-12.5);
            break;
        }
    }
    for (Flt i=0; i<37; i++) {
        if (y<=(i*25) && y>=((i-1)*25)) {
            y = ((i*25)-12.5);
            break;
        }
    }

    printf("x = %f\n", x);
    printf("y = %f\n", y);

    g.level1.walls[g.number[N_WALLS]].pos[0] = x;
    g.level1.walls[g.number[N_WALLS]].pos[1] = y;

}

void dragWall(int mousex, int mousey) 
{
    for (int i=0; i<g.number[N_WALLS]; i++) {
        if (mousex<=g.level1.walls[i].pos[0]+12.5 && mousex>=g.level1.walls[i].pos[0]-12.5) {
            if (mousey<=g.level1.walls[i].pos[1]+12.5 && mousey>=g.level1.walls[i].pos[1]-12.5) {
                g.level1.walls[i].pos[0] = (floor(mousex/25) * 25) - 12.5;
                g.level1.walls[i].pos[1] = (floor(mousey/25) * 25) - 12.5;
                break;
            }
        }
    }
}
