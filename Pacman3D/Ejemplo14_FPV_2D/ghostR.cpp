#include "ghostR.h"

ghostR::ghostR(int d, int r, float v, float px, float pz, float rs, float cr, float cg, float cb){
    direccion = d;
    radioC = r;
    velocidad = v;
    Position[0] = px;
    Position[1] = 0;
    Position[2] = pz;
    radioS = rs;
    quad = gluNewQuadric();
    color[0] = cr;
    color[1] = cg;
    color[2] = cb;
}

ghostR::~ghostR(){
    //dtor
}

void ghostR::setRadio(float r){
    radioC = r;
}

void ghostR::setPosition(float x, float z){
    Position[0] = x;
    Position[2] = z;
}

float ghostR::getVelocidad(){
    return velocidad;
}

float ghostR::getRadio(){
    return radioC;
}

float ghostR::getPositionX(){
    return Position[0];
}

float ghostR::getPositionY(){
    return Position[1];
}

float ghostR::getPositionZ(){
    return Position[2];
}

float ghostR::getSize(){
    return radioS;
}

void ghostR::setVelocidad(float v){
    velocidad = v;
}

void ghostR::draw(){
    glPushMatrix();
    glColor3f(color[0], color[1], color[2]);
    glTranslatef(Position[0], Position[1], Position[2]);
    gluSphere(quad, radioS, 50, 50);
    glPopMatrix();
}

void ghostR::update(int celda){

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
    }



    switch(direccion){
        case 1:
            Position[0] += velocidad;
            break;
        case 2:
            Position[0] -= velocidad;
            break;
        case 3:
            Position[2] += velocidad;
            break;
        case 4:
            Position[2] -= velocidad;
            break;
    }

}
