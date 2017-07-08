#ifndef UNITE_H
#define UNITE_H

#include <OgreManualObject.h>
#include "BaseApplication.h"
#include "Squad.h"

#define NUM_ANIMS 13           // number of animations the character has
#define RUN_SPEED 17           // character running speed in units per second
#define TURN_SPEED 500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 90.0f          // gravity in downward units per squared second

class Squad;

class Unite
{
public:
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

	enum UniteState
	{
		NONE,
		ROTATING,
		WAITING,
		MOVING,
		ENEMY
	};

	Unite(Squad* squad, Ogre::SceneManager* sceneMgr, Ogre::Camera* camera, std::string squadName, int row, int column);
	~Unite();

	void Update(Ogre::Real dt);

	void Start(Ogre::Vector3 target);
	void SetPosition(Ogre::Vector3 pos);
	void SetTarget(Ogre::Vector3 target) { mTarget = target; }

	void SetEnemySquad(Squad* enemy);
	void UpdateEnemyUnite();
	void StartJump();
	void TakeOutSwords();

	Ogre::String GetName() { return mName; }
	Ogre::Entity* GetEntity() { return mBodyEntity; }
	Ogre::SceneNode* GetNode() { return mBodyNode; }
	Ogre::Vector3 GetPosition() { return mBodyNode->getPosition(); }

	Ogre::Vector3 GetSize() { return mBodyEntity->getBoundingBox().getSize(); }
	Ogre::Vector3 GetHalfSize() { return mBodyEntity->getBoundingBox().getHalfSize(); }

private:
	void InitBody(Ogre::SceneManager* sceneMgr);
	void InitAnimations();

	void RotateToDirection(Ogre::Real dt);
	void TranslateToTarget(Ogre::Real dt);
	void WaitBeforeMove(Ogre::Real dt);
	void RunToEnemy();

	void StartRunningAnimaton();
	void StopRunningAnimaton();
	void JumpAnimation(Ogre::Real dt);

	void UpdateBody(Ogre::Real deltaTime);
	void UpdateAnimations(Ogre::Real deltaTime);

	void FadeAnimations(Ogre::Real deltaTime);

	void SetBaseAnimation(AnimID id, bool reset = false);
	void SetTopAnimation(AnimID id, bool reset = false);

	Ogre::String mName;
	Ogre::SceneManager* mSceneMgr;

	Squad* mSquad;
	int mColumn;
	int mRow;

	Unite* mEnemyUnite;
	Squad* mEnemySquad;

	float mWaiter;
	float mDistance;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 mTarget;

	Ogre::SceneNode* mBodyNode;
	Ogre::Real mPivotPitch;
	Ogre::Entity* mBodyEntity;
	Ogre::Entity* mLeftSword;
	Ogre::Entity* mRightSword;
	Ogre::AnimationState* mAnims[NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	UniteState mUniteState;
	bool mFadingIn[NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[NUM_ANIMS];           // which animations are fading out
	bool mSwordsDrawn;
	Ogre::Real mVerticalVelocity;     // for jumping
	Ogre::Real mTimer;                // general timer to see how long animations have been playing
};

#endif