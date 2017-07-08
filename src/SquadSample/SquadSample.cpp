#include "SquadSample.h"

SquadSample::SquadSample()
{
	mLeftMouseDown = false;
	mMiddleMouseDown = false;
	mRightMouseDown = false;
	mCtrlDown = false;
	mAltDown = false;
	mRayScnQuery = 0;
	mSquad1 = 0;
}

SquadSample::~SquadSample()
{
	mSceneMgr->destroyQuery(mRayScnQuery);

	delete mSquad1;
}

void SquadSample::createCamera()
{
	BaseApplication::createCamera();

	mCamera->setPosition(Ogre::Vector3(100, 100, -100));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

void SquadSample::createScene()
{
	mSceneMgr->setSkyBox(true, "Examples/EveningSkyBox");

	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));

	BaseApplication::createScene();

	CreatePlane();
}

void SquadSample::createFrameListener()
{
	BaseApplication::createFrameListener();

	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mTrayMgr->showCursor();

	mSquad1 = new Squad("s1", mSceneMgr, mCamera);
	mSquad1->CreateUnits();
}

bool SquadSample::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);

	mSquad1->Update(evt);

	return ret;
}

bool SquadSample::keyPressed(const OIS::KeyEvent& e)
{
	mCameraMan->injectKeyDown(e);

	if (e.key == OIS::KC_LMENU)
	{
		mAltDown = true;
	}
	else if (e.key == OIS::KC_LCONTROL)
	{
		mCtrlDown = true;
	}

	return true;
}

bool SquadSample::keyReleased(const OIS::KeyEvent& e)
{
	mCameraMan->injectKeyUp(e);

	if (e.key == OIS::KC_LMENU)
	{
		mAltDown = false;
	}
	else if (e.key == OIS::KC_LCONTROL)
	{
		mCtrlDown = false;
	}
	else if (e.key == OIS::KC_SPACE)
	{
		mSquad1->JumpAll();
	}
	else if (e.key == OIS::KC_Q)
	{
		mSquad1->TakeOutSwordsAll();
	}
	else if (e.key == OIS::KC_E)
	{
		//mSquad1->SetEnemy(mSquad2);
	}
	else if (e.key == OIS::KC_DELETE)
	{
		mSquad1->DeleteSelectedUnits();
	}
	else if (e.key == OIS::KC_UP)
	{
	}
	else if (e.key == OIS::KC_DOWN)
	{
	}

	return true;
}

bool SquadSample::mouseMoved(const OIS::MouseEvent &arg)
{
	if (mCtrlDown || mMiddleMouseDown)
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

bool SquadSample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		mLeftMouseDown = true;
	}
	else if (id == OIS::MB_Middle)
	{
		mMiddleMouseDown = true;
	}
	else if (id == OIS::MB_Right)
	{
		mRightMouseDown = true;
	}

	return true;
}

bool SquadSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		mRightMouseDown = false;

		Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
		mSquad1->SelectNodeUnderCursor(mRayScnQuery, ray);

	}
	else if (id == OIS::MB_Middle)
	{
		mMiddleMouseDown = false;
	}
	else if (id == OIS::MB_Right)
	{
		mRightMouseDown = false;

		Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
		mSquad1->SetRayTarget(ray);
	}

	return true;
}

void SquadSample::CreatePlane()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::String name = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	Ogre::MeshManager::getSingleton().createPlane("ground", name, plane, 1000, 1000, 10, 10, true, 1, 20, 20, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setMaterialName("Examples/GrassFloor");
	groundEntity->setCastShadows(false);
}
