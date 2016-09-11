#ifndef PHYSX_CUBES_SAMPLE_H
#define PHYSX_CUBES_SAMPLE_H

#include "BaseApplication.h"
#include "OgrePhysX.h"
#include "PxPhysicsAPI.h"

class PhysXCubesSample : public BaseApplication
{
public:
	PhysXCubesSample();
	virtual ~PhysXCubesSample();

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	void CreateGround();

private:
	OgrePhysX::Scene *m_physXScene;
};

#endif