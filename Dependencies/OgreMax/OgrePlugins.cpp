/*
 * OgreMax Viewer and WinViewer - Ogre3D-based viewers for .scene and .mesh files
 * Copyright 2009 AND Entertainment
 *
 * This code is available under the OgreMax Free License:
 *   -You may use this code for any purpose, commercial or non-commercial.
 *   -If distributing derived works (that use this source code) in binary or source code form, 
 *    you must give the following credit in your work's end-user documentation: 
 *        "Portions of this work provided by OgreMax (www.ogremax.com)"
 *
 * AND Entertainment assumes no responsibility for any harm caused by using this code.
 * 
 * The OgreMax Viewer and WinViewer were released at www.ogremax.com 
 */


//Includes---------------------------------------------------------------------
#include "OgrePlugins.hpp"
#include <OgreRoot.h>
#ifdef OGRE_STATIC_LIB
    #include <OgreCgPlugin.h>
    #include <OgreParticleFXPlugin.h>
    #include <OgreOctreePlugin.h>
    #include <OgreGLPlugin.h>
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        #include <OgreD3D9Plugin.h>
    #endif
#endif

using namespace Ogre;
using namespace OgreMax;


//Implementation---------------------------------------------------------------
OgrePlugins::~OgrePlugins()
{
    Destroy();
}

void OgrePlugins::Create()
{
#ifdef OGRE_STATIC_LIB
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        InstallPlugin("d3d", new D3D9Plugin);
    #endif

    InstallPlugin("ogl", new GLPlugin);
    InstallPlugin("cg", new CgPlugin);
    InstallPlugin("particlefx", new ParticleFXPlugin);
    InstallPlugin("octree", new OctreePlugin);
#endif
}

void OgrePlugins::Destroy()
{
#ifdef OGRE_STATIC_LIB
    for (Plugins::iterator pluginIterator = this->plugins.begin();
        pluginIterator != this->plugins.end();
        ++pluginIterator)
    {
        delete pluginIterator->plugin;
    }
    this->plugins.clear();
#endif
}

Plugin* OgrePlugins::GetPlugin(const String& internalName)
{
#ifdef OGRE_STATIC_LIB
    for (Plugins::iterator pluginIterator = this->plugins.begin();
        pluginIterator != this->plugins.end();
        ++pluginIterator)
    {
        if (pluginIterator->internalName == internalName)
            return pluginIterator->plugin;
    }
#endif
    return 0;
}

void OgrePlugins::InstallPlugin(const String& internalName, Ogre::Plugin* plugin)
{
    Root::getSingleton().installPlugin(plugin);
    this->plugins.push_back(PluginRecord(internalName, plugin));
}