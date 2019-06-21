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
// Trojprvkový vektor s
// potrebnými matematickými
// operáciami.
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
        GLfloat operator*(WPVector b);			// Skalárny súèin
        WPVector operator*(float b);
        WPVector vectorMultiply(WPVector);		// Vektorový súèin
        WPVector multiplyMatrix(float f,WPMatrix b);
        WPVector rotateAround(WPVector b, float angle);
        GLfloat length();
        WPVector normalize();
};

///////////////////////////
// Matica 4x4 s potrebnými
// operáciami
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
        GLfloat& access(int x, int y);  // Prístup k jednotlivým prvkom
};
#endif // MATRIX_H
