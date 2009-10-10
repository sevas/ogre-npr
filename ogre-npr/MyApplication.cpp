#include "precompiled.h"

#include "MyApplication.h"

#include <sstream>
#include <boost/foreach.hpp>



void MyApplication::createScene()
{
    mSceneMgr->setNormaliseNormalsOnScale(true);
    _createGrid(500);
    _createLight();
    //_populate();

    _createDebugOverlay();

}
//-----------------------------------------------------------------------------
bool MyApplication::frameStarted(const FrameEvent& evt)
{
    _updateDebugOverlay();
    //_drawRay(mMouseRay);
    mAnimState->addTime(evt.timeSinceLastFrame);


    return OgreApplication::frameStarted(evt);
}
//-----------------------------------------------------------------------------
bool MyApplication::frameEnded(const FrameEvent& evt)
{
    return OgreApplication::frameEnded(evt);
}
//-----------------------------------------------------------------------------
bool MyApplication::mouseMoved(const OIS::MouseEvent &e)
{
    bool ret = OgreApplication::mouseMoved(e);
    return ret;
}
//-----------------------------------------------------------------------------
bool MyApplication::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    return OgreApplication::mousePressed(e, id);
}
//-----------------------------------------------------------------------------
bool MyApplication::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
    return OgreApplication::mouseReleased(e, id);
}
//-----------------------------------------------------------------------------
bool MyApplication::keyPressed( const OIS::KeyEvent &e )
{
    return OgreApplication::keyPressed(e);
}
//-----------------------------------------------------------------------------
bool MyApplication::keyReleased( const OIS::KeyEvent &e )
{
    return OgreApplication::keyReleased(e);
}
//-----------------------------------------------------------------------------
void MyApplication::_createDebugOverlay()
{
    new TextRenderer();

    mDebugText = TextRenderer::getSingletonPtr();

    int x_offset=100, y_offset=18, w=100, h=18;

    mDebugText->addTextBox("Batches_", "#Batches : "
                            , 10, 10, w, h
                            , ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("Batches", "0"
                            , x_offset, 10, w, h
                            , ColourValue(1.0,1.0,1.0));
    mDebugText->addTextBox("FPS_", "#FPS : "
                            , 10, 10+y_offset, w, h
                            , ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("FPS", "0"
                            , x_offset, 10+y_offset, w, h
                            , ColourValue(1.0,1.0,1.0));

    mDebugText->addTextBox("Triangles_", "#tris : "
                            , 10, 10+y_offset*2, w, h
                            , ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("Triangles", "0"
                            , x_offset, 10+y_offset*2, w, h
                            , ColourValue(1.0,1.0,1.0));



}
//-----------------------------------------------------------------------------
void MyApplication::_updateDebugOverlay()
{
    mDebugText->setText("Batches", StringConverter::toString(mWindow->getBatchCount()));
    mDebugText->setText("FPS", StringConverter::toString(mWindow->getLastFPS()));
    mDebugText->setText("Triangles", StringConverter::toString(mWindow->getTriangleCount()));
}
//-----------------------------------------------------------------------------
void MyApplication::_createSphere(int id, Vector3 _pos)
{


}
//-----------------------------------------------------------------------------
void MyApplication::_createLight()
{
    mBBset = mSceneMgr->createBillboardSet("Light BB");
    mBBset->setMaterialName("Objects/Flare");
    mLightFlare = mBBset->createBillboard(Vector3::ZERO);

    mLight = mSceneMgr->createLight("main light");
    mLight->setType(Light::LT_POINT);
    mLight->setDiffuseColour(ColourValue::White);
    mLight->setSpecularColour(ColourValue::White);

    mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("light node");
    mLightNode->attachObject(mLight);
    mLightNode->attachObject(mBBset);

    Real totalTime = 10;                
    
    Animation *anim = mSceneMgr->createAnimation("Light Track", totalTime);
    anim->setInterpolationMode(Animation::IM_SPLINE);

    NodeAnimationTrack *track = anim->createNodeTrack(0, mLightNode);
    TransformKeyFrame *key;// = track->createNodeKeyFrame(0);      

    Real precision = 36, amplitude = 300;
    int keyframeIndex = 0;
    for(float phi=0.0; phi <= 2*Math::PI; phi+= Math::PI / precision)
    {
        key = track->createNodeKeyFrame(phi * (totalTime/ (2*Math::PI)));
        key->setTranslate(Vector3(amplitude*Math::Cos(phi)
                                 ,amplitude*Math::Sin(phi)
                                 ,0));
    }

    mAnimState = mSceneMgr->createAnimationState("Light Track");
    mAnimState->setEnabled(true);

    
    
}