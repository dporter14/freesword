#include <iostream>
#include <math.h>

#include "global.h"
extern Global g;

#define DIRECTION_W	 0
#define DIRECTION_A	 1
#define DIRECTION_S	 2
#define DIRECTION_D	 3

//player shape radius
#define PRADIUS 25


#define PLAYER_MAX_SPEED 10
#define N_PLAYER_MAX_SPEED -10


void movePlayer(Player *player)
{
	switch (player->dir) {
		case DIRECTION_W: 
			if (player->vel[1] < PLAYER_MAX_SPEED) {
				player->vel[1] += 10; 
			} else {
				player->vel[1] = PLAYER_MAX_SPEED;
			}
			break;
		case DIRECTION_A: 
			if (player->vel[0] > N_PLAYER_MAX_SPEED) {
				player->vel[0] += -10;
			} else {
				player->vel[0] = N_PLAYER_MAX_SPEED;
			}
			break;
		case DIRECTION_S: 
			if (player->vel[1] > N_PLAYER_MAX_SPEED) {
				player->vel[1] += -10; 
			} else { 
				player->vel[1] = N_PLAYER_MAX_SPEED;
			}
			break;
		case DIRECTION_D: 
			if (player->vel[0] < PLAYER_MAX_SPEED) {
				player->vel[0] += 10;
			} else {
				player->vel[0] = PLAYER_MAX_SPEED;
			}
			break;
	}
}

void setPlayerOrientation(Player *player)
{
	if (player->orientation[0] == -1) {
		if (player->orientation[1] == -1) {
			//face SW
			player->pointer[0] = player->pos[0] - (sqrt((PRADIUS*PRADIUS)/2));
			player->pointer[1] = player->pos[1] - (sqrt((PRADIUS*PRADIUS)/2));
		}
		else if (player->orientation[1] == 0) {
			//face West
			player->pointer[0] = player->pos[0] - PRADIUS;
			player->pointer[1] = player->pos[1];
		}
		else if (player->orientation[1] == 1) {
			//face NW
			player->pointer[0] = player->pos[0] - (sqrt((PRADIUS*PRADIUS)/2));
			player->pointer[1] = player->pos[1] + (sqrt((PRADIUS*PRADIUS)/2));
		}
	}
	else if (player->orientation[0] == 0) {
		if (player->orientation[1] == -1) {
			//face South
			player->pointer[0] = player->pos[0];
			player->pointer[1] = player->pos[1] - PRADIUS;
		}
		else if (player->orientation[1] == 1) {
			//face North
			player->pointer[0] = player->pos[0];
			player->pointer[1] = player->pos[1] + (sqrt((PRADIUS*PRADIUS)/2));
		}
	}
	else if (player->orientation[0] == 1) {
		if (player->orientation[1] == -1) {
			//face SE
			player->pointer[0] = player->pos[0] + (sqrt((PRADIUS*PRADIUS)/2));
			player->pointer[1] = player->pos[1] - (sqrt((PRADIUS*PRADIUS)/2));
		}
		else if (player->orientation[1] == 0) {
			//face East
			player->pointer[0] = player->pos[0] + PRADIUS;
			player->pointer[1] = player->pos[1];
		}
		else if (player->orientation[1] == 1) {
			//face NE
			player->pointer[0] = player->pos[0] + (sqrt((PRADIUS*PRADIUS)/2));
			player->pointer[1] = player->pos[1] + (sqrt((PRADIUS*PRADIUS)/2));
		}
	}
}

using namespace std;
void jacob_func(){
	strcpy(g.title.text,"Freesword");
	g.title.text_color = 0x00ffffff;
}