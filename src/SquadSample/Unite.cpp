#include "Unite.h"

Unite::Unite(Squad* squad, Ogre::SceneManager* sceneMgr, Ogre::Camera* camera, std::string squadName, int row, int column)
{
	mSquad = squad;
	mRow = row;
	mColumn = column;

	mSceneMgr = sceneMgr;
	mName = squadName + "_" + std::to_string(row) + "_" + std::to_string(column);
	mUniteState = NONE;

	mEnemyUnite = 0;
	mEnemySquad = 0;

	mWaiter = 0.f;

	InitBody(sceneMgr);
	InitAnimations();
}

Unite::~Unite()
{
	mBodyEntity->detachAllObjectsFromBone();
	mBodyNode->detachAllObjects();

	mSceneMgr->destroyEntity(mBodyEntity);
	mSceneMgr->destroyEntity(mLeftSword);
	mSceneMgr->destroyEntity(mRightSword);
	mSceneMgr->destroySceneNode(mBodyNode);
}

void Unite::InitBody(Ogre::SceneManager* sceneMgr)
{
	// create main model
	std::string bodyName = std::string("SinbadBody_") + mName;
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	mBodyEntity = sceneMgr->createEntity(bodyName, "Sinbad.mesh");
	mBodyNode->attachObject(mBodyEntity);
	//mBodyNode->showBoundingBox(true);

	// create swords and attach to sheath
	std::string leftSwordName = std::string("SinbadSwordLeft") + mName;
	std::string rightSwordName = std::string("SinbadSwordRight") + mName;
	Ogre::LogManager::getSingleton().logMessage("Creating swords");
	mLeftSword = sceneMgr->createEntity(leftSwordName, "Sword.mesh");
	mRightSword = sceneMgr->createEntity(rightSwordName, "Sword.mesh");
	mBodyEntity->attachObjectToBone("Sheath.L", mLeftSword);
	mBodyEntity->attachObjectToBone("Sheath.R", mRightSword);

	mVerticalVelocity = 0;
}

void Unite::InitAnimations()
{
	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] = {
		"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
		"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"
	};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEntity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	SetBaseAnimation(ANIM_IDLE_BASE);
	SetTopAnimation(ANIM_IDLE_TOP);

	// relax the hands since we're not holding anything
	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

	int baseLength = mAnims[ANIM_IDLE_BASE]->getLength();
	mAnims[ANIM_IDLE_BASE]->setTimePosition(rand() * baseLength / RAND_MAX);

	int topLength = mAnims[ANIM_IDLE_TOP]->getLength();
	mAnims[ANIM_IDLE_TOP]->setTimePosition(rand() * topLength / RAND_MAX);

	mSwordsDrawn = false;
}

void Unite::StartRunningAnimaton()
{
	if (mBaseAnimID == ANIM_IDLE_BASE)
	{
		SetBaseAnimation(ANIM_RUN_BASE, true);
		if (mTopAnimID == ANIM_IDLE_TOP)
			SetTopAnimation(ANIM_RUN_TOP, true);
	}
}

void Unite::StopRunningAnimaton()
{
	if (mBaseAnimID == ANIM_RUN_BASE)
	{
		SetBaseAnimation(ANIM_IDLE_BASE);
		if (mTopAnimID == ANIM_RUN_TOP)
			SetTopAnimation(ANIM_IDLE_TOP);
	}
}

void Unite::Start(Ogre::Vector3 target)
{
	mTarget = target;
	mDirection = mTarget - GetPosition();
	mDistance = mDirection.normalise();
	mUniteState = ROTATING;

	StartRunningAnimaton();
}

void Unite::Update(Ogre::Real dt)
{
	UpdateBody(dt);
	UpdateAnimations(dt);
}

void Unite::UpdateBody(Ogre::Real dt)
{
	if (mBaseAnimID != ANIM_DANCE)
	{
		if (mUniteState == ROTATING)
			RotateToDirection(dt);
		else if(mUniteState == WAITING)
			WaitBeforeMove(dt);
		else if (mUniteState == MOVING)
			TranslateToTarget(dt);
	}

	if (mBaseAnimID == ANIM_JUMP_LOOP)
	{
		JumpAnimation(dt);
	}
}

void Unite::RotateToDirection(Ogre::Real dt)
{
	mWaiter += dt * 2;

	Ogre::Vector3 orientation = mBodyNode->getOrientation().zAxis();
	Ogre::Quaternion toDir = orientation.getRotationTo(mDirection);
	Ogre::Real yawToDir = toDir.getYaw().valueDegrees();

	// this is how much the character CAN turn this frame
	Ogre::Real yawAtSpeed = yawToDir / Ogre::Math::Abs(yawToDir) * dt * TURN_SPEED;

	// reduce "turnability" if we're in midair
	if (mBaseAnimID == ANIM_JUMP_LOOP)
		yawAtSpeed *= 0.2f;

	if (Ogre::Math::Abs(yawAtSpeed) >= Ogre::Math::Abs(yawToDir))
	{
		yawAtSpeed = yawToDir;
		mUniteState = WAITING;

		StopRunningAnimaton();
	}

	mBodyNode->yaw(Ogre::Degree(yawAtSpeed));
}

