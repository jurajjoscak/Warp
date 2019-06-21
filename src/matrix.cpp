#include "matrix.h"

WPVector::WPVector()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

WPVector::WPVector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

GLfloat WPVector::length()
{
    return sqrt(x*x + y*y + z*z);
}

WPVector WPVector::normalize()
{
    GLfloat len = this->length();
    x = x/len;
    y = y/len;
    z = z/len;

	return *this;
}

WPVector WPVector::vectorMultiply(WPVector b)
{
    WPVector res;
    res.x = this->y*b.z - this->z*b.y;
    res.y = this->z*b.x - this->x*b.z;
    res.z = this->x*b.y - this->y*b.x;
    return res;
}

WPVector WPVector::operator+(WPVector b)
{
    WPVector res;
    res.x = this->x + b.x;
    res.y = this->y + b.y;
    res.z = this->z + b.z;
    return res;
}

WPVector WPVector::operator-(WPVector b)
{
    WPVector res;
    res.x = this->x - b.x;
    res.y = this->y - b.y;
    res.z = this->z - b.z;
    return res;
}

GLfloat WPVector::operator*(WPVector b)
{
    GLfloat res = this->x * b.x + this->y * b.y + this->z * b.z;
    return res;
}

WPVector WPVector::operator*(float b)
{
    WPVector res(this->x*b, this->y*b, this->z*b);
    return res;
}

WPVector WPVector::multiplyMatrix(float f,WPMatrix b)
{
    WPVector res;
    res.x = b.access(0,0)*this->x + b.access(1,0)*this->y + b.access(2,0)*this->z + b.access(3,0)*f;
    res.y = b.access(0,1)*this->x + b.access(1,1)*this->y + b.access(2,1)*this->z + b.access(3,1)*f;
    res.z = b.access(0,2)*this->x + b.access(1,2)*this->y + b.access(2,2)*this->z + b.access(3,2)*f;
    return res;
}

WPVector WPVector::rotateAround(WPVector b, float angle)
{
    angle = angle*PI/180;
    float c = cos(angle);
    float s = sin(angle);
    float C = 1 - c;

    float Q[3][3];
    Q[0][0] = b.x*b.x*C + c;
    Q[0][1] = b.y*b.x*C + b.z*s;
    Q[0][2] = b.z*b.x*C - b.y*s;

    Q[1][0] = b.y*b.x*C - b.z*s;
    Q[1][1] = b.y*b.y*C + c;
    Q[1][2] = b.z*b.y*C + b.x*s;

    Q[2][0] = b.x*b.z*C + b.y*s;
    Q[2][1] = b.z*b.y*C - b.x*s;
    Q[2][2] = b.z*b.z*C + c;

    WPVector res;
    res.x = this->x*Q[0][0] + this->y*Q[0][1] + this->z*Q[0][2];
    res.y = this->x*Q[1][0] + this->y*Q[1][1] + this->z*Q[1][2];
    res.z = this->x*Q[2][0] + this->y*Q[2][1] + this->z*Q[2][2];

    return res;
}

WPMatrix::WPMatrix()
{
    for(int i = 0; i < 16; i++)
        data[i] = 0.0;

    data[0] = 1.0;
    data[5] = 1.0;
    data[10] = 1.0;
    data[15] = 1.0;
}

WPMatrix WPMatrix::scale(float x, float y, float z)
{
    WPMatrix res;
    res.access(0,0) = x;
    res.access(1,1) = y;
    res.access(2,2) = z;
    return res;
}

WPMatrix WPMatrix::operator+(const WPMatrix& b)
{
    WPMatrix res;
    for(int i = 0; i < 16; i++)
        res.data[i] = this->data[i] + b.data[i];

    return res;
}

WPMatrix WPMatrix::operator+(float b)
{
    WPMatrix res;

    for(int i = 0; i < 16; i++)
        res.data[i] = this->data[i] + b;

    return res;
}

WPMatrix WPMatrix::operator-(const WPMatrix& b)
{
    WPMatrix res;
    for(int i = 0; i < 16; i++)
        res.data[i] = this->data[i] - b.data[i];

    return res;
}

WPMatrix WPMatrix::operator-(float b)
{
    WPMatrix res;
    for(int i = 0; i < 16; i++)
        res.data[i] = this->data[i] - b;

    return res;
}

WPMatrix WPMatrix::operator*(float b)
{
    WPMatrix res;
    for(int i = 0; i < 16; i++)
        res.data[i] = this->data[i] * b;

    return res;
}

WPMatrix WPMatrix::operator/(float b)
{
    WPMatrix res;
    for(int i = 0; i < 16; i++)
        res.data[i] = this->data[i] / b;

    return res;
}

WPMatrix WPMatrix::operator*(const WPMatrix& b)
{
    WPMatrix res;
    res.data[0] = this->data[0]*b.data[0]+this->data[1]*b.data[4]+this->data[2]*b.data[8]+this->data[3]*b.data[12];
    res.data[1] = this->data[0]*b.data[1]+this->data[1]*b.data[5]+this->data[2]*b.data[9]+this->data[3]*b.data[13];
    res.data[2] = this->data[0]*b.data[2]+this->data[1]*b.data[6]+this->data[2]*b.data[10]+this->data[3]*b.data[14];
    res.data[3] = this->data[0]*b.data[3]+this->data[1]*b.data[7]+this->data[2]*b.data[11]+this->data[3]*b.data[15];
    res.data[4] = this->data[4]*b.data[0]+this->data[5]*b.data[4]+this->data[6]*b.data[8]+this->data[7]*b.data[12];
    res.data[5] = this->data[4]*b.data[1]+this->data[5]*b.data[5]+this->data[6]*b.data[9]+this->data[7]*b.data[13];
    res.data[6] = this->data[4]*b.data[2]+this->data[5]*b.data[6]+this->data[6]*b.data[10]+this->data[7]*b.data[14];
    res.data[7] = this->data[4]*b.data[3]+this->data[5]*b.data[7]+this->data[6]*b.data[11]+this->data[7]*b.data[15];
    res.data[8] = this->data[8]*b.data[0]+this->data[9]*b.data[4]+this->data[10]*b.data[8]+this->data[11]*b.data[12];
    res.data[9] = this->data[8]*b.data[1]+this->data[9]*b.data[5]+this->data[10]*b.data[9]+this->data[11]*b.data[13];
    res.data[10] = this->data[8]*b.data[2]+this->data[9]*b.data[6]+this->data[10]*b.data[10]+this->data[11]*b.data[14];
    res.data[11] = this->data[8]*b.data[3]+this->data[9]*b.data[7]+this->data[10]*b.data[11]+this->data[11]*b.data[15];
    res.data[12] = this->data[12]*b.data[0]+this->data[13]*b.data[4]+this->data[14]*b.data[8]+this->data[15]*b.data[12];
    res.data[13] = this->data[12]*b.data[1]+this->data[13]*b.data[5]+this->data[14]*b.data[9]+this->data[15]*b.data[13];
    res.data[14] = this->data[12]*b.data[2]+this->data[13]*b.data[6]+this->data[14]*b.data[10]+this->data[15]*b.data[14];
    res.data[15] = this->data[12]*b.data[3]+this->data[13]*b.data[7]+this->data[14]*b.data[11]+this->data[15]*b.data[15];

    return res;
}

GLfloat& WPMatrix::access(int x, int y)
{
    return data[x*4+y];
}
