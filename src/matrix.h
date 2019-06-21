#ifndef MATRIX_H
#define MATRIX_H
#include "defines.h"
#include <iostream>
#include <iomanip>
#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <cmath>
#define PI 3.14159265358979323846
using namespace std;

class WPMatrix;

/////////////////////////////
// Trojprvkov� vektor s
// potrebn�mi matematick�mi
// oper�ciami.
////////////////////////////////
class WPVector
{
    public:
        GLfloat x;
        GLfloat y;
        GLfloat z;

        WPVector();
        WPVector(float x, float y, float z);
        WPVector operator+(WPVector b);
        WPVector operator-(WPVector b);
        GLfloat operator*(WPVector b);			// Skal�rny s��in
        WPVector operator*(float b);
        WPVector vectorMultiply(WPVector);		// Vektorov� s��in
        WPVector multiplyMatrix(float f,WPMatrix b);
        WPVector rotateAround(WPVector b, float angle);
        GLfloat length();
        WPVector normalize();
};

///////////////////////////
// Matica 4x4 s potrebn�mi
// oper�ciami
//////////////////////////
class WPMatrix
{
    public:
        static WPMatrix scale(float x, float y, float z);
        GLfloat data [16];
        WPMatrix();
        WPMatrix operator+(const WPMatrix& b);
        WPMatrix operator+(float b);
        WPMatrix operator-(const WPMatrix& b);
        WPMatrix operator-(float b);
        WPMatrix operator*(const WPMatrix& b);
        WPMatrix operator*(float b);
        WPMatrix operator/(float b);
        GLfloat& access(int x, int y);  // Pr�stup k jednotliv�m prvkom
};
#endif // MATRIX_H
