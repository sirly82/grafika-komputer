// praktikum 4 transformasi 3D grafika komputer
// adhi prahara. 2018


// deklarasikan semua header disini
#include <windows.h>
#include <GL/glut.h>


// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;                                        // sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;        // posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;        // posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;        // posisi kamera di sumbu Z


float objectAngleX = 0.0f;                        // sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;                        // sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;                        // sudut tranformasi obyek di sumbu Z


float objectScaleX = 1.0f;        // skala perbesaran obyek ke arah X
float objectScaleY = 1.0f;        // skala perbesaran obyek ke arah Y
float objectScaleZ = 1.0f;        // skala perbesaran obyek ke arah Z


float objectPositionX = 0.0f;        // posisi obyek di sumbu X
float objectPositionY = 0.0f;        // posisi obyek di sumbu Y
float objectPositionZ = 0.0f;        // posisi obyek di sumbu Z


// fungsi untuk menggambar obyek
void drawObject()
{
        // obyek bisa dimasukkan diantara glPushMatrix() dan glPopMatrix() 
        // fungsinya agar obyek tidak terpengaruh atau mempengaruhi obyek lain
        // saat diwarnai, ditransformasi dan sebagainya
        glPushMatrix();


        // operasi transformasi translasi obyek 
        // ke arah sumbu X, Y atau Z
        glTranslatef(objectPositionX, objectPositionY, objectPositionZ);


        // operasi transformasi scaling obyek 
        // memperbesar atau mengecilkan obyek
        // ke arah sumbu X, Y atau Z
        glScalef(objectScaleX, objectScaleY, objectScaleZ);


        // operasi transformasi rotasi obyek ke arah kanan-kiri
        glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);


        glPushMatrix();


        // operasi transformasi rotasi obyek ke arah atas-bawah
        glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

        // bila menggambar obyek harus diawali glBegin(tipe obyek) dan diakhiri dengan glEnd()
        // kecuali menggunakan fungsi yang sudah ada di GLUT-OpenGL seperti dibawah ini
//        glutSolidCube(1.0f); // menggambar obyek kubus
        
        //Menggambar obyek segitiga
        glBegin(GL_TRIANGLES);
        	//segmen pertama - warna biru
        	glColor3f(0.2f, 0.3f, 0.5f);
        	glVertex3f(0.0f, 1.0f, 0.0f);
        	glVertex3f(-0.5f, 0.0f, 0.0f);
        	glVertex3f(0.5f, 0.0f, 0.0f);
        	
        	//segmen kedua - warna kuning
        	glColor3f(1.0f, 1.0f, 0.0f);
        	glVertex3f(0.0f, 1.0f, 0.0f);
        	glVertex3f(-0.5f, 0.0f, 0.0f);
        	glVertex3f(0.0f, -1.0f, 0.0f);
        	
        	//segmen ketiga - warna merah
        	glColor3f(0.7f, 0.0f, 0.0f);
        	glVertex3f(0.0f, 1.0f, 0.0f);
        	glVertex3f(0.5f, 0.0f, 0.0f);
        	glVertex3f(0.0f, -1.0f, 0.0f);
		glEnd();

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
void init(void)
{
        // inisialisasi warna latar belakang layar dalam hal ini warna putih (1.0, 1.0, 1.0, 0.0)
        glClearColor(1.0, 1.0, 1.0, 0.0);
        glEnable(GL_DEPTH_TEST);                                // mengaktifkan depth buffer
        glMatrixMode(GL_PROJECTION);                
        glLoadIdentity();
        gluPerspective(45.0, 1.0, 1.0, 100.0);        // set proyeksi ke perspektif
        glMatrixMode(GL_MODELVIEW);                                
        glLoadIdentity();                                                
        // inisialisasi kamera pandang
        gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
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
                glutPostRedisplay();        // update obyek
                break;
        // masukkan perintah disini bila tombol kanan ditekan
        case GLUT_KEY_RIGHT:                
                // dalam hal ini perintah rotasi obyek ke kanan sebanyak 1 derajat 
                objectAngleY += 1.0f;
                glutPostRedisplay();        // update obyek
                break;
        // masukkan perintah disini bila tombol atas ditekan
        case GLUT_KEY_UP:                
                // dalam hal ini perintah rotasi obyek ke atas sebanyak 1 derajat 
                objectAngleX -= 1.0f;
                glutPostRedisplay();        // update obyek
                break;
        // masukkan perintah disini bila tombol bawah ditekan
        case GLUT_KEY_DOWN:                
                // dalam hal ini perintah rotasi obyek ke bawah sebanyak 1 derajat 
                objectAngleX += 1.0f;
                glutPostRedisplay();        // update obyek
                break;
        // zoom in
        case GLUT_KEY_PAGE_UP:
                // masukkan perintah disini bila tombol PgUp ditekan
                posX += rotX * fraction;                
                posZ += rotZ * fraction;
                glutPostRedisplay();        // update obyek
                break;
        // zoom out
        case GLUT_KEY_PAGE_DOWN:
                // masukkan perintah disini bila tombol PgDn ditekan
                posX -= rotX * fraction;
                posZ -= rotZ * fraction;
                glutPostRedisplay();        // update obyek
                break;
        }
}


