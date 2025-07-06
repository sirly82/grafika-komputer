// praktikum 09 grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include "GL/glut.h"

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z

// container untuk membuat tipe data 3D (X, Y, Z)
struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	//
	Vec3() { }
	~Vec3() { }
};

// kurva
Vec3 controlPoint[4][4];
bool showPoints = false;

GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 100.0 };

GLUnurbsObj *theNurb;


// fungsi untuk menggambar obyek
void drawObject()
{
	glPushMatrix();
	// operasi transformasi rotasi obyek ke arah kanan-kiri
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	// operasi transformasi rotasi obyek ke arah atas-bawah
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
	glScalef(0.5, 0.5, 0.5);
	// set warna obyek ke warna hijau (0.0f, 1.0f, 0.0f)
	glColor3f(0.0f, 1.0f, 0.0f);
	//
	GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &controlPoint[0][0].X, 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(theNurb);
	if (showPoints)
	{
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
			glVertex3f(controlPoint[i][j].X, controlPoint[i][j].Y,
			controlPoint[i][j].Z);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();

	glPopMatrix();
}

// taruh semua obyek yang akan digambar di fungsi display()
void display()
{
	// bersihkan dan reset layar dan buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// posisikan kamera pandang
	// dalam hal ini sumbu Y ada diatas dan posisi kamera pandang di (posX, posY, posZ)
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

	// panggil fungsi untuk menggambar obyek
	drawObject();
	
	// tampilkan obyek ke layar
	// gunakan glFlush() bila memakai single buffer
	// gunakan glutSwapBuffers() bila memakai double buffer
	glutSwapBuffers();
}

// inisialisasikan variabel, pencahayaan, tekstur dan pengaturan kamera pandang di fungsi init()
// inisialisasi
void init(void)
{
	// inisialisasi warna latar belakang
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); // mengaktifkan depth buffer
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 100.0);// set proyeksi ke perspektif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// inisialisasi kamera pandang
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	int u, v;
	for (u = 0; u < 4; u++)
	{
		for (v = 0; v < 4; v++)
		{
			controlPoint[u][v].X = 2.0*((GLfloat)u - 1.5);
			controlPoint[u][v].Y = 2.0*((GLfloat)v - 1.5);
			if ((u == 1 || u == 2)) // || (v == 1 || v == 2)
				controlPoint[u][v].Z = 3.0;
			else
				controlPoint[u][v].Z = -3.0;
		}
	}
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
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
	// tampilkan point
	case GLUT_KEY_F1:
		showPoints = !showPoints;
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
	glutCreateWindow("SIRLY ZIADATUL MUSTAFIDAH / 2200018345 - TEKNIK REPRESENTASI PERMUKAAN");
	
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