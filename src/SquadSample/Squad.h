#ifndef SQUAD_H
#define SQUAD_H

#include <OgreManualObject.h>
#include "BaseApplication.h"
#include "Unite.h"

class Unite;

//////////////////////////////////////////////////////////////////////////

class Squad
{
public:
	Squad(std::string name, Ogre::SceneManager* sceneMgr, Ogre::Camera* camera);
	~Squad();

	void InitUniteSize();
	void CreateUnits();

	void Update(const Ogre::FrameEvent& evt);
	void UpdateUnitesTarget();

	void IncColumns();
	void IncRows();

	void SetEnemy(Squad* enemy);
	void SetRayTarget(Ogre::Ray cursorRay);
	void JumpAll();
	void TakeOutSwordsAll();
	
	void SelectNodeUnderCursor(Ogre::RaySceneQuery* rayScnQuery, Ogre::Ray cursorRay);
	Unite* FindUniteByName(Ogre::String name);
	void DeleteSelectedUnits();

	bool IsFinished(float step) { return (mDistance -= step) <= 0.f; }

	std::vector<std::vector<Unite*>> GetUnits() { return mUnits; }
	Ogre::Vector3 GetCenter() { return mCenter; }
	Ogre::Vector3 GetDirection() { return mDirection; }
	float GetWidth() { return mWidth; }
	float GetHeight() { return mHeight; }
	float GetStep() { return mStep; }

private:
	int mRows;
	int mColumns;
	float mStep;
	Ogre::Vector3 mUniteSize;

	float mWidth;
	float mHeight;
	float mDistance;
	float mSpeed;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 mCenter;
	Ogre::Vector3 mTarget;

	std::string mName;
	std::vector<std::vector<Unite*>> mUnits;

	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
};

#endif