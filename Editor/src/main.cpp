#include <windows.h>
#include "BaseApplication.h"

#define WIN32_LEAN_AND_MEAN

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	try 
	{
		BaseApplication app;
		app.go();
	}
	catch( Ogre::Exception& e ) 
	{
		MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	return 0;
}