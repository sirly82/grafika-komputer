// deklarasikan semua header disini
#include <windows.h>
#include <GL/glut.h>
#include "COLOR.h"
#include "CUBE_GRID.h"
#include "METABALL.h"
#include "VECTOR3D.h"
#include "TIMER.h"

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z

const int minGridSize = 10;
int gridSize = 40;
float threshold = 6.0f;

const int numDiffuseColors = 5;
COLOR diffuseColors[numDiffuseColors];
int currentDiffuseColor = 0;

COLOR backgroundColor(0.773f, 0.773f, 0.773f, 1.0f);
CUBE_GRID cubeGrid;

const int numMetaballs = 5;
METABALL metaballs[numMetaballs];
TIMER timers;

//set up lighting
float shininess = 32.0f;
float ambient[] = { 0.0f, 0.0f, 0.2f, 1.0f };
float position[] = { -1.0f, 1.0f, 1.0f, 0.0f };
float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// fungsi ini digunakan untuk menggambar obyek
void drawObject()
{
	glPushMatrix();
	
//	glRotatef(objectAngle, objectRotation.X, objectRotation.Y,
//		objectRotation.Z);
	glRotatef(objectAngleY, 0.0, 1.0f, 0.0f);
		
	// dinormalisasi dulu
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseColors[currentDiffuseColor]);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glEnable(GL_LIGHT1);
	
	// set pencahayaan
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	
	// update posisi metaball
	float c = 2.0f*(float)cos(timers.GetTime() / 600);
	
	metaballs[0].position.x = -4.0f*(float)cos(timers.GetTime()/700) - c;
	metaballs[0].position.y = 4.0f*(float)sin(timers.GetTime()/600) - c;
	
	metaballs[1].position.x = 5.0f*(float)sin(timers.GetTime()/400) + c;
	metaballs[1].position.y = 5.0f*(float)cos(timers.GetTime()/400) - c;
	
	metaballs[2].position.x = -5.0f*(float)cos(timers.GetTime()/400) -
		0.2f*(float)sin(timers.GetTime() / 600);
	metaballs[2].position.y = 5.0f*(float)sin(timers.GetTime()/500) -
		0.2f*(float)sin(timers.GetTime() / 400);
		
	// bersihkan layar
	for (int i = 0; i<cubeGrid.numVertices; i++)
	{ 
		cubeGrid.vertices[i].value = 0.0f;
		cubeGrid.vertices[i].normal.LoadZero();
	}
	
	// hitung bidang skalar disetiap titik
	VECTOR3D ballToPoint;
	float squaredRadius;
	VECTOR3D ballPosition;
	float normalScale;
	for (int i = 0; i<numMetaballs; i++)
	{
		squaredRadius = metaballs[i].squaredRadius;
		ballPosition = metaballs[i].position;
		for (int j = 0; j<cubeGrid.numVertices; j++)
		{
			ballToPoint.x = cubeGrid.vertices[j].position.x -
				ballPosition.x;
			ballToPoint.y = cubeGrid.vertices[j].position.y -
				ballPosition.y;
			ballToPoint.z = cubeGrid.vertices[j].position.z -
				ballPosition.z;
			// hitung jarak bola ke titik
			float squaredDistance = ballToPoint.x*ballToPoint.x +
				ballToPoint.y*ballToPoint.y + ballToPoint.z*ballToPoint.z;
			if (squaredDistance == 0.0f)
				squaredDistance = 0.0001f;
			// value = r^2/d^2
			cubeGrid.vertices[j].value += squaredRadius / squaredDistance;
			// normal = (r^2 * v)/d^4
			normalScale = squaredRadius /(squaredDistance*squaredDistance);
			cubeGrid.vertices[j].normal.x += ballToPoint.x*normalScale;
			cubeGrid.vertices[j].normal.y += ballToPoint.y*normalScale;
			cubeGrid.vertices[j].normal.z += ballToPoint.z*normalScale;
		}
	}
	glPopMatrix();
}

