#include "Squad.h"

Squad::Squad(std::string name, Ogre::SceneManager* sceneMgr, Ogre::Camera* camera)
{
	mName = name;
	mSceneMgr = sceneMgr;
	mCamera = camera;
	mColumns = 10;
	mRows = 1;
	mStep = 5;

	InitUniteSize();

	mWidth = mUniteSize.x * mColumns + mStep * (mColumns - 1);
	mHeight = mUniteSize.z * mRows + mStep * (mRows - 1);

	mDistance = 0.f;
	mSpeed = 25.f;
	mDirection = Ogre::Vector3(0, 0, 1);
	mCenter = Ogre::Vector3(0, 0, 0);
	mTarget = Ogre::Vector3(0, 0, 0);
}

Squad::~Squad()
{
	for (auto units : mUnits)
		for (auto unite : units)
			delete unite;

	mUnits.clear();
}

void Squad::InitUniteSize()
{
	Unite* unite = new Unite(this, mSceneMgr, mCamera, mName, 0, 0);
	mUniteSize = unite->GetSize();

	delete unite;
}

void Squad::CreateUnits()
{
	for (int i = 0; i < mRows; i++)
	{
		std::vector<Unite*> row;

		for (int j = 0; j < mColumns; j++)
		{
			float offsetX = (mUniteSize.x + mStep) * j;
			float offsetZ = (mUniteSize.z + mStep) * i;

			Ogre::Vector3 pos = mCenter;
			pos.x += offsetX + mUniteSize.x / 2 - mWidth / 2;
			pos.z += offsetZ + mUniteSize.z / 2 - mHeight / 2;
			pos.y = mUniteSize.y / 2 + 0.2f;

			Unite* unite = new Unite(this, mSceneMgr, mCamera, mName, i, j);
			unite->SetPosition(pos);

			row.push_back(unite);
		}

		mUnits.push_back(row);
	}
}

void Squad::Update(const Ogre::FrameEvent& evt)
{
	float dt = evt.timeSinceLastFrame;
	float step = mSpeed * dt;

	if (!IsFinished(step))
		mCenter += mDirection * step;

	for (auto units : mUnits)
		for (auto unite : units)
			unite->Update(dt);
}

void Squad::SetRayTarget(Ogre::Ray ray)
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	auto test = ray.intersects(plane);

	if (test.first)
	{
		mTarget = ray.getPoint(test.second);

		UpdateUnitesTarget();

		mDirection = mTarget - mCenter;
		mDistance = mDirection.normalise();
	}
}

void Squad::UpdateUnitesTarget()
{
	Ogre::Vector3 newDir = mTarget - mCenter;
	Ogre::Quaternion q = mDirection.getRotationTo(newDir);
	Ogre::Real a = -q.getYaw().valueRadians();

	for (int i = 0; i < mRows; i++)
	{
		for (int j = 0; j < mColumns; j++)
		{
			float offsetX = (mUniteSize.x + mStep) * j;
			float offsetZ = (mUniteSize.z + mStep) * i;

			Ogre::Vector3 pos = mTarget;
			pos.x += offsetX + mUniteSize.x / 2 - mWidth / 2;
			pos.z += offsetZ + mUniteSize.z / 2 - mHeight / 2;
			pos.y = mUniteSize.y / 2 + 0.2f;

			Ogre::Vector3 d = pos - mTarget;
			pos.x = mTarget.x + d.x * Ogre::Math::Cos(a) - d.z * Ogre::Math::Sin(a);
			pos.z = mTarget.z + d.x * Ogre::Math::Sin(a) + d.z * Ogre::Math::Cos(a);

			mUnits[i][j]->Start(pos);
		}
	}
}

void Squad::IncColumns()
{
}

void Squad::IncRows()
{
}

void Squad::SetEnemy(Squad* enemy)
{
	for (auto units : mUnits)
		for (auto unite : units)
			unite->SetEnemySquad(enemy);
}

void Squad::JumpAll()
{
	for (auto units : mUnits)
		for (auto unite : units)
			unite->StartJump();
}

void Squad::TakeOutSwordsAll()
{
	for (auto units : mUnits)
		for (auto unite : units)
			unite->TakeOutSwords();
}

void Squad::SelectNodeUnderCursor(Ogre::RaySceneQuery* rayScnQuery, Ogre::Ray cursorRay)
{
	rayScnQuery->setRay(cursorRay);
	rayScnQuery->setSortByDistance(true);

	for (auto entry : rayScnQuery->execute())
	{
		Ogre::String name = entry.movable ? entry.movable->getName() : "";

		if (name != "" && name != "PlayerCam")
		{
			Unite* unite = FindUniteByName(name);

			if (unite != 0)
			{
				bool selected = unite->GetNode()->getShowBoundingBox();
				unite->GetNode()->showBoundingBox(!selected);

				break;
			}
		}
	}
}

Unite* Squad::FindUniteByName(Ogre::String name)
{
	for (auto units : mUnits)
		for (auto unite : units)
		{
			bool isBody = name.find("Body") != -1;
			bool sameUnite = name.find(unite->GetName()) != -1;

			if (isBody && sameUnite)
				return unite;
		}

	return 0;
}

void Squad::DeleteSelectedUnits()
{
	auto i = mUnits.begin();
	while (i != mUnits.end())
	{
		auto j = i->begin();
		while (j != i->end())
		{
			if ((*j)->GetNode()->getShowBoundingBox())
			{
				Unite* unite = *j;
				j = i->erase(j);

				delete unite;
			}
			else
				j++;
		}
		i++;
	}
}
