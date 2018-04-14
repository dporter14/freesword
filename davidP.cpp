//Author: David Porter

#include <iostream>     
#include <ctime>
#include "global.h"

void david_func()
{
	//strcpy(g.title.text,"This is a David Print");
	//g.title.text_color = 0x00ff0000;
}
/*
void Animation::sword_stab()
{
        static double tix = 0.0;
        static char * info_here = g.info.get_place();
        double startTime, endTime;
        startTime = current_time();

        Player* actor = (Player*)actors[0];
        static Vec orig_pos;
        static Vec orig_dir;
        if (frame == 0) {
                VecCopy(actor->rhand_pos, orig_pos);
                VecCopy(actor->rhand_dir, orig_dir);
        }
}
*/
void Object::initSpriteTex(Image *pic, int enum_img)
{
	Image *tmp = g.spriteImage + enum_img;

	tmp = &pic[0];

	Log("Dimensions: %d %d\n", tmp->width, tmp->height);

	glGenTextures(1, &g.spriteTextures[enum_img]);

	glBindTexture(GL_TEXTURE_2D, g.spriteTextures[enum_img]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			tmp->width, tmp->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, tmp->data);
}
void Character::drawSprite()
{

	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(pos[0], pos[1], 0.0f);
	/*
float cx = 0;
	float cy = 0;
	//radius
	float ra = pradius;
	int num_segments = 100;

	float theta = 2 * PI / float(num_segments); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = ra;//we start at angle = 0 
	float y = 0; 
	glBindTexture(GL_TEXTURE_2D, g.spriteTextures[SI_PLAYER_FRONT]);
	
	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glTexCoord2f(((x/pradius)+1)/2, ((1-(y/pradius))+1)/2);
		glVertex2f(x + cx, y + cy);//output vertex 

		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	*/
	
	glBindTexture(GL_TEXTURE_2D, g.spriteTextures[SI_PLAYER_FRONT]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-scale[0],  scale[1], scale[2]);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( scale[0],  scale[1], scale[2]);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( scale[0], -scale[1], scale[2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-scale[0], -scale[1], scale[2]);
	glEnd();
	glBindTexture(GL_TEXTURE_2D,0);
	
	glPopMatrix();

/*
	if (g.state[S_DEBUG]) {
		glColor3f(0,0,1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], -hitbox.scale[1]);
		glVertex2f(hitbox.scale[0], -hitbox.scale[1]);
		glEnd();
	}

	glRotatef(rot,0,0,1);

	//hand stuff
	glColor3f(0.6f, 0.6f, 0.6f);
	glTranslatef(rhand_pos[0], rhand_pos[1], 0.0);

	glRotatef(rhand_rot,0,0,1);
	
	glBegin(GL_POLYGON);
	glVertex2f(-3.0f, 0.0f);
	glVertex2f(-3.0f, 60.0f);
	glVertex2f(3.0f, 60.0f);
	glVertex2f(3.0f, 0.0f);
	glEnd();
	
	
	glPopMatrix();

	if (g.state[S_DEBUG]) {
		for(int i=0; i<nattacks; i++){
			glColor3f(1,1,0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(attacks[i].pos[0]+attacks[i].scale[0], 
				attacks[i].pos[1]+attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]-attacks[i].scale[0], 
				attacks[i].pos[1]+attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]-attacks[i].scale[0], 
				attacks[i].pos[1]-attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]+attacks[i].scale[0], 
				attacks[i].pos[1]-attacks[i].scale[1]);
			glEnd();
		}
	}
	*/
}

void Character::draw()
{
	//lab7 profiling
	static double tix = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();

	//draw player
	drawSprite();

	glColor3f(color[0],color[1],color[2]);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0.0);

/*	//starting coordinates
	float cx = 0;
	float cy = 0;
	//radius
	float ra = pradius;
	int num_segments = 100;

	float theta = 2 * PI / float(num_segments); 
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = ra;//we start at angle = 0 
	float y = 0; 

	glBegin(GL_POLYGON); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 

		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	} 
	glEnd();
*/
	if (g.state[S_DEBUG]) {
		glColor3f(0,0,1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], hitbox.scale[1]);
		glVertex2f(-hitbox.scale[0], -hitbox.scale[1]);
		glVertex2f(hitbox.scale[0], -hitbox.scale[1]);
		glEnd();
	}
	
	/*
	//player direction
	Vec up, upz, cross;
	VecMake(0, 1, 0, up);
	VecMake(0, 0, 1, upz);
	float angl = acos(VecDot(dir, up));	
	VecCross(up, dir, cross);
	if (VecDot(upz, cross)<0)
		angl = -angl;	
	angl = angl*180/PI;
	*/
	
	glRotatef(rot,0,0,1);
	
	/*
	//head triangle 
	glPushMatrix();
	glTranslatef(0, pradius+1, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-10.0f, 0.0f);
	glVertex2f(0.0f, 10.0f);
	glVertex2f(10.0f, 0.0f);
	glEnd();
	glPopMatrix();
	*/

	//hand stuff
	glColor3f(0.6f, 0.6f, 0.6f);
	glTranslatef(rhand_pos[0], rhand_pos[1], 0.0);
	
	/*
	angl = acos(VecDot(rhand_dir, up));	
	VecCross(up, rhand_dir, cross);
	if (VecDot(upz, cross)<0)
		angl = -angl;	
	angl = angl*180/PI;
	*/
	glRotatef(rhand_rot,0,0,1);
	
	glBegin(GL_POLYGON);
	glVertex2f(-3.0f, 0.0f);
	glVertex2f(-3.0f, 60.0f);
	glVertex2f(3.0f, 60.0f);
	glVertex2f(3.0f, 0.0f);
	glEnd();
	
	
	glPopMatrix();

	if (g.state[S_DEBUG]) {
		for(int i=0; i<nattacks; i++){
			glColor3f(1,1,0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(attacks[i].pos[0]+attacks[i].scale[0], 
				attacks[i].pos[1]+attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]-attacks[i].scale[0], 
				attacks[i].pos[1]+attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]-attacks[i].scale[0], 
				attacks[i].pos[1]-attacks[i].scale[1]);
			glVertex2f(attacks[i].pos[0]+attacks[i].scale[0], 
				attacks[i].pos[1]-attacks[i].scale[1]);
			glEnd();
		}
	}

	//return time spent
	endTime = current_time();
	tix += endTime - startTime;
	sprintf(info_here, "Character Draw Function: %f", tix);
}

/*
void Animation::my_anim(){
	if (frame==0){
		cout << "hello world" << endl;
	}
}
*/