// fungsi untuk mengatur masukan dari keyboard 
void keyboard1(unsigned char key, int x, int y)
{
        float fraction = 0.5f;


        switch (key)
        {
        case 'w':        // bila tombol 'w' pada keyboard ditekan
                                // translasi ke atas
                        objectPositionY += fraction;
                        glutPostRedisplay();
                        break;
        case 's':         // bila tombol 's' pada keyboard ditekan
                                // translasi ke bawah
                        objectPositionY -= fraction;
                        glutPostRedisplay();
                break;
        case 'a':        // bila tombol 'a' pada keyboard ditekan
                                // translasi ke kiri
                        objectPositionX -= fraction;
                        glutPostRedisplay();
                break;
        case 'd':        // bila tombol 'd' pada keyboard ditekan
                                // translasi ke kanan
                        objectPositionX += fraction;
                        glutPostRedisplay();
                break;        
        case 'q':        // bila tombol 'q' pada keyboard ditekan
                                // translasi ke depan
                        objectPositionZ += fraction;
                        glutPostRedisplay();
                break;
        case 'e':        // bila tombol 'e' pada keyboard ditekan
                                // translasi ke belakang
                        objectPositionZ -= fraction;
                        glutPostRedisplay();
                break;
        case 't':        // bila tombol 't' pada keyboard ditekan
                                // perbesar ke arah sumbu Y
                        objectScaleY += 0.1f;
                        glutPostRedisplay();
                break;
        case 'g':        // bila tombol 'g' pada keyboard ditekan
                                // perkecil ke arah sumbu Y
                        objectScaleY = (objectScaleY - 0.1f, 1.0f);
                        glutPostRedisplay();
                break;
        case 'f':        // bila tombol 'f' pada keyboard ditekan
                                // perbesar ke arah sumbu X
                        objectScaleX += 0.1f;
                        glutPostRedisplay();
                break;
        case 'h':        // bila tombol 'h' pada keyboard ditekan
                                // perkecil ke arah sumbu X
                        objectScaleX = (objectScaleX - 0.1f, 1.0f);
                        glutPostRedisplay();
                break;
        case 'r':        // bila tombol 'r' pada keyboard ditekan
                                // perbesar ke arah sumbu Z
                        objectScaleZ += 0.1f;
                        glutPostRedisplay();
                break;
        case 'y':        // bila tombol 'y' pada keyboard ditekan
                                // perkecil ke arah sumbu Z
                        objectScaleZ = (objectScaleZ - 0.1f, 1.0f);
                        glutPostRedisplay();
                break;
        case 27:        // bila tombol 'esc' pada keyboard ditekan
                                // keluar program
                        exit(0);
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
        glutInitWindowSize(480, 480);                // besarnya jendela dalam piksel dalam hal ini 300x300
        glutInitWindowPosition(100, 100);        // posisi jendela dilayar komputer dalam piksel
        // judul jendela (wajib diubah dengan informasi NAMA / NIM - JUDUL PRAKTIKUM masing-masing)
        glutCreateWindow("SIRLY ZIADATUL MUSTAFIDAH / 2200018345 - TRANSFORMASI 2D DAN 3D");
        
        // panggil fungsi init untuk inisialisasi awal
        init();


        // event handler untuk display, reshape dan keyboard
        glutDisplayFunc(display);   // display
        glutReshapeFunc(reshape);   // reshape
        glutSpecialFunc(keyboard);  // keyboard
        glutKeyboardFunc(keyboard1);  // keyboard
        //glutTimerFunc(0, timer, 0); // aktifkan timer bila perlu


        // looping
        glutMainLoop();


        return 0;
}