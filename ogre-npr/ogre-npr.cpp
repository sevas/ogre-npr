/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"

#include <Ogre.h>
#include "NPRDemoApp.h"




#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
{
   // Create application object
   NPRDemoApp app;

   try
   {
      app.go();
   }
   catch( Ogre::Exception& e )
   {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
   }

   return 0;
}

#ifdef __cplusplus
}
#endif