/// taruh semua fungsi obyek yang akan digambar di fungsi display()
void display()
{
	// bersihkan dan reset layar dan buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
		
	glShadeModel(GL_SMOOTH);
	
	// panggil fungsi untuk menggambar obyek
	drawObject();
	
	glEnable(GL_LIGHTING);
	glTranslatef(0.0f, 0.0f, -30.0f);
	glRotatef((float)timers.GetTime() / 30, 1.0f, 0.0f, 1.0f);
	cubeGrid.DrawSurface(threshold);
	glDisable(GL_LIGHTING);
	
	cubeGrid.DrawSurface(threshold);
	
	glutSwapBuffers();
}
// inisialisasikan variabel, pencahayaan, tekstur,
// pengaturan pandangan kamera dan sebagainya di fungsi init()
void init(void)
{
	// inisialisasi warna latar belakang layar
	// dalam hal ini warna putih warna putih (1.0, 1.0, 1.0, 0.0)
	glClearColor(1.0, 1.0, 1.0, 0.0);
	// mengaktifkan depth buffer
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// set proyeksi ke proyeksi perspektif
	gluPerspective(45.0, 1.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// inisialisasi kamera pandang
	// kamera berada di posisi (0.0f, 0.0f, 0.0f)
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	//set up grid
	if (!cubeGrid.CreateMemory())
		return;
	if (!cubeGrid.Init(gridSize))
		return;
		
	//set up metaballs
	for (int i = 0; i<numMetaballs; i++)
		metaballs[i].Init(VECTOR3D(0.0f, 0.0f, 0.0f), 5.0f + float(i));
		
	//Set Up Colors
	diffuseColors[0].Set(0.129f, 0.126f, 0.142f, 1.0f);
	diffuseColors[1].Set(0.047f, 0.839f, 0.271f, 1.0f);
	diffuseColors[2].Set(0.976f, 0.213f, 0.847f, 1.0f);
	
	timers.Reset();
}

// fungsi ini digunakan bila layar akan diresize (default)
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

// fungsi untuk mengatur masukan dari keyboard 
// untuk arah kiri, kanan, atas, bawah, PgUp, dan PgDn
void keyboard(int key, int x, int y)
{
	float fraction = 0.1f;

	switch (key) 
	{
	// masukkan perintah disini bila tombol kiri ditekan
	case GLUT_KEY_LEFT:
		// dalam hal ini perintah rotasi obyek ke kiri sebanyak 1 derajat 
		objectAngleY -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol kanan ditekan
	case GLUT_KEY_RIGHT:		
		// dalam hal ini perintah rotasi obyek ke kanan sebanyak 1 derajat 
		objectAngleY += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol atas ditekan
	case GLUT_KEY_UP:		
		// dalam hal ini perintah rotasi obyek ke atas sebanyak 1 derajat 
		objectAngleX -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol bawah ditekan
	case GLUT_KEY_DOWN:		
		// dalam hal ini perintah rotasi obyek ke bawah sebanyak 1 derajat 
		objectAngleX += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// zoom in
	case GLUT_KEY_PAGE_UP:
		// masukkan perintah disini bila tombol PgUp ditekan
		posX += rotX * fraction;		
		posZ += rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
	// zoom out
	case GLUT_KEY_PAGE_DOWN:
		// masukkan perintah disini bila tombol PgDn ditekan
		posX -= rotX * fraction;
		posZ -= rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
	}
}

// timer untuk animasi (gunakan bila perlu)
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(55, timer, 0);
}

// program utama
int main(int argc, char** argv)
{
	// inisialisasi jendela OpenGL
	// GLUT_SINGLE berarti memakai single buffer
	// GLUT_DOUBLE berarti memakai double buffer
	// GLUT_RGB berarti mode tampilan yang dipakai RGB
	// GLUT_RGBA berarti mode tampilan yang dipakai RGBA
	// GLUT_DEPTH berarti memakai depth buffer
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// set ukuran jendela tampilan
	glutInitWindowSize(480, 480);		// besarnya jendela dalam piksel dalam hal ini 300x300
	glutInitWindowPosition(100, 100);	// posisi jendela dilayar komputer dalam piksel
	// judul jendela (wajib diubah dengan informasi NAMA / NIM - JUDUL PRAKTIKUM masing-masing)
	glutCreateWindow("SIRLY ZIADATUL MUSTAFIDAH / 2200018345 - TEKNIK PEMODELAN OBYEK 3D");
	
	// panggil fungsi init untuk inisialisasi awal
	init();

	// event handler untuk display, reshape dan keyboard
	glutDisplayFunc(display);   // display
	glutReshapeFunc(reshape);   // reshape
	glutSpecialFunc(keyboard);  // keyboard
	//glutTimerFunc(0, timer, 0); // aktifkan timer bila perlu

	// looping
	glutMainLoop();

	return 0;
}
