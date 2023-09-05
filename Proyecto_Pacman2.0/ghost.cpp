#include "ghost.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include <iostream>
using namespace std;

ghost::ghost(int textura, float x, float y){
    txt = textura;
    Position[0] = x;
    Position[1] = y;
    direccion = 0;
}

ghost::~ghost(){
    //dtor
}

void ghost::draw(){
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex2f((Position[0] + 11), (Position[1] + 11));

    glTexCoord2f(1.0, 0.0);
    glVertex2f(Position[0], (Position[1] + 11));

    glTexCoord2f(1.0, 1.0);
    glVertex2f(Position[0], Position[1]);

    glTexCoord2f(0.0, 1.0);
    glVertex2f((Position[0] + 11), Position[1]);

   glEnd();
}

float ghost::getVelocidad(){
    return velocidad;
}

void ghost::update(int celda){

    if(Position[0] > 224){
        Position[0] = 7;
        Position[1] = 110;
    }
    else if(Position[0] < 0){
        Position[0] = 224;
        Position[1] = 110;
    }

    if(direccion == 0){

        switch(celda){
            case 5:
                direccion = ((rand() % 2) == 1) ? 1:3;
                break;
            case 6:
                direccion = ((rand() % 2) == 1) ? 1:4;
                break;
        }
    }

    if(celda != 0){
    /*cout << "CELDA: " << celda << "     CoordX Rojo: " << Position[0] << "      CoordY Rojo: " << Position[1]
    << endl<<"----DirPrev: " << direccion;*/
        switch(celda){

            case 3:
                direccion = (direccion == 1) ? 3:2;
                break;
            case 4:
                direccion = (direccion == 1) ? 4:2;
                break;
            case 5:
                direccion = (direccion == 2) ? 3:1;
                break;
            case 6:
                direccion = (direccion == 2) ? 4:1;
                break;
            case 7:
                if(direccion == 1){
                    direccion = ((rand() % 2) + 3);
                }
                else if(direccion == 3){
                    direccion = ((rand() % 2) + 2);
                }
                else if (direccion == 4){
                    direccion = ((rand() % 2) == 1) ? 4:2;
                }
                break;
            case 8:
                if(direccion == 2){
                    direccion = ((rand() % 2) + 3);
                }
                else if(direccion == 3){
                    direccion = ((rand() % 2) == 1) ? 3:1;
                }
                else if (direccion == 4){
                    direccion = ((rand() % 2) == 1) ? 4:1;
                }
                break;
            case 9:
                if(direccion == 2){
                    direccion = ((rand() % 2) + 2);
                }
                else if(direccion == 1){
                    direccion = ((rand() % 2) == 1) ? 1:3;
                }
                else if (direccion == 4){
                    direccion = ((rand() % 2) == 1) ? 2:1;
                }
                break;
            case 10:
                if(direccion == 2){
                    direccion = ((rand() % 2) == 1) ? 4:2;
                }
                else if(direccion == 1){
                    direccion = ((rand() % 2) == 1) ? 1:4;
                }
                else if (direccion == 3){
                    direccion = ((rand() % 2) == 1) ? 2:1;
                }
                break;
            case 11:
                if(direccion == 1){
                    switch(rand() % 3){
                        case 0:
                            direccion = 1;
                            break;
                        case 1:
                            direccion = 3;
                            break;
                        case 2:
                            direccion = 4;
                            break;
                    }
                }
                else if (direccion == 2){
                    switch(rand() % 3){
                        case 0:
                            direccion = 2;
                            break;
                        case 1:
                            direccion = 3;
                            break;
                        case 2:
                            direccion = 4;
                            break;
                    }
                }
                else if (direccion == 3){
                    switch(rand() % 3){
                        case 0:
                            direccion = 3;
                            break;
                        case 1:
                            direccion = 1;
                            break;
                        case 2:
                            direccion = 2;
                            break;
                    }
                }
                else if (direccion == 4){
                    switch(rand() % 3){
                        case 0:
                            direccion = 4;
                            break;
                        case 1:
                            direccion = 1;
                            break;
                        case 2:
                            direccion = 2;
                            break;
                    }
                }
                break;

        }
    //cout << "      DirPost: " << direccion << endl <<endl;
    }



    switch(direccion){
        case 1:
            Position[1] -= velocidad;
            break;
        case 2:
            Position[1] += velocidad;
            break;
        case 3:
            Position[0] += velocidad;
            break;
        case 4:
            Position[0] -= velocidad;
            break;
    }


}
