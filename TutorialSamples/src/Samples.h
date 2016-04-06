#ifndef SAMPLE_H
#define SAMPLE_H

#include <windows.h>
#include "BaseApplication.h"
#include "TerrainSample.h"
#include "PhysicsSample.h"
#include "LightSample.h"
#include "InputSample.h"
#include "InputBufferSample.h"
#include "MultiMgrSample.h"
#include "TerrainCollisionSample.h"
#include "AnimationSample.h"
#include "EntitySelectSample.h"
#include "MyGUISample.h"
#include "MultiSelectSample.h"

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