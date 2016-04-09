#include "SkyXTerrainSample.h"

SkyXTerrainSample::SkyXTerrainSample()
{
	m_skyX = 0;
	m_controller = 0;

	m_info = 0;
}

SkyXTerrainSample::~SkyXTerrainSample()
{
	mRoot->removeFrameListener(m_skyX);
	mWindow->removeListener(m_skyX);

	delete m_skyX;
}

Ogre::String SkyXTerrainSample::GetSkyXOptions(const SkyX::AtmosphereManager::Options& options)
{
	Ogre::Vector3 time = m_controller->getTime();

	Ogre::String hour = Ogre::StringConverter::toString((int)time.x);
	Ogre::String minute = Ogre::StringConverter::toString((time.x - (int)time.x) * 60);

	Ogre::String info = "SkyX 0.4 demo (Press F1 to show/hide information)\n";
	info += "------------------------------------------------------------\n";
	info += "Time: " + hour + ":" + minute + " [1, Shift+1] (+/-).\n";
	info += "Rayleigh multiplier: " + Ogre::StringConverter::toString(options.RayleighMultiplier) + " [2, Shift+2] (+/-).\n";
	info += "Mie multiplier: " + Ogre::StringConverter::toString(options.MieMultiplier) + " [3, Shift+3] (+/-).\n";
	info += "Exposure: " + Ogre::StringConverter::toString(options.Exposure) + " [4, Shift+4] (+/-).\n";
	info += "Inner radius: " + Ogre::StringConverter::toString(options.InnerRadius) + " [5, Shift+5] (+/-).\n";
	info += "Outer radius: " + Ogre::StringConverter::toString(options.OuterRadius) + " [6, Shift+6] (+/-).\n";
	info += "Number of samples: " + Ogre::StringConverter::toString(options.NumberOfSamples) + " [7, Shift+7] (+/-).\n";
	info += "Height position: " + Ogre::StringConverter::toString(options.HeightPosition) + " [8, Shift+8] (+/-).\n";
	info += "Moon phase: " + Ogre::StringConverter::toString(m_controller->getMoonPhase()) + " [9, Shift+9] (+/-).\n";

	return info;
}

void SkyXTerrainSample::createScene()
{
	mCamera->setFarClipDistance(30000);
	mCamera->setNearClipDistance(20);
	mCamera->setPosition(20000, 1800, 20000);
	mCamera->setDirection(1, 0, 1);

	m_controller = new SkyX::BasicController();
	m_controller->setMoonPhase(0.75f);

	m_skyX = new SkyX::SkyX(mSceneMgr, m_controller);
	m_skyX->create();

	mRoot->addFrameListener(m_skyX);
	mWindow->addListener(m_skyX);

	m_skyX->getCloudsManager()->add(SkyX::CloudLayer::Options());
}

void SkyXTerrainSample::createFrameListener()
{
	BaseApplication::createFrameListener();

	/*m_info = mTrayMgr->createTextBox(OgreBites::TL_TOPLEFT, "TInfo", "", 420, 200);
	mTrayMgr->moveWidgetToTray(m_info, OgreBites::TL_TOPLEFT, 0);*/
}

// может быть нужно переместить в frameStarted?
bool SkyXTerrainSample::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (!BaseApplication::frameRenderingQueued(fe))
	{
		return false;
	}

	m_skyX->setTimeMultiplier(0.1f);

	SkyX::AtmosphereManager::Options options = m_skyX->getAtmosphereManager()->getOptions();

	// Time
	if (mKeyboard->isKeyDown(OIS::KC_1) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_skyX->setTimeMultiplier(1.0f);
	if (mKeyboard->isKeyDown(OIS::KC_1) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		m_skyX->setTimeMultiplier(-1.0f);

	// Rayleigh multiplier
	if (mKeyboard->isKeyDown(OIS::KC_2) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.RayleighMultiplier += fe.timeSinceLastFrame * 0.025f;
	if (mKeyboard->isKeyDown(OIS::KC_2) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.RayleighMultiplier -= fe.timeSinceLastFrame * 0.025f;

	// Mie multiplier
	if (mKeyboard->isKeyDown(OIS::KC_3) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.MieMultiplier += fe.timeSinceLastFrame * 0.025f;
	if (mKeyboard->isKeyDown(OIS::KC_3) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.MieMultiplier -= fe.timeSinceLastFrame * 0.025f;

	// Exposure
	if (mKeyboard->isKeyDown(OIS::KC_4) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.Exposure += fe.timeSinceLastFrame * 0.5f;
	if (mKeyboard->isKeyDown(OIS::KC_4) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.Exposure -= fe.timeSinceLastFrame * 0.5f;

	// Inner radius
	if (mKeyboard->isKeyDown(OIS::KC_5) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.InnerRadius += fe.timeSinceLastFrame * 0.25f;
	if (mKeyboard->isKeyDown(OIS::KC_5) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.InnerRadius -= fe.timeSinceLastFrame * 0.25f;

	// Outer radius
	if (mKeyboard->isKeyDown(OIS::KC_6) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.OuterRadius += fe.timeSinceLastFrame * 0.25f;
	if (mKeyboard->isKeyDown(OIS::KC_6) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.OuterRadius -= fe.timeSinceLastFrame * 0.25f;

	// Number of samples
	if (mKeyboard->isKeyDown(OIS::KC_7) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.NumberOfSamples++;
	if (mKeyboard->isKeyDown(OIS::KC_7) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.NumberOfSamples--;

	// Height position
	if (mKeyboard->isKeyDown(OIS::KC_8) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.HeightPosition += fe.timeSinceLastFrame * 0.05f;
	if (mKeyboard->isKeyDown(OIS::KC_8) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
		options.HeightPosition -= fe.timeSinceLastFrame * 0.05f;

	// Moon phase
	if (mKeyboard->isKeyDown(OIS::KC_9) && !mKeyboard->isKeyDown(OIS::KC_LSHIFT))
	{
		m_controller->setMoonPhase(m_controller->getMoonPhase() + fe.timeSinceLastFrame * 0.25f);

		if (m_controller->getMoonPhase() > 1)
		{
			m_controller->setMoonPhase(1);
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_9) && mKeyboard->isKeyDown(OIS::KC_LSHIFT))
	{
		m_controller->setMoonPhase(m_controller->getMoonPhase() - fe.timeSinceLastFrame * 0.25f);

		if (m_controller->getMoonPhase() < -1)
		{
			m_controller->setMoonPhase(-1);
		}
	}

	m_skyX->getAtmosphereManager()->setOptions(options);
	//m_info->setCaption(GetSkyXOptions(options));

	return true;
}

bool SkyXTerrainSample::keyPressed(const OIS::KeyEvent& ke)
{
	return true;
}

bool SkyXTerrainSample::keyReleased(const OIS::KeyEvent& ke)
{
	return true;
}
