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
#include <fstream>

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

const int MAXBUTTONS = 4;
const int MAXENEMIES = 100;
const int MAXANIMATIONS = 10;
const int MAXSPRITES = 10;

enum clickState {C_NONE, C_QUIT, C_RESUME, C_EDITOR, C_};

class Button {
    public:
	    Rect r;
	    char text[32];
	    int over;
	    int down;
	    int click;
	    float color[3];
	    float dColor[3];
	    unsigned int text_color;
	    void draw();
        bool isOver(float, float);
		clickState state;

		void setRect(float, float, float, float);
		void setColor(float, float, float);
};

enum wep_type {W_NONE, W_SWORD};

class Weapon {
    wep_type type;
    Weapon(){
        type=W_SWORD;
    }
};

class Animation;
class Hitbox;
class Sprite;

class Object {
	public:
		Vec color;
		Vec pos;
		Vec scale;
		Flt rot;
		Animation* anim_handler; //
		Hitbox* hitbox;
		void drawSprite();
		Sprite* sprt;
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
			VecCopy(p, pos);
			VecCopy(s, scale);
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

		void init(anim_type);
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

class Animator {
	public:
		void play();
		Rect r;
		Animator(){
			nanims=0;
		}
		Animation* init(anim_type);
		void checkin(Animation* anim);
		
	private:
		Animation anims[MAXANIMATIONS];
		int nanims;
		
};

enum char_state {S_CHAR_ALIVE, S_CHAR_DEAD, S_CHAR_DYING, S_CHAR_IDLE, S_CHAR_ANGRY};
		
class Character : public Object {
	public:
		//define color and radius of circle until we have sprite
		//char* sprite_file;
		//Vec color; //inherit from object
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
		Player() {

		}
        ~Player(){}
        
        
    private:
};

class Enemy : public Character {
	Flt v_fov, v_dist, v_close;
    public:
    	void draw();
    	void setVel(Flt x, Flt y);
		void addVel(Flt x, Flt y);
		void attackPlayer();
        void kill();
        void see();
        Enemy() {
        	v_fov = 45;
        	v_dist = 500;
        	v_close = 75;
        	state = S_CHAR_IDLE;
        }
    private:
};



/* MASON FUNCTIONS */


class Menu {

    public:
        Menu();
        void draw();
        Vec pos;
        Button buttons[MAXBUTTONS];
		int nButtons;

        clickState getOver(float, float);

    private:
};
void mason_func();
void pauseMenu();
void displayTitle();
void displayEnemiesKilled();

/* David FUNCTIONS	*/

void david_func();
enum Sprite_box {SB_PLAYER_F, SB_PLAYER_B, SB_TILE_WOOD, SB_TILE_STONE, SB_};
enum Sprite_sheet {SS_PLAYER, SS_TILES, SS_};
class Texture 
{
	public:
		Image *img;
		GLuint tex;
		float h,w;

		void init(Image *pic);
};

class Sprite {
	public:
		int nframes;
		int frame;
		Texture* spriteTex;
		Vec start, pos;
		float w, h;

		void set_texture(Texture* tt) {
			spriteTex = tt;
			VecMake(0, 0, 0, pos);
			w=h=1;
		}
		
		void init(float x, float y, float h, float w, int n);
			
		void nextFrame() {
			frame = (frame+1)%nframes;
			VecCopy(start,pos);
			pos[0] += w*frame;
		}
};
enum Z_layers {ZL_SWORD, ZL_ENEMY, ZL_PLAYER, ZL_};

void initSpriteTextures();


		
/* JACOB FUNCTIONS */

class Wall : public Object {

    public:
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
        //false -> left/up true -> right/down
        bool openedFrom;

		Hitbox trigger;
        
        void draw();
        //function to open/close door
        void swing();
        void initDoor(Flt, Flt, Flt, Flt, bool);
        //level edit function
        void rotate();
        Door(){trigger.dynamic=0;}
};

class Level {
    public:
        Enemy enemies[1000];
        Wall walls[1000];
        Door doors[1000];

        void buildLevel1();
    private:
};

void toggleEditMode();
void initWalls();
void interactDoor();
void collide(Door);
void buildLevel1();
void createWall(int, int);

void interactDoor();
/*
void doorCollision(Door, Enemy&);
void doorCollision(Door, Player);
void wallCollision(Wall, Enemy, int);
void wallCollision(Wall, Player);
*/
void dragWall(int, int);
void createDoor(int, int);
void dragDoor(int, int);
void rotateDoor(int, int);
void saveLevel();
void saveLevel();
void loadLevel();

enum MouseList {M_1, M_2, M_3, M_};
void wallCollision(Wall&, Character&);

/* TAYLOR FUNCTIONS */

Flt angleBetween(Vec, Vec);
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
void characterCollision(Character&, Character&);

/* END FUNCTIONS */


enum KeyList {K_SHIFT, K_W, K_A, K_S, K_D, K_};
enum State {S_PAUSED, S_GAMEOVER, S_WINNER, S_PLAYER, S_DEBUG, S_LEVELEDIT, S_};
/*
	paused: game paused?
	gameover: gameover?
	player:
		0 - idle
		1 - dead
		2 - attacking
*/	
enum NumberOf {N_ENEMIES, N_ANIMS, N_BUTTONS, N_WALLS, N_DOORS, N_};

struct Global {
	// screen res
	int xres, yres;
	// mouse position
	int savex, savey;
	Player player;
	Enemy enemies[MAXENEMIES];
	
	
	Menu pauseMenu;
	Image *bgImage;
	GLuint bgTexture;

	//Image *spriteImage;
	//GLuint spriteTextures[SI_];
	//float* spriteBox[SI_];

	Texture spriteTextures[SS_];
	Sprite sprites[SB_];

	Button title;
	Button button[MAXBUTTONS];
    
	bool isPressed[K_];
    bool isClicked[M_];
	int state[S_];
	int number[N_];
	int eKilled;
	
	
	//Wall n, e, s, w;
	bool wallChange, doorChange;
    Door doors[4];
	Info info;
	Animator animator;
	//
	int currentLevel;
    Level level1;

	#ifdef USE_OPENAL_SOUND
		ALuint alBufferDrip, alBufferTick;
		ALuint alSourceDrip, alSourceTick;
	#endif
	Global() {
		xres = 1200; yres = 900;
//		xres = 1087; yres = 800;
		savex = savey = 0;
		
		bgImage=NULL;

		eKilled = 0;

		//spriteImage=NULL;
		
        
		title.r.left = xres/2;
		title.r.bot	= yres-100;
		title.r.center = 1;
<<<<<<< HEAD
		strcpy(title.text,"");
=======
		strcpy(title.text, "");
>>>>>>> textures
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
		for (int i=0; i<M_; i++) {
            isClicked[i] = false;
        }
        wallChange = true;
        doorChange = true;
	}
};
extern Global g;


#endif
