#include "precompiled.h"

#include "MyApplication.h"

#include <sstream>
#include <boost/foreach.hpp>

//#define ANIMATE_LIGHT

void MyApplication::createScene()
{
	const RenderSystemCapabilities* caps = Root::getSingleton().getRenderSystem()->getCapabilities();
	if (!caps->hasCapability(RSC_VERTEX_PROGRAM) || !(caps->hasCapability(RSC_FRAGMENT_PROGRAM)))
	{
		OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Your card does not support vertex and fragment programs, so cannot "
			"run this demo. Sorry!", 
			"createScene");
	}




    mSceneMgr->setNormaliseNormalsOnScale(true);
    _createGrid(500);
    _createLight();
    _populate();

    _createDebugOverlay();

}
//-----------------------------------------------------------------------------
bool MyApplication::frameStarted(const FrameEvent& evt)
{
    _updateDebugOverlay();

#ifdef ANIMATE_LIGHT
		mAnimState->addTime(evt.timeSinceLastFrame);
#endif

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
void MyApplication::_populate()
{
	_loadMesh("TorusKnot01", Vector3(50, 0, 0));
	_loadMesh("Teapot01", Vector3(-50, 0, 0));
	_loadMesh("Gengon01", Vector3(50, 0, 50));
	_loadMesh("Cone01", Vector3(-50, 0, 50));
	_loadMesh("Box01", Vector3(50, 0, -50));
	_loadMesh("Cylinder01", Vector3(-50, 0, -50));
}
//-----------------------------------------------------------------------------
void MyApplication::_loadMesh(const String &_name, const Vector3 &_pos)
{
	Entity *ent = mSceneMgr->createEntity(_name, _name+".mesh");
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name+"Node", _pos);
	
	
	ent->getSubEntity(0)->setMaterialName("Examples/CelShading");

	/* ent->getSubEntity(0)->setCustomParameter(0, Vector4(35.0, 0.0, 0.0, 0.0));
	ent->getSubEntity(0)->setCustomParameter(1, Vector4(1.0, 0.5, 0.5, 1.0));
	ent->getSubEntity(0)->setCustomParameter(2, Vector4(0.7, 0.2, 0.2, 1.0));*/

	ent->getSubEntity(0)->setCustomParameter(0, Vector4(10.0f, 0.0f, 0.0f, 0.0f));
	ent->getSubEntity(0)->setCustomParameter(1, Vector4(0.0f, 0.5f, 0.0f, 1.0f));
	ent->getSubEntity(0)->setCustomParameter(2, Vector4(0.3f, 0.5f, 0.3f, 1.0f));

	
	
	node->attachObject(ent);
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


#ifdef ANIMATE_LIGHT
	Real totalTime = 10;                
    
	Animation *anim = mSceneMgr->createAnimation("Light Track", totalTime);
	anim->setInterpolationMode(Animation::IM_SPLINE);

	NodeAnimationTrack *track = anim->createNodeTrack(0, mLightNode);
	TransformKeyFrame *key;      

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
#else
	mLightNode->setPosition(0, 500, 0);
#endif
}