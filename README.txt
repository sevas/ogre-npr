This work is an attempt to implement different non-photorealistic rendering (NPR) schemes
using the Ogre3D rendering engine (version 1.6.x). Right now, only a first version of line 
strokes rendering (the google sketchup look) is implemented.


Currently, only the Visual Studio 2008 project files are shipped. This may change in the future.
This is work in progress.


THe project files expect the several environnement variables to be set : 
$(BOOST136_HOME) : must point to the installation path of the boost librairy (version 1.36.0)
$(OGRE16_HOME) : must point to the Ogre 1.6 SDK installation path


The following dlls are supposed to be copied in the execution directories

For the debug build: 

Debug\cg.dll
Debug\OgreMain_d.dll
Debug\OIS_d.dll
Debug\RenderSystem_Direct3D9_d.dll
Debug\RenderSystem_GL_d.dll
Debug\Plugin_ParticleFX_d.dll
Debug\Plugin_CgProgramManager_d.dll
Debug\Plugin_OctreeSceneManager_d.dll



For the release build:

Release\cg.dll
Release\OgreMain.dll
Release\OIS.dll
Release\RenderSystem_Direct3D9.dll
Release\RenderSystem_GL.dll
Release\Plugin_ParticleFX.dll
Release\Plugin_CgProgramManager.dll
Release\Plugin_OctreeSceneManager.dll


Those dlls are shipped with the Ogre SDK.
