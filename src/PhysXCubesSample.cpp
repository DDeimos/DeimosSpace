#include "PhysXCubesSample.h"

CCube::CCube(int count, Ogre::SceneManager* sceneMgr, OgrePhysX::Scene* physXScene)
{
	mName = "cube_" + std::to_string(count);
	mSceneMgr = sceneMgr;
	mPhysXScene = physXScene;
	mScale = Ogre::Vector3(0.1f, 0.1f, 0.1f);
	mEntity = 0;
	mNode = 0;
}

CCube::~CCube()
{
	mNode->detachObject(mEntity);
	mSceneMgr->destroyEntity(mEntity);
	mSceneMgr->destroySceneNode(mNode);

	if (mPhysXScene != 0)
	{
		mPhysXScene->destroyRenderableBinding(mBinding);
		mPhysXScene->removeActor(mActor);
	}
}

void CCube::Initialize()
{
	InitEntity();
	InitNode();
	InitPhys();
}

void CCube::InitEntity()
{
	mEntity = mSceneMgr->createEntity(mName, "cube.mesh");
	mEntity->setMaterialName("Examples/BeachStones");
}

void CCube::InitNode()
{
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(mName);
	mNode->setScale(mScale);
	mNode->attachObject(mEntity);
}

void CCube::InitPhys()
{
	if (mPhysXScene != 0)
	{
		mActor = mPhysXScene->createRigidDynamic(mEntity, 0.1f, mScale);
		mBinding = mPhysXScene->createRenderedActorBinding(mActor, new OgrePhysX::NodeRenderable(mNode));
	}
}

void CCube::AddForce(Ogre::Camera* camera, physx::PxVec3 force)
{
	auto pxActor = mActor.getPxActor();

	if (pxActor != 0)
	{
		mActor.setGlobalPosition(camera->getPosition());
		pxActor->addForce(force, physx::PxForceMode::eVELOCITY_CHANGE);
	}
}

void CCube::SetPosition(Ogre::Vector3 pos)
{
	if (mActor.getPxActor() != 0)
		mActor.setGlobalPosition(pos);
	else
		mNode->setPosition(pos);
}

void CCube::SetPosUnderCursor(Ogre::RaySceneQuery* rayScnQuery, Ogre::Ray ray)
{
	rayScnQuery->setRay(ray);
	rayScnQuery->setSortByDistance(true);

	for (auto entry : rayScnQuery->execute())
	{
		if (entry.movable->getName() != "cube_0")
		{
			Ogre::Vector3 pos = ray.getPoint(entry.distance);
			Ogre::Vector3 size = mEntity->getBoundingBox().getHalfSize();
			pos.y += size.y * mScale.y;
			SetPosition(pos);
			break;
		}
	}
}

void CCube::SetPosUnderCursor2(Ogre::RaySceneQuery* rayScnQuery, Ogre::Ray ray, CCube* cube)
{
	rayScnQuery->setRay(ray);
	rayScnQuery->setSortByDistance(true);

	for (auto entry : rayScnQuery->execute())
	{
		if (entry.movable->getName() != "cube_0")
		{
			Ogre::Vector3 pointPos = ray.getPoint(entry.distance);
			Ogre::Vector3 offset = mEntity->getBoundingBox().getSize() * mScale;
			Ogre::Vector3 p1 = this->GetPosition();
			Ogre::Vector3 p2 = cube->GetPosition();

			auto dx = p1.x - p2.x;
			auto dz = p1.z - p2.z;

			if (abs(dx) < offset.x && abs(dz) < offset.z)
			{
				if (dx > 0)
				{
					pointPos.x += offset.x - dx;
				}
				else
				{
					pointPos.x += -offset.x - dx;
				}

				if (dz > 0)
				{
					pointPos.z += offset.z - dz;
				}
				else
				{
					pointPos.z += -offset.z - dz;
				}
			}

			SetPosition(pointPos);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

PhysXCubesSample::PhysXCubesSample()
{
	mRightMouseDown = false;
	mIsAltDown = false;
	mRayScnQuery = 0;
	mPhysXScene = 0;
}

PhysXCubesSample::~PhysXCubesSample()
{
	mSceneMgr->destroyQuery(mRayScnQuery);
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

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	CreatePlane();

	OgrePhysX::World::getSingleton().init();
	OgrePhysX::World::getSingleton().setupOgreFramelistener();

	mPhysXScene = OgrePhysX::World::getSingleton().addScene("Main", mSceneMgr);
	mPhysXScene->setGravity(physx::PxVec3(0, -9.81f * 5.f, 0));

	//PhyX plane geometry always has the normal (1, 0, 0), so we have to rotate the plane shape in order to create a plane with a normal (0, 1, 0)
	OgrePhysX::Actor<physx::PxRigidStatic> ground = mPhysXScene->createRigidStatic(OgrePhysX::Geometry::planeGeometry(), physx::PxTransform(physx::PxQuat(Ogre::Math::PI/2, physx::PxVec3(0,0,1))));

	CreateEmptyCube();
}

void PhysXCubesSample::createFrameListener()
{
	BaseApplication::createFrameListener();

	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
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

	if (ke.key == OIS::KC_LMENU)
	{
		mIsAltDown = true;
		Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);

		mTargetCube->SetPosUnderCursor(mRayScnQuery, ray);
		mTargetCube->GetEntity()->setVisible(true);
	}

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
	else if (ke.key == OIS::KC_SPACE)
		CreateForceCube();
	else if (ke.key == OIS::KC_BACK)
		ClearAllCubes();
	else if (ke.key == OIS::KC_LMENU)
	{
		mIsAltDown = false;
		mTargetCube->GetEntity()->setVisible(false);
	}

	return true;
}

bool PhysXCubesSample::mouseMoved(const OIS::MouseEvent &arg)
{
	if (mRightMouseDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * 0.2));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * 0.2));
	}
	else
	{
		mTrayMgr->injectMouseMove(arg);

		if(mTargetCube->GetEntity()->getVisible())
		{
			Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
			mTargetCube->SetPosUnderCursor(mRayScnQuery, ray);
		}
	}

	return true;
}

