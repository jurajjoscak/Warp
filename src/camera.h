#ifndef CAMERA_H
#define CAMERA_H
#include "matrix.h"

/////////////////////////////////////////
// Kamera produkuje transformaènú maticu
// pod¾a polohy a orientácie
/////////////////////////////////////////
class WPCamera
{
    private:
        WPMatrix projection;
        WPVector location;
        WPVector face;
        WPVector up;
        WPVector right;
    public:
        WPCamera(float near, float far, float left, float right, float top, float bottom);	// Kamera s perspektívnou projekciou
        WPCamera(float x, float y, float z);												// Kamera s ortografickou projekciou
        WPMatrix getMatrix();
        WPMatrix getMatrixNoProjection();
        void scale(float x, float y, float z);
        void translate(float x, float y, float z);
        void translateRelative(float x, float y, float z);
        void rotateRelative(float x, float y, float z);			// Rotuje okolo face, right a up vektorov
        WPVector getPosition();
        WPVector getFace();
        WPVector getUp();
        WPVector getRight();
};
#endif // CAMERA_H
