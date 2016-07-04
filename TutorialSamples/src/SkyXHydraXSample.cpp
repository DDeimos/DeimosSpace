#include "SkyXHydraXSample.h"
#include "Modules/ProjectedGrid/ProjectedGrid.h"
#include "Modules/SimpleGrid/SimpleGrid.h"

SkyXHydraXSample::SkyXHydraXSample()
{
	m_skyX = 0;
	m_controller = 0;
	m_hydraX = 0;
}

SkyXHydraXSample::~SkyXHydraXSample()
{
	mRoot->removeFrameListener(m_skyX);
	mWindow->removeListener(m_skyX);

	delete m_skyX;
	delete m_hydraX;
}

void SkyXHydraXSample::createScene()
{
	TerrainSample::createScene();

	mCamera->setFarClipDistance(30000);

	m_controller = new SkyX::BasicController();
	m_controller->setMoonPhase(0.75f);

	m_skyX = new SkyX::SkyX(mSceneMgr, m_controller);
	m_skyX->create();

	mRoot->addFrameListener(m_skyX);
	mWindow->addListener(m_skyX);

	m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());

	m_hydraX = new Hydrax::Hydrax(mSceneMgr, mCamera, mWindow->getViewport(0));

	Hydrax::Module::ProjectedGrid* mModule = new Hydrax::Module::ProjectedGrid( // модуль проекционной сетки
		m_hydraX,  // указатель на главный класс Hydrax
		new Hydrax::Noise::Perlin(),  // модуль для создания ряби
		Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),  // водная поверхность
		Hydrax::MaterialManager::NM_VERTEX,  // режим карты нормалей
		Hydrax::Module::ProjectedGrid::Options(256));  // опции сетки

	m_hydraX->setModule(mModule);
	m_hydraX->loadCfg("HydraxDemo.hdx");
	m_hydraX->create();
	m_hydraX->getMaterialManager()->addDepthTechnique(mTerrainGroup->getTerrain(0, 0)->getMaterial()->createTechnique());
	m_origWaterColor = m_hydraX->getWaterColor();
}

void SkyXHydraXSample::createFrameListener()
{
	TerrainSample::createFrameListener();
}

bool SkyXHydraXSample::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (!TerrainSample::frameRenderingQueued(fe))
	{
		return false;
	}

	m_skyX->setTimeMultiplier(0.2f);


	Ogre::Vector3 cameraPos = mCamera->getDerivedPosition();
	Ogre::Vector3 sunDir = m_controller->getSunDirection();
	float radius = m_skyX->getMeshManager()->getSkydomeRadius(mCamera);
	m_hydraX->setSunPosition(cameraPos + sunDir * radius);

	Ogre::Vector3 sunColor = m_skyX->getVCloudsManager()->getVClouds()->getSunColor();
	//m_hydraX->setSunColor(sunColor);

	Ogre::Vector3 ambientColor = m_skyX->getVCloudsManager()->getVClouds()->getSunColor();
	//m_hydraX->setWaterColor(ambientColor);

	Ogre::Vector3 col = m_hydraX->getWaterColor();
	float height = m_hydraX->getSunPosition().y / 10.0f;

	Hydrax::HydraxComponent c = m_hydraX->getComponents();
	/*if(height < 0)
	{
		if(m_hydraX->isComponent(Hydrax::HYDRAX_COMPONENT_CAUSTICS))
			m_hydraX->setComponents(Hydrax::HydraxComponent(c ^ Hydrax::HYDRAX_COMPONENT_CAUSTICS));
	} else
	{
		if(!m_hydraX->isComponent(Hydrax::HYDRAX_COMPONENT_CAUSTICS))
			m_hydraX->setComponents(Hydrax::HydraxComponent(c | Hydrax::HYDRAX_COMPONENT_CAUSTICS));
	}*/

	/*if(height < -99.0f)
	{
		col = m_origWaterColor * 0.1f;
		height = 9999.0f;
	}
	else if(height < 1.0f)
	{
		col = m_origWaterColor * (0.1f + (0.009f * (height + 99.0f)));
		height = 100.0f / (height + 99.001f);
	}
	else if(height < 2.0f)
	{
		col += m_origWaterColor;
		col /= 2.0f;
		float percent = (height - 1.0f);
		col = (col * percent) + (m_origWaterColor * (1.0f - percent));
	}
	else
	{
		col += m_origWaterColor;
		col	/= 2.0f;
	}
	m_hydraX->setWaterColor(col);
	m_hydraX->setSunArea(height);*/

	m_hydraX->update(fe.timeSinceLastFrame);

	return true;
}

bool SkyXHydraXSample::keyPressed(const OIS::KeyEvent& ke)
{
	mCameraMan->injectKeyDown(ke);

	return true;
}

bool SkyXHydraXSample::keyReleased(const OIS::KeyEvent& ke)
{
	mCameraMan->injectKeyUp(ke);

	return true;
}
