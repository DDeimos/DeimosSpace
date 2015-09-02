#include "Samples.h"	

CSamples::CSamples()
{
	m_samples = map<Ogre::String, BaseApplication*>();
}

CSamples::~CSamples()
{
	for(auto i = m_samples.begin(); i != m_samples.end(); i++)
	{
		delete i->second;
	}

	m_samples.clear();
}

void CSamples::Load()
{
	InitSamples();
	LoadStartSample();
}

void CSamples::InitSamples()
{
	m_samples["TerrainSample"] = new TerrainSample();
	m_samples["PhysicsSample"] = new PhysicsSample();
	m_samples["TutorialSampleCLS"] = new TutorialSampleCLS();
}

void CSamples::LoadStartSample()
{
	Ogre::ConfigFile cf;
	cf.load("samples.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName;

	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

		for (Ogre::ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
		{
			if (i->first == "StartSample")
			{
				auto app = m_samples.find(i->second);

				if (app != m_samples.end())
				{
					RunStartSample(app->second);
				}
			}
		}
	}
}

void CSamples::RunStartSample(BaseApplication* app)
{
	try 
	{
		app->go();
	}
	catch( Ogre::Exception& e ) 
	{
		MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
}