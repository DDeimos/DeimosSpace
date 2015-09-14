#include "BaseApplication.h"
 
class InputSample : public BaseApplication
{
public:
  InputSample();
  virtual ~InputSample();
 
protected:
  virtual void createCamera();
  virtual void createScene();
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
 
private:
  bool processUnbufferedInput(const Ogre::FrameEvent& fe);
 
};