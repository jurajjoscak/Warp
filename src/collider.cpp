#include "collider.h"

WPCollider::WPCollider(float x, float y, float z):
    cam(1.0,1.0,1.0),
    box(x,y,z)
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint depthbuffer;
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 64, 64);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
}

float WPCollider::checkDepth(WPWarpable& mesh)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0,0,64,64);
    mesh.draw(cam);

    GLfloat tex[4096];

    glReadPixels(0,0,64,64,GL_DEPTH_COMPONENT,GL_FLOAT,tex);

    float min_depth = 1.0;
    for(int i = 0; i<4096; i++)		// Vyberie minimálnu håbku
    {
        if(tex[i] < min_depth)
            min_depth = tex[i];
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return min_depth;
}

void WPCollider::rotateRelative(float x, float y, float z)
{
    cam.rotateRelative(x,y,z);
}

void WPCollider::translate(float x, float y, float z)
{
    cam.translate(x,y,z);
}

float WPCollider::checkFront(WPWarpable& mesh)
{
    cam.scale(1/box.x,1/box.y,1);
    float res = checkDepth(mesh);
    cam.scale(box.x, box.y, 1);
    return res;
}

float WPCollider::checkBack(WPWarpable& mesh)
{
    cam.scale(1/box.x,1/box.y,1);
    cam.rotateRelative(0,180,0);
    float res = checkDepth(mesh);
    cam.scale(box.x, box.y, 1);
    cam.rotateRelative(0,180,0);
    return res;
}

float WPCollider::checkRight(WPWarpable& mesh)
{
    cam.scale(1/box.x,1/box.y,1);
    cam.rotateRelative(0,-90,0);
    float res = checkDepth(mesh);
    cam.scale(box.x, box.y, 1);
    cam.rotateRelative(0,90,0);
    return res;
}

float WPCollider::checkLeft(WPWarpable& mesh)
{
    cam.scale(1/box.x,1/box.y,1);
    cam.rotateRelative(0,90,0);
    float res = checkDepth(mesh);
    cam.scale(box.x, box.y, 1);
    cam.rotateRelative(0,-90,0);
    return res;
}

float WPCollider::checkUp(WPWarpable& mesh)
{
    cam.scale(1/box.x,1/box.y,1);
    cam.rotateRelative(90,0,0);
    float res = checkDepth(mesh);
    cam.scale(box.x, box.y, 1);
    cam.rotateRelative(-90,0,0);
    return res;
}

float WPCollider::checkDown(WPWarpable& mesh)
{
    cam.scale(1/box.x,1/box.y,1);
    cam.rotateRelative(-90,0,0);
    float res = checkDepth(mesh);
    cam.scale(box.x, box.y, 1);
    cam.rotateRelative(90,0,0);
    return res;
}

WPVector WPCollider::getFace()
{
    return cam.getFace();
}

WPVector WPCollider::getRight()
{
    return cam.getRight();
}

WPVector WPCollider::getUp()
{
    WPVector res(0.0,1.0,0.0);
    return res;
}

float WPCollider::getBoxX()
{
    return box.x;
}

float WPCollider::getBoxY()
{
    return box.y;
}

float WPCollider::getBoxZ()
{
    return box.z;
}
