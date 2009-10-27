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


#ifndef OgreMax_OgrePlugins_INCLUDED
#define OgreMax_OgrePlugins_INCLUDED


//Includes---------------------------------------------------------------------
#include <OgrePlugin.h>


//Classes----------------------------------------------------------------------
namespace OgreMax
{
    /** Manages the creation and destruction of Ogre plugins */
    class OgrePlugins
    {
    public:
        ~OgrePlugins();

        void Create();
        void Destroy();

        Ogre::Plugin* GetPlugin(const Ogre::String& internalName);
        void InstallPlugin(const Ogre::String& internalName, Ogre::Plugin* plugin);

    private:
        struct PluginRecord
        {
            PluginRecord(const Ogre::String& internalName, Ogre::Plugin* plugin)
            {
                this->internalName = internalName;
                this->plugin = plugin;
            }

            Ogre::String internalName;
            Ogre::Plugin* plugin;
        };
        typedef std::list<PluginRecord> Plugins;
        Plugins plugins;
    };
}

#endif