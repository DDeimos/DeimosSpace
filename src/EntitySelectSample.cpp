#include "EntitySelectSample.h"

CCreature::CCreature(std::string name, bool robotMode, Ogre::Entity* entity, Ogre::SceneNode* node, float speed)
{
	m_name = name;
	m_mode = robotMode ? eCreatureMode::ROBOT : eCreatureMode::NINJA;
	m_entity = entity;
	m_node = node;
	m_speed = speed;

	SetAnimation("Idle");
}

CCreature::~CCreature()
{
}

void CCreature::SetAnimation(std::string name)
{
	mAnimation = m_entity->getAnimationState(name);
	mAnimation->setLoop(true);
	mAnimation->setEnabled(true);
}

void CCreature::SetTarget(Ogre::Vector3 scenePos)
{
	m_direction = scenePos - m_node->getPosition();
	m_distance = m_direction.normalise();
	m_direction.y = 0; // движение только по горизонтали

	UpdateRotation();
	SetAnimation("Walk");
}

void CCreature::UpdateRotation()
{
	Ogre::Vector3 src = m_node->getOrientation() * Ogre::Vector3::UNIT_X;

	if ((1.0 + src.dotProduct(m_direction)) < 0.0001) // деление на ноль
	{
		m_node->yaw(Ogre::Degree(180));
	}
	else
	{
		Ogre::Quaternion quater = src.getRotationTo(m_direction);
		m_node->rotate(quater);
	}
}

void CCreature::Move(float dt, Ogre::TerrainGroup* terrainGroup)
{
	float step = m_speed * dt;

	if (!IsFinished(step))
	{
		m_node->translate(m_direction * step);

		Ogre::Vector3 pos = m_node->getPosition();
		Ogre::Ray ray(Ogre::Vector3(pos.x, 5000.0, pos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
		Ogre::TerrainGroup::RayResult result = terrainGroup->rayIntersects(ray);

		if (result.terrain)
		{
			m_node->setPosition(pos.x, result.position.y, pos.z);
		}
	}
	else
	{
		SetAnimation("Idle");
	}

	mAnimation->addTime(dt);
}

EntitySelectSample::EntitySelectSample()
{
	m_creatures = std::vector<CCreature*>();
	m_mode = true;
	m_leftMouseDown = false;
	m_rightMouseDown = false;
	m_altDown = false;
	m_movableFound = false;
	m_rotateSpeed = 0.1;
}

EntitySelectSample::~EntitySelectSample()
{
	mSceneMgr->destroyQuery(m_rayScnQuery);

	for (auto creature : m_creatures)
	{
		delete creature;
	}
	m_creatures.clear();
}

void EntitySelectSample::createCamera()
{
	BaseApplication::createCamera();

	mCamera->setPosition(Ogre::Vector3(500, 500, -500));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
}

void EntitySelectSample::createScene()
{
	TerrainSample::createScene();
}

void EntitySelectSample::createFrameListener()
{
	TerrainSample::createFrameListener();

	m_rayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	mTrayMgr->showCursor(); // to get the mouse cursor on the screen
}

void EntitySelectSample::chooseSceneManager()
{
	BaseApplication::chooseSceneManager();
}

bool EntitySelectSample::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	if (!TerrainSample::frameRenderingQueued(evt)) return false;

	if (!mInfoLabel->isVisible())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->setCaption(m_mode ? "Robot" : "Ninja");
		mInfoLabel->show();
	}

	Ogre::Vector3 camPos = mCamera->getPosition();
	Ogre::Ray camRay(Ogre::Vector3(camPos.x, 5000.0, camPos.z), Ogre::Vector3::NEGATIVE_UNIT_Y);
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(camRay);

	if (result.terrain)
	{
		Ogre::Real terrainHeight = result.position.y;

		if (camPos.y < (terrainHeight + 10.0))
			mCamera->setPosition(camPos.x, terrainHeight + 10.0, camPos.z);
	}

	CCreature* creature = GetSelectedCreature();
	if (creature != 0)
	{
		creature->Move(evt.timeSinceLastFrame, mTerrainGroup);
	}

	return true;
}

