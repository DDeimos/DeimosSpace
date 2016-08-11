# DeimosSpace

# Repositories:  
Ogre3D - https://bitbucket.org/sinbad/ogre/  
Dependencies - https://bitbucket.org/cabalistic/ogredeps  
Boost - http://www.boost.org/  
SkyX - https://bitbucket.org/ogreaddons/skyx (optional)  
HydraX - https://bitbucket.org/ogreaddons/hydrax (optional)  
MyGUI - https://github.com/MyGUI/mygui  
PhysX - https://github.com/NVIDIAGameWorks/PhysX-3.3  
OgrePhysX - https://bitbucket.org/ogreaddons/ogrephysx (optional)  
  
# Environments:  
OGRE_HOME - project path  
OGRE_DEPENDENCIES_DIR - project path  
BOOST_ROOT - boost path  
MYGUI_DEPENDENCIES_DIR - project path  
OGRE_SOURCE_DIR - project path (in CMake)  
PHYSX_DIR - physx path (to PhysX SDK)  
  
All externals should be in the same folder as project.  
In CMake field CMAKE_INSTALL_PREFIX should be changed to the project path.  
In CMake building of any samples should be disabled.  
In PhysX solution set "Runtime Library" (in Code Generation) to "Multi-Threaded DLL" for all projects.  