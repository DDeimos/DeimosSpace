#include "EntitySelectSample.h"

CCreature::CCreature(std::string name, bool robotMode, Ogre::SceneNode* node)
{
	m_name = name;
	m_mode = robotMode ? eCreatureMode::ROBOT : eCreatureMode::NINJA;
	m_node = node;
	m_offset = Ogre::Vector2(0, 0);
}

CCreature::~CCreature()
{
}

EntitySelectSample::EntitySelectSample()
{
	m_creatures = std::vector<CCreature*>();
	m_mode = true;
	m_leftMouseDown = false;
	m_rightMouseDown = false;
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
		mTrayMgr->hideCursor();
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
	if (m_rightMouseDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * m_rotateSpeed));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * m_rotateSpeed));
	}

	return true;
}

bool EntitySelectSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		m_leftMouseDown = false;
	}
	else if (id == OIS::MB_Right)
	{
		mTrayMgr->showCursor();
		m_rightMouseDown = false;
	}

	return true;
}

bool EntitySelectSample::keyReleased(const OIS::KeyEvent& ke)
{
	if (!BaseApplication::keyReleased(ke)) return false;

	if (ke.key == OIS::KC_SPACE)
	{
		m_mode = !m_mode;
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
		Ogre::Entity *ent = mSceneMgr->createEntity(name, m_mode ? "robot.mesh" : "ninja.mesh");
		Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name, result.position);
		ent->setQueryFlags(m_mode ? eCreatureMode::ROBOT : eCreatureMode::NINJA);
		node->attachObject(ent);
		node->showBoundingBox(true);
		m_creatures.push_back(new CCreature(name, m_mode, node));
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
				Ogre::Vector3 creaturePos = creature->GetNode()->getPosition();
				Ogre::Vector2 originPos = mTrayMgr->sceneToScreen(mCamera, creaturePos);

				creature->SetOffset(mTrayMgr->getCursorPos() - originPos);
				creature->GetNode()->showBoundingBox(true);
				break;
			}
		}
		else
		{
			MessageBoxA( NULL, name.c_str(), "Movable is not found!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
	}

	return m_movableFound;
}

void EntitySelectSample::MoveNodeUnderCursor()
{
	CCreature* creature = GetSelectedCreature();
	auto a = mTrayMgr->getCursorPos();
	auto b = creature->GetOffset();
	Ogre::Vector2 pos = mTrayMgr->getCursorPos() - creature->GetOffset();
	Ogre::Ray mouseRay = mTrayMgr->screenToScene(mCamera, pos);

	if (creature)
	{
		Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);

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
