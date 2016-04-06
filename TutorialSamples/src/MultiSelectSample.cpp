#include "MultiSelectSample.h"

SelectionBox::SelectionBox(const Ogre::String& name) : Ogre::ManualObject(name)
{
	setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
	setUseIdentityProjection(true);
	setUseIdentityView(true);
	setQueryFlags(0);
}

SelectionBox::~SelectionBox()
{
}

void SelectionBox::setCorners(float left, float top, float right, float bottom)
{
	left = 2 * left - 1;
	right = 2 * right - 1;
	top = 1 - 2 * top;
	bottom = 1 - 2 * bottom;

	clear();
	begin("Examples/KnotTexture", Ogre::RenderOperation::OT_LINE_STRIP);
	position(left, top, -1);
	position(right, top, -1);
	position(right, bottom, -1);
	position(left, bottom, -1);
	position(left, top, -1);
	end();

	setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}

void SelectionBox::setCorners(const Ogre::Vector2& topLeft,	const Ogre::Vector2& bottomRight)
{
	setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

//////////////////////////////////////////////////////////////////////////

MultiSelectSample::MultiSelectSample()
{
	m_selecting = false;
	m_leftMouseDown = false;
	m_rightMouseDown = false;
	m_altDown = false;
	m_rotateSpeed = 0.1f;
	m_selectionBox = 0;
	m_volumeQuery = 0;

	m_creatures = std::vector<Ogre::SceneNode*>();
}

MultiSelectSample::~MultiSelectSample()
{
	mSceneMgr->destroyQuery(m_volumeQuery);

	if (m_selectionBox)
	{
		delete m_selectionBox;
	}
}

void MultiSelectSample::createScene()
{
	TerrainSample::createScene();

	m_selectionBox = new SelectionBox("SelectionBox");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(m_selectionBox);
	m_volumeQuery = mSceneMgr->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
}

void MultiSelectSample::createFrameListener()
{
	TerrainSample::createFrameListener();

	mTrayMgr->showCursor(); // to get the mouse cursor on the screen
}

void MultiSelectSample::chooseSceneManager()
{
	BaseApplication::chooseSceneManager();
}

bool MultiSelectSample::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	if (!TerrainSample::frameRenderingQueued(evt)) return false;

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

bool MultiSelectSample::keyPressed(const OIS::KeyEvent& ke)
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

bool MultiSelectSample::keyReleased(const OIS::KeyEvent& ke)
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

bool MultiSelectSample::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MB_Left)
	{
		m_start = mTrayMgr->getCursorPos();
		m_stop = m_start;

		m_selecting = true;
		m_selectionBox->clear();
		m_selectionBox->setVisible(true);

		m_leftMouseDown = true;
	}
	else if (id == OIS::MB_Right)
	{
		m_rightMouseDown = true;
	}

	return true;
}

bool MultiSelectSample::mouseMoved(const OIS::MouseEvent &arg)
{
	mTrayMgr->injectMouseMove(arg);

	if (m_leftMouseDown)
	{
		if (m_selecting)
		{
			m_stop = mTrayMgr->getCursorPos();
			m_selectionBox->setCorners(m_start, m_stop);
		}
	}

	if (m_altDown)
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * m_rotateSpeed));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * m_rotateSpeed));
	}

	return true;
}

bool MultiSelectSample::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (m_leftMouseDown)
	{
		PerformSelection(m_start, m_stop);
		m_selecting = false;
		m_selectionBox->setVisible(false);

		m_leftMouseDown = false;
	}
	else if (m_rightMouseDown)
	{
		CreateNodeUnderCursor();

		m_rightMouseDown = false;
	}

	return true;
}

void MultiSelectSample::CreateNodeUnderCursor()
{
	Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera);
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(mouseRay);

	if (result.terrain)
	{
		std::string name = "T " + std::to_string(m_creatures.size() + 1);
		Ogre::Entity *entity = mSceneMgr->createEntity(name, "robot.mesh");
		Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name, result.position);
		node->attachObject(entity);
		m_creatures.push_back(node);
	}
}

void MultiSelectSample::PerformSelection(const Ogre::Vector2& first, const Ogre::Vector2& second)
{
	Ogre::RealRect r(first.x, first.y, second.x, second.y);

	if (r.left > r.right)
		std::swap(r.left, r.right);

	if (r.top > r.bottom)
		std::swap(r.top, r.bottom);

	if ((r.right - r.left) * (r.bottom - r.top) < 0.0001)
	{
		return;
	}

	// screenToScene
	Ogre::Ray topLeft = mCamera->getCameraToViewportRay(r.left, r.top);
	Ogre::Ray topRight = mCamera->getCameraToViewportRay(r.right, r.top);
	Ogre::Ray bottomLeft = mCamera->getCameraToViewportRay(r.left, r.bottom);
	Ogre::Ray bottomRight = mCamera->getCameraToViewportRay(r.right, r.bottom);

	Ogre::Plane frontPlane = Ogre::Plane(topLeft.getOrigin(), topRight.getOrigin(), bottomRight.getOrigin());
	Ogre::Plane topPlane = Ogre::Plane(topLeft.getOrigin(), topLeft.getPoint(10), topRight.getPoint(10));
	Ogre::Plane leftPlane = Ogre::Plane(topLeft.getOrigin(), bottomLeft.getPoint(10), topLeft.getPoint(10));
	Ogre::Plane bottomPlane = Ogre::Plane(bottomLeft.getOrigin(), bottomRight.getPoint(10), bottomLeft.getPoint(10));
	Ogre::Plane rightPlane = Ogre::Plane(topRight.getOrigin(), topRight.getPoint(10), bottomRight.getPoint(10));

	Ogre::PlaneBoundedVolume volume;
	volume.planes.push_back(frontPlane);
	volume.planes.push_back(topPlane);
	volume.planes.push_back(leftPlane);
	volume.planes.push_back(bottomPlane);
	volume.planes.push_back(rightPlane);

	Ogre::PlaneBoundedVolumeList volumes;
	volumes.push_back(volume);

	m_volumeQuery->setVolumes(volumes);
	Ogre::SceneQueryResult result = m_volumeQuery->execute();

	DeselectAll();

	for (auto it = result.movables.begin(); it != result.movables.end(); ++it)
	{
		(*it)->getParentSceneNode()->showBoundingBox(true);
	}
}

void MultiSelectSample::DeselectAll()
{
	for (auto it = m_creatures.begin(); it != m_creatures.end(); ++it)
	{
		(*it)->showBoundingBox(false);
	}
}
