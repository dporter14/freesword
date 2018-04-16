#include <iostream>
#include <ctime>
#include <cstdio>
#include "GL/glx.h"
#include "fonts.h"
#include "global.h"
#include <sys/timeb.h>

void mason_func()
{
	//strcpy(g.title.text,"Mason: Test test");
	//g.title.text_color = 0x00ff00ff;
}

Menu::Menu()
{
	m_height = 0;
	m_width = 0;
	m_buttonTitle = "";
}

void Menu::setPos(int x, int y) {
       VecMake(x, y, 0, pos); 
}

void Button::draw() {
    int w = 2;
    for(int i = 0; i < g.number[N_BUTTONS]; i++) {
        if (over) {
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);

            glBegin(GL_LINE_LOOP);
                glVertex2i(r.left-w,  r.bot-w);
                glVertex2i(r.left-w,  r.top+w);
                glVertex2i(r.right+w, r.top+w);
                glVertex2i(r.right+w, r.bot-w);
                glVertex2i(r.left-w,  r.bot-w);
            glEnd();

            glLineWidth(1);
        }
        if (down) {
            glColor3fv(dcolor);
        } else {
            glColor3fv(color);
        }

        glBegin(GL_QUADS);
            glVertex2i(r.left, r.bot);
            glVertex2i(r.left, r.top);
            glVertex2i(r.right, r.top);
            glVertex2i(r.right, r.bot);
        glEnd();
        r.left = r.centerx;
        r.bot  = r.centery-8;
        r.center = 1;
        if (down) {
            ggprint16(&r, 0, text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, text_color, text);
        }
    }
}

void Menu::draw() {
	static double tix = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();
	
	
	g.state[S_PAUSED] = true;

	glColor3ub(0, 153, 0);
	glPushMatrix();
	glTranslatef(g.xres/2, g.yres/2, 0.0);
	glBegin(GL_QUADS);
		glVertex2i(-100, -200);
		glVertex2i(-100,  200);
		glVertex2i( 100,  200);
		glVertex2i( 100, -200);
	glEnd();
	
	glTranslatef(-50, 50.0, 0.0);
	Rect r;
	r.bot = 50;
	r.left = 50;
	r.center = 1;
	ggprint8b(&r, 16, 0x00000000, "Pause Menu");

	glPopMatrix();
	for(int i = 0; i < g.number[N_BUTTONS]; i++) 
	    button[i].draw();

	//return time spent
	endTime = current_time();
	tix += endTime - startTime;
	sprintf(info_here, "Pause Menu Function: %f", tix);
}
void displayEnemiesKilled() {

	Rect killedRect;
	killedRect.bot  = 0;
	killedRect.right = g.xres;
	killedRect.top = 20.0;
	killedRect.left = g.xres;
	
	glColor3f(255,0,0);
	glPushMatrix();
	glTranslatef(-50.0, 0.0, 0.0);
	
	glBegin(GL_QUADS);
		glVertex2i(killedRect.left-50.0, killedRect.bot);
		glVertex2i(killedRect.left-50.0, killedRect.top);
		glVertex2i(killedRect.right+50.0, killedRect.top);
		glVertex2i(killedRect.right+50.0, killedRect.bot);
	glEnd();

	ggprint8b(&killedRect, 16, 0x0000ff00, "Enemies Slain: %i", g.eKilled);
	glPopMatrix();
}
