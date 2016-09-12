#include "PhysXCubesSample.h"

PhysXCubesSample::PhysXCubesSample()
{
	m_cubesCount = 0;
	m_rightMouseDown = false;
}

PhysXCubesSample::~PhysXCubesSample()
{
	mSceneMgr->destroyQuery(m_rayScnQuery);
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

	CreatePlane();

	OgrePhysX::World::getSingleton().init();
	OgrePhysX::World::getSingleton().setupOgreFramelistener();

	m_physXScene = OgrePhysX::World::getSingleton().addScene("Main", mSceneMgr);
	m_physXScene->setGravity(physx::PxVec3(0, -9.81f * 2.f, 0));

	//PhyX plane geometry always has the normal (1, 0, 0), so we have to rotate the plane shape in order to create a plane with a normal (0, 1, 0)
	OgrePhysX::Actor<physx::PxRigidStatic> ground = m_physXScene->createRigidStatic(OgrePhysX::Geometry::planeGeometry(), physx::PxTransform(physx::PxQuat(Ogre::Math::PI/2, physx::PxVec3(0,0,1))));
}

void PhysXCubesSample::createFrameListener()
{
	BaseApplication::createFrameListener();

	m_rayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mTrayMgr->showCursor();
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

bool PhysXCubesSample::mouseMoved(const OIS::MouseEvent &arg)
{
	if (m_rightMouseDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * 0.2));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * 0.2));
	}
	else
	{
		mTrayMgr->injectMouseMove(arg);
	}

	return true;
}

bool PhysXCubesSample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
		m_rightMouseDown = true;
	}

	return true;
}

bool PhysXCubesSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		CreateCube();
	}
	else if (id == OIS::MB_Right)
	{
		m_rightMouseDown = false;
	}

	return true;
}

void PhysXCubesSample::CreatePlane()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::String name = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	Ogre::MeshManager::getSingleton().createPlane("ground", name, plane, 1000, 1000, 10, 10, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setMaterialName("Examples/GrassFloor");
	groundEntity->setCastShadows(false);
}

void PhysXCubesSample::CreateCube()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
	auto point = ray.intersects(plane);

	if (point.first)
	{
		std::string count = std::to_string(++m_cubesCount);
		Ogre::Entity* entity = mSceneMgr->createEntity("cube_" + count, "cube.mesh");

		Ogre::Vector3 scale(0.1f, 0.1f, 0.1f);
		Ogre::Vector3 pos = ray.getPoint(point.second);
		Ogre::Vector3 size = entity->getBoundingBox().getHalfSize();
		pos.y += size.y * scale.y;

		Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("cube_" + count);
		node->setScale(scale);
		node->attachObject(entity);

		auto actor = m_physXScene->createRigidDynamic(entity, 100.f, scale);
		m_physXScene->createRenderedActorBinding(actor, new OgrePhysX::NodeRenderable(node));
		actor.setGlobalPosition(pos);
	}
}
