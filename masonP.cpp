#include <iostream>
#include <ctime>
#include <cstdio>
#include "GL/glx.h"
#include "fonts.h"
#include "global.h"
#include <sys/timeb.h>

//typedef std::chrono::high_resolution_clock Clock;

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

double DisplayTime2() {
    	//timespec ftimestart, ftimeend;
	//clock_gettime(CLOCK_REALTIME, &ftimestart);
	
    	std::clock_t start;
	start = std::clock();

	int a = 23;
	int b = 14;
	// Slow the function down.
	for(int loop = 1; loop < 4000; loop++) {
		int d = a / b / loop;	
	}
	double t2 = (std::clock() - start);
	return t2;
}
void DisplayTime1() {

    	struct timespec ftimestart, ftimeend;
	clock_gettime(CLOCK_REALTIME, &ftimestart);
	Rect r;
	r.bot = 100;
	r.left = 10;
	r.center = 0;
	clock_gettime(CLOCK_REALTIME, &ftimeend);

	double t1 = ;

	ggprint8b(&r, 16, 0x00ff00ff, "Time function1: %lf", t1		    );
	ggprint8b(&r, 16, 0x00ff00ff, "Time function2: %lf", DisplayTime2() );
}

/*
bool pause() 
{
	bool paused = true;
	Rect rects [2];


	static float angle = 0.0;
	glColor3ub(0, 255, 0);
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-50.0, -50.0, 0);
	glBegin(GL_QUADS);
		glVertex2i(0,     0);
		glVertex2i(0,   200);
		glVertex2i(200, 100);
		glVertex2i(200,   0);
	glEnd();
	glClear(GL_COLOR_BUFFER_BIT);

	glPopMatrix();	
}*/
