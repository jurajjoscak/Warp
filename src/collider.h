#ifndef COLLIDER_H_
#define COLLIDER_H_
#include "camera.h"
#include "warpable.h"

/////////////////////////////////////////
// Vykres�uje sc�nu zo �iestich str�n a
// kontroluje h�bku
/////////////////////////////////////////
class WPCollider
{
    private:
        WPCamera cam;		// Poz�cia sa ded� s poz�cie kamery
        WPVector box;

        GLuint framebuffer;
        GLuint depthbuffer;

    public:
        WPCollider(float x, float y, float z);
        float checkDepth(WPWarpable& mesh);
        void rotateRelative(float x, float y, float z);
        void translate(float x, float y, float z);
        WPCamera& getCamera(){return cam;}
        float checkFront(WPWarpable& mesh);
        float checkBack(WPWarpable& mesh);
        float checkRight(WPWarpable& mesh);
        float checkLeft(WPWarpable& mesh);
        float checkUp(WPWarpable& mesh);
        float checkDown(WPWarpable& mesh);
        WPVector getFace();
        WPVector getRight();
        WPVector getUp();
        float getBoxX();
        float getBoxY();
        float getBoxZ();
};
#endif // COLLIDER_H_
