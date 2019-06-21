#include "light.h"

WPLight::WPLight()
{
    position[0] = position[1] = position[2] = 0.0;
    intensity[0] = intensity[1] = intensity[2] = 0.0;
}

void WPLight::setPosition(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void WPLight::setIntensity(float r, float g, float b)
{
    intensity[0] = r;
    intensity[1] = g;
    intensity[2] = b;
}

WPVector WPLight::getPosition()
{
    return WPVector(position[0],position[1], position[2]);
}

WPVector WPLight::getIntensity()
{
    return WPVector(intensity[0],intensity[1], intensity[2]);
}
