#include "MyGUISample.h"

MyGUISample::MyGUISample()
{
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
	BaseApplication::createScene();

	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mWindow, mSceneMgr);

	mGui = new MyGUI::Gui();
	mGui->initialise();

	MyGUI::ButtonPtr mButton = mGui->createWidget<MyGUI::Button>("Button", 10, 10, 300, 50, MyGUI::Align::Default, "Main");
	mButton->setCaption("Allahu Akbar");
}

void MyGUISample::createFrameListener()
{
	BaseApplication::createFrameListener();
}

void MyGUISample::chooseSceneManager()
{
	BaseApplication::chooseSceneManager();
}

bool MyGUISample::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	if (!BaseApplication::frameRenderingQueued(evt))
		return false;

	return true;
}
