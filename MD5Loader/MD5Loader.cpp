#include "MD5Loader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

MD5Loader::MD5Loader(void)
{
}


MD5Loader::~MD5Loader(void)
{
	if (joints)
		delete[] joints;

	if (meshes)
	{
		for (int i=0; i < numMeshes; i++) 
		{
			if (meshes[i].triangles)
				delete[] meshes[i].triangles;

			if (meshes[i].vertex)
				delete[] meshes[i].vertex;

			if (meshes[i].weights)
				delete[] meshes[i].weights;

		}
		delete[] meshes;
	}
}

void MD5Loader::load(char *filename) 
{
	FILE *f;
	char data[512];
	f=fopen(filename,"rb");
	int ver,curMesh=0;

	while(!feof(f)) 
	{
		fgets(data,sizeof(data),f);

		if (sscanf(data," MD5Version %d", &ver) == 1) 
		{

		} 
		else if (sscanf(data," numJoints %d", &numJoints) == 1) 
		{
			if (numJoints > 0) 
			{
				joints = new Joint[numJoints];
			}
		}
		else if (sscanf(data," numMeshes %d", &numMeshes) == 1)
		{
			if (numMeshes > 0) 
			{
				meshes=new Mesh[numMeshes];
			}
		}
		else if (strncmp(data,"joints {",8) == 0)
		{
			for (int i = 0; i < numJoints; ++i)
			{
				Joint *j=&joints[i];
				fgets(data,sizeof(data),f);
				if (sscanf (data,"%s %d ( %f %f %f ) ( %f %f %f )",j->name,&j->parentIndex,
					&j->translation.x,&j->translation.y,&j->translation.z,
					&j->rotation.x,&j->rotation.y,&j->rotation.z) == 8)
				{
					calcW(&j->rotation);
				}


			}
		}
		else if (strncmp(data,"mesh {",6) == 0)
		{
			Mesh *ms=&meshes[curMesh];
			int vert_index = 0;
			int tri_index = 0;
			int weight_index = 0;
			float fdata[4];
			int idata[3];

			while (data[0] != '}' && !feof(f)) 
			{
				fgets(data,sizeof(data),f);

				if (sscanf (data," numverts %d", &ms->numVert) == 1) 
				{
					if (ms->numVert > 0)
					{
						ms->vertex = new WeightedVertex[ms->numVert];
					}

				}
				else if (sscanf (data," numtris %d", &ms->numTris) == 1)
				{
					if (ms->numTris > 0)
					{
						ms->triangles = new Triangle[ms->numTris];
					}
				}
				else if (sscanf (data," numweights %d", &ms->numWeights) == 1) 
				{
					if (ms->numWeights > 0)
					{
						ms->weights=new Weight[ms->numWeights];
					}
				}
				else if (sscanf (data," vert %d ( %f %f ) %d %d", &vert_index, &fdata[0], &fdata[1], 
					&idata[0], &idata[1]) == 5)
				{
					ms->vertex[vert_index].texCoord.x=fdata[0];
					ms->vertex[vert_index].texCoord.y=fdata[1];
					ms->vertex[vert_index].weightIndex=idata[0];
					ms->vertex[vert_index].weightNum=idata[1];
				}
				else if (sscanf (data," tri %d %d %d %d",&tri_index,&idata[0],&idata[1],&idata[2]) == 4)
				{
					ms->triangles[tri_index].index[0]=idata[0];
					ms->triangles[tri_index].index[1]=idata[1];
					ms->triangles[tri_index].index[2]=idata[2];
				}
				else if (sscanf (data," weight %d %d %f ( %f %f %f )",
					&weight_index, &idata[0], &fdata[0], &fdata[1],
					&fdata[2], &fdata[3]) == 6)
				{
					ms->weights[weight_index].boneIndex=idata[0];
					ms->weights[weight_index].bias=fdata[0];
					ms->weights[weight_index].position.x=fdata[1];
					ms->weights[weight_index].position.y=fdata[2];
					ms->weights[weight_index].position.z=fdata[3];
				}
			}
			prepareMesh(&meshes[curMesh]);

			curMesh++;
		}
	}

	fclose(f);

}


void MD5Loader::prepareMesh(Mesh *ms) 
{
	for (int i=0; i < ms->numVert; ++i) 
	{
		WeightedVertex *vert=&ms->vertex[i];
		vert->pos=Vector3(0.0f,0.0f,0.0f);
		for (int j=0; j < vert->weightNum; j++) 
		{
			Weight *we=&ms->weights[vert->weightIndex + j];
			Joint *jo=&joints[we->boneIndex];

			Vector3 rot(jo->rotation.multiply3d(we->position));
			vert->pos += (jo->translation + rot) * we->bias;
		}

	}
}

void MD5Loader::draw()
{
	
	glPointSize (5.0f);
	glColor3f (1.0f, 0.0f, 0.0f);
	glRotatef(-90.0f,1.0,0.0,0.0);
	glBegin (GL_POINTS);
	for (int i = 0; i < numJoints; ++i)
		glVertex3f (joints[i].translation.x,joints[i].translation.y,joints[i].translation.z);
	glEnd ();
	glPointSize (1.0f);

	/* Draw each bone */
	glColor3f (0.0f, 1.0f, 0.0f);
	glBegin (GL_LINES);
	for (int i = 0; i < numJoints; ++i)
	{
		if (joints[i].parentIndex != -1)
		{
			glVertex3f (joints[joints[i].parentIndex].translation.x,joints[joints[i].parentIndex].translation.y,
				joints[joints[i].parentIndex].translation.z);
			glVertex3f (joints[i].translation.x,joints[i].translation.y,joints[i].translation.z);
		}
	}
	glEnd();
	glPopMatrix();
}
