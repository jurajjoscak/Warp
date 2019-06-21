#include "player.h"

WPPlayer::WPPlayer(WPSpace *h):
    main_camera(NEAR,FAR,LEFT,RIGHT,TOP,BOTTOM),
    collider(0.5,0.5,0.5)
{
    world = h;
    cursor = 10.0;
    primary_light.positionLocation = glGetUniformLocation(world->warped->shader_program, "PrimaryLightPosition");
    primary_light.intensityLocation = glGetUniformLocation(world->warped->shader_program, "PrimaryLightIntensity");
    secondary_light.positionLocation = glGetUniformLocation(world->warped->shader_program, "SecondaryLightPosition");
    secondary_light.intensityLocation = glGetUniformLocation(world->warped->shader_program, "SecondaryLightIntensity");

    primary_light.intensity[0] = 1.0;
    primary_light.intensity[1] = 1.0;
    primary_light.intensity[2] = 1.0;

    secondary_light.intensity[0] = 0.0;
    secondary_light.intensity[1] = 0.0;
    secondary_light.intensity[2] = 2.0;
}

void WPPlayer::lookAround(float x, float y)
{
    main_camera.rotateRelative(x,y,0.0);
    collider.rotateRelative(0.0,y,0.0);
}

WPCamera& WPPlayer::getCamera()
{
    return main_camera;
}

void WPPlayer::walk(float amount)
{
    WPVector dir = collider.getFace();
    speed = speed + dir*amount;
}

void WPPlayer::strafe(float amount)
{
    WPVector dir = main_camera.getRight();
    speed = speed + dir*amount;
}

void WPPlayer::fly(float amount)
{
    WPVector dir(0.0,1.0,0.0);
    speed = speed + dir*amount;
}

void WPPlayer::jump()
{
    speed.y = MAX_V_SPEED/1.75;
}

void WPPlayer::compensateCollisions()
{
    WPVector zero(0.0,0.0,0.0);
    float tmp;
    bool collision = false;

    WPVector front = zero-collider.getFace();
    WPVector back = collider.getFace();
    WPVector left = zero-collider.getRight();
    WPVector right = collider.getRight();
    WPVector up = zero-collider.getUp();
    WPVector down = collider.getUp();

    tmp = collider.checkBack(*(world->warped));
    if(tmp < 1.0)
    {
        front = front*tmp;
        collision = true;
    }

    tmp = collider.checkFront(*(world->warped));
    if(tmp < 1.0)
    {
        back = back*tmp;
        collision = true;
    }

    tmp = collider.checkLeft(*(world->warped));
    if(tmp < 1.0)
    {
        right = right*tmp;
        collision = true;
    }

    tmp = collider.checkRight(*(world->warped));
    if(tmp < 1.0)
    {
        left = left*tmp;
        collision = true;
    }

    tmp = collider.checkUp(*(world->warped));
    if(tmp < 1.0)
    {
        up = up*tmp;
        collision = true;
        grounded = true;
    }
    else
        grounded = false;

    tmp = collider.checkDown(*(world->warped));
    if(tmp < 1.0)
    {
        down = down*tmp;
        collision = true;

    }

    if(collision)
    {
        front = front+back+right+left+up+down;		// Ak sa dotýka viacerými stranami, vektory posunu sèíta

        main_camera.translate(front.x, front.y, front.z);
        collider.translate(front.x, front.y, front.z);
        front.normalize();
        speed = speed + (front*(speed*(zero - front)));		// Zložka rýchlosti v smere proti prekážke sa vynuluje
    }
}

void WPPlayer::translate(float x, float y, float z)
{
    main_camera.translate(x,y,z);
    collider.translate(x,y,z);
}

bool WPPlayer::control()
{
    int mousx;
    int mousy;
    const Uint8 *keyboard_state;
	Uint32 mouse_state;

    SDL_PumpEvents();
    mouse_state = SDL_GetMouseState(&mousx, &mousy);
	WPVector mouse;
	mouse.x = (mousx - SCREEN_W / 2);		// Pozícia myši relatívne k stredu obrazovky
	mouse.y = (mousy - SCREEN_H / 2);
	mouse.z = 0.0;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_MOUSEWHEEL)
			mouse.z = event.wheel.y;
	}


    keyboard_state = SDL_GetKeyboardState(NULL);

    if(grounded)
    {
        controlled = false;
        if(keyboard_state[SDL_SCANCODE_W])
        {
            walk(0.01*fpscompensate);
            controlled = true;
        }
        if(keyboard_state[SDL_SCANCODE_S])
        {
            walk(-0.01*fpscompensate);
            controlled = true;
        }
        if(keyboard_state[SDL_SCANCODE_A])
        {
            strafe(-0.01*fpscompensate);
            controlled = true;
        }
        if(keyboard_state[SDL_SCANCODE_D])
        {
            strafe(0.01*fpscompensate);
            controlled = true;
        }
        if(keyboard_state[SDL_SCANCODE_SPACE])
        {
            jump();
            controlled = true;
        }
    }
    if(keyboard_state[SDL_SCANCODE_LSHIFT])
    {
        cursor += 0.1*fpscompensate;
    }
    if(keyboard_state[SDL_SCANCODE_LCTRL])
    {
        cursor += -0.1*fpscompensate;
    }
    if(keyboard_state[SDL_SCANCODE_R])
    {
        world->reset(0.02*fpscompensate);
    }
    if(keyboard_state[SDL_SCANCODE_ESCAPE])
    {
        return 0;
    }

	if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		if(mouse.x != 0.0)
			world->contract(mouse.x*0.01*fpscompensate, secondary_light.getPosition(), collider.getRight()*mouse.x, secondary_light.getIntensity().z);
		if (mouse.y != 0.0)
			world->contract(mouse.y*0.01*fpscompensate, secondary_light.getPosition(), collider.getFace()*mouse.y, secondary_light.getIntensity().z);
		if (mouse.z != 0.0)
			world->contract(mouse.z*0.1*fpscompensate, secondary_light.getPosition(), collider.getUp()*mouse.z, secondary_light.getIntensity().z);
	}
	else
	{
		lookAround(mouse.y*0.25, mouse.x*0.25);
		secondary_light.intensity[2] += mouse.z*0.1*fpscompensate;
	}

    return 1;
}

