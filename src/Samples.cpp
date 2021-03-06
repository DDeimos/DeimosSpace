#include "Samples.h"	

CSamples::CSamples()
{
	m_app = 0;
}

CSamples::~CSamples()
{
	delete m_app;
}

BaseApplication* CSamples::GetSampleObject(std::string startSample)
{
	if(startSample == "TerrainSample") 
		return new TerrainSample();
	else if(startSample == "LightSample") 
		return new LightSample();
	else if(startSample == "InputSample") 
		return new InputSample();
	else if(startSample == "InputBufferSample") 
		return new InputBufferSample();
	else if(startSample == "MultiMgrSample") 
		return new MultiMgrSample();
	else if(startSample == "EntitySelectSample") 
		return new EntitySelectSample();
	else if(startSample == "MyGUISample") 
		return new MyGUISample();
	else if(startSample == "MultiSelectSample") 
		return new MultiSelectSample();
	else if(startSample == "SkyXTerrainSample") 
		return new SkyXTerrainSample();
	else if(startSample == "SkyXWeatherSample") 
		return new SkyXWeatherSample();
	else if(startSample == "HydraXTerrainSample") 
		return new HydraXTerrainSample();
	else if(startSample == "SkyXHydraXSample") 
		return new SkyXHydraXSample();
	else if(startSample == "OgreSkyWaterSample") 
		return new OgreSkyWaterSample();
	else if(startSample == "OgrePhysXSample") 
		return new OgrePhysXSample();
	else if(startSample == "PhysXCubesSample") 
		return new PhysXCubesSample();
	else if(startSample == "SquadSample") 
		return new SquadSample();

	return 0;
}

void CSamples::Load()
{
	Ogre::ConfigFile cf;
#ifdef _DEBUG
	cf.load("samples_d.cfg");
#else
	cf.load("samples.cfg");
#endif

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
				m_app = GetSampleObject(i->second);

				if (m_app != 0)
				{
					RunStartSample();
				}
			}
		}
	}
}

void CSamples::RunStartSample()
{
	try 
	{
		m_app->go();
	}
	catch( Ogre::Exception& e ) 
	{
		MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}
}