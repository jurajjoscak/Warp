#include <vector>
#include "warpable.h"
#include "matrix.h"

/////////////////////////
// Simuluje FFD mriežku
// pomocou 3D textury
/////////////////////////
using namespace std;

class WPSpace
{
    private:
        GLuint compute_program;
        GLfloat density;

        GLuint warpTexture;
        GLuint warpTexWidth;
        GLuint warpTexHeight;
        GLuint warpTexDepth;
        vector<GLfloat> texdata;
    public:
        WPWarpable *warped;
        void loadShaders();
        WPSpace(float density, WPWarpable* o);
        void contract(float amount, WPVector center, WPVector direction, float radius);
        void reset(float amount);
};
