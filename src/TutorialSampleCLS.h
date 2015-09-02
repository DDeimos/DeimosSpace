#include "BaseApplication.h"

class TutorialSampleCLS : public BaseApplication
{
public:
	TutorialSampleCLS();
	virtual ~TutorialSampleCLS();

protected:
	virtual void createScene();
	virtual void createCamera();

	void InitAmbientLight();
	void CreateSpotLight();
	void CreatePointLight();
	void CreateDirectionalLight();
	void CreateGround();
	void CreateNinja();
};