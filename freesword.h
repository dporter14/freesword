#ifndef _FREESWORD
#define _FREESWORD

#include "defs.h"

enum wep_type {W_SWORD};

class Weapon {
    wep_type type;
    Weapon(){
        type=W_SWORD;
    }
};

class Animation;

class Object {
	public:
		Animation* anim_handler; //		
};

enum anim_type {A_SWORD_SLASH, A_TEST};

class Animation {
	public:
		int frame, nframes;
		bool done;
		
		anim_type type;
		
		Object *actors[2];
		int nactors;
		
		void init();
		void play();
		void add_actor(Object* actor);
		void set_frames(int frames);
		void set_duration(float duration);
		
		
		Animation(){};
		~Animation(){};
		
		void sword_slash();
		void test();
		
		
};

class Character : public Object{
	public:
		//define color and radius of circle until we have sprite
		//char* sprite_file;
		Vec color;
		Flt pradius;
		
		Vec pos; // char's position
		Vec vel; // char's velocity
		Vec dir; // char's orientation
		Vec rhand_pos; //pos of right hand
		Vec rhand_dir; //orientation of right hadn
		bool status; //0 alive 1 dead
		
		//constructor
		Character(){
			anim_handler=NULL;
		} 
		~Character(){} //destructor
		void move();
		void lookAt(Flt x, Flt y);
		void setPosition(Flt x, Flt y);
		void setVelocity(Flt x, Flt y);
		void addVelocity(Flt x, Flt y);
		void draw();
	private:

};

class Player : public Character {
    public:
        void init();
        Player(){}
        ~Player(){}
    private:
};

class Enemy : public Character {
    public:
        void attackPlayer();
    private:
};

class Wall {

    public:
        //wall width and height
        Flt width, height;
        //coordinates for center of wall
        Flt x, y;
        
        void draw();
};



/*//////////////////////////////////////////////////
  MASON FUNCTIONS			############
  *///////////////////////////////////////////////////
class Menu {

    public:
        Menu();
    private:
        double m_height, m_width;
        std::string m_buttonTitle;		
};
class mainMenu : public Menu {

    public:

    private:

};

void mason_func();

/////// END Mason FUNCTIONS ////////////////////////

/*//////////////////////////////////////////////////
  David Functions		####################
  *///////////////////////////////////////////////////

void david_func();

/////// End David Functions/////////////////////////

/*//////////////////////////////////////////////////
  JACOB FUNCTIONS			############
  *///////////////////////////////////////////////////

void jacob_func();
void initWalls();
/////// End Jacob Functions/////////////////////////

/*//////////////////////////////////////////////////
  TAYLOR FUNCTIONS			############
  *///////////////////////////////////////////////////

void taylor_func();
void spawnEnemy(Flt x, Flt y);

/////// End Taylor Functions/////////////////////////

#endif

