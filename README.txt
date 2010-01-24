This work is an attempt to implement different non-photorealistic rendering (NPR) schemes
using the Ogre3D rendering engine (version 1.6.x). 
Currently render meshes with a basic strokes rendering (the google sketchup look) along with a simple gooch shading implementation.

The project ships with Visual Studio 2008 solution file for windows, along with CMakeLists.txt files with building rules for MacOS X. In the near future, CMake will be used for every platform.


Build on Windows
----------------

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


Build on MacOS X
----------------

Dependencies are : 
- MacOS X SDK v10.4u (specifically IOKit and Carbon)
- OgreSDK 1.6.x Installed under /Library/Frameworks (default path)
- The boost libraries, from macports. Installed under /opt/local/include by macports.

In the future, CMake macros will be bundled to find those dependencies automatically in their usual places.

With those dependencies set, simply run 
    $ cmake .

in the top level directory. CMake will produce Unix Makefiles. 
Run
    $ make

to build the ogre-npr.app MacOS X bundle.

