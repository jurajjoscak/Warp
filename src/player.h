#ifndef PLAYER_H_
#define PLAYER_H_
#include "collider.h"
#include "space.h"
#include "light.h"

/////////////////////////////
// Hráè so všektou fyzikou,
// ovládaním atd.
////////////////////////////
class WPPlayer
{
    public:
        WPCamera main_camera;
        WPCollider collider;
        WPVector speed;
        WPSpace* world;
        float cursor;				// Priemer a poloha kurzora
        float aim_depth;

        bool grounded = false;			// True, ak sa dotýka zeme
        bool controlled = false;		// True, ak je stlaèené W,S,A,D alebo medzerník
        Uint32 frametime = 0;
        float fpscompensate = 1.0;

        WPLight primary_light;
        WPLight secondary_light;	// Kurzor


        WPPlayer(WPSpace *h);
        void lookAround(float x, float y);
        WPCamera& getCamera();
        WPCamera& getColCamera(){return collider.getCamera();}
        void walk(float amount);
        void strafe(float amount);
        void fly(float amount);
        void jump();
        void translate(float x, float y, float z);
        void compensateCollisions();  // Skontroluje kolízie
        bool control();
        void update();			// Na konci uvedie hráèa do korektného stavu
        void draw();
		bool outOfBounds();
};
#endif
