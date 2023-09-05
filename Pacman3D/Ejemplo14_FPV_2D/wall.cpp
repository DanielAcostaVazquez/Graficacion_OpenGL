#include "wall.h"

wall::wall(float x, float z, float xSize, float zSize)
{
    Position[0] = x;
    Position[1] = 0;
    Position[2] = z;
    Size[0] = xSize;
    Size[2] = zSize;

}

wall::~wall()
{
    //dtor
}

void wall::setRadio(float r)
{
    radio = r;
}

void wall::setPosition(float nP[3])
{
    Position[0] = nP[0];
    Position[1] = nP[1];
    Position[2] = nP[2];
}

float wall::getPositionX(){
    return Position[0];
}

float wall::getPositionY(){
    return Position[1];
}

float wall::getPositionZ(){
    return Position[2];
}

float wall::getSizeX(){
    return Size[0];
}

float wall::getSizeY(){
    return Size[1];
}

float wall::getSizeZ(){
    return Size[2];
}

float wall::getRadio(){
    return radio;
}

void wall::draw()
{
    glPushMatrix();
    glTranslatef(Position[0], Position[1], Position[2]);
    glScaled(Size[0], Size[1], Size[2]);
    //Se dibuja el cubo
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexCoords);
    glColorPointer(3, GL_FLOAT, 0, vertexColors);
    glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT,elementArray);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

