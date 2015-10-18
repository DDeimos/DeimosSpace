#include "TerrainCollisionSample.h"

TerrainCollisionSample::TerrainCollisionSample()
{
}

TerrainCollisionSample::~TerrainCollisionSample()
{
	mSceneMgr->destroyQuery(mRaySceneQuery);
}

void TerrainCollisionSample::createScene()
{
	TerrainSample::createScene();
}

void TerrainCollisionSample::createFrameListener()
{
	TerrainSample::createFrameListener();

	mCount = 0;
	mCurrentObject = NULL;
	mLMouseDown = false;
	mRMouseDown = false;
	mRotateSpeed = 0.1;

	// Create RaySceneQuery
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	//SdkTray - version - to get the mouse cursor on the screen:
	mTrayMgr->showCursor();
}

void TerrainCollisionSample::chooseSceneManager()
{
	BaseApplication::chooseSceneManager();
}

bool TerrainCollisionSample::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	if (!TerrainSample::frameRenderingQueued(evt))
		return false;

	// Setup the scene query
	Ogre::Vector3 camPos = mCamera->getPosition();
	Ogre::Ray cameraRay(Ogre::Vector3(camPos.x, 5000.0f, camPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
	mRaySceneQuery->setRay(cameraRay);

	// Perform the scene query
	Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();
	// Get the results, set the camera height
	if (itr != result.end() && itr->worldFragment)
	{
		Ogre::Real terrainHeight = itr->worldFragment->singleIntersection.y;
		if ((terrainHeight + 10.0f) > camPos.y)
		{
			mCamera->setPosition( camPos.x, terrainHeight + 10.0f, camPos.z );
		}
	}

	handleCameraCollision();

	return true;
}

bool TerrainCollisionSample::mouseMoved(const OIS::MouseEvent &arg)
{
	// Update SdkTrays with the mouse motion
	mTrayMgr->injectMouseMove(arg);

	if (mLMouseDown)
	{
		Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera);

		mRaySceneQuery->setRay(mouseRay);
		Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin();
		if (itr != result.end() && itr->worldFragment)
		{
			mCurrentObject->setPosition(itr->worldFragment->singleIntersection);
		}

	}
	else if (mRMouseDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * mRotateSpeed));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * mRotateSpeed));
	}

	return true;
}

bool TerrainCollisionSample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera);

		mRaySceneQuery->setRay(mouseRay);
		// Execute query
		Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin( );

		// Get results, create a node/entity on the position
		if (itr != result.end() && itr->worldFragment)
		{
			char name[16];
			sprintf( name, "Robot%d", mCount++ );
			Ogre::Entity *ent = mSceneMgr->createEntity(name, "robot.mesh");
			mCurrentObject = mSceneMgr->getRootSceneNode()->createChildSceneNode(std::string(name)+ "Node", itr->worldFragment->singleIntersection);
			mCurrentObject->attachObject(ent);
			mCurrentObject->setScale(0.1f, 0.1f, 0.1f);
		}

		mLMouseDown = true;
	}
	else if (id == OIS::MB_Right)
	{
		mTrayMgr->hideCursor();
		mRMouseDown = true;
	}

	return true;
}

bool TerrainCollisionSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		mLMouseDown = false;
	}
	else if (id == OIS::MB_Right)
	{
		mTrayMgr->showCursor();
		mRMouseDown = false;
	}

	return true;
}

void TerrainCollisionSample::handleCameraCollision()
{
	Ogre::Vector3 camPos = mCamera->getPosition();
	Ogre::Ray camRay(
		Ogre::Vector3(camPos.x, 5000.0, camPos.z),
		Ogre::Vector3::NEGATIVE_UNIT_Y);

	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(camRay);

	if (result.terrain)
	{
		Ogre::Real terrainHeight = result.position.y;

		if (camPos.y < (terrainHeight + 10.0))
			mCamera->setPosition(camPos.x, terrainHeight + 10.0, camPos.z);
	}
}
