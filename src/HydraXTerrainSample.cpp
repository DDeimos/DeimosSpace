#include "HydraXTerrainSample.h"
#include "Modules/ProjectedGrid/ProjectedGrid.h"
#include "Modules/SimpleGrid/SimpleGrid.h"

HydraXTerrainSample::HydraXTerrainSample()
{
	m_hydraX = 0;
}

HydraXTerrainSample::~HydraXTerrainSample()
{
	delete m_hydraX;
}

void HydraXTerrainSample::createScene()
{
	TerrainSample::createScene();

	m_hydraX = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

	Hydrax::Module::ProjectedGrid* mModule = new Hydrax::Module::ProjectedGrid( // модуль проекционной сетки
		m_hydraX,  // указатель на главный класс Hydrax
		new Hydrax::Noise::Perlin(),  // модуль для создания ряби
		Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),  // водная поверхность
		Hydrax::MaterialManager::NM_VERTEX,  // режим карты нормалей
		Hydrax::Module::ProjectedGrid::Options());  // опции сетки

	Hydrax::Module::SimpleGrid::Options options;
	options.MeshSize = Hydrax::Size(5000);
	Hydrax::Module::SimpleGrid* mModule2 = new Hydrax::Module::SimpleGrid(m_hydraX, new Hydrax::Noise::Perlin(), Hydrax::MaterialManager::NM_VERTEX, options);

	m_hydraX->setModule(mModule);
	m_hydraX->loadCfg("HydraxDemo.hdx");
	m_hydraX->create();
	m_hydraX->getMaterialManager()->addDepthTechnique(mTerrainGroup->getTerrain(0, 0)->getMaterial()->createTechnique());
}

void HydraXTerrainSample::createFrameListener()
{
	TerrainSample::createFrameListener();
}

bool HydraXTerrainSample::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (!TerrainSample::frameRenderingQueued(fe))
	{
		return false;
	}

	m_hydraX->update(fe.timeSinceLastFrame);

	return true;
}

bool HydraXTerrainSample::keyPressed(const OIS::KeyEvent& ke)
{
	mCameraMan->injectKeyDown(ke);

	return true;
}

bool HydraXTerrainSample::keyReleased(const OIS::KeyEvent& ke)
{
	mCameraMan->injectKeyUp(ke);

	return true;
}
