#include "TerrainCollisionSample.h"

TerrainCollisionSample::TerrainCollisionSample()
{
}

TerrainCollisionSample::~TerrainCollisionSample()
{
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

	Ogre::Vector3 camPos = mCamera->getPosition();
	Ogre::Ray camRay(Ogre::Vector3(camPos.x, 5000.0, camPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(camRay);

	if (result.terrain)
	{
		Ogre::Real terrainHeight = result.position.y;

		if (camPos.y < (terrainHeight + 10.0))
			mCamera->setPosition(camPos.x, terrainHeight + 10.0, camPos.z);
	}

	return true;
}

bool TerrainCollisionSample::mouseMoved(const OIS::MouseEvent &arg)
{
	// Update SdkTrays with the mouse motion
	mTrayMgr->injectMouseMove(arg);

	if (mLMouseDown)
	{
		Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera);
		Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);

		if (result.terrain)
		{
			mCurrentObject->setPosition(result.position);
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
		Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);

		if (result.terrain)
		{
			std::string name = "T " + std::to_string(++mCount);
			Ogre::Entity *ent = mSceneMgr->createEntity(name, "robot.mesh");
			mCurrentObject = mSceneMgr->getRootSceneNode()->createChildSceneNode(name + " Node", result.position);
			mCurrentObject->attachObject(ent);
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
