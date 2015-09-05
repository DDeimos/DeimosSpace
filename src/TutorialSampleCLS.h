#include "BaseApplication.h"

class TutorialSampleCLS : public BaseApplication
{
public:
	TutorialSampleCLS();
	virtual ~TutorialSampleCLS();

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	void InitAmbientLight();
	void CreateSpotLight();
	void CreatePointLight();
	void CreateDirectionalLight();
	void CreateGround();
	void CreateNinja();

private:
	Ogre::Light* directionalLight;
	Ogre::Real m_dirLightAxis;
};