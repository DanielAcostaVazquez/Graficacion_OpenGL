

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "wall.h"
#include "ghost.h"
#include "ghostR.h"

//Variables dimensiones de la pantalla
int WIDTH=500;
int HEIGTH=500;
//Variables para establecer los valores de gluPerspective
float FOVY=60.0;
float ZNEAR=0.01;
float ZFAR=100.0;
//Variables para definir la posicion del observador
//Por default se inicializa en (0,0,0) y mirando hacia x positivo
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X=8.0;//0
float EYE_Y=0.0; //50
float EYE_Z=0.0; //0
float CENTER_X=1;//0
float CENTER_Y=0;
float CENTER_Z=0;
float UP_X=0; //1
float UP_Y=1; //0
float UP_Z=0;
//Variables para matrices de rotacion y traslación
float Theta=0;
//float Radio=1.0;
float PI = 3.14159265359;
float Direction[3] = {1.0,0.0,0.0};

float FEye_X=0.0;
float FEye_Y=0.0;
float FEye_Z=0.0;

float FEye[3];

int score;
int time;

#define NObjetos 42
void *objetos[NObjetos];

int control_matrix[10][10] = {
{3, 0, 7, 0, 4, 3, 0, 7, 0, 4},
{9, 0, 11, 7, 8, 8, 7, 11, 0, 10},
{5, 0, 10, 5, 4, 3, 6, 9, 0, 6},
{0, 0, 0, 3, 8, 8, 4, 0, 0, 0},
{0, 0, 9, 10, 0, 0, 9, 10, 0, 0},
{0, 0, 0, 9, 0, 0, 10, 0, 0, 0},
{3, 0, 11, 8, 4, 3, 8, 11, 0, 4},
{5, 4, 9, 7, 8, 8, 7, 10, 3, 6},
{3, 8, 6, 5, 4, 3, 6, 5, 8, 4},
{5, 0, 0, 0, 8, 8, 0, 0, 0, 6}};

int X[61], Y[55];

//Recordar que (pi/180 = r/g) donde "r" son radianes y "g" grados
//Se aplica la formula r
float RadToDeg(float r)
{
      return ((180.0*r)/PI);
}

float DegToRad(float g)
{
      return ((g*PI)/180.0);
}


//X[0] = X(30) | X[60] = X(-30)
//Y[0] = Z(-27) | Y[54] = Z(27)

//X seria nuestro y en ambiente 2d y Z nuestro x
void init_MatAdy(){
    for(int i = 0; i < 61; i++){
        X[i] = -1;
    }

    for(int i = 0; i < 55; i++){
        Y[i] = -1;
    }

    X[2] = 0;
    X[10] = 1;
    X[16] = 2;
    X[22] = 3;
    X[28] = 4;
    X[34] = 5;
    X[40] = 6;
    X[46] = 7;
    X[52] = 8;
    X[58] = 9;

    Y[2] = 0;
    Y[6] = 1;
    Y[12] = 2;
    Y[18] = 3;
    Y[24] = 4;
    Y[30] = 5;
    Y[36] = 6;
    Y[42] = 7;
    Y[48] = 8;
    Y[52] = 9;


}

int obtenerCeldaG(float px, float pz, float v){
    int x = (abs(px - floor(px)) < v) ? int(px):0;
    int y = (abs(pz - floor(pz)) < v) ? int(pz):0;

    if((X[30 - x] != -1) && (Y[27 + y] != -1)){
        return control_matrix[X[30 - x]][Y[27 + y]];
    }
    return 0;
}

float heuristica(float x, float z){
    return (sqrt(((EYE_Z - z)*(EYE_Z - z)) + ((EYE_X - x)*(EYE_X - x))));
}

float moveUp(float x, float z){
    do{
        if(x == 20){
            x += 8;
        }
        else{
            x += 6;
        }
    }
    while(obtenerCeldaG(x,z,0.01) != 0);
    return x;
}

float moveDown(float x, float z){
    do{
        if(x == 28){
            x -= 8;
        }
        else{
            x -= 6;
        }
    }
    while(obtenerCeldaG(x,z,0.01) != 0);
    return x;
}

