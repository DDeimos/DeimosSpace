#ifndef TERRAIN_SAMPLE_H
#define TERRAIN_SAMPLE_H

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "BaseApplication.h"

class TerrainSample : public BaseApplication
{
public:
	TerrainSample();
	virtual ~TerrainSample();

protected:
	virtual void createScene();
	virtual void createFrameListener();
	virtual void destroyScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain* terrain);
	void configureTerrainDefaults(Ogre::Light* light);

	bool mTerrainsImported;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	OgreBites::Label* mInfoLabel;
};

#endif