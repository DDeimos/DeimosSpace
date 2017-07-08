#ifndef SQUAD_SAMPLE_H
#define SQUAD_SAMPLE_H

#include <OgreManualObject.h>
#include "BaseApplication.h"
#include "Squad.h"

class SquadSample : public BaseApplication
{
public:
	SquadSample();
	virtual ~SquadSample();

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	void CreatePlane();

private:
	bool mLeftMouseDown;
	bool mMiddleMouseDown;
	bool mRightMouseDown;
	bool mCtrlDown;
	bool mAltDown;
	Ogre::RaySceneQuery* mRayScnQuery;

	Squad* mSquad1;
};

#endif