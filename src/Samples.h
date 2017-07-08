#ifndef SAMPLE_H
#define SAMPLE_H

#include <windows.h>
#include "BaseApplication.h"
#include "TerrainSample.h"
#include "LightSample.h"
#include "InputSample.h"
#include "InputBufferSample.h"
#include "MultiMgrSample.h"
#include "EntitySelectSample.h"
#include "MyGUISample.h"
#include "MultiSelectSample.h"
#include "SkyXTerrainSample.h"
#include "SkyXWeatherSample.h"
#include "HydraXTerrainSample.h"
#include "SkyXHydraXSample.h"
#include "OgreSkyWaterSample.h"
#include "OgrePhysXSample.h"
#include "PhysXCubesSample.h"
#include "SquadSample/SquadSample.h"

using namespace std;

class CSamples
{
public:
	CSamples();
	~CSamples();

	void Load();
	void RunStartSample();
	BaseApplication* GetSampleObject(std::string startSample);

private:
	BaseApplication* m_app;
};

#endif