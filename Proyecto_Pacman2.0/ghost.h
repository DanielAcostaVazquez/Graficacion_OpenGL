
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class ghost{
    public:
        ghost(int, float, float);
        ~ghost();
        void draw();
        void update(int);
        int txt, direccion;
        float Position[2];
        float getVelocidad();
    private:
        float velocidad = 0.01;
};
