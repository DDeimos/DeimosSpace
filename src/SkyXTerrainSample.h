#ifndef SKYX_TERRAIN_SAMPLE_H
#define SKYX_TERRAIN_SAMPLE_H

#include <SkyX.h>
#include "TerrainSample.h"

class SkyXTerrainSample : public TerrainSample
{
public:
	SkyXTerrainSample();
	virtual ~SkyXTerrainSample();

	virtual void createScene();
	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	Ogre::String GetSkyXOptions(const SkyX::AtmosphereManager::Options& options);

protected:
	SkyX::SkyX* m_skyX;
	SkyX::BasicController* m_controller;

	OgreBites::TextBox* m_info;
};

#endif