bool EntitySelectSample::keyPressed(const OIS::KeyEvent& ke)
{
	if (!TerrainSample::keyPressed(ke))
	{
		return false;
	}

	if (ke.key == OIS::KeyCode::KC_LMENU)
	{
		mTrayMgr->hideCursor();
		m_altDown = true;
	}

	return true;
}

bool EntitySelectSample::keyReleased(const OIS::KeyEvent& ke)
{
	if (!TerrainSample::keyReleased(ke))
	{
		return false;
	}

	if (m_altDown)
	{
		m_altDown = false;
		mTrayMgr->showCursor();
	}

	return true;
}

bool EntitySelectSample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		ShowNodesBoxes(false);

		if (!SelectNodeUnderCursor())
		{
			CreateNodeUnderCursor();
		}

		m_leftMouseDown = true;
	}
	else if (id == OIS::MB_Right)
	{
		m_rightMouseDown = true;
	}

	return true;
}

bool EntitySelectSample::mouseMoved(const OIS::MouseEvent &arg)
{
	// Update SdkTrays with the mouse motion
	mTrayMgr->injectMouseMove(arg);

	if (m_leftMouseDown)
	{
		MoveNodeUnderCursor();
	}
	if (m_altDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * m_rotateSpeed));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * m_rotateSpeed));
	}

	return true;
}

bool EntitySelectSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (m_leftMouseDown)
	{
		m_leftMouseDown = false;
	}
	else if (m_rightMouseDown)
	{
		m_rightMouseDown = false;
		CCreature* creature = GetSelectedCreature();

		if (creature)
		{
			Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
			Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(ray);

			if (result.terrain)
			{
				creature->SetTarget(result.position);
			}
		}
	}

	return true;
}

void EntitySelectSample::CreateNodeUnderCursor()
{
	Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera);
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);

	if (result.terrain)
	{
		std::string name = "T " + std::to_string(m_creatures.size() + 1);
		Ogre::Entity *entity = mSceneMgr->createEntity(name, m_mode ? "robot.mesh" : "ninja.mesh");
		Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name, result.position);
		entity->setQueryFlags(m_mode ? eCreatureMode::ROBOT : eCreatureMode::NINJA);
		node->attachObject(entity);
		node->showBoundingBox(true);
		m_creatures.push_back(new CCreature(name, m_mode, entity, node, 50.f));
	}
}

bool EntitySelectSample::SelectNodeUnderCursor()
{
	m_movableFound = false;
	Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
	m_rayScnQuery->setRay(ray);
	m_rayScnQuery->setSortByDistance(true);
	m_rayScnQuery->setQueryMask(m_mode ? eCreatureMode::ROBOT : eCreatureMode::NINJA);

	for (auto entry : m_rayScnQuery->execute())
	{
		std::string name = entry.movable ? entry.movable->getName() : "";
		m_movableFound = name != "" && name != "PlayerCam";

		if (m_movableFound)
		{
			CCreature* creature = FindCreatureByName(name);

			if (creature != 0)
			{
				creature->GetNode()->showBoundingBox(true);
				break;
			}
		}
		else
		{
			// ???
			//MessageBoxA( NULL, name.c_str(), "Movable is not found!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
	}

	return m_movableFound;
}

void EntitySelectSample::MoveNodeUnderCursor()
{
	CCreature* creature = GetSelectedCreature();

	if (creature)
	{
		Ogre::Ray ray = mTrayMgr->getCursorRay(mCamera);
		Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(ray);

		if (result.terrain)
		{
			creature->GetNode()->setPosition(result.position);
		}
	}
}

void EntitySelectSample::ShowNodesBoxes(bool show)
{
	for (auto creature : m_creatures)
	{
		creature->GetNode()->showBoundingBox(show);
	}
}

CCreature* EntitySelectSample::FindCreatureByName(std::string name)
{
	for (auto creature : m_creatures)
	{
		if (creature->GetName() == name)
		{
			return creature;
		}
	}

	return 0;
}

CCreature* EntitySelectSample::GetSelectedCreature()
{
	for (auto creature : m_creatures)
	{
		if (creature->GetNode()->getShowBoundingBox())
		{
			return creature;
		}
	}

	return 0;
}
