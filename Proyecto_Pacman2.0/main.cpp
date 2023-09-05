
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include "pacman.h"
#include "ghost.h"
#include <math.h>
#include <iostream>
using namespace std;
//se define la cantidad de texturas que se manejaran
#define NTextures 5
GLuint	texture[NTextures];

char* filename = "maze.bmp";
char* filenamePac = "pacman.bmp";
char* filenamePac2 = "pacman_closed.bmp";
char* filenameGhostR = "fantasma_rojo.bmp";
char* filenameGhostB = "fantasma_azul.bmp";

pacman manpac;
ghost red(3, 7.0, 231.0);
ghost blue(4, 207.0, 231.0);

int control_matrix[10][10] = {
{3, 0, 7, 0, 4, 3, 0, 7, 0, 4},
{9, 0, 11, 7, 8, 8, 7, 11, 0, 10},
{5, 0, 10, 5, 4, 3, 6, 9, 0, 6},
{0, 0, 0, 3, 8, 8, 4, 0, 0, 0},
{0, 0, 11, 10, 0, 0, 9, 11, 0, 0},
{0, 0, 0, 9, 0, 0, 10, 0, 0, 0},
{3, 0, 11, 8, 4, 3, 8, 11, 0, 4},
{5, 4, 9, 7, 8, 8, 7, 10, 3, 6},
{3, 8, 6, 5, 4, 3, 6, 5, 8, 4},
{5, 0, 0, 0, 8, 8, 0, 0, 0, 6}};

int X[224], Y[248];


void loadTextureFromFile(char *filename, int id)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texture[id]);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, texture[id]);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0,
                     GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
    theTexMap.Reset();
}

void init_MatAdy(){
    for(int i = 0; i < 224; i++){
        X[i] = -1;
    }

    for(int i = 0; i < 248; i++){
        Y[i] = -1;
    }

    X[7] = 0;
    X[22] = 1;
    X[47] = 2;
    X[71] = 3;
    X[95] = 4;
    X[119] = 5;
    X[143] = 6;
    X[167] = 7;
    X[190] = 8;
    X[207] = 9;

    Y[7] = 0;
    Y[38] = 1;
    Y[62] = 2;
    Y[86] = 3;
    Y[110] = 4;
    Y[133] = 5;
    Y[157] = 6;
    Y[182] = 7;
    Y[206] = 8;
    Y[231] = 9;


}

int obtenerCelda(float p[2]){
    int x = int(p[0]);
    int y = int(p[1]);

    if(X[x] != -1 && Y[y] != -1){
        return control_matrix[Y[y]][X[x]];
    }
    return 0;
}

int obtenerCeldaG(float p[2]){
    int x = (abs(p[0] - floor(p[0])) < red.getVelocidad()) ? int(p[0]):0;
    int y = (abs(p[1] - floor(p[1])) < red.getVelocidad()) ? int(p[1]):0;

    if(X[x] != -1 && Y[y] != -1){
        return control_matrix[Y[y]][X[x]];
    }
    return 0;
}

void initPacman(){
    manpac.draw();
}

/*
 * Draw the texture in the OpenGL graphics window
 */
void drawScene(void)
{
    //Se actualizan las variables de control de los objetos
    red.update(obtenerCeldaG(red.Position));
    blue.update(obtenerCeldaG(blue.Position));
    manpac.update(obtenerCelda(manpac.Position));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //activa la textura
    glEnable(GL_TEXTURE_2D);

    //se dibuja el fantasma rojo
    glBindTexture(GL_TEXTURE_2D, texture[red.txt]);
    red.draw();

    //se dibuja el fantasma azul
    glBindTexture(GL_TEXTURE_2D, texture[blue.txt]);
    blue.draw();

    //Se dibuja el pacman
    glBindTexture(GL_TEXTURE_2D, texture[manpac.txtAct]);
    manpac.draw();

    glBindTexture(GL_TEXTURE_2D, texture[1]);


    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex2f(0.0, 0.0);

    glTexCoord2f(1.0, 0.0);
    glVertex2f(224.0, 0.0);

    glTexCoord2f(1.0, 1.0);
    glVertex2f(224.0, 248.0);

    glTexCoord2f(0.0, 1.0);
    glVertex2f(0.0, 248.0);

    glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 224, 248, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    loadTextureFromFile(filenamePac, 0);
    loadTextureFromFile( filename , 1);
    loadTextureFromFile(filenamePac2, 2);
    loadTextureFromFile(filenameGhostR, 3);
    loadTextureFromFile(filenameGhostB, 4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);

    init_MatAdy();
    initPacman();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		default:
			break;
   }
}

void SpecialInput(int key, int x, int y){
    int celda = obtenerCelda(manpac.Position);
    switch(key){
        case GLUT_KEY_UP:
            switch(celda){
                case 0:
                    if(manpac.estado == 2){
                        manpac.estado = 1;
                    }
                    else{
                        manpac.bufferKey = 1;
                    }
                    break;
                case 3:
                case 4:
                case 7:
                    manpac.bufferKey = 1;
                    break;
                case 5:
                case 6:
                case 8:
                case 9:
                case 10:
                case 11:
                    manpac.estado = 1;
                    break;
            }
            break;
        case GLUT_KEY_DOWN:
            switch(celda){
                case 0:
                    if(manpac.estado == 1){
                        manpac.estado = 2;
                    }
                    else{
                        manpac.bufferKey = 2;
                    }
                    break;
                case 5:
                case 6:
                case 8:
                    manpac.bufferKey = 2;
                    break;
                case 3:
                case 4:
                case 7:
                case 9:
                case 10:
                case 11:
                    manpac.estado = 2;
                    break;
            }
            break;
        case GLUT_KEY_RIGHT:
            switch(celda){
                case 0:
                    if(manpac.estado == 4){
                        manpac.estado = 3;
                    }
                    else{
                        manpac.bufferKey = 3;
                    }
                    break;
                case 4:
                case 6:
                case 10:
                    manpac.bufferKey = 3;
                    break;
                case 3:
                case 5:
                case 7:
                case 8:
                case 9:
                case 11:
                    manpac.estado = 3;
                    break;
            }
            break;
        case GLUT_KEY_LEFT:
            switch(celda){
                case 0:
                    if(manpac.estado == 3){
                        manpac.estado = 4;
                    }
                    else{
                        manpac.bufferKey = 4;
                    }
                    break;
                case 3:
                case 5:
                case 9:
                    manpac.bufferKey = 4;
                    break;
                case 4:
                case 6:
                case 7:
                case 8:
                case 10:
                case 11:
                    manpac.estado = 4;
                    break;
            }
            break;
        case GLUT_KEY_F3:
            manpac.estado = 0;
            break;
    }
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(224, 248);
	glutInitWindowPosition(100, 100);
    glutCreateWindow("Proyecto");
    init();
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput);
	glutMainLoop();
	return 0;
}
