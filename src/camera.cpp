#include "camera.h"

WPCamera::WPCamera(float near, float far, float left, float right, float top, float bottom)
{
    projection.access(0,0) = (2*near)/(right-left);
    projection.access(2,0) = (right+left)/(right-left);
    projection.access(1,1) = (2*near)/(top-bottom);
    projection.access(2,1) = (top+bottom)/(top-bottom);
    projection.access(2,2) = -(far+near)/(far-near);
    projection.access(3,2) = (-2*far*near)/(far-near);
    projection.access(2,3) = -1.0;
    projection.access(3,3) = 0.0;

    face.z = -1.0;
    up.y = 1.0;
    this->right.x = 1.0;
}

WPCamera::WPCamera(float x, float y, float z):
    projection()
{
    projection.access(0,0) = 1/x;
    projection.access(1,1) = 1/y;
    projection.access(2,2) = -1/(z/2);
    projection.access(3,2) = -z;

    face.z = -1.0;
    up.y = 1.0;
    right.x = 1.0;
}

WPMatrix WPCamera::getMatrix()
{
    return getMatrixNoProjection()*projection;
}

WPMatrix WPCamera::getMatrixNoProjection()
{
    up.normalize();
    right.normalize();
    face.normalize();

    WPMatrix lokat;
    lokat.access(0,0) = right.x;
    lokat.access(1,0) = right.y;
    lokat.access(2,0) = right.z;

    lokat.access(0,1) = up.x;
    lokat.access(1,1) = up.y;
    lokat.access(2,1) = up.z;

    lokat.access(0,2) = -face.x;
    lokat.access(1,2) = -face.y;
    lokat.access(2,2) = -face.z;

    WPMatrix loc;
    loc.access(3,0) = -location.x;
    loc.access(3,1) = -location.y;
    loc.access(3,2) = -location.z;

    lokat = loc*lokat;

    return lokat;
}

void WPCamera::translate(float x, float y, float z)
{
    location.x += x;
    location.y += y;
    location.z += z;
}

void WPCamera::translateRelative(float x, float y, float z)
{
    translate(right.x*x+up.x*y+face.x*z,
              right.y*x+up.y*y+face.y*z,
              right.z*x+up.z*y+face.z*z);
}

void WPCamera::rotateRelative(float x, float y, float z)
{
    WPVector globalUp(0.0,1.0,0.0);

    face = face.rotateAround(globalUp, y);
    up = up.rotateAround(globalUp, y);
    right = right.rotateAround(globalUp, y);

    up = up.rotateAround(right,x);
    face = face.rotateAround(right,x);
}

WPVector WPCamera::getPosition()
{
    return location;
}

WPVector WPCamera::getFace()
{
    return face;
}

WPVector WPCamera::getUp()
{
    return up;
}

WPVector WPCamera::getRight()
{
    return right;
}

void WPCamera::scale(float x, float y, float z)
{
    WPMatrix tmp;
    tmp.access(0,0) = x;
    tmp.access(1,1) = y;
    tmp.access(2,2) = z;

    projection = projection*tmp;
}
