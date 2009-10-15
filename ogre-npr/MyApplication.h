
#ifndef _MYAPPLICATION__H_
#define _MYAPPLICATION__H_

#include <vector>
#include <list>
#include "OgreApplication.h"
#include "TextRenderer.h"

using namespace Ogre;


class MyApplication : public OgreApplication
{
protected:

public:
    void createScene();

protected:
    bool frameStarted(const FrameEvent& evt);
    bool frameEnded(const FrameEvent& evt);
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

	void _populate();
    void _createLight();
    void _createSphere(int, Vector3);
	void _loadMesh(const String&, const Vector3&);

	void _createDebugOverlay();
	void _updateDebugOverlay();

protected:
    BillboardSet *mBBset;
    Billboard *mLightFlare;
    Light *mLight;
    SceneNode *mLightNode;

    TextRenderer *mDebugText;

    AnimationState *mAnimState;

};


#endif _MYAPPLICATION__H_