void Unite::WaitBeforeMove(Ogre::Real dt)
{
	mWaiter -= dt;

	if (mWaiter <= 0)
	{
		mUniteState = MOVING;
		StartRunningAnimaton();
	}
}

void Unite::TranslateToTarget(Ogre::Real dt)
{
	// move in current body direction (not the goal direction)
	Ogre::Real weight = mAnims[mBaseAnimID]->getWeight();
	Ogre::Real delta = dt * weight * RUN_SPEED;
	mBodyNode->translate(0, 0, delta, Ogre::Node::TS_LOCAL);

	mDistance -= delta;

	if (mDistance <= 0)
	{
		mUniteState = NONE;
		StopRunningAnimaton();
	}
}

void Unite::StartJump()
{
	if (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)
	{
		// jump if on ground
		SetBaseAnimation(ANIM_JUMP_START, true);
		SetTopAnimation(ANIM_NONE);
		mTimer = 0;
	}
}

void Unite::JumpAnimation(Ogre::Real dt)
{
	// if we're jumping, add a vertical offset too, and apply gravity
	mBodyNode->translate(0, mVerticalVelocity * dt, 0, Ogre::Node::TS_LOCAL);
	mVerticalVelocity -= GRAVITY * dt;

	Ogre::Vector3 pos = GetPosition();
	Ogre::Real height = GetHalfSize().y + 0.5f;

	if (pos.y <= height)
	{
		// if we've hit the ground, change to landing state
		pos.y = height;
		mBodyNode->setPosition(pos);
		SetBaseAnimation(ANIM_JUMP_END, true);
		mTimer = 0;
	}
}

void Unite::TakeOutSwords()
{
	if (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)
	{
		// take swords out (or put them back, since it's the same animation but reversed)
		SetTopAnimation(ANIM_DRAW_SWORDS, true);
		mTimer = 0;
	}
}

void Unite::UpdateAnimations(Ogre::Real dt)
{
	Ogre::Real baseAnimSpeed = 1;
	Ogre::Real topAnimSpeed = 1;

	mTimer += dt;

	if (mTopAnimID == ANIM_DRAW_SWORDS)
	{
		// flip the draw swords animation if we need to put it back
		topAnimSpeed = mSwordsDrawn ? -1 : 1;

		// half-way through the animation is when the hand grasps the handles...
		if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
			mTimer - dt < mAnims[mTopAnimID]->getLength() / 2)
		{
			// so transfer the swords from the sheaths to the hands
			mBodyEntity->detachAllObjectsFromBone();
			mBodyEntity->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mLeftSword);
			mBodyEntity->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mRightSword);
			// change the hand state to grab or let go
			mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);
		}

		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE)
				SetTopAnimation(ANIM_IDLE_TOP);
			else
			{
				SetTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
			mSwordsDrawn = !mSwordsDrawn;
		}
	}
	else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
	{
		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE)
				SetTopAnimation(ANIM_IDLE_TOP);
			else
			{
				SetTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
		}

		// don't sway hips from side to side when slicing. that's just embarrassing.
		if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
	}
	else if (mBaseAnimID == ANIM_JUMP_START)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// takeoff animation finished, so time to leave the ground!
			SetBaseAnimation(ANIM_JUMP_LOOP, true);
			// apply a jump acceleration to the character
			mVerticalVelocity = JUMP_ACCEL;
		}
	}
	else if (mBaseAnimID == ANIM_JUMP_END)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// safely landed, so go back to running or idling
			if (mDistance <= 0.f)
			{
				SetBaseAnimation(ANIM_IDLE_BASE);
				SetTopAnimation(ANIM_IDLE_TOP);
			}
			else
			{
				SetBaseAnimation(ANIM_RUN_BASE, true);
				SetTopAnimation(ANIM_RUN_TOP, true);
			}
		}
	}

	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(dt * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(dt * topAnimSpeed);

	// apply smooth transitioning between our animations
	FadeAnimations(dt);
}

void Unite::FadeAnimations(Ogre::Real deltaTime)
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Ogre::Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Ogre::Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void Unite::SetBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void Unite::SetTopAnimation(AnimID id, bool reset)
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void Unite::SetPosition(Ogre::Vector3 pos)
{
	mBodyNode->setPosition(pos);
}

void Unite::SetEnemySquad(Squad* enemy)
{
	mEnemySquad = enemy;
	UpdateEnemyUnite();
}

void Unite::UpdateEnemyUnite()
{
	if (mEnemySquad == 0)
		return;

	float minDist = FLT_MAX;
	for (auto units : mEnemySquad->GetUnits())
	{
		for (auto unite : units)
		{
			Ogre::Vector3 currPos = GetPosition();
			Ogre::Vector3 enemyPos = unite->GetPosition();

			float dist = currPos.distance(enemyPos);

			if (minDist > dist)
			{
				bool newTarget = mEnemyUnite != unite;
				mEnemyUnite = unite;
				minDist = dist;

				if (newTarget)
					RunToEnemy();
			}
		}
	}
}

void Unite::RunToEnemy()
{
	Ogre::Vector3 currPos = GetPosition();
	Ogre::Vector3 enemyPos = mEnemyUnite->GetPosition();

	mDirection = enemyPos - currPos;
	mDistance = mDirection.normalise();
	mUniteState = ROTATING;

	StartRunningAnimaton();
}
