
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class pacman{
public:
    pacman();
    ~pacman();
    void draw();
    void update(int);
    int estado, tempo, txtAct, bufferKey;
    float Position[2] = {7.0, 7.0};

private:
    float velocidad = 0.012;
};
