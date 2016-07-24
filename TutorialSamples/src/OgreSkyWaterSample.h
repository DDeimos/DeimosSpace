#ifndef OGRE_SKY_WATER_SAMPLE_H
#define OGRE_SKY_WATER_SAMPLE_H

#include "BaseApplication.h"

class OgreSkyWaterSample : public BaseApplication
{
public:
	OgreSkyWaterSample();
	virtual ~OgreSkyWaterSample();

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:

};

#endif