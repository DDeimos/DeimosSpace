#ifndef INPUTBUFFERSAMPLE_H
#define INPUTBUFFERSAMPLE_H

#include "BaseApplication.h"

class InputBufferSample : public BaseApplication
{
public:
	InputBufferSample();
	virtual ~InputBufferSample();

private:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::SceneNode* mCamNode;
	Ogre::Vector3 mDirection;

};

#endif