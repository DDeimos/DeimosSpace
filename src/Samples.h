#ifndef SAMPLE_H
#define SAMPLE_H

#include <windows.h>
#include "BaseApplication.h"
#include "TerrainSample.h"
#include "PhysicsSample.h"
#include "LightSample.h"
#include "InputSample.h"
#include "MultiMgrSample.h"

using namespace std;

class CSamples
{
public:
	CSamples();
	~CSamples();

	void Load();
	void InitSamples();
	void LoadStartSample();
	void RunStartSample(BaseApplication* app);

private:
	map<Ogre::String, BaseApplication*> m_samples;
};

#endif