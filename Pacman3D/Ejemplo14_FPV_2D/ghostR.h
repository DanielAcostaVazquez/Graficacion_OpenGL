#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

class ghostR{
    public:
        ghostR(int, int, float, float, float, float, float, float, float);
        ~ghostR();
        void draw();
        void update(int);
        void setRadio(float);
        void setPosition(float, float);
        float getVelocidad();
        float getRadio();
        float getPositionX();
        float getPositionY();
        float getPositionZ();
        float getSize();
        void setVelocidad(float);
    private:
        int direccion;
        int radioC, radioS;
        float velocidad = 0.01;
        float Position[3];
        float color[3];
        GLUquadric *quad;
};
