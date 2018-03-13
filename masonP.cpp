#include <iostream>
#include "GL/glx.h"
#include "fonts.h"
#include "global.h"


void mason_func()
{
	strcpy(g.title.text,"Mason: Test test");
	g.title.text_color = 0x00ff00ff;
}

Menu::Menu() 
{
	m_height = 0;
	m_width = 0;
	m_buttonTitle = "";
}

void draw() {

	for(int i=0; i<g.nbuttons; i++) {
        if (g.button[i].over) {
            //int w=2;
            glColor3f(1.0f, 1.0f, 0.0f);
            //draw a highlight around button
            glLineWidth(3);
			/*
            glBegin(GL_LINE_LOOP);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.top+w);
                glVertex2i(g.button[i].r.right+w, g.button[i].r.top+w);
                glVertex2i(g.button[i].r.right+w, g.button[i].r.bot-w);
                glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
            glEnd();
			*/
            glLineWidth(1);
        }
        if (g.button[i].down) {
            glColor3fv(g.button[i].dcolor);
        } else {
            glColor3fv(g.button[i].color);
        }
		/*
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
        }*/
    }
}
bool pauseMenu() {

	g.paused = true;
	//Button menuButt[3];

	//static float angle = 0.0;
	glColor3ub(0, 255, 0);
	glPushMatrix();
	glTranslatef(100, 100, 0.0);
	glTranslatef(-50.0, -50.0, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,     0);
		glVertex2i(0,   200);
		glVertex2i(200, 100);
		glVertex2i(200,   0);
	glEnd();
	glClear(GL_COLOR_BUFFER_BIT);

	glPopMatrix();
	return g.paused;
}