bool PhysXCubesSample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Right)
	{
		mRightMouseDown = true;
	}

	return true;
}

bool PhysXCubesSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		if (mIsAltDown)
		{
			for (int i = 0; i < 10; i++)
			{
				CreateCube();
			}
		}
		else
			TrySelectCube();
	}
	else if (id == OIS::MB_Right)
	{
		mRightMouseDown = false;
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

void PhysXCubesSample::CreateJoint(CCube* cubeA, CCube* cubeB, bool b)
{
	Ogre::Vector3 posA = cubeA->GetActor().getGlobalPosition();
	Ogre::Vector3 posB = cubeB->GetActor().getGlobalPosition();
	posB = posA;
	if (b)
		posB.y += 10;
	else
		posB.x -= 10;
	cubeB->SetPosition(posB);

	Ogre::Vector3 center = (posA + posB) / 2;
	auto actorA = cubeA->GetBinding()->getActor();
	auto actorB = cubeB->GetBinding()->getActor();

	actorA->setSolverIterationCounts(16);
	actorB->setSolverIterationCounts(16);

	physx::PxTransform transformA = physx::PxTransform::createIdentity();
	Ogre::Quaternion qA = cubeA->GetNode()->getOrientation();
	transformA.q = OgrePhysX::Convert::toPx(qA);
	transformA.p = OgrePhysX::Convert::toPx(center - posA);

	physx::PxTransform transformB = physx::PxTransform::createIdentity();
	Ogre::Quaternion qB = cubeB->GetNode()->getOrientation();
	transformB.q = OgrePhysX::Convert::toPx(qB);
	transformB.p = OgrePhysX::Convert::toPx(center - posB);

	physx::PxFixedJoint *joint = PxFixedJointCreate(*OgrePhysX::getPxPhysics(), actorA, transformA, actorB, transformB);
	joint->setConstraintFlag(physx::PxConstraintFlag::eCOLLISION_ENABLED, false);
	joint->setBreakForce(4000 * 1000, 4000 * 1000);
	mJoints.push_back(joint);
}

void PhysXCubesSample::CreateCube()
{
	int count = mCubes.size() + 1;
	Ogre::Vector3 pos = mTargetCube->GetPosition();
	CCube* cube = new CCube(count, mSceneMgr, mPhysXScene);
	cube->Initialize();
	cube->SetPosition(pos);
	mCubes[cube->GetName()] = cube;

	if (count > 1)
	{
		std::string first = std::to_string(count - 1);
		std::string second = std::to_string(count);
		CreateJoint(mCubes["cube_" + first], mCubes["cube_" + second], count != 2);
	}
}

void PhysXCubesSample::CreateForceCube()
{
	int count = mCubes.size() + 1;
	physx::PxVec3 force = OgrePhysX::Convert::toPx(mCamera->getDirection());
	CCube* cube = new CCube(count, mSceneMgr, mPhysXScene);
	cube->Initialize();

	cube->AddForce(mCamera, force * 300);
	mCubes[cube->GetName()] = cube;
}

void PhysXCubesSample::CreateEmptyCube()
{
	CCube* cube = new CCube(mCubes.size(), mSceneMgr);
	cube->Initialize();
	mTargetCube = cube;
	mTargetCube->GetEntity()->setVisible(false);
}

void PhysXCubesSample::TrySelectCube()
{
	if (CCube* cube = FindCubeUnderCursor())
		SelectCube(cube);
	else
		UnselectAllCubes();
}

void PhysXCubesSample::SelectCube(CCube* cube)
{
	cube->GetNode()->showBoundingBox(true);
}

void PhysXCubesSample::UnselectAllCubes()
{
	for (auto cube : mCubes)
	{
		auto node = cube.second->GetNode();
		node->showBoundingBox(false);
	}
}

void PhysXCubesSample::ClearAllCubes()
{
	for (auto cube : mCubes)
		delete cube.second;

	for (auto joint : mJoints)
		delete joint;

	mCubes.clear();
	mJoints.clear();
}

CCube* PhysXCubesSample::FindCubeUnderCursor()
{
	Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
	mRayScnQuery->setRay(ray);
	mRayScnQuery->setSortByDistance(true);

	for (auto entry : mRayScnQuery->execute())
	{
		Ogre::String name = entry.movable->getName();
		bool isCube = name.find("cube_") != -1;

		if (isCube && name != "cube_0")
			return mCubes[name];
	}

	return 0;
}
