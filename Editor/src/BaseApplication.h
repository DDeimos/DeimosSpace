#ifndef BASE_APPLICATION_H
#define BASE_APPLICATION_H

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkCameraMan.h>

class BaseApplication 
	: public Ogre::FrameListener, 
	public Ogre::WindowEventListener, 
	public OIS::KeyListener, 
	public OIS::MouseListener
{
public:
	BaseApplication(void);
	virtual ~BaseApplication(void);

	virtual void go(void);

protected:
	virtual bool setup();
	virtual bool configure(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createViewports(void);
	virtual void setupResources(void);
	virtual void loadResources(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	bool mShutDown;
	Ogre::Root *mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;

	Ogre::Camera* mCamera;
	OgreBites::SdkCameraMan* mCameraMan;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
};

#endif