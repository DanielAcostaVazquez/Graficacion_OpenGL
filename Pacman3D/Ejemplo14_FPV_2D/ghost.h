
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

class ghost{
    public:
        ghost(int, int, float, float, float, float, float, float, float, int);
        ~ghost();
        void draw();
        void update(int, int);
        void setRadio(float);
        void setPosition(float, float);
        float getVelocidad();
        float getRadio();
        float getPositionX();
        float getPositionY();
        float getPositionZ();
        float getSize();
        int getDireccion();
        int getIaLvl();
        void setVelocidad(float);
    private:
        int direccion;
        int radioC, radioS;
        int iaLvl;
        float velocidad = 0.01;
        float Position[3];
        float color[3];
        GLUquadric *quad;
};
