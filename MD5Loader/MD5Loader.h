#ifndef MD5_L_INC
#define MD5_L_INC
#include "mathlib.h"
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include "opengl.h"
#include "terrain.h"
class MD5Loader
{
	struct Joint {
		char name[64];
		int parentIndex;
		Vector3		translation;
		Quaternion	rotation;
	};

	Joint *joints;
	int numJoints;

	struct Weight {
		int boneIndex;
		float bias;
		Vector3 position;
	};

	struct WeightedVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
		int weightIndex;
		int weightNum;
	};

	struct Triangle {
		int index[3];
	};


	struct Mesh {
		WeightedVertex *vertex;
		Weight *weights;
		Triangle *triangles;

		int numTris;
		int numVert;
		int numWeights;
	};

	Mesh *meshes;
	int numMeshes;


public:
	MD5Loader(void);
	~MD5Loader(void);
	void load(char* filename);
	void calcW(Quaternion *quot) 
	{
		float t=1.0f - (quot->x * quot->x) - (quot->y * quot->y) - (quot->z * quot->z);
		if (t < 0.0f)
			quot->w=0.0f;
		else
			quot->w = -sqrt(t);
	}
	void prepareMesh( Mesh *ms);
	void draw();

};

#endif