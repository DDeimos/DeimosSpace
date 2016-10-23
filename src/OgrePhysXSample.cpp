#include "OgrePhysXSample.h"

OgrePhysXSample::OgrePhysXSample()
{

}

OgrePhysXSample::~OgrePhysXSample()
{

}

void OgrePhysXSample::createCamera()
{
	BaseApplication::createCamera();
}

void OgrePhysXSample::createScene()
{
	//BaseApplication::createScene();

	// Set default ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));

	// Set some camera params
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(1000);
	mCamera->setPosition(0,5,-15);
	mCamera->setDirection(Ogre::Vector3(0,0,1));

	// Light
	Ogre::Light *mLight = mSceneMgr->createLight("Light0");
	mLight->setPosition(0,0,-3);
	mLight->setDiffuseColour(0.4f, 0.5f, 1.0f);
	mLight->setSpecularColour(1,1,1);

	//Start OgrePhysX
	OgrePhysX::World::getSingleton().init();
	OgrePhysX::World::getSingleton().setupOgreFramelistener();

	mPhysXScene = OgrePhysX::World::getSingleton().addScene("Main", mSceneMgr);

	//PhyX plane geometry always has the normal (1, 0, 0), so we have to rotate the plane shape in order to create a plane with a normal (0, 1, 0)
	OgrePhysX::Actor<physx::PxRigidStatic> ground = mPhysXScene->createRigidStatic(OgrePhysX::Geometry::planeGeometry(), physx::PxTransform(physx::PxQuat(Ogre::Math::PI/2, physx::PxVec3(0,0,1))));
	//ground wraps the underlying PxRigidStatic and provides some helper methods
	ground.setGlobalPosition(Ogre::Vector3(0, -2, 0));

	/*OgrePhysX::Destructible *destructible6 = mPhysXScene->createDestructible("meteor.xml", 85, 85, 60, Ogre::Vector3(2.0f, 2.0f, 2.0f));
	destructible6->setGlobalPosition(Ogre::Vector3(0, 2, 0));

	OgrePhysX::Destructible *destructible2 = mPhysXScene->createDestructible("meteor.xml", 60, 60, 60, Ogre::Vector3(1.5f, 1.5f, 1.5f));
	destructible2->setGlobalPosition(Ogre::Vector3(-4, 50, -6));

	OgrePhysX::Destructible *destructible1 = mPhysXScene->createDestructible("meteor.xml", 100, 100, 60, Ogre::Vector3(1.1f, 1.1f, 1.1f));
	destructible1->setGlobalPosition(Ogre::Vector3(3, 100, -5));

	OgrePhysX::Destructible *destructible3 = mPhysXScene->createDestructible("meteor.xml", 100, 100, 80, Ogre::Vector3(1.0f, 1.0f, 1.0f));
	destructible3->setGlobalPosition(Ogre::Vector3(0, 150, 0));

	OgrePhysX::Destructible *destructible4 = mPhysXScene->createDestructible("meteor.xml", 100, 100, 80, Ogre::Vector3(1.0f, 1.0f, 1.0f));
	destructible4->setGlobalPosition(Ogre::Vector3(1, 200, 1));*/

	for (int i = 1; i < 100; i++)
	{
		Ogre::Real x = Ogre::Math::RangeRandom(-10, 10);
		Ogre::Real y = Ogre::Math::RangeRandom(-10, 10);
		OgrePhysX::Destructible *destructible = mPhysXScene->createDestructible("meteor.xml", 100, 100, 80, Ogre::Vector3(1.0f, 1.0f, 1.0f));
		destructible->setGlobalPosition(Ogre::Vector3(x, i * 25, y));
	}

	//Brocken.xml and the mesh parts were created by this tool: http://www.ogre3d.org/forums/viewtopic.php?f=11&t=68189
}

bool OgrePhysXSample::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	return ret;
}