float moveLeft(float x, float z){
    do{
        if(z == -21 || z == 25){
            z -= 4;
        }
        else{
            z -= 6;
        }
    }
    while(obtenerCeldaG(x,z,0.01) != 0);
    return z;
}

float moveRight(float x, float z){
    do{
        if(z == -25 || z == 21){
            z += 4;
        }
        else{
            z += 6;
        }
    }
    while(obtenerCeldaG(x,z,0.01) != 0);
    return z;
}

int noHijos(int c){
    switch(c){
        case 3:
        case 4:
        case 5:
        case 6:
            return 1;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            return 2;
            break;
        case 11:
            return 3;
            break;
    }
    return 0;
}

float minAi(float x, float z, int celda, int dir, int h);

float maxAi(float x, float z, int celda, int dir, int h){
    float a, b, c;
    switch(celda){
        case 3:
            if(dir == 1){
                if(h == 0){
                    return heuristica(x, moveRight(x,z));
                }
                else{
                    return minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }
            }
            else{
                if(h == 0){
                    return heuristica(moveDown(x,z), z);
                }
                else{
                    return minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }
            }
            break;
        case 4:
            if(dir == 1){
                if(h == 0){
                    return heuristica(x, moveLeft(x,z));
                }
                else{
                    return minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }
            }
            else{
                if(h == 0){
                    return heuristica(moveDown(x,z), z);
                }
                else{
                    return minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }
            }
            break;
        case 5:
            if(dir == 2){
                if(h == 0){
                    return heuristica(x, moveRight(x,z));
                }
                else{
                    return minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }
            }
            else{
               if(h == 0){
                    return heuristica(moveUp(x,z), z);
                }
                else{
                    return minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                }
            }
            break;
        case 6:
            if(dir == 2){
                if(h == 0){
                    return heuristica(x, moveLeft(x,z));
                }
                else{
                    return minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }
            }
            else{
                if(h == 0){
                    return heuristica(moveUp(x,z),z);
                }
                else{
                    return minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                }
            }
            break;
        case 7:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(x, moveRight(x,z));
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    b = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 8:
            if(dir == 2){
                if(h == 0){
                    a = heuristica(x, moveRight(x,z));
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    b = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 9:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(moveDown(x,z), z);
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 10:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(moveDown(x,z), z);
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }

                if(a > b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 11:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(x, moveRight(x,z));
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    c = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a >= b && a >= c){
                    return a;
                }
                else if (b >= a && b>= c){
                   return b;
                }
                else{
                    return c;
                }
            }
            else if (dir == 2){
                if (h == 0){
                    a = heuristica(moveDown(x,z),z);
                    b = heuristica(x, moveRight(x,z));
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    c = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a >= b && a >= c){
                    return a;
                }
                else if (b >= a && b>= c){
                    return b;
                }
                else{
                   return c;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(moveDown(x,z),z);
                    c = heuristica(x, moveRight(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    c = minAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if (a >= b && a >= c){
                    return a;
                }
                else if (b >= a && b >= c){
                    return b;
                }
                else{
                    return c;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(moveDown(x,z),z);
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = minAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = minAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    c = minAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a >= b && a >= c){
                    return a;
                }
                else if (b >= a && b>= c){
                    return b;
                }
                else{
                    return c;
                }
            }
    }
    return 0;
}

float minAi(float x, float z, int celda, int dir, int h){
    float a, b, c;
    switch(celda){
        case 3:
            if(dir == 1){
                if(h == 0){
                    return heuristica(x, moveRight(x,z));
                }
                else{
                    return maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }
            }
            else{
                if(h == 0){
                    return heuristica(moveDown(x,z), z);
                }
                else{
                    return maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }
            }
            break;
        case 4:
            if(dir == 1){
                if(h == 0){
                    return heuristica(x, moveLeft(x,z));
                }
                else{
                    return maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }
            }
            else{
                if(h == 0){
                    return heuristica(moveDown(x,z), z);
                }
                else{
                    return maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }
            }
            break;
        case 5:
            if(dir == 2){
                if(h == 0){
                    return heuristica(x, moveRight(x,z));
                }
                else{
                    return maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }
            }
            else{
               if(h == 0){
                    return heuristica(moveUp(x,z), z);
                }
                else{
                    return maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                }
            }
            break;
        case 6:
            if(dir == 2){
                if(h == 0){
                    return heuristica(x, moveLeft(x,z));
                }
                else{
                    return maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }
            }
            else{
                if(h == 0){
                    return heuristica(moveUp(x,z),z);
                }
                else{
                    return maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                }
            }
            break;
        case 7:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(x, moveRight(x,z));
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 8:
            if(dir == 2){
                if(h == 0){
                    a = heuristica(x, moveRight(x,z));
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 9:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(moveDown(x,z), z);
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 10:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(moveDown(x,z), z);
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }

                if(a < b){
                    return a;
                }
                else{
                   return b;
                }
            }
            break;
        case 11:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(x, moveRight(x,z));
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    c = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a <= b && a <= c){
                    return a;
                }
                else if (b <= a && b <= c){
                   return b;
                }
                else{
                    return c;
                }
            }
            else if (dir == 2){
                if (h == 0){
                    a = heuristica(moveDown(x,z),z);
                    b = heuristica(x, moveRight(x,z));
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    c = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a <= b && a <= c){
                    return a;
                }
                else if (b <= a && b <= c){
                    return b;
                }
                else{
                   return c;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(moveDown(x,z),z);
                    c = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    c = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if (a <= b && a <= c){
                    return a;
                }
                else if (b <= a && b <= c){
                    return b;
                }
                else{
                    return c;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(moveDown(x,z),z);
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    c = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a <= b && a <= c){
                    return a;
                }
                else if (b <= a && b<= c){
                    return b;
                }
                else{
                    return c;
                }
            }
    }
    return 0;
}

int minimax(float x, float z, int celda, int dir, int h){
    float a, b, c;
    switch(celda){
        case 3:
            if(dir == 1){
                return 3;
            }
            else{
                return 2;
            }

            break;
        case 4:
            if(dir == 1){
                return 4;
            }
            else{
                return 2;
            }
            break;
        case 5:
            if(dir == 2){
                return 3;
            }
            else{
               return 1;
            }
            break;
        case 6:
            if(dir == 2){
                return 4;
            }
            else{
                return 1;
            }
            break;
        case 7:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(x, moveRight(x,z));
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return 3;
                }
                else{
                   return 4;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return 2;
                }
                else{
                   return 3;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return 2;
                }
                else{
                    return 4;
                }
            }
            break;
        case 8:
            if(dir == 2){
                if(h == 0){
                    a = heuristica(x, moveRight(x,z));
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return 3;
                }
                else{
                   return 4;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return 1;
                }
                else{
                    return 3;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                     return 1;
                }
                else{
                    return 4;
                }
            }
            break;
        case 9:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                   return 1;
                }
                else{
                    return 3;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if(a < b){
                    return 2;
                }
                else{
                    return 3;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(moveDown(x,z),z);
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }

                if(a < b){
                    return 1;
                }
                else{
                  return 2;
                }
            }
            break;
        case 10:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return 1;
                }
                else{
                    return 4;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z), z);
                    b = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if(a < b){
                    return 2;
                }
                else{
                    return 4;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z), z);
                    b = heuristica(moveDown(x,z),z);
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                }

                if(a < b){
                    return 1;
                }
                else{
                    return 2;
                }
            }
            break;
        case 11:
            if(dir == 1){
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(x, moveRight(x,z));
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    c = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a <= b && a <= c){
                    return 1;
                }
                else if (b <= a && b<= c){
                   return 3;
                }
                else{
                    return 4;
                }
            }
            else if (dir == 2){
                if(h == 0){
                    a = heuristica(moveDown(x,z),z);
                    b = heuristica(x, moveRight(x,z));
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    b = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                    c = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a <= b && a <= c){
                    return 2;
                }
                else if (b <= a && b<= c){
                    return 3;
                }
                else{
                   return 4;
                }
            }
            else if (dir == 3){
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(moveDown(x,z),z);
                    c = heuristica(x, moveRight(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    c = maxAi(x, moveRight(x,z), obtenerCeldaG(x, moveRight(x,z), 0.01), 3, h-1);
                }

                if (a <= b && a <= c){
                    return 1;
                }
                else if (b <= a && b <= c){
                    return 2;
                }
                else{
                    return 3;
                }
            }
            else{
                if(h == 0){
                    a = heuristica(moveUp(x,z),z);
                    b = heuristica(moveDown(x,z),z);
                    c = heuristica(x, moveLeft(x,z));
                }
                else{
                    a = maxAi(moveUp(x,z), z, obtenerCeldaG(moveUp(x,z), z, 0.01), 1, h-1);
                    b = maxAi(moveDown(x,z), z, obtenerCeldaG(moveDown(x,z), z, 0.01), 2, h-1);
                    c = maxAi(x, moveLeft(x,z), obtenerCeldaG(x, moveLeft(x,z), 0.01), 4, h-1);
                }

                if (a <= b && a <= c){
                    return 1;
                }
                else if (b <= a && b<= c){
                    return 2;
                }
                else{
                    return 4;
                }
            }
    }
    return 0;
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glShadeModel(GL_FLAT);

    //unidades duplicadas
    //paredes
    objetos[0] = new wall(30, 0, 0.1, 27);
    objetos[1] = new wall(0, -27, 30, 0.1);
    objetos[2] = new wall(0, 27, 30, 0.1);
    objetos[3] = new wall(-30, 0, 0.1, 27);

    //primera fila
    objetos[4] = new wall(24, -20, 2, 3);
    objetos[5] = new wall(24, -9, 2, 4);
    objetos[6] = new wall(26, 0, 4, 1);
    objetos[7] = new wall(24, 9, 2, 4);
    objetos[8] = new wall(24, 20, 2, 3);

    //segunda fila
    objetos[9] = new wall(17, -20, 1, 3);
    objetos[10] = new wall(17, 0, 1, 7);
    objetos[11] = new wall(17, 20, 1, 3);

    //tercera fila
    objetos[12] = new wall(11, -12, 7, 1);
    objetos[13] = new wall(11, -9, 1, 4);
    objetos[14] = new wall(14, 0, 4, 1);
    objetos[15] = new wall(11, 9, 1, 4);
    objetos[16] = new wall(11, 12, 7, 1);

    //cuarta fila
    objetos[17] = new wall(2, -22, 10, 5);
    objetos[18] = new wall(-4, -12, 4, 1);
    //centro
    objetos[19] = new wall(2, 0, 4, 7);

    objetos[20] = new wall(2, 22, 10, 5);
    objetos[21] = new wall(-4, 12, 4, 1);

    //quinta fila
    objetos[22] = new wall(-13, -20, 1, 3);
    objetos[23] = new wall(-16, -18, 4, 1);
    objetos[24] = new wall(-13, -9, 1, 4);//-8

    objetos[25] = new wall(-7, 0, 1, 7);
    objetos[26] = new wall(-10, 0, 4, 1);

    objetos[27] = new wall(-13, 20, 1, 3);
    objetos[28] = new wall(-16, 18, 4, 1);
    objetos[29] = new wall(-13, 9, 1, 4);//

    //sexta fila
    objetos[30] = new wall(-19, -25, 1, 2);
    objetos[31] = new wall(-25, -14, 1, 9);
    objetos[32] = new wall(-22, -12, 4, 1);

    objetos[33] = new wall(-19, 0, 1, 7);
    objetos[34] = new wall(-22, 0, 4, 1);

    objetos[35] = new wall(-19, 25, 1, 2);
    objetos[36] = new wall(-25, 14, 1, 9);
    objetos[37] = new wall(-22, 12, 4, 1);
    //fin paredes

    //fantasmas
    objetos[38] = new ghost(0, 1, 0.0025, -28.0, -25.0, 1, 1.0, 0.0, 0.0, 0);//0.01
    objetos[39] = new ghost(0, 1, 0.0025, 28.0, 25.0, 1, 0.8, 0.3, 0.4, 0);
    objetos[40] = new ghostR(0, 1, 0.0025, -28.0, 25.0, 1, 0.3, 0.8, 1.0);
    objetos[41] = new ghostR(0, 1, 0.0025, 28.0, -25.0, 1, 0.9, 0.6, 0.0);

    init_MatAdy();


}

