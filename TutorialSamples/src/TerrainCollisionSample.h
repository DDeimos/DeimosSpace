#ifndef TERRAIN_COLLISION_SAMPLE_H
#define TERRAIN_COLLISION_SAMPLE_H

#include "TerrainSample.h"

class TerrainCollisionSample : public TerrainSample
{
public:
	TerrainCollisionSample();
	virtual ~TerrainCollisionSample();

protected:
	virtual void createScene(void);
	virtual void chooseSceneManager(void);
	virtual void createFrameListener(void);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

private:
	Ogre::SceneNode *mCurrentObject; // The newly created object
	bool mLMouseDown, mRMouseDown; // True if the mouse buttons are down
	double mRotateSpeed; // The speed of camera rotation
	int mCount; // The number of robots on the screen
};

#endif