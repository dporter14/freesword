//Author: Jacob Abbott
#include <iostream>
#include <math.h>
#include <ctime>
#include "global.h"


void toggleEditMode() 
{
	if (g.state[S_LEVELEDIT] != 1) {
		g.state[S_LEVELEDIT] = 1;
		strcpy(g.title.text,"Level Editor Enabled");
		g.title.text_color = 0x39ff14;
	} else {
		g.state[S_LEVELEDIT] = 0;
		strcpy(g.title.text, "");
		g.title.text_color = 0x00ffffff;
	}
}


//init player
void Player::init()
{
    //player shape radius
    pradius = 30;
    VecMake(50,75,0,scale);
    //player shape
    state = 0;
    max_speed = 8;
    //VecMake(0,1,0,dir);
    rot = 0;
    VecMake(0,0,0,vel);
    VecMake(50,50,0,pos);
    VecMake(1.0, 1.0, 1.0, color);
    VecMake(16, 0, 0,rhand_pos);
    //VecMake(0,1,0,rhand_dir);
    rhand_rot = 0;

	VecMake(0, -30, 0, hitbox_offset); // shift hitbox
    VecAdd(pos, hitbox_offset, hitbox.pos);
    
    VecMake(30, 30, 0, hitbox.scale);
    hitbox.dynamic=1;
	
	sprt = &g.sprites[SB_PLAYER_F];
}

//move player according to its velocity
void Character::move()
{
	pos[0] += vel[0];
	pos[1] += vel[1];
	VecAdd(pos, hitbox_offset, hitbox.pos);
    
}

//manually move player
void Character::setPos(Flt x, Flt y)
{
	pos[0] = x;
	pos[1] = y;
	VecAdd(pos, hitbox_offset, hitbox.pos);
    
}

//relatively move player
void Character::addPos(Flt x, Flt y)
{
	pos[0] += x;
	pos[1] += y;
	VecAdd(pos, hitbox_offset, hitbox.pos);
    
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
	swapSprites();
	Vec base, upz, cross;
	VecMake(0, 1, 0, base);
	VecMake(0, 0, 1, upz);
	float angl = acos(VecDot(dir, base));	
	VecCross(base, dir, cross);
	if (VecDot(upz, cross)<0)
		angl = -angl;
	rot = angl*180/PI;

}

void Player::die() 
{
	state = S_CHAR_DEAD;
	g.state[S_GAMEOVER] = 1;
}

void Wall::initWall(Flt initx, Flt inity, Flt width, Flt height)
{   
	initx = round((initx/25))*25;
	inity = round((inity/25))*25;

	printf("initx: %f\n", initx);
	printf("inity: %f\n", inity);

	pos[0] = initx;
	pos[1] = inity;
	scale[0] = width;
	scale[1] = height;
	left = pos[0]-(scale[0]);
	right = pos[0]+(scale[0]);
	top = pos[1]+(scale[1]);
	bot = pos[1]-(scale[1]);

	VecMake(1.0, 1.0, 1.0, color);
	hitbox.type = H_HURTBOX;
	VecCopy(pos, hitbox.pos);
	VecCopy(scale, hitbox.scale);
	
	sprt = &g.sprites[SB_TILE_STONE];
}

