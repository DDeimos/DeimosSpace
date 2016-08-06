/*#include "MyGUISample.h"
#include "utils.h"

MyGUISample::MyGUISample()
{
	mNodeCount = 0;
	mLMouseDown = false;
	mRMouseDown = false;
}

MyGUISample::~MyGUISample()
{
	mGui->shutdown();
	delete mGui;

	mPlatform->shutdown();
	delete mPlatform;
}

void MyGUISample::createScene()
{
	TerrainSample::createScene();

	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mWindow, mSceneMgr);

	mGui = new MyGUI::Gui();
	mGui->initialise();

	InitMeshesList();
}

void MyGUISample::InitMeshesList()
{
	mMeshesList = mGui->createWidget<MyGUI::ListBox>("ListBox", 5, 5, 120, 590, MyGUI::Align::Default, "Main");
	auto files = Directory::GetFiles("../../DataFiles/models/", "*.mesh");

	for (std::string file : files)
	{
		mMeshesList->addItem(file);
	}
}

void MyGUISample::CreateNodeUnderCursor()
{
	size_t index = mMeshesList->getIndexSelected();

	if (index != MyGUI::ITEM_NONE)
	{
		OIS::MouseState state = mMouse->getMouseState();
		Ogre::Vector2 pos = Ogre::Vector2((float)state.X.abs / state.width, (float)state.Y.abs / state.height);
		Ogre::Ray ray = mCamera->getCameraToViewportRay(pos.x, pos.y);
		Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(ray);

		if (result.terrain)
		{
			std::string item = mMeshesList->getItemNameAt(index);
			std::string name = item + "_" + std::to_string(++mNodeCount);
			Ogre::Entity *ent = mSceneMgr->createEntity(name, item);
			Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name, result.position);
			node->attachObject(ent);
		}
	}
}

void MyGUISample::createFrameListener()
{
	TerrainSample::createFrameListener();
}

void MyGUISample::chooseSceneManager()
{
	TerrainSample::chooseSceneManager();
}

bool MyGUISample::frameRenderingQueued(const Ogre::FrameEvent &evt)
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

bool MyGUISample::mouseMoved(const OIS::MouseEvent &evt)
{
	if (mRMouseDown && !TerrainSample::mouseMoved(evt))
		return false;

	if (!MyGUI::InputManager::getInstance().injectMouseMove(evt.state.X.abs, evt.state.Y.abs, evt.state.Z.abs))
		return false;

	return true;
}

bool MyGUISample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		mLMouseDown = true;
	}
	else if (id == OIS::MB_Right)
	{
		mRMouseDown = true;
	}

	MyGUI::MouseButton button = MyGUI::MouseButton::Enum(id);
	if (!MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, button))
		return false;

	return true;
}

bool MyGUISample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		mLMouseDown = false;

		MyGUI::MouseButton button = MyGUI::MouseButton::Enum(id);
		if (!MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, button))
		{
			CreateNodeUnderCursor();
			return false;
		}
	}
	else if (id == OIS::MB_Right)
	{
		mRMouseDown = false;
	}

	return true;
}
*/