void WPPlayer::update()
{
    fpscompensate = (SDL_GetTicks() - frametime)/NORMAL_FRAMETIME;		// Vypoèíta èas posledného framu a nastaví násobiš
    frametime = SDL_GetTicks();

    translate(speed.x,speed.y,speed.z);
    compensateCollisions();

    speed = speed + WPVector(0.0,-0.01*fpscompensate,0.0);		// Gravitácia

    WPVector tmp = speed;
    tmp.y = 0;
    if(tmp.length() > MAX_H_SPEED)				// Rýchlostný limit
        tmp = tmp*(MAX_H_SPEED/tmp.length());

    speed.x = tmp.x;
    speed.z = tmp.z;

    if(speed.y > MAX_V_SPEED)
        speed.y = MAX_V_SPEED;
    else if(speed.y < -MAX_V_SPEED)
        speed.y = -MAX_V_SPEED;

    if(!controlled)			// Zastaví hráèa, ak nie je ovládaný
    {
        if(fabs(speed.x)-0.01*fpscompensate > 0.0)
        {
            if(speed.x < 0.0)
                speed.x += 0.01*fpscompensate;
            else
                speed.x -= 0.01*fpscompensate;
        }
        else
            speed.x = 0.0;

        if(fabs(speed.z)-0.01*fpscompensate > 0.0)
        {
            if(speed.z < 0.0)
                speed.z += 0.01*fpscompensate;
            else
                speed.z -= 0.01*fpscompensate;
        }
        else
            speed.z = 0.0;
    }

    primary_light.position[0] = main_camera.getPosition().x;
    primary_light.position[1] = main_camera.getPosition().y;
    primary_light.position[2] = main_camera.getPosition().z;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,SCREEN_W,SCREEN_H);

    glReadPixels(SCREEN_W/2, SCREEN_H/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &aim_depth);		// Obmedzí vzdialenos kurzora na najbližšiu prekážku
    aim_depth = 2.0*aim_depth - 1.0;
    aim_depth = 2.0 * NEAR * FAR / (FAR + NEAR - aim_depth * (FAR - NEAR));

    if(cursor > CURSOR_MAX)
        cursor = CURSOR_MAX;
    if(cursor < 0.0)
        cursor = 0.0;

    if(cursor < aim_depth)
    {
        secondary_light.position[0] = main_camera.getPosition().x + main_camera.getFace().x*cursor;
        secondary_light.position[1] = main_camera.getPosition().y + main_camera.getFace().y*cursor;
        secondary_light.position[2] = main_camera.getPosition().z + main_camera.getFace().z*cursor;
    }
    else
    {
        secondary_light.position[0] = main_camera.getPosition().x + main_camera.getFace().x*aim_depth;
        secondary_light.position[1] = main_camera.getPosition().y + main_camera.getFace().y*aim_depth;
        secondary_light.position[2] = main_camera.getPosition().z + main_camera.getFace().z*aim_depth;
    }

	if (secondary_light.intensity[2] < 0.0)
		secondary_light.intensity[2] = 0.0;
	else if (secondary_light.intensity[2] > 10.0)
		secondary_light.intensity[2] = 10.0;
}

void WPPlayer::draw()
{
    glUniform3fv(primary_light.positionLocation,1, primary_light.position);
    glUniform3fv(primary_light.intensityLocation,1, primary_light.intensity);
    glUniform3fv(secondary_light.positionLocation,1, secondary_light.position);
    glUniform3fv(secondary_light.intensityLocation,1, secondary_light.intensity);
    world->warped->draw(main_camera);
}

bool WPPlayer::outOfBounds()
{
	if (main_camera.getPosition().y+10.0 < world->warped->boundbox_bottom())
		return true;

	return false;
}
