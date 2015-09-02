#include "TutorialSampleCLS.h"

TutorialSampleCLS::TutorialSampleCLS()
{

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

	CreateSpotLight();
	//CreatePointLight();
	CreateDirectionalLight();
	
	CreateGround();
	CreateNinja();
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
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(200, 200, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
}

void TutorialSampleCLS::CreatePointLight()
{
	Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setDiffuseColour(0.3, 0.3, 0.3);
	pointLight->setSpecularColour(0.3, 0.3, 0.3);
	pointLight->setPosition(Ogre::Vector3(0, 150, 250));
}

void TutorialSampleCLS::CreateDirectionalLight()
{
	Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));
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