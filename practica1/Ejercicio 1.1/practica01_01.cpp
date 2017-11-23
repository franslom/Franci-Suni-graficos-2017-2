// Para compilar
// g++ practica01.cpp -o gl -lGL -lGLU -lglut


#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif


//#define NUMVTX (8)
#define NUMVTX (10)
#define NUMT (16)

static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=0.0f;

/*GLfloat v[NUMVTX][3]={{-1,0,1},{-1,0,-1},
                      {0,-1,1},{0,-1,-1},
                      {1,1,1},{1,1,-1},
                      {-1,0,1},{-1,0,-1}};*/

/*GLfloat c[NUMVTX][3]={{1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0}};*/

GLfloat v[NUMVTX][3]={
                      {1,5,0},{4,5,0},
                      {1,4,0},{4,4,0},
                      {1,4,-1},{4,4,-1},
                      {1,5,-1},{4,5,-1},
                      {1,5,0},{4,5,0}};

GLfloat v1[NUMVTX][3]={
                      {1,0,0},{1,4,0},
                      {2,0,0},{2,4,0},
                      {2,0,-1},{2,4,-1},
                      {1,0,-1},{1,4,-1},
                      {1,0,0},{1,4,0}};

GLfloat v2[NUMVTX][3]={
                      {2,3,0},{3,3,0},
                      {2,2,0},{3,2,0},
                      {2,2,-1},{3,2,-1},
                      {2,3,-1},{3,3,-1},
                      {2,3,0},{3,3,0}};


GLfloat c[NUMVTX][3]={{1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1}};

GLfloat t[NUMT][3]={{4,5,0},{4,4,0},
					{4,4,-1},{4,5,-1},
					{1,5,0},{1,4,0},
					{1,4,-1},{1,5,-1},
					{3,3,0},{3,2,0},
					{3,2,-1},{3,3,-1},
					{1,0,0},{2,0,0},
					{2,0,-1},{1,0,-1}};

GLfloat n[NUMVTX][3]={{0,0,1},{0,0,1},
						{0,0,1},{0,0,1},
						{0,-1,0},{0,-1,0},
						{0,0,-1},{0,0,-1},
						{0,1,0},{0,1,0}};

GLfloat n_t[NUMT][3]={{1,0,0},{1,0,0},{1,0,0},{1,0,0},
						{-1,0,0},{-1,0,0},{-1,0,0},{-1,0,0},
						{1,0,0},{1,0,0},{1,0,0},{1,0,0},
						{0,-1,0},{0,-1,0},{0,-1,0},{0,-1,0}};
//GLfloat n[3]={0.0,0.0,-1.0};
void drawgraphix()
{
	
	int i;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	glTranslatef(-2.0f,-2.0f,1.0f);

	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);


	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTX; i++) 
	{
		glColor3fv(c[i]);
		glNormal3fv(n[i]);
		glVertex3fv(v[i]);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTX; i++) 
	{
		glColor3fv(c[i]);
		glNormal3fv(n[i]);
		glVertex3fv(v1[i]);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTX; i++) 
	{
		glColor3fv(c[i]);
		glNormal3fv(n[i]);
		glVertex3fv(v2[i]);
	}
	glEnd();

	
	glColor3fv(c[2]);
	glBegin(GL_QUADS);
	for (i=0; i<NUMT; i++) 
	{
		glVertex3fv(t[i]);
	}
 	glEnd();

 	glLineWidth(2.5); 
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(-15, 0.0, 0.0);
	glVertex3f(15, 0, 0);
	glVertex3f(0.0,-15, 0.0);
	glVertex3f(0.0,15, 0.0);
	glVertex3f(0.0,0.0, -10);
	glVertex3f(0.0,0.0, 1);
	glEnd();



	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	//glOrtho(0.0f, winwidth, winheight, 0.0f, 0.0f, 1.0f);
}

void mousefunc(int button,int state,int x,int y)
{
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
		}

		mx=x;
		my=y;

		drawgraphix();

		flag=2;
	}
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
}


void idlefunc()
{
	glutPostRedisplay();
}

void init(void) 
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 20.0 };
   GLfloat light_position[] = { 1.0, 7.0, 5.0, 0.0 };
   GLfloat mat_ambient[] = {0.33, 0.22, 0.03, 1.0};
   GLfloat mat_diffuse[] = {0.78, 0.57, 0.11, 1.0};

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
   glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}
int main(int argc,char **argv)
{
   winwidth=winheight=812;

   

   glutInit(&argc,argv);
   glutInitWindowSize(winwidth,winheight);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   glutCreateWindow("Practica 1 - Ejercicio 1.1");
   init();

   glutDisplayFunc(drawgraphix);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyboardfunc);
   glutIdleFunc(idlefunc);

   glutMainLoop();

   return(0);
}

