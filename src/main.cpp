#include "main.h"
#include "Samples.h"

#define WIN32_LEAN_AND_MEAN

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	CSamples sample;
	sample.Load();

	return 0;
}