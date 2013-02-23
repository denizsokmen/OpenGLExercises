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

#include "ObjLoader.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

ObjLoader::ObjLoader(void)
{

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
	
	/* Loading the vertices, normals, tex. coords and faces*/
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
	
	
	/*After loading, the faces will determine what
	is going to be buffered to graphics card's memory.*/

	VBOVertex newVert[3];
	int v[3],n[3],t[3];
	for (size_t i=0; i < faceList.size(); i++) {	
		sscanf(faceList[i].c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",
			&v[0],&t[0],&n[0],
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
	}

	// create handle
	glGenBuffers(1,&VBO); 
	// use the handle
	glBindBuffer(GL_ARRAY_BUFFER,VBO); 
	// upload the buffer to graphics card memory, specify that it will never be changed.
	glBufferData(GL_ARRAY_BUFFER,modeldata.size()*sizeof(VBOVertex),&modeldata[0],GL_STATIC_DRAW); 
	 // release the handle
	glBindBuffer(GL_ARRAY_BUFFER,0);
}



void ObjLoader::render() {
	// use the handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	// specify the pointer to the texture coordinates
    glTexCoordPointer(2, GL_FLOAT, sizeof(VBOVertex), BUFFER_OFFSET(6 * sizeof(float))); 

    glEnableClientState(GL_NORMAL_ARRAY);
	// specify the pointer to the normal coordinates
    glNormalPointer(GL_FLOAT, sizeof(VBOVertex), BUFFER_OFFSET(3 * sizeof(float))); 

    glEnableClientState(GL_VERTEX_ARRAY);
	// specify the pointer to the vertex coordinates
    glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex),0); 
	
	// draw
    glDrawArrays(GL_TRIANGLES,0,modeldata.size());
  

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// release the handle
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}
