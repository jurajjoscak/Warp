#ifndef WARPABLE_H
#define WARPABLE_H
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "matrix.h"
#include "camera.h"

using namespace std;

//////////////////
// UV súradnice
/////////////////
class UVCoord
{
    public:
        GLfloat u;
        GLfloat v;
        UVCoord();
        UVCoord(float a, float b);
};

//////////////////////////////
// Geometria levelu s texturou
// a shaderom
//////////////////////////////
class WPWarpable
{
    private:
        GLuint vao;
        GLuint vbo;
        GLfloat* data;
        GLuint tex;
		GLuint bump;
		vector<GLfloat> lightPositions;
		vector<GLfloat> lightIntensities;
		GLuint lightPositionsLoc;
		GLuint lightIntensitiesLoc;
        GLuint MVPlocation;
        GLuint LevelSizeLoc;
        GLuint LevelOriginLoc;
		GLuint FadeoutLoc;
        WPVector LevelSize;
        WPVector LevelOrigin;			// Posun oproti stredu súradnicového systému
        void loadTexture(string name);
    public:
        unsigned int vbo_size;
        GLuint shader_program;
		WPVector finish;
		GLfloat fadeout;
        void LoadShaders();
        WPWarpable(string filename);
        void draw(WPCamera& k);
        float boundbox_left();
        float boundbox_right();
        float boundbox_top();
        float boundbox_bottom();
        float boundbox_near();
        float boundbox_far();
        void boundbox(float &l, float &r, float &t, float &b, float &n, float &f);
        GLuint getMVPlocation();
        WPVector getLevelSize();
        WPVector getLevelOrigin();
};
#endif // WARPABLE_H
