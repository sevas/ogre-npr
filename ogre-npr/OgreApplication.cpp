/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"

#include "OgreApplication.h"

using namespace Ogre;


//-----------------------------------------------------------------------------
OgreApplication::OgreApplication(const String &_title)
    :mRoot(NULL)
    ,mCamera(NULL)
    ,mSceneMgr(NULL)
    ,mWindow(NULL)
    ,mInputManager(NULL)
    ,mKeyboard(NULL)
    ,mMouse(NULL)
    ,mRotateSpeed(0.5)
    ,mTitle(_title)
{
    
}
//-----------------------------------------------------------------------------
OgreApplication::~OgreApplication()
{
    delete mRoot;
}
//-----------------------------------------------------------------------------
void OgreApplication::go()
{
    if (!initialise())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-----------------------------------------------------------------------------
bool OgreApplication::initialise()
{
    mRoot = new Root();

    // add resource locations
    addResourceLocations();

    // if we cannot initialise Ogre, just abandon the whole deal
    if ( !initOgreCore() ) return false;

    createSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();

    // Initialise resources
    initResources();

    // Create the scene
    createScene();

    createInputSystem();
    createFrameListener();

    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::initOgreCore()
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, mTitle);
        return true;
    }
    else
    {
        return false;
    }
}
//-----------------------------------------------------------------------------
void OgreApplication::createSceneManager()
{
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC);
}
//-----------------------------------------------------------------------------
void OgreApplication::createCamera()
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setNearClipDistance(1);

    mCameraBaseNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Camera Base Node");
    mCameraTargetNode = mCameraBaseNode->createChildSceneNode("Camera Target Node");
    mCameraNode = mCameraTargetNode->createChildSceneNode("Camera Node");
    mCameraNode->attachObject(mCamera);

    mCameraNode->translate(Vector3(0, 0, 200));

}
//-----------------------------------------------------------------------------
void OgreApplication::createViewports()
{
    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0.5,0.5,0.5));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    mCamera->setQueryFlags(0);
}
//-----------------------------------------------------------------------------
void OgreApplication::createResourceListener(){}
//-----------------------------------------------------------------------------
void OgreApplication::createFrameListener()
{
    mRoot->addFrameListener(this);
}
//-----------------------------------------------------------------------------
void OgreApplication::createInputSystem()
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));


    unsigned int width, height, depth;
    int top, left;
    mWindow->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
}
//-----------------------------------------------------------------------------
void OgreApplication::addResourceLocations()
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}
//-----------------------------------------------------------------------------
void OgreApplication::initResources()
{
    // Initialise, parse scripts etc
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-----------------------------------------------------------------------------
void OgreApplication::destroyScene()
{
//    mInputManager->destroyInputObject(mMouse);
//    mInputManager->destroyInputObject(mKeyboard);
//    OIS::InputManager::destroyInputSystem(mInputManager);

    mSceneMgr->destroyAllEntities();
}
//-----------------------------------------------------------------------------
//Adjust mouse clipping area
void OgreApplication::windowResized(RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//-----------------------------------------------------------------------------
//Unattach OIS before window shutdown (very important under Linux)
void OgreApplication::windowClosed(RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//-----------------------------------------------------------------------------
bool OgreApplication::frameStarted(const FrameEvent& evt)
{
    mKeyboard->capture();
    mMouse->capture();
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::frameEnded(const FrameEvent& evt)
{
    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::keyPressed( const OIS::KeyEvent &e )
{
    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::keyReleased( const OIS::KeyEvent &e )
{
    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::mouseMoved( const OIS::MouseEvent &e )
{

    if (e.state.buttonDown(OIS::MB_Right))
    {
        int dx = e.state.X.rel * mRotateSpeed;
        int dy = e.state.Y.rel * mRotateSpeed;

        mCameraTargetNode->yaw(Degree(dx), SceneNode::TS_PARENT);
        mCameraTargetNode->pitch(Degree(dy));
    }
    else if (e.state.buttonDown(OIS::MB_Middle))
    {
        int dy = e.state.Y.rel;

        mCameraNode->translate(Vector3(0.0, 0.0, dy)); 
    }

       
   return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    return true;
}
//-----------------------------------------------------------------------------
void OgreApplication::_createGrid(int _units)
{
    SceneNode *gridNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WorldGrid Node");
    ManualObject *axes = mSceneMgr->createManualObject("AXES");


    axes->begin("WorldGrid/Axes", RenderOperation::OT_LINE_LIST);
    // X axis
    axes->position(-_units, 0.0, 0.0);     
    axes->colour(0.1, 0.0, 0.0);
    
    axes->position( _units, 0.0, 0.0);     
    axes->colour(1.0, 0.0, 0.0);
    
    // Y Axis
    axes->position(0.0, -_units, 0.0);     
    axes->colour(0.0, 0.1, 0.0);
    
    axes->position(0.0,  _units, 0.0);     
    axes->colour(0.0, 1.0, 0.0);
  
    // Z axis
    axes->position( 0.0, 0.0, -_units);     
    axes->colour(0.0, 0.0, 0.1);
    
    axes->position( 0.0, 0.0,  _units);  
    axes->colour(0.0, 0.0, 1.0);
    
    axes->end();
    gridNode->attachObject(axes);
    axes->setQueryFlags(0x00);

    ManualObject *grid = mSceneMgr->createManualObject("Grid Lines");

    grid->begin("WorldGrid/Lines", RenderOperation::OT_LINE_LIST);
    float c;
    for (int i = 10; i<=_units ; i+=10)
    {
        c = (i%100) ? 0.6 : 0.7;

        grid->position(-_units, 0, i);
        grid->colour(c, c, c);
        grid->position( _units, 0, i);
        grid->colour(c, c, c);

        grid->position(-_units, 0, -i);
        grid->colour(c, c, c);
        grid->position( _units, 0, -i);
        grid->colour(c, c, c);


        grid->position(i, 0, -_units);
        grid->colour(c, c, c);
        grid->position(i, 0,  _units);
        grid->colour(c, c, c);

        grid->position(-i, 0, -_units);
        grid->colour(c, c, c);
        grid->position(-i, 0,  _units);
        grid->colour(c, c, c);
    }


    grid->end();
    grid->setQueryFlags(0x00);
    gridNode->attachObject(grid);
}