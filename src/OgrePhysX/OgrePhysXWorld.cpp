/*
This source file is part of OgrePhysX.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "OgrePhysXWorld.h"
#include "OgrePhysXLogOutputStream.h"
#include "OgrePhysXScene.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"
#include "PxScene.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxExtensionsAPI.h"

namespace OgrePhysX
{
	class OgreErrorCallback : public PxErrorCallback
	{
	public:
		OgreErrorCallback() {}
		~OgreErrorCallback() {}

		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line);
	};

	void OgreErrorCallback::reportError(PxErrorCode::Enum e, const char* message, const char* file, int line)
	{
		const char* errorCode = NULL;

		switch (e)
		{
		case PxErrorCode::eINVALID_PARAMETER:
			errorCode = "invalid parameter";
			break;
		case PxErrorCode::eINVALID_OPERATION:
			errorCode = "invalid operation";
			break;
		case PxErrorCode::eOUT_OF_MEMORY:
			errorCode = "out of memory";
			break;
		case PxErrorCode::eDEBUG_INFO:
			errorCode = "info";
			break;
		case PxErrorCode::eDEBUG_WARNING:
			errorCode = "warning";
			break;
		case PxErrorCode::ePERF_WARNING:
			errorCode = "performance warning";
			break;
		case PxErrorCode::eABORT:
			errorCode = "abort";
			break;
		default:
			errorCode = "unknown error";
			break;
		}

		PX_ASSERT(errorCode);
		if(errorCode)
		{
			char buffer[1024];
			sprintf(buffer, "%s (%d) : %s : %s\n", file, line, errorCode, message);

			//physx::shdfnd::printString(buffer);
			Ogre::LogManager::getSingleton().logMessage(buffer);

			// in debug builds halt execution for abort codes
			PX_ASSERT(e != PxErrorCode::eABORT);

			// in release builds we also want to halt execution 
			// and make sure that the error message is flushed  
			while (e == PxErrorCode::eABORT)
			{
				//physx::shdfnd::printString(buffer);
				Ogre::LogManager::getSingleton().logMessage(buffer);
				//physx::shdfnd::Thread::sleep(1000);
			}
		}	
	}



	World::World()
	{
		mPxPhysics = nullptr;
		mSimulating = false;
	}

	World::~World()
	{
	}

	PxPhysics* World::getPxPhysics()
	{
		return mPxPhysics;
	}
	PxCooking* World::getCookingInterface()
	{
		return mCookingInterface;
	}

	World::OgreFrameListener* World::setupOgreFramelistener()
	{
		OgreFrameListener *frameListener = new OgreFrameListener(this);
		Ogre::Root::getSingleton().addFrameListener(frameListener);
		return frameListener;
	}

	void World::shutdown()
	{
		if (mPxPhysics)
		{
			getCookingInterface()->release();
			clearScenes();
			mPxPhysics->release();
			mPxPhysics = nullptr;
			delete mAllocator;
			mAllocator = nullptr;
			delete mErrorOutputStream;
			mErrorOutputStream = nullptr;
		}
	}

	void World::init()
	{
		if (!mPxPhysics)
		{
			/*mAllocator = new PxDefaultAllocator();
			mErrorOutputStream = new LogOutputStream();

			mPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mAllocator, *mErrorOutputStream, PxTolerancesScale());

			if (!mPxPhysics)
			{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: SDK initialisation failed.");
			return;
			}

			PxCookingParams params;
			mCookingInterface = PxCreateCooking(PX_PHYSICS_VERSION, &mPxPhysics->getFoundation(), params);
			if (!mCookingInterface)
			{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Cooking initialisation failed.");
			return;
			}

			if (!PxInitExtensions(*mPxPhysics))
			{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Extension initialisation failed.");
			return;
			}*/
			static OgreErrorCallback gErrorCallback;
			static PxDefaultAllocator gDefaultAllocatorCallback;

			PxFoundation* mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gErrorCallback);
			if (!mFoundation)
			{
				Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: PxCreateFoundation failed.");
				return;
			}

			bool recordMemoryAllocations = true;
			/*PxProfileZoneManager mProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(mFoundation);
			if (!mProfileZoneManager)
			{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: SDK initialisation failed.");
			return;
			}*/

			mPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
				PxTolerancesScale(), recordMemoryAllocations, NULL /*&mProfileZoneManager*/ );
			if (!mPxPhysics)
			{
				Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: SDK initialisation failed.");
				return;
			}

			PxTolerancesScale sc;
			PxCookingParams params(sc);
			mCookingInterface = PxCreateCooking(PX_PHYSICS_VERSION, mPxPhysics->getFoundation(), params);
			if (!mCookingInterface)
			{
				Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Cooking initialisation failed.");
				return;
			}

			if (!PxInitExtensions(*mPxPhysics))
			{
				Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Extension initialisation failed.");
				return;
			}


			//if (mPxPhysics->getPvdConnectionManager())
			//	PxExtensionVisualDebugger::connect(mPxPhysics->getPvdConnectionManager(), "localhost", 5425, 500, true);

			//create default material
			mDefaultMaterial = mPxPhysics->createMaterial(1.0f, 1.0f, 0.1f);

			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] SDK created.");
		}
	}

	Scene* World::addScene(Ogre::String name, Ogre::SceneManager *ogreSceneMgr)
	{
		Scene *scene = new Scene(ogreSceneMgr);
		mScenes.insert(std::make_pair/*<Ogre::String, Scene*>*/(name, scene));
		return scene;
	}
	Scene* World::addScene(Ogre::String name, Ogre::SceneManager *ogreSceneMgr, PxSceneDesc &desc)
	{
		Scene *scene = new Scene(ogreSceneMgr, desc);
		mScenes.insert(std::make_pair/*<Ogre::String, Scene*>*/(name, scene));
		return scene;
	}
	Scene* World::getScene(Ogre::String name)
	{
		auto i = mScenes.find(name);
		if (i != mScenes.end()) return i->second;
		else
		{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Could not find Scene \"" + name + "\".");
			return 0;
		}
	}
	void World::destroyScene(Ogre::String name)
	{
		auto i = mScenes.find(name);
		if (i != mScenes.end())
		{
			delete i->second;
			mScenes.erase(i);
		}
		else
		{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Error: Could not find Scene \"" + name + "\".");
		}
	}
	void World::clearScenes()
	{
		for (auto i = mScenes.begin(); i != mScenes.end(); i++)
		{
			delete i->second;
		}
		mScenes.clear();
	}

	void World::simulate(float time)
	{
		if (mSimulating)
		{
			Ogre::LogManager::getSingleton().logMessage("[OgrePhysX] Warning: startSimulate called while simulation is in progress!");
		}

		mSimulating = true;
		for (auto i = mScenes.begin(); i != mScenes.end(); i++)
		{
			i->second->simulate(time);
		}
		mSimulating = false;
	}

	void World::syncRenderables()
	{
		for (auto i = mScenes.begin(); i != mScenes.end(); i++)
		{
			i->second->syncRenderables();
		}
	}

	World& World::getSingleton()
	{
		static World instance;
		return instance;
	}

	PxMaterial& World::getDefaultMaterial()
	{
		return *mDefaultMaterial;
	}

	void World::registerOgreMaterialName(const Ogre::String &matName, PxMaterial *pxMat)
	{
		auto i = mMaterialBindings.find(matName);
		if (i != mMaterialBindings.end()) i->second = pxMat;
		else 
			mMaterialBindings.insert(std::make_pair/*<Ogre::String, PxMaterial*>*/(matName, pxMat));
	}

	PxMaterial* World::getOgreMaterialByName(const Ogre::String &matName)
	{
		auto i  = mMaterialBindings.find(matName);
		if (i != mMaterialBindings.end()) return i->second;
		return mDefaultMaterial;
	}


}