#include<GL/glut.h>

void garis();
main (int argc, char** argv)
{ 
	glutInit(&argc,argv); 
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); 
	glutInitWindowSize(1200,720); 
	glutInitWindowPosition(0,0); 
	glutCreateWindow("Project Membuat Garis"); 
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glMatrixMode(GL_PROJECTION); 
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); 
	glutDisplayFunc(garis); 
	glutMainLoop();
}

void garis()
{ 
	glClear(GL_COLOR_BUFFER_BIT); 
	glBegin(GL_LINES); 
	
	glColor3ub(255, 0, 0); 
	glVertex2f(0.35, 0.7); 
	glVertex2f(0.6, 0.7);
	
	glColor3ub(0, 255, 0); 
	glVertex2f(0.35, 0.5); 
	glVertex2f(0.6, 0.5);
	
	glColor3ub(0, 0, 255); 
	glVertex2f(0.35, 0.3); 
	glVertex2f(0.6, 0.3);
	
	glPointSize(50.0f); 
	glEnd(); 
	glFlush();
}