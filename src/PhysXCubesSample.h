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
	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	void CreatePlane();
	OgrePhysX::Actor<physx::PxRigidDynamic> CreateCube();
	OgrePhysX::Actor<physx::PxRigidDynamic> CreateCubeWithForce(physx::PxVec3 force);

private:
	int m_cubesCount;
	bool m_rightMouseDown;
	Ogre::RaySceneQuery* m_rayScnQuery;
	OgrePhysX::Scene *m_physXScene;
};

#endif