#ifndef SKYX_HYDRAX_SAMPLE_H
#define SKYX_HYDRAX_SAMPLE_H

#include <SkyX.h>
#include <HydraX.h>
#include "TerrainSample.h"

class SkyXHydraXSample : public TerrainSample
{
public:
	SkyXHydraXSample();
	virtual ~SkyXHydraXSample();

	virtual void createScene();
	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

protected:
	Ogre::Vector3 m_origWaterColor;

	SkyX::SkyX* m_skyX;
	SkyX::BasicController* m_controller;

	Hydrax::Hydrax* m_hydraX;
};

#endif