void Wall::draw(){

	//lab7 changes
	static double runt = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();
	//printf("Blah: %s",sprt);
	drawSprite();
	/*
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
	*/
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

//rewritten 
/*
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
*/

void wallCollision(Wall& o1, Character& o2){
	/*static char* derp = g.info.get_place();
	if(&g.player == &o2)
		sprintf(derp, "col: %0.2f", g.player.vel[0]);
	*/
	Hitbox h1 = o1.hitbox, h2 = o2.hitbox;
	//Log("Dimensions: %0.3f %0.3f\n", h1.scale[0], h2.scale[0]);
	float t = 0;
	bool flag=0;
	float xt1, xt2, yt1, yt2, xt, yt;
	if(h1.intersect(h2)){ 
		
		// magic
		if (ABS(o2.vel[0]) > 1e-8) {
			flag = 1;
			xt1 = ((h1.pos[0]-h1.scale[0]) - 
				(h2.pos[0]+h2.scale[0])) / (o2.vel[0]);
			xt2 = ((h1.pos[0]+h1.scale[0]) - 
				(h2.pos[0]-h2.scale[0])) / (o2.vel[0]);
			//Log("Dimensions: %0.3f %0.3f\n", xt1, xt2);
		} else {
			xt1 = xt2 = 1e10;
		}
		
		if (ABS(o2.vel[1]) > 1e-8) {
			flag = 1;
			yt1 = ((h1.pos[1]-h1.scale[1]) - 
				(h2.pos[1]+h2.scale[1])) / (o2.vel[1]);
			yt2 = ((h1.pos[1]+h1.scale[1]) - 
				(h2.pos[1]-h2.scale[1])) / (o2.vel[1]);
		} else {
			yt1 = yt2 = 1e10;
		}
		
		if (flag) {
			xt =  (ABS(xt1) < ABS(xt2)) ? xt1 : xt2;
			yt =  (ABS(yt1) < ABS(yt2)) ? yt1 : yt2;
			t =  (ABS(xt) < ABS(yt)) ? xt : yt;
			t = CLAMP(t * 1.0001, -1.5, 1.5); //magic
			//Log("Dimensions: %0.3f %0.3f\n", xt, yt);
			
			if (ABS(xt) < ABS(yt)) {
				o2.addPos(o2.vel[0]*t, 0);
				
				float temp = -o2.vel[0];
				o2.setVel(temp*0.2, o2.vel[1]);
			} else {
				o2.addPos(0, o2.vel[1]*t);
				
				float temp = -o2.vel[1];
				o2.setVel(o2.vel[0], temp*0.2);
			}
		}
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
	isHoriz = horz;
	isOpen = false;
	
	Flt tempf;
	if (isHoriz) {
		Wall::initWall(initx, inity, width, height);
	} else {
		tempf = width;
		width = height;
		height = tempf;
		Wall::initWall(initx, inity, width, height);
	}

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
	for (int i=0; i<g.number[N_DOORS]; i++) {
		if(g.player.hitbox.intersect(g.level.doors[i].trigger))
			g.level.doors[i].swing();
	/*
		if (g.player.pos[0] <= g.level.doors[i].right+50 && 
				g.player.pos[0] >= g.level.doors[i].left-50) {
			if (g.player.pos[1] <= g.level.doors[i].top+50 &&
					g.player.pos[1] >= g.level.doors[i].bot-50) {
				g.level.doors[i].swing();
			}
		}
		*/
	}
}

//rewritten and handled by wall collision
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
//rewritten and handled by wall collision
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

void createWall(int mousex, int mousey) 
{
	for (int i=0; i<g.number[N_WALLS]; i++) {
		//std::cout << round((mousex/25))*25 << " " << round((mousey/25))*25 << std::endl;
		if (g.level.walls[i].pos[0] == round((mousex/25))*25 && g.level.walls[i].pos[1] == round((mousey/25))*25) {
			return;
		}
	}
	if (g.number[N_WALLS] < 1000) {
		g.level.walls[g.number[N_WALLS]].initWall(mousex, mousey, 25.0, 25.0);
		g.number[N_WALLS]++;
		std::cout << "# of walls: " << g.number[N_WALLS] << std::endl;   
	} else {
		std::cout << "Error: Too many walls.\n" << std::endl;
	}
	
}

void dragWall(int mousex, int mousey) 
{
	static int selectedWall;
	if (g.doorChange == false)
		return;
	if (g.wallChange == true && g.number[N_WALLS]>0) {
		for (int i=0; i<g.number[N_WALLS]; i++) {
			if (mousex<=g.level.walls[i].pos[0]+12.5 && mousex>=g.level.walls[i].pos[0]-12.5) {
				if (mousey<=g.level.walls[i].pos[1]+12.5 && mousey>=g.level.walls[i].pos[1]-12.5) {
					printf("Wall #%d\n", i);
					selectedWall = i;
					g.level.walls[selectedWall].initWall(mousex, mousey, 25.0, 25.0);
					g.wallChange = false;
					return;
				}
			}
		}
		return;
	} else if (g.wallChange == false && g.number[N_WALLS]>0) {
		g.level.walls[selectedWall].initWall(mousex, mousey, 25.0, 25.0);
	}
}

void createDoor(int mousex, int mousey)
{
	if (g.wallChange == false)
		return;
	for (int i=0; i<g.number[N_DOORS]; i++) {
		if (g.level.doors[i].pos[0] == round((mousex/25))*25 && g.level.doors[i].pos[1] == round((mousey/25))*25) {
			return;
		}
	}
	if (g.number[N_DOORS] < 1000) {
		g.level.doors[g.number[N_DOORS]].initDoor(mousex, mousey, 50.0, 12.5, true);
		g.number[N_DOORS]++;
	}
}

void dragDoor(int mousex, int mousey) 
{
	static int selectedDoor;
	if (g.doorChange == true && g.number[N_DOORS]>0) {
		for (int i=0; i<g.number[N_DOORS]; i++) {
			if (mousex<=g.level.doors[i].pos[0]+12.5 && mousex>=g.level.doors[i].pos[0]-12.5) {
				if (mousey<=g.level.doors[i].pos[1]+12.5 && mousey>=g.level.doors[i].pos[1]-12.5) {
					printf("Door #%d\n", i);
					selectedDoor = i;
					g.level.doors[selectedDoor].initDoor(mousex, mousey, 50.0, 12.5, g.level.doors[selectedDoor].isHoriz);
					g.doorChange = false;
					return;
				}
			}
		}
		return;
	} else if (g.doorChange == false && g.number[N_DOORS]>0) {
		g.level.doors[selectedDoor].initDoor(mousex, mousey, 50.0, 12.5, g.level.doors[selectedDoor].isHoriz);
	}
}

void rotateDoor(int mousex, int mousey) 
{
	int selectedDoor;
	if (g.number[N_DOORS]>0) {
		for (int i=0; i<g.number[N_DOORS]; i++) {
			if (mousex<=g.level.doors[i].pos[0]+12.5 && mousex>=g.level.doors[i].pos[0]-12.5) {
				if (mousey<=g.level.doors[i].pos[1]+12.5 && mousey>=g.level.doors[i].pos[1]-12.5) {
					printf("Rotate door #%d\n", i);
					selectedDoor = i;
					g.level.doors[selectedDoor].rotate();
					break;
				}
			}
		}
	}

}

void Door::rotate()
{
	Flt tmp = scale[1];
	scale[1] = scale[0];
	scale[0] = tmp;
	left = pos[0]-(scale[0]);
	right = pos[0]+(scale[0]);
	top = pos[1]+(scale[1]);
	bot = pos[1]-(scale[1]);
	isHoriz = !isHoriz;

	VecCopy(pos, hitbox.pos);
	VecCopy(scale, hitbox.scale);
	Vec temp;
	VecMake(20, 20, 0, temp);
	VecAdd(temp, hitbox.scale, temp);
	VecCopy(pos, trigger.pos);
	VecCopy(temp, trigger.scale);

}

void saveLevel()
{
	char levelName[10] = "levelx";
	std::cout << "This will overwrite any existing file with your provided name.\nEnter level name: ";
	std::cin >> levelName;
	std::ofstream levelOF;
	levelOF.open (levelName);
	for (int i=0; i<g.number[N_WALLS]; i++) {
		levelOF << "wall " << g.level.walls[i].pos[0] << " " << g.level.walls[i].pos[1] << "\n";
		printf("Saved Wall to level file\n");
	}
	for (int i=0; i<g.number[N_DOORS]; i++) {
		levelOF << "door " << g.level.doors[i].pos[0] << " " << g.level.doors[i].pos[1] << " " << g.level.doors[i].isHoriz << "\n";
		printf("Saved Door to level file\n");
	}
	for (int i=0; i<g.number[N_ENEMIES]; i++) {
		levelOF << "enemy " << g.enemies[i].pos[0] << " " << g.enemies[i].pos[1] << g.enemies[i].rot << "\n";
		printf("Saved enemy to level file\n");
	}
	for (std::map<std::string,int>::iterator it=g.tilemap.begin(); it!=g.tilemap.end(); ++it){
    	levelOF << "tile " << it->first << " " << it->second << "\n";
    	printf("Saved floor tile to level file\n");
    }

	levelOF << "end\n";
	levelOF.close();
}

void loadLevel(char *levelName)
{
	std::ifstream levelread;
	levelread.open (levelName);
	if (levelread.is_open()) {
			char object[6];
			Flt x, y, horiz, rot;
		while(!levelread.eof()) {
			levelread >> object;
			std::cout << object << std::endl;
			if (!strcmp("wall", object)) {
				levelread >> x;
				levelread >> y;
				g.level.walls[g.number[N_WALLS]].initWall(x, y, 25.0, 25.0); 
				g.number[N_WALLS]++;   
				std::cout << "Spawned wall\n" << std::endl;
			} else if (!strcmp("door", object)) {
				levelread >> x;
				levelread >> y;
				levelread >> horiz;
				g.level.doors[g.number[N_DOORS]].initDoor(x, y, 50.0, 12.5, horiz);
				g.number[N_DOORS]++; 
				std::cout << "Spawned door\n" << std::endl;  
			} else if (!strcmp("enemy", object)) {
				levelread >> x;
				levelread >> y;
				levelread >> rot;
				spawnEnemy(x, y, rot);
				std::cout << "Spawned enemy" << std::endl;
			} else if (!strcmp("tile",object)) {
				int tile, x, y;
				levelread >> x;
				levelread >> y;
				levelread >> tile;
				char temp[20];
				sprintf(temp,"%d %d",x,y);
				g.tilemap[temp]=tile;
			} else if (!strcmp("end", object)) {
				break;
			} else {
				std::cout << "Error: Unrecognized data\n" << std::endl;
				exit(1);
			}
				
		}
	} else {
		printf("Unable to open file\n");
		exit(1);
	}
	levelread.close();
}