void LookAt()
{
    float x, z;
    x = Direction[0];
    z = Direction[2];
    Direction[0] = x * cos(DegToRad(Theta)) + z * sin(DegToRad(Theta));
    Direction[2] = -x * sin(DegToRad(Theta)) + z * cos(DegToRad(Theta));
    CENTER_X = EYE_X + Direction[0];
    CENTER_Z = EYE_Z + Direction[2];
}

bool checkWallCollision(float p[3]){
    wall *aux;
    for(int i = 0; i < 38; i++){
        aux = (wall *)objetos[i];
        if(i <= 3){
            if(p[0] <= -29.5 || p[0] >= 29.5){
                return true;
            }
            if(p[2] <= -26.5 || p[2] >= 26.5){
                return true;
            }
        }
        else{
            if((p[0] <= (aux->getPositionX() + aux->getSizeX())) &&(p[0] >= (aux->getPositionX() - aux->getSizeX()))){
                if((p[2] <= (aux->getPositionZ() + aux->getSizeZ())) && (p[2] >= (aux->getPositionZ() - aux->getSizeZ()))){
                    return true;
                }
            }
        }
    }
    //std::cout << "x= " << EYE_X << " y= " << EYE_Y << " z= " << EYE_Z << std::endl;
    return false;
}

void SpecialInput(int key, int x, int y)
{
    switch(key){
                 case GLUT_KEY_UP:
                     FEye_X += Direction[0];
                     FEye_Y += Direction[1];
                     FEye_Z += Direction[2];
                     FEye[0] = FEye_X;
                     FEye[1] = FEye_Y;
                     FEye[2] = FEye_Z;
                     if(!checkWallCollision(FEye)){
                     EYE_X += Direction[0];
                     EYE_Y += Direction[1];
                     EYE_Z += Direction[2];
                     CENTER_X = EYE_X + Direction[0];
                     CENTER_Y = EYE_Y + Direction[1];
                     CENTER_Z = EYE_Z + Direction[2];
                     }
                     break;
                case GLUT_KEY_DOWN:
                     FEye_X -= Direction[0];
                     FEye_Y -= Direction[1];
                     FEye_Z -= Direction[2];
                     FEye[0] = FEye_X;
                     FEye[1] = FEye_Y;
                     FEye[2] = FEye_Z;
                     if(!checkWallCollision(FEye)){
                     EYE_X -= Direction[0];
                     EYE_Y -= Direction[1];
                     EYE_Z -= Direction[2];
                     CENTER_X = EYE_X + Direction[0];
                     CENTER_Y = EYE_Y + Direction[1];
                     CENTER_Z = EYE_Z + Direction[2];
                     }
                     break;
                case GLUT_KEY_LEFT:
                     Theta = 3.0f;
                     //Theta = (Theta < 0.0) ? 359.0 : Theta;
                     LookAt();
                      break;
                case GLUT_KEY_RIGHT:
                     Theta = -3.0f;
                     //Theta = (Theta > 359.0) ? 0.0 : Theta;
                     LookAt();
                     break;
    }

    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glutPostRedisplay();
}

