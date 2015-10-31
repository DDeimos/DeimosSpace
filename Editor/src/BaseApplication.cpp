#include "BaseApplication.h"


BaseApplication::BaseApplication(void)
	: mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mCameraMan(0),
	mShutDown(false),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0)
{
	mResourcesCfg = Ogre::StringUtil::BLANK;
	mPluginsCfg = Ogre::StringUtil::BLANK;
}

BaseApplication::~BaseApplication(void)
{
	if (mCameraMan) delete mCameraMan;

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

bool BaseApplication::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

		return true;
	}
	else
	{
		return false;
	}
}

void BaseApplication::chooseSceneManager(void)
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}

void BaseApplication::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Ogre::Vector3(0, 500, 200));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(0);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void BaseApplication::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);
}

void BaseApplication::createViewports(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void BaseApplication::setupResources(void)
{
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName;

	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

		for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
}

void BaseApplication::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void BaseApplication::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	if (!setup()) return;

	mRoot->startRendering();
}

bool BaseApplication::setup(void)
{
	mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	if (!configure()) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Load resources
	loadResources();

	createFrameListener();

	return true;
};

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed() || mShutDown)
		return false;

	mKeyboard->capture();
	mMouse->capture();

	return true;
}

bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if(arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}

	mCameraMan->injectKeyDown(arg);

	return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	mCameraMan->injectKeyUp(arg);
	return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	mCameraMan->injectMouseMove(arg);
	return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mCameraMan->injectMouseDown(arg, id);
	return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}