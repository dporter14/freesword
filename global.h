#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <iostream>

#include "defs.h"
#include "fonts.h"
#include "log.h"

#ifdef __MACH__
	#include <mach/clock.h>
	#include <mach/mach.h>
	#include <mach/mach_init.h>
	#include <mach/mach_time.h>
#endif

#define USE_OPENAL_SOUND

#ifdef USE_OPENAL_SOUND
	#ifdef __MACH__
		#include <AL/alut.h>
	#else
		#include </usr/include/AL/alut.h>
	#endif //USE_OPENAL_SOUND
#endif

#define MAXBUTTONS 4
#define MAXENEMIES 100
#define MAXANIMATIONS 10

typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

enum wep_type {W_NONE, W_SWORD};

class Weapon {
    wep_type type;
    Weapon(){
        type=W_SWORD;
    }
};

class Animation;
class Hitbox;
//enum obj_type {O_PLAYER, O_ENEMY};

class Object {
	public:
		Vec pos;
		Vec scale;
		Flt rot;
		Animation* anim_handler; //
		Hitbox* hitbox;
		
		virtual void draw() = 0;
};

enum hit_type {H_HURTBOX, H_ATTACK, H_TRIGGER};

class Hitbox {
	public:
		Vec pos;
		Vec scale;
		Flt rot;
		hit_type type;
		//bool collision; // will this collide with other hitboxes?
		bool active; // is this thing on?
		bool dynamic; // can it move?
		Hitbox () {}
		Hitbox (hit_type t, Vec p, Vec s, Flt r = 0) {
			type = t;
			VecCopy(p,pos);
			VecCopy(s,scale);
			active = 1;
			rot=r;
		}
		 
		bool intersect(Hitbox other) {
			return (
				pos[0]+scale[0] > other.pos[0]-other.scale[0] &&
				pos[0]-scale[0] < other.pos[0]+other.scale[0] &&
				pos[1]+scale[1] > other.pos[1]-other.scale[1] &&
				pos[1]-scale[1] < other.pos[1]+other.scale[1]
			);
		}
};

enum anim_type {A_SWORD_SLASH, A_SWORD_SLASH2, A_TEST};

class Animation {
	public:
		int frame, nframes;
		bool done;
		bool can_cancel;
		
		int type;

		Object *actors[2];
		int nactors;

		void init(int);
		void play();
		void add_actor(Object* actor);
		void set_frames(int frames);
		void set_duration(float duration);
		void cancel();

		Animation(){};
		~Animation(){};

		void sword_slash();
		void sword_slash2();
		void test();


};

enum char_state {S_CHAR_ALIVE, S_CHAR_DEAD, S_CHAR_DYING};
		
class Character : public Object {
	public:
		//define color and radius of circle until we have sprite
		//char* sprite_file;
		Vec color;
		Flt pradius;
		Flt max_speed;
        
		//Vec pos; // inherited from object
		Vec vel; // char's velocity
		//Vec dir; // char's orientation
		Vec rhand_pos; //pos of right hand
		Flt rhand_rot; //orientation of right hand
		
		int state; //0 alive 1 dead
		Hitbox hitbox;
		// will change to support multiple attacks at once
		Hitbox attacks[1];
		int nattacks;
		//constructor
		Character(){
			anim_handler=NULL;
			state=S_CHAR_ALIVE;
		}
		~Character(){} //destructor
		void move();
		void lookAt(Flt x, Flt y);
		void setPos(Flt x, Flt y);
		void addPos(Flt x, Flt y);
		virtual void setVel(Flt x, Flt y) = 0; //redefined in chilren
		virtual void addVel(Flt x, Flt y) = 0;
		void draw();
	private:

};

class Player : public Character {
    public:
    	void init();
        void setVel(Flt x, Flt y);
		void addVel(Flt x, Flt y);
		Player(){}
        ~Player(){}
        
        
    private:
};

class Enemy : public Character {
    public:
    	void setVel(Flt x, Flt y);
		void addVel(Flt x, Flt y);
		void attackPlayer();
        void kill();
    private:
};



