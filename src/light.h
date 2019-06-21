#include "matrix.h"

///////////////////////////////
// Svetlo s vlastnou pozíciou
// a farbou
///////////////////////////////
class WPLight
{
    public:
        GLfloat position[3];
        GLfloat intensity[3];
        GLuint positionLocation;
        GLuint intensityLocation;
        WPLight();
        void setPosition(float x, float y, float z);
        void setIntensity(float r, float g, float b);
        WPVector getPosition();
        WPVector getIntensity();
};
