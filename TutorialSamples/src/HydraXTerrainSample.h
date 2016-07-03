#ifndef HYDRAX_TERRAIN_SAMPLE_H
#define HYDRAX_TERRAIN_SAMPLE_H

#include <HydraX.h>
#include "TerrainSample.h"

class HydraXTerrainSample : public TerrainSample
{
public:
	HydraXTerrainSample();
	virtual ~HydraXTerrainSample();

	virtual void createScene();
	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

protected:
	Hydrax::Hydrax* m_hydraX;
};

#endif