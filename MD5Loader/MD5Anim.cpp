#include "MD5Anim.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

MD5Anim::MD5Anim(void)
{
}


MD5Anim::~MD5Anim(void)
{
	if (frameJoints)
	{
		for (int i=0; i < numFrames; i++)
		{
			if (frameJoints[i])
				delete[] frameJoints[i];
		}

		delete[] frameJoints;
	}

	if (bboxes)
		delete[] bboxes;

	if (baseFrames)
		delete[] baseFrames;

	if (jointInfos)
		delete[] jointInfos;

}

void MD5Anim::load(char *filename) 
{
	FILE *f;
	char data[512];
	f=fopen(filename,"rb");
	int frameind;
	float *frameData;

	while(!feof(f))
	{
		fgets(data,sizeof(data),f);

		if (sscanf(data," numFrames %d", &numFrames) == 1)
		{
			if (numFrames > 0)
			{
				bboxes=new Bounds[numFrames];
				frameJoints=new Joint*[numFrames];
			}

		}
		else if(sscanf(data," numJoints %d", &numJoints) == 1)
		{
			for(int i=0; i < numFrames; i++) 
			{
				frameJoints[i] = new Joint[numJoints];
			}

			jointInfos=new Hierarchy[numJoints];
			baseFrames=new BaseFrame[numJoints];
		}

		else if (sscanf(data," frameRate %d", &frameRate) == 1)
		{

		}
		else if (sscanf(data," numAnimatedComponents %d", &numAnimComp) == 1)
		{
			if (numAnimComp > 0)
				frameData=new float[numAnimComp];
		}
		else if (strncmp(data,"hierarchy {", 11) == 0)
		{
			for (int i=0; i < numJoints; i++) 
			{
				fgets(data, sizeof(data),f);

				sscanf(data, " %s %d %d %d", jointInfos[i].name,&jointInfos[i].parent, &jointInfos[i].flags,
										&jointInfos[i].startIndex);
			}

		}
		else if (strncmp(data,"bounds {", 8) == 0)
		{
			for (int i=0; i < numFrames; i++)
			{
				fgets(data,sizeof(data),f);
				sscanf(data," ( %f %f %f ) ( %f %f %f )", &bboxes[i].min.x, &bboxes[i].min.y, &bboxes[i].min.z, 
													&bboxes[i].max.x, &bboxes[i].max.y, &bboxes[i].max.z);
			}

		}
		else if (strncmp(data, "baseframe {", 11) == 0)
		{
			for (int i=0; i < numJoints; i++ )
			{
				fgets(data,sizeof(data),f);
				if (sscanf(data," ( %f %f %f ) ( %f %f %f )", &baseFrames[i].translation.x, &baseFrames[i].translation.y, 
														&baseFrames[i].translation.z,&baseFrames[i].rotation.x, 
														&baseFrames[i].rotation.y, &baseFrames[i].rotation.z) == 6)
				{
					calcW(&baseFrames[i].rotation);
				}
			}
		}
		else if (sscanf( data, " frame %d", &frameind) == 1)
		{
			for (int i=0; i < numAnimComp; i++)
				fscanf(f, "%f", &frameData[i]);

			createFrameSkeleton(frameData,frameJoints[frameind]);
			//build skeleton
		}
	}
	fclose(f);

	delete[] frameData;

}


void MD5Anim::createFrameSkeleton(float *frameData,  Joint *skelFrame)
{

	for (int i=0; i < numJoints; i++)
	{
		Vector3 animPos;
		Quaternion animRot;

		int k=0;

		animPos=baseFrames[i].translation;
		animRot=baseFrames[i].rotation;

		if (jointInfos[i].flags & 1) 
		{
			animPos.x=frameData[jointInfos[i].startIndex + k];
			k++;
		}
		if (jointInfos[i].flags & 2) 
		{
			animPos.y=frameData[jointInfos[i].startIndex + k];
			k++;
		}
		if (jointInfos[i].flags & 4) 
		{
			animPos.z=frameData[jointInfos[i].startIndex + k];
			k++;
		}
		if (jointInfos[i].flags & 8) 
		{
			animRot.x=frameData[jointInfos[i].startIndex + k];
			k++;
		}
		if (jointInfos[i].flags & 16) 
		{
			animRot.y=frameData[jointInfos[i].startIndex + k];
			k++;
		}
		if (jointInfos[i].flags & 32) 
		{
			animRot.z=frameData[jointInfos[i].startIndex + k];
			k++;
		}

		calcW(&animRot);

		Joint *curJoint=&skelFrame[i];
		curJoint->parent=jointInfos[i].parent;

		if (curJoint->parent < 0) 
		{
			curJoint->translation=animPos;
			curJoint->rotation=animRot;
		}
		else
		{
			Joint *parentJoint=&skelFrame[curJoint->parent];
			Vector3 rotated=parentJoint->rotation.multiply3d(curJoint->translation);
			curJoint->translation=parentJoint->translation + rotated;
			curJoint->rotation=parentJoint->rotation*curJoint->rotation;

			curJoint->rotation.normalize();
		}



	}

}
