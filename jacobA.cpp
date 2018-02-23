#include <math.h>

#include "defs.h"

#define DIRECTION_W	 0
#define DIRECTION_A	 1
#define DIRECTION_S	 2
#define DIRECTION_D	 3

//player shape radius
#define PRADIUS 25

#define PLAYER_MAX_SPEED 10

struct Global;

void movePlayer()
{
	vel[0] = CLAMP(vel[0], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);
	vel[1] = CLAMP(vel[1], -PLAYER_MAX_SPEED, PLAYER_MAX_SPEED);
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