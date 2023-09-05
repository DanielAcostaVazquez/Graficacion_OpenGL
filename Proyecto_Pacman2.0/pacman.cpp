#include "pacman.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include <iostream>
using namespace std;


pacman::pacman()
{
    estado = 0;
    tempo = 0;
    txtAct = 0;
    bufferKey = -1;
}

pacman::~pacman()
{
    //dtor
}

void pacman::draw()
{
   glBegin(GL_QUADS);

   switch(estado){
    case 1:
        glTexCoord2f(0.0, 0.0);
        glVertex2f(Position[0], (Position[1] + 11));

        glTexCoord2f(1.0, 0.0);
        glVertex2f(Position[0], Position[1]);

        glTexCoord2f(1.0, 1.0);
        glVertex2f((Position[0] + 11), Position[1]);

        glTexCoord2f(0.0, 1.0);
        glVertex2f((Position[0] + 11), (Position[1] + 11));
        break;
    case 2:
        glTexCoord2f(0.0, 0.0);
        glVertex2f((Position[0] + 11), Position[1]);

        glTexCoord2f(1.0, 0.0);
        glVertex2f((Position[0] + 11), (Position[1] + 11));

        glTexCoord2f(1.0, 1.0);
        glVertex2f(Position[0], (Position[1] + 11));

        glTexCoord2f(0.0, 1.0);
        glVertex2f(Position[0], Position[1]);
        break;
    case 3:
        glTexCoord2f(0.0, 0.0);
        glVertex2f(Position[0], Position[1]);

        glTexCoord2f(1.0, 0.0);
        glVertex2f((Position[0] + 11), Position[1]);

        glTexCoord2f(1.0, 1.0);
        glVertex2f((Position[0] + 11), (Position[1] + 11));

        glTexCoord2f(0.0, 1.0);
        glVertex2f(Position[0], (Position[1] + 11));
        break;
    case 4:
        glTexCoord2f(0.0, 0.0);
        glVertex2f((Position[0] + 11), (Position[1] + 11));

        glTexCoord2f(1.0, 0.0);
        glVertex2f(Position[0], (Position[1] + 11));

        glTexCoord2f(1.0, 1.0);
        glVertex2f(Position[0], Position[1]);

        glTexCoord2f(0.0, 1.0);
        glVertex2f((Position[0] + 11), Position[1]);
        break;
    default:
        glTexCoord2f(0.0, 0.0);
        glVertex2f(Position[0], Position[1]);

        glTexCoord2f(1.0, 0.0);
        glVertex2f((Position[0] + 11), Position[1]);

        glTexCoord2f(1.0, 1.0);
        glVertex2f((Position[0] + 11), (Position[1] + 11));

        glTexCoord2f(0.0, 1.0);
        glVertex2f(Position[0], (Position[1] + 11));
   }
   glEnd();

}


void pacman::update(int celda)
{
    tempo += 1;
    if(tempo > 1000){
        txtAct = (txtAct == 0) ? 2:0;
        tempo = 0;
    }

    if(Position[0] > 224){
        Position[0] = 7;
        Position[1] = 110;
    }
    else if(Position[0] < 0){
        Position[0] = 224;
        Position[1] = 110;
    }

    if(celda != 0 && bufferKey != -1){
        estado = bufferKey;
        bufferKey = -1;
    }
    switch(estado){
        case 1:
            switch(celda){
                case 3:
                case 4:
                case 7:
                    estado = 0;
                    break;
                default:
                    Position[1] -= velocidad;
            }
            break;
        case 2:
            switch(celda){
                case 5:
                case 6:
                case 8:
                    estado = 0;
                    break;
                default:
                    Position[1] += velocidad;

            }
            break;
        case 3:
            switch(celda){
                case 4:
                case 6:
                case 10:
                    estado = 0;
                    break;
                default:
                    Position[0] += velocidad;
            }
            break;
        case 4:
            switch(celda){
                case 3:
                case 5:
                case 9:
                    estado = 0;
                    break;
                default:
                    Position[0] -= velocidad;
            }
    }
/*
    switch(estado){
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
*/
    //draw();
    //cout << "X=" << Position[0] << "; Y= " << Position[1] << endl;
}

