#ifndef MYGUI_SAMPLE_H
#define MYGUI_SAMPLE_H

#include "TerrainSample.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

class MyGUISample : public TerrainSample
{
public:
	MyGUISample();
	virtual ~MyGUISample();

protected:
	virtual void createScene(void);
	virtual void chooseSceneManager(void);
	virtual void createFrameListener(void);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void InitMeshesList();
	void CreateNodeUnderCursor();

private:
	MyGUI::Gui* mGui;
	MyGUI::OgrePlatform* mPlatform;
	MyGUI::ListBox* mMeshesList;
	int mNodeCount;
	bool mLMouseDown, mRMouseDown;
};

#endif