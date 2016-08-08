# DeimosSpace

# Repositories:  
Ogre3D - https://bitbucket.org/sinbad/ogre/  
Dependencies - https://bitbucket.org/cabalistic/ogredeps  
Boost - http://www.boost.org/  
SkyX - https://bitbucket.org/ogreaddons/skyx  
HydraX - https://bitbucket.org/ogreaddons/hydrax  
MyGUI - https://github.com/MyGUI/mygui  
  
# Environments:  
OGRE_HOME - project path  
OGRE_DEPENDENCIES_DIR - project path  
BOOST_ROOT - boost path  
MYGUI_DEPENDENCIES_DIR - project path  
OGRE_SOURCE_DIR - project path (in CMake)  
  
All externals should be in the same folder as project.  
In CMake field CMAKE_INSTALL_PREFIX should be changed to the project path.  
In CMake building of any samples should be disabled.  