#ifndef MYGUI_SAMPLE_H
#define MYGUI_SAMPLE_H

#include "BaseApplication.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

class MyGUISample : public BaseApplication
{
public:
	MyGUISample();
	virtual ~MyGUISample();

protected:
	virtual void createScene(void);
	virtual void chooseSceneManager(void);
	virtual void createFrameListener(void);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

private:
	MyGUI::Gui* mGui;
	MyGUI::OgrePlatform* mPlatform;
};

#endif