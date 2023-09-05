
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

class wall{
public:
    wall(float, float, float, float);
    ~wall();
    void draw();
    void setRadio(float);
    void setPosition(float [3]);
    float getRadio();
    float getPositionX();
    float getPositionY();
    float getPositionZ();
    float getSizeX();
    float getSizeY();
    float getSizeZ();

private:
    float vertexCoords[24] = {  // Coordinates for the vertices of a cube.
                   1,1,1,   1,1,-1,   1,-1,-1,   1,-1,1,
                  -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1  };

    float vertexColors[24] = {  // An RGB color value for each vertex
                   0,0,1,   0,0,1,   0,0,1,   0,0,1,
                   0,0,1,   0,0,1,   0,0,1,   0,0,1  };

    int elementArray[24] = {  // Vertex number for the six faces.
                  0,1,2,3, 0,3,7,4, 0,4,5,1,
                  6,2,1,5, 6,5,4,7, 6,7,3,2  };

    float Position[3] = {0.0, 0.0, 0.0};

    float Size[3] = {1.0, 2.0, 1.0};

    float radio = 1;
};

