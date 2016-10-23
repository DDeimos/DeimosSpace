#ifndef PHYSX_CUBES_SAMPLE_H
#define PHYSX_CUBES_SAMPLE_H

#include "BaseApplication.h"
#include "OgrePhysX.h"
#include "PxPhysicsAPI.h"

class CCube
{
public:
	CCube(int count, Ogre::SceneManager* sceneMgr, OgrePhysX::Scene* physXScene = 0);
	~CCube();

	void Initialize();

	void AddForce(Ogre::Camera* camera, physx::PxVec3 force);
	void SetPosUnderCursor(Ogre::RaySceneQuery* rayScnQuery, Ogre::Ray ray);
	void SetPosition(Ogre::Vector3 pos);

	Ogre::String GetName() { return mName; }
	Ogre::Entity* GetEntity() { return mEntity; }
	Ogre::SceneNode* GetNode() { return mNode; }

	Ogre::Vector3 GetScale() { return mScale; }
	Ogre::Vector3 GetSize() { return mEntity->getBoundingBox().getSize(); }
	Ogre::Vector3 GetHalfSize() { return mEntity->getBoundingBox().getHalfSize(); }

	OgrePhysX::RenderedActorBinding* GetBinding() { return mBinding; }
	OgrePhysX::Actor<physx::PxRigidDynamic> GetActor() { return mActor; }

private:
	void InitEntity();
	void InitNode();
	void InitPhys();

	Ogre::String mName;
	Ogre::Vector3 mScale;
	Ogre::Entity* mEntity;
	Ogre::SceneNode* mNode;
	OgrePhysX::RenderedActorBinding* mBinding;
	OgrePhysX::Actor<physx::PxRigidDynamic> mActor;

	Ogre::SceneManager* mSceneMgr;
	OgrePhysX::Scene* mPhysXScene;
};

class PhysXCubesSample : public BaseApplication
{
public:
	PhysXCubesSample();
	virtual ~PhysXCubesSample();

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual void createFrameListener();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	void CreatePlane();
	void CreateJoint(CCube* cubeA, CCube* cubeB);
	void CreateCube();
	void CreateForceCube();
	void CreateEmptyCube();

	void TrySelectCube();
	void SelectCube(CCube* cube);
	void UnselectAllCubes();

	void ClearAllCubes();

	CCube* FindCubeUnderCursor();

private:
	bool mRightMouseDown;
	bool mIsAltDown;
	Ogre::RaySceneQuery* mRayScnQuery;
	OgrePhysX::Scene* mPhysXScene;
	std::map<Ogre::String, CCube*> mCubes;
	CCube* mTargetCube;
};

#endif