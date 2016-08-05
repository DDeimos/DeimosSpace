#ifndef MULTI_SELECT_SAMPLE_H
#define MULTI_SELECT_SAMPLE_H

#include <OgreManualObject.h>
#include "TerrainSample.h"

class SelectionBox : public Ogre::ManualObject
{
public:
	SelectionBox(const Ogre::String& name);
	virtual ~SelectionBox();

	void setCorners(float left, float top, float right, float bottom);
	void setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& bottomRight);
};

class MultiSelectSample : public TerrainSample
{
public:
	MultiSelectSample();
	virtual ~MultiSelectSample();

protected:
	virtual void createScene(void);
	virtual void chooseSceneManager(void);
	virtual void createFrameListener(void);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	void CreateNodeUnderCursor();
	void PerformSelection(const Ogre::Vector2& first, const Ogre::Vector2& second);
	void DeselectAll();

private:
	bool m_selecting;
	bool m_leftMouseDown, m_rightMouseDown, m_altDown;
	float m_rotateSpeed;
	SelectionBox* m_selectionBox;
	std::vector<Ogre::SceneNode*> m_creatures;

	Ogre::Vector2 m_start, m_stop;
	Ogre::PlaneBoundedVolumeListSceneQuery* m_volumeQuery;
};

#endif