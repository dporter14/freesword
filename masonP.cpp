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
    m_position = 0;
}
void Button::draw() {

	for(int i=0; i<g.number[N_BUTTONS]; i++) {
        if (g.button[i].over) {
            int w=2;
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);

            glBegin(GL_LINE_LOOP);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.top+w);
                glVertex2i(g.button[i].r.right+w, g.button[i].r.top+w);
                glVertex2i(g.button[i].r.right+w, g.button[i].r.bot-w);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
            glEnd();

            glLineWidth(1);
        }
        if (g.button[i].down) {
            glColor3fv(g.button[i].dcolor);
        } else {
            glColor3fv(g.button[i].color);
        }

        glBegin(GL_QUADS);
            glVertex2i(g.button[i].r.left,  g.button[i].r.bot);
            glVertex2i(g.button[i].r.left,  g.button[i].r.top);
            glVertex2i(g.button[i].r.right, g.button[i].r.top);
            glVertex2i(g.button[i].r.right, g.button[i].r.bot);
        glEnd();
        r.left = g.button[i].r.centerx;
        r.bot  = g.button[i].r.centery-8;
        r.center = 1;
        if (g.button[i].down) {
            ggprint16(&r, 0, g.button[i].text_color, "Pressed!");
        } else {
            ggprint16(&r, 0, g.button[i].text_color, g.button[i].text);
        }

    }
}

void pauseMenu() {
	static double tix = 0.0;
	static float angle = 0.0;
	static char* info_here = g.info.get_place();
	double startTime, endTime;
	startTime = current_time();
	
	
	g.state[S_PAUSED] = true;
	g.button[0].draw();	

	glColor3ub(0, 153, 0);
	glPushMatrix();
	glTranslatef(g.xres/2, g.yres/2, 0.0);
	glBegin(GL_QUADS);
		glVertex2i(-100, -200);
		glVertex2i(-100,  200);
		glVertex2i(100, 200);
		glVertex2i(100, -200);
	glEnd();
	
	glTranslatef(-50, 50.0, 0.0);
	Rect r;
	r.bot = 50;
	r.left = 50;
	r.center = 1;
	ggprint8b(&r, 16, 0x00000000, "Pause Menu");

	glPopMatrix();
	
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
