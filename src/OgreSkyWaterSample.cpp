#include "OgreSkyWaterSample.h"

OgreSkyWaterSample::OgreSkyWaterSample()
{

}

OgreSkyWaterSample::~OgreSkyWaterSample()
{

}

void OgreSkyWaterSample::createCamera()
{
	BaseApplication::createCamera();

	mCamera->setPosition(Ogre::Vector3(200, 200, -200));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

void OgreSkyWaterSample::createScene()
{
	//BaseApplication::createScene();

	mSceneMgr->setSkyDome(true, "Examples/CloudySky");
}

bool OgreSkyWaterSample::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	return ret;
}
