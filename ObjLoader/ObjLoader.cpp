

#include "ObjLoader.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

ObjLoader::ObjLoader(void)
{
	numVerts=0;
}


ObjLoader::~ObjLoader(void)
{
	glDeleteBuffers(1,&VBO);
}


void ObjLoader::loadModel(const char* fileName) {

	ifstream fs(fileName,ios::in);
	string line;
	vector<string> faceList;
	vector<VertexCoord>	vertexData;
	vector<NormalCoord>	normalData;
	vector<TextureCoord>	textureData;
	vector<ColorComp>	colorData;
	
	while (getline(fs,line)) {
		if (line.substr(0,2) == "v ") {
			istringstream s(line.substr(2));
			VertexCoord ver;
			s >> ver.x >> ver.y >> ver.z;
			vertexData.push_back(ver);
		} 
		else if (line.substr(0,3) == "vt ") {
			istringstream s(line.substr(3));
			TextureCoord ver;
			s >> ver.u >> ver.v;
			textureData.push_back(ver);
		} 
		else if (line.substr(0,3) == "vn ") {
			istringstream s(line.substr(2));
			NormalCoord ver;
			s >> ver.x >> ver.y >> ver.z;
			normalData.push_back(ver);
		} 
		else if (line.substr(0,2) == "f ") {
			faceList.push_back(line);
		} 
		else if (line[0] == '#'){		} 
		else {		}
	}
	fs.close();
	

	VBOVertex newVert[3];
	int v[3],n[3],t[3];
	for (size_t i=0; i < faceList.size(); i++) {	
		sscanf(faceList[i].c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&v[0],&t[0],&n[0],
			&v[1],&t[1],&n[1],
			&v[2],&t[2],&n[2]);
	
		for (int j=0;j < 3; j++) {
			newVert[j].x=vertexData[v[j]-1].x;
			newVert[j].y=vertexData[v[j]-1].y;
			newVert[j].z=vertexData[v[j]-1].z;
			newVert[j].u=textureData[t[j]-1].u;
			newVert[j].v=textureData[t[j]-1].v;
			newVert[j].nx=normalData[n[j]-1].x;
			newVert[j].ny=normalData[n[j]-1].y;
			newVert[j].nz=normalData[n[j]-1].z;
			modeldata.push_back(newVert[j]);
		}
		numVerts++;
	}


	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,modeldata.size()*sizeof(VBOVertex),&modeldata[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}



void ObjLoader::render() {
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(55.0f,400.0f,55.0);
	glScalef(1.0f,1.0f,1.0f);
	glRotatef(180.0,0.0,0.0,1.0);
	glRotatef(90.0,1.0,0.0,0.0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(VBOVertex), BUFFER_OFFSET(6 * sizeof(float)));

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(VBOVertex), BUFFER_OFFSET(3 * sizeof(float)));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex),0);
	
    glDrawArrays(GL_TRIANGLES,0,modeldata.size());
  

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopMatrix();
}