/*//////////// MASON FUNCTIONS ///////////////////////////////*/
class Menu {

    public:
        Menu();
        void draw();
    private:
        double m_height, m_width;
        std::string m_buttonTitle;
        
};
class mainMenu : public Menu {

    public:

    private:

};

void mason_func();
void pauseMenu();

/*//////////   David FUNCTIONS	//////////////////////////////*/

void david_func();

/*///////////   JACOB FUNCTIONS	/////////////////////////////*/

class Wall : public Object {

    public:
        //wall width and height
        Flt width, height;
        //coordinates for center of wall
        //Vec pos; // inherited from object
		Hitbox hitbox;
		
        Flt left, right, top, bot;

        void draw();
        void initWall(Flt, Flt, Flt, Flt);
        Wall(){hitbox.dynamic=0;}
};

class Door : public Wall {

    public:
        //Flt width, height; //inherited from wall
        //center
        //Vec pos; // inherited from object
		
        //Flt left, right, top, bot; //inherited from wall
		bool isHoriz;
        bool isOpen;
		Hitbox trigger;
        
        void draw();
        //function to open/close door
        void swing();
        void initDoor(Flt, Flt, Flt, Flt, bool);
        Door(){trigger.dynamic=0;}
};

class Level {
    public:
        Enemy enemies[100];
        Wall walls[100];
        Door doors[100];
        int nenemies;
        int nwalls;
        int ndoors;

        void buildLevel1();
    private:
};

void jacob_func();
void initWalls();
void interactDoor();
void collide(Door);
void buildLevel1();

void interactDoor();
void doorCollision(Door, Enemy&);
void doorCollision(Door, Player);
void wallCollision(Wall, Enemy, int);
void wallCollision(Wall, Player);


/*/////////////    TAYLOR FUNCTIONS     ////////////////////*/

double current_time();
class Info {
	public:
		Rect r;
		Info(){
			nstats=0;
		}
		char* get_place(){
			return stats[nstats++];
		}
		void draw();
	private:
		char stats[50][50];
		int nstats;
		
};

void taylor_func();
void spawnEnemy(Flt x, Flt y);
void object_collision(Character&, Character&);

/*/////////////    END FUNCTIONS     ////////////////////*/


enum KeyList {K_SHIFT, K_W, K_A, K_S, K_D, K_};
enum State {S_PAUSED, S_GAMEOVER, S_WINNER, S_PLAYER, S_DEBUG, S_};
/*
	paused: game paused?
	gameover: gameover?
	player:
		0 - idle
		1 - dead
		2 - attacking
*/	
enum NumberOf {N_ENEMIES, N_ANIMS, N_BUTTONS, N_WALLS, N_};

struct Global {
	// screen res
	int xres, yres;
	// mouse position
	int savex, savey;
	Player player;
	Enemy enemies[MAXENEMIES];
	Animation anims[MAXANIMATIONS];
	
	Image *bgImage;
	GLuint bgTexture;
	Button button[MAXBUTTONS];
	Button menuButt[3];
	Button title;

	bool isPressed[K_];
	int state[S_];
	int number[N_];
	Wall n, e, s, w;
	Door doors[4];
	Info info;
	//
	int currentLevel;
    Level level1;

	#ifdef USE_OPENAL_SOUND
		ALuint alBufferDrip, alBufferTick;
		ALuint alSourceDrip, alSourceTick;
	#endif
	Global() {
		xres = 1200;		yres = 900;
//		xres = 1087; yres = 800;
		savex = savey = 0;
		
		bgImage=NULL;

		title.r.left = xres/2;
		title.r.bot	= yres-100;
		title.r.center = 1;
		strcpy(title.text,"Freesword");
		title.text_color = 0x00ffffff;

		for(int i = 0; i<K_; i++) {
			isPressed[i] = false;
		}
		for(int i = 0; i<S_; i++) {
			state[i] = 0;
		}
		for(int i = 0; i<N_; i++) {
			number[i] = 0;
		}
		
	}
};
extern Global g;


#endif