void Keyboard (unsigned char key, int x, int y){
    FEye_X = EYE_X;
    FEye_Y = EYE_Y;
    FEye_Z = EYE_Z;

    switch(key){
        case 'w':
        case 'W':
            FEye_X += Direction[0];
            FEye_Y += Direction[1];
            FEye_Z += Direction[2];
            FEye[0] = FEye_X;
            FEye[1] = FEye_Y;
            FEye[2] = FEye_Z;
            if(!checkWallCollision(FEye)){
                EYE_X += Direction[0];
                EYE_Y += Direction[1];
                EYE_Z += Direction[2];
                CENTER_X = EYE_X + Direction[0];
                CENTER_Y = EYE_Y + Direction[1];
                CENTER_Z = EYE_Z + Direction[2];
            }
            break;
        case 's':
        case 'S':
            FEye_X -= Direction[0];
            FEye_Y -= Direction[1];
            FEye_Z -= Direction[2];
            FEye[0] = FEye_X;
            FEye[1] = FEye_Y;
            FEye[2] = FEye_Z;
            if(!checkWallCollision(FEye)){
                EYE_X -= Direction[0];
                EYE_Y -= Direction[1];
                EYE_Z -= Direction[2];
                CENTER_X = EYE_X + Direction[0];
                CENTER_Y = EYE_Y + Direction[1];
                CENTER_Z = EYE_Z + Direction[2];
            }
            break;
        case 'a':
        case 'A':
            Theta = 3.0f;
            //Theta = (Theta < 0.0) ? 359.0 : Theta;
            LookAt();
            break;
        case 'd':
        case 'D':
            Theta = -3.0f;
            //Theta = (Theta > 359.0) ? 0.0 : Theta;
            LookAt();
            break;
    }
    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glutPostRedisplay();

}

