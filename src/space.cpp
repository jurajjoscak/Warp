#include "space.h"
#include <sstream>

WPSpace::WPSpace(float density, WPWarpable *o) :
    texdata((((o->boundbox_right()-o->boundbox_left())/density)*((o->boundbox_top()-o->boundbox_bottom())/density)*((o->boundbox_near()-o->boundbox_far())/density)*4), 0.0f)
{
    warped = o;
    float n = warped->boundbox_near();
    float f = warped->boundbox_far();
    float l = warped->boundbox_left();
    float r = warped->boundbox_right();
    float t = warped->boundbox_top();
    float b = warped->boundbox_bottom();
    this->density = density;

    glGenTextures(1, &warpTexture);
    glBindTexture(GL_TEXTURE_3D, warpTexture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    warpTexWidth = (r-l)/density;
    warpTexHeight = (t-b)/density;
    warpTexDepth = (n-f)/density;

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, warpTexWidth, warpTexHeight, warpTexDepth, 0, GL_RGBA, GL_FLOAT, &texdata[0]);


    loadShaders();
    glBindImageTexture(0, warpTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void WPSpace::contract(float amount, WPVector center, WPVector direction, float radius)
{
    direction.normalize();
    WPVector offset = warped->getLevelOrigin();

    GLint old_prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &old_prog);

    glUseProgram(compute_program);

    glBindImageTexture(0, warpTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

    glUniform1i(glGetUniformLocation(compute_program, "warpTex"), 0);
    glUniform1f(glGetUniformLocation(compute_program, "amount"), amount);
    glUniform1f(glGetUniformLocation(compute_program, "radius"), radius);
    glUniform1f(glGetUniformLocation(compute_program, "density"), density);
    glUniform3f(glGetUniformLocation(compute_program, "center"), center.x, center.y, center.z);
    glUniform3f(glGetUniformLocation(compute_program, "direction"), direction.x, direction.y, direction.z);
    glUniform3f(glGetUniformLocation(compute_program, "offset"), offset.x, offset.y, offset.z);
	glUniform1f(glGetUniformLocation(compute_program, "width"), warpTexWidth);
    glUniform1i(glGetUniformLocation(compute_program, "reset"), GL_FALSE);

	if(warpTexWidth % 32 != 0)											// Nech je dosù workgroups
		glDispatchCompute(warpTexWidth/32+1, warpTexHeight, warpTexDepth);
	else
		glDispatchCompute(warpTexWidth/32, warpTexHeight, warpTexDepth);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(old_prog);						// Vr·ti pÙvodn˝ shader
    glBindTexture(GL_TEXTURE_3D, warpTexture);
}

void WPSpace::reset(float amount)
{
    WPVector offset = warped->getLevelOrigin();

    GLint old_prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &old_prog);

    glUseProgram(compute_program);

    glBindImageTexture(0, warpTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

    glUniform1i(glGetUniformLocation(compute_program, "warpTex"), 0);
    glUniform1f(glGetUniformLocation(compute_program, "amount"), amount);
    glUniform1f(glGetUniformLocation(compute_program, "radius"), 0.0);
    glUniform1f(glGetUniformLocation(compute_program, "density"), density);
    glUniform3f(glGetUniformLocation(compute_program, "center"), 0.0, 0.0, 0.0);
    glUniform3f(glGetUniformLocation(compute_program, "direction"), 0.0, 0.0, 0.0);
    glUniform3f(glGetUniformLocation(compute_program, "offset"), offset.x, offset.y, offset.z);
    glUniform1i(glGetUniformLocation(compute_program, "reset"), GL_TRUE);

	if (warpTexWidth % 32 != 0)											// Nech je dosù workgroups
		glDispatchCompute(warpTexWidth / 32 +1, warpTexHeight, warpTexDepth);
	else
		glDispatchCompute(warpTexWidth / 32, warpTexHeight, warpTexDepth);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(old_prog);
    glBindTexture(GL_TEXTURE_3D, warpTexture);
}

void WPSpace::loadShaders()
{
    // Create the shaders
 	GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);
 	//GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
 	int InfoLogLength;

 	// Compile Vertex Shader
 	printf("Compiling shader : compute\n");

 	ifstream t("src/WP_COMPUTE.gls");
    stringstream vbuffer;
    vbuffer << t.rdbuf();
	string src = vbuffer.str();
	GLchar const * ComputeSourcePointer = src.c_str();
	GLint const lencshader = src.length();

	//cout << vbuffer.str().c_str();

 	glShaderSource(ComputeShaderID, 1, &ComputeSourcePointer, &lencshader);
 	glCompileShader(ComputeShaderID);

 	// Check Vertex Shader
 	glGetShaderiv(ComputeShaderID, GL_COMPILE_STATUS, &Result);
 	glGetShaderiv(ComputeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
 	if ( InfoLogLength > 0 ){
 		std::vector<char> ComputeShaderErrorMessage(InfoLogLength+1);
 		glGetShaderInfoLog(ComputeShaderID, InfoLogLength, NULL, &ComputeShaderErrorMessage[0]);
 		printf("%s\n", &ComputeShaderErrorMessage[0]);
 	}

    // Link the program
 	printf("Linking program\n");
 	compute_program = glCreateProgram();
 	glAttachShader(compute_program, ComputeShaderID);
 	glLinkProgram(compute_program);
 	// Check the program
 	glGetProgramiv(compute_program, GL_LINK_STATUS, &Result);
 	glGetProgramiv(compute_program, GL_INFO_LOG_LENGTH, &InfoLogLength);
 	if ( InfoLogLength > 0 ){
 		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
 		glGetProgramInfoLog(compute_program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
 		printf("%s\n", &ProgramErrorMessage[0]);
 	}

 	glDetachShader(compute_program, ComputeShaderID);
 	glDeleteShader(ComputeShaderID);

 	glUseProgram(compute_program);
}
