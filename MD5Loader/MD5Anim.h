#ifndef INC_MD5ANIM
#define INC_MD5ANIM
#include "mathlib.h"
class MD5Anim
{
public:
	int numFrames;
	int frameRate;
	int numJoints;
	int numAnimComp;

	struct Hierarchy 
	{
		char name[64];
		int parent;
		int flags;
		int startIndex;
	};

	Hierarchy *jointInfos;

	struct Bounds
	{
		Vector3 min;
		Vector3 max;
	};

	Bounds *bboxes;

	struct BaseFrame
	{
		Vector3 translation;
		Quaternion rotation;
	};

	BaseFrame *baseFrames;

	
	

	struct Joint
	{
		int parent;
		Vector3 translation;
		Quaternion rotation;
	};

	Joint **frameJoints;




	MD5Anim(void);
	~MD5Anim(void);
	void load(char *filename);
	void createFrameSkeleton(float *frameData,  Joint *skelFrame);
	void calcW(Quaternion *quot) 
	{
		float t=1.0f - (quot->x * quot->x) - (quot->y * quot->y) - (quot->z * quot->z);
		if (t < 0.0f)
			quot->w=0.0f;
		else
			quot->w = -sqrt(t);
	}
};

#endif