
#include "ghost.h"

ghost::ghost(int d, int r, float v, float px, float pz, float rs, float cr, float cg, float cb, int lvl){
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
    iaLvl = lvl;
}

ghost::~ghost(){
    //dtor
}

void ghost::setRadio(float r){
    radioC = r;
}

void ghost::setPosition(float x, float z){
    Position[0] = x;
    Position[2] = z;
}

float ghost::getVelocidad(){
    return velocidad;
}

float ghost::getRadio(){
    return radioC;
}

float ghost::getPositionX(){
    return Position[0];
}

float ghost::getPositionY(){
    return Position[1];
}

float ghost::getPositionZ(){
    return Position[2];
}

float ghost::getSize(){
    return radioS;
}

int ghost::getDireccion(){
    return direccion;
}

int ghost::getIaLvl(){
    return iaLvl;
}

void ghost::setVelocidad(float v){
    velocidad = v;
}

void ghost::draw(){
    glPushMatrix();
    glColor3f(color[0], color[1], color[2]);
    glTranslatef(Position[0], Position[1], Position[2]);
    gluSphere(quad, radioS, 50, 50);
    glPopMatrix();
}

void ghost::update(int celda, int d){

    if(direccion == 0){

        switch(celda){
            case 3:
                direccion = ((rand() % 2) == 1) ? 2:3;
                break;
            case 4:
                direccion = ((rand() % 2) == 1) ? 2:4;
                break;
            case 5:
                direccion = ((rand() % 2) == 1) ? 1:3;
                break;
            case 6:
                direccion = ((rand() % 2) == 1) ? 1:4;
                break;
        }
    }
    else{
        if(celda != 0) direccion = d;
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
