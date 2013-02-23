#ifndef OBJ_H_INC
#define OBJ_H_INC
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include "opengl.h"
#include "terrain.h"

#define VBO_VERTEX 0
#define VBO_NORMAL 1
#define VBO_COLOR 2
#define VBO_UV 3
#define VBO_INDEX 4

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
	int numVerts;
	std::vector<VBOVertex> modeldata;
	
public:
	ObjLoader(void);
	~ObjLoader(void);
	void loadModel(const char* fileName);
	void render();
};

#endif