void drawWalls(){
    wall *aux;
    for(int i = 0; i < 38; i++){
        aux = (wall *)objetos[i];
        aux->draw();
    }
}

void drawGhosts(){
    ghost *aux;
    ghostR *aux2;
    for(int i = 38; i < NObjetos; i++){
        if(i == 40 || i == 41){
            aux2 = (ghostR *)objetos[i];
            aux2->draw();
        }
        else{
            aux = (ghost *)objetos[i];
            aux->draw();
        }
    }
}

void updateGhosts(){
    ghost *aux;
    ghostR *aux2;
    int c;
    for(int i = 38; i < NObjetos; i++){
        if(i == 40 || i == 41){
            aux2 = (ghostR *)objetos[i];
            c = obtenerCeldaG(aux2->getPositionX(), aux2->getPositionZ(), aux2->getVelocidad());
        }
        else{
            aux = (ghost *)objetos[i];
            c = obtenerCeldaG(aux->getPositionX(), aux->getPositionZ(), aux->getVelocidad());
        }


        if(c != 0){
            if(i == 40 || i == 41){
                aux2->update(c);
            }
            else{
                aux->update(c, minimax(aux->getPositionX(),aux->getPositionZ(), c, aux->getDireccion(), aux->getIaLvl()));
            }

        }
        else{
            if(i == 40 || i == 41){
               aux2->update(c);
            }
            else{
              aux->update(c,0);
            }

        }

    }
}

