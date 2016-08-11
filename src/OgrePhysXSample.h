#ifndef OGRE_PHYSX_SAMPLE_H
#define OGRE_PHYSX_SAMPLE_H

#include "BaseApplication.h"
#include "OgrePhysX.h"
#include "PxPhysicsAPI.h"

class OgrePhysXSample : public BaseApplication
{
public:
	OgrePhysXSample();
	virtual ~OgrePhysXSample();

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	OgrePhysX::Scene *m_physXScene;
};

#endif