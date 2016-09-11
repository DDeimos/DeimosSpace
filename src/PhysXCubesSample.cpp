#include "PhysXCubesSample.h"

PhysXCubesSample::PhysXCubesSample()
{

}

PhysXCubesSample::~PhysXCubesSample()
{

}

void PhysXCubesSample::createCamera()
{
	BaseApplication::createCamera();

	mCamera->setPosition(Ogre::Vector3(100, 100, -100));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

void PhysXCubesSample::createScene()
{
	BaseApplication::createScene();

	CreateGround();

	OgrePhysX::World::getSingleton().init();
	OgrePhysX::World::getSingleton().setupOgreFramelistener();

	m_physXScene = OgrePhysX::World::getSingleton().addScene("Main", mSceneMgr);

	//PhyX plane geometry always has the normal (1, 0, 0), so we have to rotate the plane shape in order to create a plane with a normal (0, 1, 0)
	OgrePhysX::Actor<physx::PxRigidStatic> ground = m_physXScene->createRigidStatic(OgrePhysX::Geometry::planeGeometry(), physx::PxTransform(physx::PxQuat(Ogre::Math::PI/2, physx::PxVec3(0,0,1))));
}

bool PhysXCubesSample::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	return ret;
}

bool PhysXCubesSample::keyPressed(const OIS::KeyEvent& ke)
{
	mCameraMan->injectKeyDown(ke);

	return true;
}

bool PhysXCubesSample::keyReleased(const OIS::KeyEvent& ke)
{
	mCameraMan->injectKeyUp(ke);

	if(ke.key == OIS::KC_1)
		mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");
	else if(ke.key == OIS::KC_2)
		mSceneMgr->setSkyBox(true, "Examples/StormySkyBox");
	else if(ke.key == OIS::KC_3)
		mSceneMgr->setSkyBox(true, "Examples/EarlyMorningSkyBox");
	else if(ke.key == OIS::KC_4)
		mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");
	else if(ke.key == OIS::KC_5)
		mSceneMgr->setSkyBox(true, "Examples/EveningSkyBox");

	return true;
}

void PhysXCubesSample::CreateGround()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::String name = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	Ogre::MeshManager::getSingleton().createPlane("ground", name, plane, 1000, 1000, 10, 10, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setMaterialName("Examples/GrassFloor");
	groundEntity->setCastShadows(false);
}
