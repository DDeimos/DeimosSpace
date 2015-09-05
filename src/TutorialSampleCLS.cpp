#include "TutorialSampleCLS.h"

TutorialSampleCLS::TutorialSampleCLS()
{
	m_dirLightAxis = 0;
}

TutorialSampleCLS::~TutorialSampleCLS()
{

}

void TutorialSampleCLS::createCamera()
{
	BaseApplication::createCamera();

	mCamera->setPosition(Ogre::Vector3(200, 200, -200));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

void TutorialSampleCLS::createScene()
{
	//BaseApplication::createScene();

	InitAmbientLight();

	//CreateSpotLight();
	CreatePointLight();
	CreateDirectionalLight();
	
	CreateGround();
	CreateNinja();
}

bool TutorialSampleCLS::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	BaseApplication::frameRenderingQueued(evt);

	directionalLight->setDirection(Ogre::Vector3(0, Ogre::Math::Cos(m_dirLightAxis), Ogre::Math::Sin(m_dirLightAxis)));
	m_dirLightAxis = m_dirLightAxis < Ogre::Math::TWO_PI ? m_dirLightAxis + 0.0001 : 0;

	return true;
}

void TutorialSampleCLS::InitAmbientLight()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
}

void TutorialSampleCLS::CreateSpotLight()
{
	Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDiffuseColour(0, 1, 0);
	spotLight->setSpecularColour(0, 1, 0);
	spotLight->setDirection(0, -1, -1);
	spotLight->setPosition(Ogre::Vector3(0, 200, 200));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
}

void TutorialSampleCLS::CreatePointLight()
{
	Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setDiffuseColour(0, 0, 10);
	pointLight->setSpecularColour(0, 0, 10);
	pointLight->setPosition(Ogre::Vector3(0, 50, 0));
	pointLight->setAttenuation(600, 1.0, 0.007, 0.0002);
}

void TutorialSampleCLS::CreateDirectionalLight()
{
	directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(1, 0.8, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(1, 0.8, 0));
}

void TutorialSampleCLS::CreateGround()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::String name = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	Ogre::MeshManager::getSingleton().createPlane("ground", name, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setMaterialName("Examples/Rockwall");
	groundEntity->setCastShadows(false);
}

void TutorialSampleCLS::CreateNinja()
{
	Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
	ninjaEntity->setCastShadows(true);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);
}