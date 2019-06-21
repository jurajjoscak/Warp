#include "warpable.h"
#include <sstream>
using namespace std;

UVCoord::UVCoord()
{
    u = 0.0;
    v = 0.0;
}

UVCoord::UVCoord(float a, float b)
{
    u = a;
    v = b;
}

WPWarpable::WPWarpable(string filename)
{
	fadeout = 1.0;

    // Preparsuje súbor

    ifstream f(("resources\\models\\" + filename + ".obj").data());
    string line;
    stringstream iss;
    string val;
    float x,y,z;
    vector<WPVector*> vertices;
    vector<UVCoord*> uvs;
    vector<GLuint> triangles;
    vector<GLuint> tex_triangles;


	getline(f, line);
	iss.str("");
	iss.clear();

	iss.str(line.data());
	iss >> val;
	finish.x = atof(val.data());
	iss >> val;
	finish.y = atof(val.data());
	iss >> val;
	finish.z = atof(val.data());


    while(getline(f,line))
    {
        iss.str("");
        iss.clear();

        iss.str(line.data());
        iss >> val;
        if(val=="v")
        {
            iss >> val;
            x = atof(val.data());
            iss >> val;
            y = atof(val.data());
            iss >> val;
            z = atof(val.data());

            vertices.push_back(new WPVector(x,y,z));
        }
		if (val == "l")
		{
			iss >> val;
			x = atof(val.data());
			iss >> val;
			z = atof(val.data());
			iss >> val;
			y = atof(val.data());

			lightPositions.push_back(x);
			lightPositions.push_back(y);
			lightPositions.push_back(-z);

			iss >> val;
			x = atof(val.data());
			iss >> val;
			y = atof(val.data());
			iss >> val;
			z = atof(val.data());

			lightIntensities.push_back(x);
			lightIntensities.push_back(y);
			lightIntensities.push_back(z);
		}
        else if(val=="vt")
        {
            iss >> val;
            x = atof(val.data());
            iss >> val;
            y = 1- atof(val.data());

            uvs.push_back(new UVCoord(x,y));
        }
        else if(val=="f")
        {
            for(int i = 0; i < 3; i++)
            {
                getline(iss,val,'/');
                x = atoi(val.data())-1;
                iss >> val;
                y = atoi(val.data())-1;

                triangles.push_back(x);
                tex_triangles.push_back(y);
            }
        }
    }


    f.close();

    //////////////////////
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    vbo_size = triangles.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*5*vbo_size, NULL, GL_STATIC_DRAW);

    // Naplní vbo vertexmi

    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for(unsigned int i = 0; i<vbo_size; i++)
    {
        data[i*5] = vertices[(triangles[i])]->x;
        data[i*5+1] = vertices[(triangles[i])]->y;
        data[i*5+2] = vertices[(triangles[i])]->z;
        data[i*5+3] = uvs[(tex_triangles[i])]->v;
        data[i*5+4] = uvs[(tex_triangles[i])]->u;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);

    /////////////////////////////////
    for(unsigned int i = 0; i< vertices.size();i++)
    {
        delete vertices[i];
    }


    for(unsigned int i = 0; i< uvs.size();i++)
    {
        delete uvs[i];
    }
    LoadShaders();

    loadTexture(filename.data());

    MVPlocation = glGetUniformLocation(shader_program, "MVP");
    LevelSizeLoc = glGetUniformLocation(shader_program, "LevelSize");
    LevelOriginLoc = glGetUniformLocation(shader_program, "LevelOrigin");
	FadeoutLoc = glGetUniformLocation(shader_program, "fadeout");
	lightPositionsLoc = glGetUniformLocation(shader_program, "lightPositions");
	lightIntensitiesLoc = glGetUniformLocation(shader_program, "lightIntensities");


    LevelOrigin.x = boundbox_left();
    LevelOrigin.y = boundbox_bottom();
    LevelOrigin.z = boundbox_far();
    LevelSize.x = boundbox_right()-LevelOrigin.x;
    LevelSize.y = boundbox_top()-LevelOrigin.y;
    LevelSize.z = boundbox_near()-LevelOrigin.z;

	lightIntensities.resize(30, 0.0);
	lightPositions.resize(30, 0.0);
}

void WPWarpable::draw(WPCamera& k)
{
    glUseProgram(shader_program);
    glUniformMatrix4fv(MVPlocation, 1, GL_FALSE, k.getMatrix().data);
    glUniform3f(LevelSizeLoc, LevelSize.x, LevelSize.y, LevelSize.z);
    glUniform3f(LevelOriginLoc, LevelOrigin.x, LevelOrigin.y, LevelOrigin.z);
	
	GLuint TexLoc = glGetUniformLocation(shader_program, "tex");
	GLuint BumpLoc = glGetUniformLocation(shader_program, "bump");

	glUniform1i(TexLoc, 1);
	glUniform1i(BumpLoc, 2);
	glUniform1f(FadeoutLoc, fadeout);
	
	glUniform3fv(lightPositionsLoc, 10, lightPositions.data());
	glUniform3fv(lightIntensitiesLoc, 10, lightIntensities.data());

	//std::cout << std::fixed << std::setprecision(6) << "X = " << lightPositions[0] << "Y = " << lightPositions[1] << "Z = " << lightPositions[2] << "\n";

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bump);

	glActiveTexture(GL_TEXTURE0);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(void*)0);
    glVertexAttribPointer(1, 2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES,0,vbo_size);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}