bool checkPlayerCollision(){
    ghost *aux;
    ghostR *aux2;
    float d;
    for(int i = 38; i < NObjetos; i++){
        if(i == 40 || i == 41){
            aux2 = (ghostR *)objetos[i];
            d = heuristica(aux2->getPositionX(), aux2->getPositionZ());
            if(d <= (aux2->getRadio())){
                std::cout << "Tu puntuacion final es de: " << score << std::endl;
                exit(0);
            }
        }
        else{
            aux = (ghost *)objetos[i];
            d = heuristica(aux->getPositionX(), aux->getPositionZ());
            if(d <= (aux->getRadio())){
                std::cout << "Tu puntuacion final es de: " << score << std::endl;
                exit(0);
            }
        }
    }
}

void updateScore(){
    time++;
    if(time>1000){
        score += 100;
        time = 0;

    }
    std::string t = "Pacman 3D   Score: "+std::to_string(score);
    glutSetWindowTitle(t.c_str());
}
//--------------------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawWalls();
    updateGhosts();
    drawGhosts();
    checkPlayerCollision();
    updateScore();
    glFlush();
    //std::cout << "x= " << EYE_X << " y= " << EYE_Y << " z= " << EYE_Z << std::endl;
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WIDTH, HEIGTH);
    glutCreateWindow("Pacman 3D   Score: 0");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(SpecialInput);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
    return 0;
}

