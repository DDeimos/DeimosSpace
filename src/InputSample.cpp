#include "InputSample.h"

#define METHOD
static bool mouseDownLastFrame = false;
static Ogre::Real toggleTimer = 0.0;
static Ogre::Real rotate = 0.13;
static Ogre::Real move = 30;


InputSample::InputSample()
{
}
 
InputSample::~InputSample()
{
}

void InputSample::createCamera()
{
	BaseApplication::createCamera();

	mCamera->setPosition(Ogre::Vector3(200, 200, -200));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
}
 
void InputSample::createScene()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
	Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(250, 150, 250);
	pointLight->setDiffuseColour(Ogre::ColourValue::White);
	pointLight->setSpecularColour(Ogre::ColourValue::White);
 
	Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
	Ogre::SceneNode* ninjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
	ninjaNode->attachObject(ninjaEntity); 
}

bool InputSample::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
#ifdef METHOD	
	bool leftMouseDown = mMouse->getMouseState().buttonDown(OIS::MB_Left);
	if (leftMouseDown && !mouseDownLastFrame)
	{
		Ogre::Light* light = mSceneMgr->getLight("PointLight");
		light->setVisible(!light->isVisible());
	}
	mouseDownLastFrame = leftMouseDown;
#else
	if ((toggleTimer < 0) && mMouse->getMouseState().buttonDown(OIS::MB_Right))
	{
		toggleTimer  = 0.5; 
		Ogre::Light* light = mSceneMgr->getLight("PointLight");
		light->setVisible(!light->isVisible());
	}
#endif	
	return true;
}

bool InputSample::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	bool ret = BaseApplication::frameRenderingQueued(fe);
	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;

	if (mKeyboard->isKeyDown(OIS::KC_I))
		dirVec.z -= move;

	if (mKeyboard->isKeyDown(OIS::KC_K))
		dirVec.z += move;

	if (mKeyboard->isKeyDown(OIS::KC_U))
		dirVec.y += move;
	if (mKeyboard->isKeyDown(OIS::KC_O))
		dirVec.y -= move;

	if (mKeyboard->isKeyDown(OIS::KC_J))
	{    
		if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			mSceneMgr->getSceneNode("NinjaNode")->yaw(Ogre::Degree(5 * rotate));
		else
			dirVec.x -= move;
	}
 
	if (mKeyboard->isKeyDown(OIS::KC_L))
	{
		if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			mSceneMgr->getSceneNode("NinjaNode")->yaw(Ogre::Degree(-5 * rotate));
		else
			dirVec.x += move;
	}

	mSceneMgr->getSceneNode("NinjaNode")->translate(
	dirVec * fe.timeSinceLastFrame,
	Ogre::Node::TS_LOCAL);

#ifdef METHOD
	if(!processUnbufferedInput(fe))
		return false;
#else
	toggleTimer -= fe.timeSinceLastFrame;
#endif	

	return ret;	
}
 
// MAIN FUNCTION OMITTED FOR SPACE