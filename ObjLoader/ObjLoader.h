/*
   Deniz Sökmen
   e-mail:sokmendeniz@gmail.com
   
   A simple loader for .obj file format.
   In .obj format, defined the model's vertex,
   normal and texture coordinates. 
   These coordinates aren't directly drawn,
   it is the "faces" which determines what to draw.
   
   There are sure plenty more improvements to do.
   
*/

#ifndef OBJ_H_INC
#define OBJ_H_INC
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>


struct VertexCoord
{
	float x,y,z;
};

struct TextureCoord
{
	float u,v;
};

struct NormalCoord
{
	float x,y,z;
};

struct ColorComp
{
	float r,g,b;
};

struct VBOVertex
{
	float x,y,z;
	float nx,ny,nz;
	float u,v;
};



class ObjLoader
{
	GLuint VBO;
	std::vector<VBOVertex> modeldata;
	
public:
	ObjLoader(void);
	~ObjLoader(void);
	void loadModel(const char* fileName);
	void render();
};

#endif