void WPWarpable::LoadShaders()
{
    // Create the shaders
 	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
 	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
 	int InfoLogLength;

 	// Compile Vertex Shader
 	printf("Compiling shader : vertex\n");

 	ifstream t("src/WP_VERTEX_SHADER.gls");
    stringstream vbuffer;
    vbuffer << t.rdbuf();
	string vsrc = vbuffer.str();
    char const * VertexSourcePointer = vsrc.c_str();
	const GLint lenvshader = vsrc.length();

 	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , &lenvshader);
 	glCompileShader(VertexShaderID);

 	// Check Vertex Shader
 	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
 	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
 	if ( InfoLogLength > 0 ){
 		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
 		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
 		printf("%s\n", &VertexShaderErrorMessage[0]);
 	}

 	// Compile Fragment Shader
 	printf("Compiling shader : fragment\n");

 	ifstream f("src/WP_FRAGMENT_SHADER.gls");
    stringstream fbuffer;
    fbuffer << f.rdbuf();
	string fsrc = fbuffer.str();
    char const * FragmentSourcePointer = fsrc.c_str();
	const GLint lenfshader = fsrc.length();

 	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , &lenfshader);
 	glCompileShader(FragmentShaderID);

 	// Check Fragment Shader
 	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
 	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
 	if ( InfoLogLength > 0 ){
 		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
 		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
 		printf("%s\n", &FragmentShaderErrorMessage[0]);
        }


    // Link the program
 	printf("Linking program\n");
 	shader_program = glCreateProgram();
 	glAttachShader(shader_program, VertexShaderID);
 	glAttachShader(shader_program, FragmentShaderID);
 	glLinkProgram(shader_program);
 	// Check the program
 	glGetProgramiv(shader_program, GL_LINK_STATUS, &Result);
 	glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &InfoLogLength);
 	if ( InfoLogLength > 0 ){
 		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
 		glGetProgramInfoLog(shader_program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
 		printf("%s\n", &ProgramErrorMessage[0]);
 	}

 	glDetachShader(shader_program, VertexShaderID);
 	glDetachShader(shader_program, FragmentShaderID);
 	glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

 	glUseProgram(shader_program);
}

void WPWarpable::loadTexture(string name)
{
    SDL_Surface * textura = SDL_LoadBMP(("resources\\textures\\" + name + ".bmp").data());
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textura->w, textura->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textura->pixels);

    SDL_FreeSurface(textura);

	textura = SDL_LoadBMP(("resources\\textures\\" + name + "-normal.bmp").data());
	glGenTextures(1, &bump);
	glBindTexture(GL_TEXTURE_2D, bump);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textura->w, textura->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textura->pixels);

	SDL_FreeSurface(textura);
}

float WPWarpable::boundbox_left()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float res = data[0];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5] < res)
            res = data[i*5];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}

float WPWarpable::boundbox_right()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float res = data[0];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5] > res)
            res = data[i*5];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}

float WPWarpable::boundbox_top()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float res = data[0];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5+1] > res)
            res = data[i*5+1];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}

float WPWarpable::boundbox_bottom()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float res = data[0];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5+1] < res)
            res = data[i*5+1];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}

float WPWarpable::boundbox_near()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float res = data[0];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5+2] > res)
            res = data[i*5+2];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}

float WPWarpable::boundbox_far()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    float res = data[0];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5+2] < res)
            res = data[i*5+2];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    return res;
}

void WPWarpable::boundbox(float &l, float &r, float &t, float &b, float &n, float &f)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    data = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    l = r = data[0];
    t = b = data[1];
    n = f = data[2];
    for(unsigned int i = 1; i < vbo_size; i++)
    {
        if(data[i*5] < l)
            l = data[i*5];
        if(data[i*5] > r)
            r = data[i*5];
        if(data[i*5+1] < b)
            b = data[i*5+1];
        if(data[i*5+1] > t)
            t = data[i*5+1];
        if(data[i*5+2] > f)
            f = data[i*5+2];
        if(data[i*5+1] < n)
            n = data[i*5+2];
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

GLuint WPWarpable::getMVPlocation()
{
    return MVPlocation;
}

WPVector WPWarpable::getLevelSize()
{
    return LevelSize;
}

WPVector WPWarpable::getLevelOrigin()
{
    return LevelOrigin;
}
