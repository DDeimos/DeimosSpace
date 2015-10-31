#include "TerrainSample.h"

TerrainSample::TerrainSample()
	: mTerrainGroup(0),
	mTerrainGlobals(0),
	mInfoLabel(0)
{
}

TerrainSample::~TerrainSample()
{
}

void TerrainSample::createScene()
{
	// Fog
	/*Ogre::ColourValue fadeColour(0.6, 0.6, 0.6);
	mSceneMgr->setFog(Ogre::FOG_EXP, fadeColour, 0.002);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);*/

	BaseApplication::createScene();

	// House
	Ogre::Entity* tudorEntity = mSceneMgr->createEntity("tudorhouse.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node");
	node->attachObject(tudorEntity);
	node->setPosition(Ogre::Vector3(1683, 550, 1316));

	//Configuring the Terrain
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	configureTerrainDefaults(mLight);

	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	mTerrainGroup->loadAllTerrains(true);

	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();

		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();	
}

void TerrainSample::createFrameListener()
{
	BaseApplication::createFrameListener();

	//Terrain Create Label
	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}

void TerrainSample::destroyScene()
{
	//Cleaning Up
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}

bool TerrainSample::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (!BaseApplication::frameRenderingQueued(fe))
		return false;

	//Terrain Loading Label
	if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->show();
		if (mTerrainsImported)
		{
			mInfoLabel->setCaption("Building terrain, please wait...");
		}
		else
		{
			mInfoLabel->setCaption("Updating textures, patience...");
		}
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(mInfoLabel);
		mInfoLabel->hide();
		if (mTerrainsImported)
		{
			mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}

	return true;
}

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	//Writing getTerrainImage
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}

void TerrainSample::defineTerrain(long x, long y)
{
	//Writing defineTerrain
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	bool exists = Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename);

	if (exists)
		mTerrainGroup->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);

		mTerrainsImported = true;
	}
}

void TerrainSample::initBlendMaps(Ogre::Terrain* terrain)
{
	//Writing initBlendMaps
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}

void TerrainSample::configureTerrainDefaults(Ogre::Light* light)
{
	//Writing configureTerrainDefaults
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(3000);

	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

	Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = 513;
	importData.worldSize = 12000.0;
	importData.inputScale = 600;
	importData.minBatchSize = 33;
	importData.maxBatchSize = 65;

	importData.layerList.resize(3);

	importData.layerList[0].worldSize = 100;
	importData.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	importData.layerList[1].worldSize = 30;
	importData.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	importData.layerList[2].worldSize = 200;
	importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");	
}
