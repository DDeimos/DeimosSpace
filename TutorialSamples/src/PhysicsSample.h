#ifndef PHYSICS_SAMPLE_H
#define PHYSICS_SAMPLE_H

#include "BaseApplication.h"

class PhysicsSample : public BaseApplication
{
public:
	PhysicsSample();
	virtual ~PhysicsSample();

protected:
	virtual void createScene();
};

#endif